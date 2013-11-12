//General Purpose MCU Firmware for ATMega8 - 28pin

/*
 - Analog Inputs - PORT C
    + AN0: PC0 = PIN 23
    + AN1: PC1 = PIN 24
    + AN2: PC2 = PIN 25
    + AN3: PC3 = PIN 26
    + AN4: PC4 = PIN 27
    + AN5: PC5 = PIN 28

 - Digital Inputs - PORTB
    + GPI0: PD0 = PIN 2
    + GPI1: PD1 = PIN 3
    + GPI2: PD2 = PIN 4
    + GPI3: PD3 = PIN 5
    + GPI4: PD4 = PIN 6
    + GPI5: PD5 = PIN 11
    + GPI6: PD6 = PIN 12
    + GPI7: PD7 = PIN 13

 - Digital Outputs - PORTD
    + GPO0: PB0 = PIN 14
    + GPO1: PB1 = PIN 15
    + GPO2: PB2 = PIN 16
    + GPO3: PB3 = PIN 17
    + GPO4: PB4 = PIN 18
    + GPO5: PB5 = PIN 19
    + GPO6: PB6 = PIN 9
    + GPO7: PB7 = PIN 10

*/

#define NUMDATABYTES 4
#define PACKETLENGTH NUMDATABYTES+4

#define MOTORFUTUREGROWTH 0x64

#define STARTBYTE1 '#'
#define STARTBYTE2 '%'

#define BAUD 57600

enum mcuPorts
{
        mcuPortB = 0,
        mcuPortC,
        mcuPortD,
        mcuOther
};

char port[28] = {mcuPortC,mcuPortD,mcuPortD,mcuPortD,mcuPortD,mcuPortD,mcuOther,mcuOther,mcuPortB,mcuPortB,
                                mcuPortD,mcuPortD,mcuPortD,mcuPortB,mcuPortB,mcuPortB,mcuPortB,mcuPortB,mcuPortB,
                                mcuOther,mcuOther,mcuOther,mcuPortC,mcuPortC,mcuPortC,mcuPortC,mcuPortC,mcuPortC};

char pin[28] =  {6, 0, 1, 2, 3, 4, 0, 0, 6, 7, 5, 6, 7, 0,
                                1, 2, 3, 4, 5, 0, 0, 0, 0, 1, 2, 3, 4, 5};

enum state
{
    waitForStartBytes = 0,
    getHeader,
    getData,
    processChecksum
};

enum commandTypes
{
    all =                       0xFF,
    queryReport =               0x00,
    ping =                      0x0D,
    pong =                      0x84,

    setServoOutputs =           0x0B,
    setDigitalOutput =          0x0C,
    setAnalogInputs =           0x0E,

    reportDigitalInputs =       0x82,
    reportAnalogInputs =        0x83,

    reportMotorStatus =         0x81,
    setMotorAddress =           0x0A,

    reportMCUId =               0x8F
};

// Motor Addresses
enum motorAddresses
{
    axialRight =                0x50,
    axialLeft =                 0x52,

    lateralTop =                0x60,

    verticalFrontRight =        0x70,
    verticalFrontLeft =         0x72,
    verticalBackRight =         0x74,
    verticalBackLeft =          0x76
};

// Software I2C connections
sbit Soft_I2C_Scl_Output    at PORTC5_bit;
sbit Soft_I2C_Sda_Output    at PORTC4_bit;
sbit Soft_I2C_Scl_Input     at PINC5_bit;
sbit Soft_I2C_Sda_Input     at PINC4_bit;
sbit Soft_I2C_Scl_Direction at DDC5_bit;
sbit Soft_I2C_Sda_Direction at DDC4_bit;
// End Software I2C connections

void Initialize()
{
    Delay_ms(2000);
    // Initialize USART
    UART1_Init(BAUD);
    Delay_ms(2000);
    // Initialize I2C
    Soft_I2C_Init();
    Delay_ms(2000);
}

void TxPacket(char buffer[])
{
    int i;
    for (i=0; i<PACKETLENGTH; i++)
    {
        UART1_Write(buffer[i]);
    }
}

void TxString(char header, char payload[])
{
    char buffer[PACKETLENGTH];
    char checksum;

    checksum  = buffer[0] = STARTBYTE1;
    checksum += buffer[1] = STARTBYTE2;
    checksum += buffer[2] = header;
    checksum += buffer[3] = payload[0];
    checksum += buffer[4] = payload[1];
    checksum += buffer[5] = payload[2];
    checksum += buffer[6] = payload[3];
    buffer[7] = checksum;

    TxPacket(buffer);
}

void TxPacketBytes(char header, char data0, char data1, char data2, char data3)
{
    char buffer[8];
    char checksum;

    checksum  = buffer[0] = STARTBYTE1;
    checksum += buffer[1] = STARTBYTE2;
    checksum += buffer[2] = header;
    checksum += buffer[3] = data0;
    checksum += buffer[4] = data1;
    checksum += buffer[5] = data2;
    checksum += buffer[6] = data3;
    buffer[7] = checksum;

    TxPacket(buffer);
}

void FPing()
{
    TxPacketBytes(pong, 'P', 'O', 'N', 'G');
}

void FReportMCUId()
{
     TxPacketBytes(reportMCUId, 'M', 'T', 'R', '2');
}

void FReportMotorStatus(char dataIn[])
{
    char message[8];
    message[7] = dataIn[0];

    Soft_I2C_Start();
    // Write to motors: Address
    Soft_I2C_Write(dataIn[0] | 0x01);

    // Current address
    message[0] = (Soft_I2C_Read(1) & 0xFE);

    // Status Byte
    // 7 6 5 4 3 2 1 bits
    // 7-4 Software Revision
    // 3-2 Future Growth
    // 1 Motor type
    //   - 0-Brushed
    //   - 1-Brushless
    // 0 Current Limitation
    //   - 0-Limited
    //   - 1-Unlimited
    message[1] = Soft_I2C_Read(1);

    // Fault Byte
    // 7-5 Future Growth
    // 4 Water Detect (0 none, 1 water)
    // 3 Ground Fault
    // 2 Hall Sensor error
    // 1 Stalled Motor
    // 0 Over Temperature
    message[2] = Soft_I2C_Read(1);

    // Current Byte
    // 0x00-0xFF If 12 in decimal, current is 1.2A
    message[3] = Soft_I2C_Read(1);

    // SPEED, current speed
    message[4] = Soft_I2C_Read(1);

    // Temperature, in Celcius
    message[5] = Soft_I2C_Read(1);

    // Checksum
    message[6] = Soft_I2C_Read(0);

    // End I2C communication
    Soft_I2C_Stop();

    TxPacketBytes(reportMotorStatus,
				  message[0],message[2],
				  message[3],message[5]);
}

void FCommandMotors(char address, char command)
{
    // Send START
    Soft_I2C_Start();

    // Write to motors: Address
    Soft_I2C_Write(address);

    // Write to motors: Speed
    Soft_I2C_Write(command);

    // Write to motors: Info
    Soft_I2C_Write(MOTORFUTUREGROWTH);

    // Write to motors: Checksum
    Soft_I2C_Write(address + command + MOTORFUTUREGROWTH);

    // Stop
    Soft_I2C_Stop();
}

void FSetServoOutputs(char dataIn[])
{
    if (dataIn[0] == axialRight)
    {
        FCommandMotors(axialRight, dataIn[1]);
    }
    else if (dataIn[0] == axialLeft)
    {
        FCommandMotors(axialLeft, dataIn[1]);
    }
    else if (dataIn[0] == lateralTop)
    {
        FCommandMotors(lateralTop, dataIn[1]);
    }
    else if (dataIn[0] == verticalFrontRight)
    {
        FCommandMotors(verticalFrontRight, dataIn[1]);
    }
    else if (dataIn[0] == verticalFrontLeft)
    {
        FCommandMotors(verticalFrontLeft, dataIn[1]);
    }
    else if (dataIn[0] == verticalBackRight)
    {
        FCommandMotors(verticalBackRight, dataIn[1]);
    }
        else if (dataIn[0] == verticalBackLeft)
    {
        FCommandMotors(verticalBackLeft, dataIn[1]);
    }
}

void FSetMotorAddress(char dataIn[])
{
    // Send start
    Soft_I2C_Start();
    // Write to motors: Address
    Soft_I2C_Write(dataIn[0]);
    // Write to motors: Speed
    Soft_I2C_Write(0x06);
    // Write to motors: Info
    Soft_I2C_Write(0x72);
    // Write to motors: Checksum
    //checkSum = dataIn[0] + 0x06 + 0x72;
    Soft_I2C_Write(dataIn[0] + 0x06 + 0x72);
    // Stop
    Soft_I2C_Stop();

    //Delay_ms(100);

    Soft_I2C_Start();
    // Write to motors: Address
    Soft_I2C_Write(dataIn[0]);
    // Write to motors: Speed
    Soft_I2C_Write(dataIn[1]);
    // Write to motors: Checksum
    //checkSum = dataIn[0] + dataIn[1];
    Soft_I2C_Write(dataIn[0] + dataIn[1]);
    //Soft_I2C_Write(dataIn[0]+dataIn[0]+dataIn[1]+0x06+0x72 + dataIn[1]);
    // Stop
    Soft_I2C_Stop();
}

//MAIN
void main()
{
    char currentByte, previousByte;
    char header, payload[NUMDATABYTES], checksum;
    char i; //Keeps track of data index
    unsigned int rxChecksumErrorCnt, rxInvalidHeaderCnt;
    enum state currentState;
    int direction;
    char openFlag;
    int closeCounter;

    // Initialize
    Initialize();
    // Initialize Serial Comm
    currentByte = previousByte = 0;
    currentState = waitForStartBytes;
    rxChecksumErrorCnt = 0;
    rxInvalidHeaderCnt = 0;
    openFlag = 0;
    closeCounter = 0;

    while (1)
    {

        if (UART1_Data_Ready())
        {
            previousByte = currentByte;
            currentByte = UART1_Read();

            if ((previousByte == STARTBYTE1) && (currentByte == STARTBYTE2))
            {
                currentState = getHeader;
                checksum = previousByte + currentByte;
            }
            else
            {
                switch (currentState)
                {
                    case waitForStartBytes:
                        //Do nothing wait for if statement above to succeed
                        break;

                    case getHeader:
                        checksum += header = currentByte;
                        i = 0;
                        currentState = getData;
                        break;

                    case getData:
                        checksum += payload[i] = currentByte;
                        i++;
                        if (i >= NUMDATABYTES)  // was >=
                        {
                            currentState = processChecksum;
                        }
                        break;

                    case processChecksum:
                        if (checksum == currentByte) //Valid message received
                        {
                            switch (header) //Switch on header to supported messages
                            {
                                case ping:
                                    FPing();
                                    break;

                                case setMotorAddress:
                                    FSetMotorAddress(payload);
                                    break;
									
								case setServoOutputs:
                                    FSetServoOutputs(payload);
                                    break;
									
								case reportMotorStatus:
                                    FReportMotorStatus(payload);
                                    break;

                                case reportMCUId:
                                    FReportMCUId();
                                    break;

                                default:
                                    rxInvalidHeaderCnt++;
                                    //Do something if count exceeds some value
                                    break;
                            }
                        }
                        else //Checksum failed, throw out message
                        {
                            rxChecksumErrorCnt++;
                            //Do something if error count exceeds some value
                        }

                        currentState = waitForStartBytes;
                        break;

                    default:
                        currentState = waitForStartBytes;
                        break;
                }
            }
        }
        else
        {
         // Nothing
        }
    }
}