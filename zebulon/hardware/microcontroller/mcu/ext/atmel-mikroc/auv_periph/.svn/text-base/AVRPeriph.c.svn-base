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

// Specific to the sensor board
enum DigitalOutputs
{
        dropper2 = 0,	//PB5
        dropper1,		//PB4
        grabberArm,		//PB3
        extra,			//PB2
        torpedo2,		//PB1
        torpedo1		//PB0
};

void Initialize()
{
    Delay_ms(2000);
    // Initialize USART
    UART1_Init(BAUD);
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
     TxPacketBytes(reportMCUId, 'P', 'E', 'R', 'F');
}

void FSetDigitalOutput(char dataIn[])
{
        char portSwitch, pinSwitch;

        portSwitch = port[dataIn[0]-1];
        pinSwitch = pin[dataIn[0]-1];

        switch(portSwitch)
        {
                case mcuPortB:
                        switch(pinSwitch)
                        {
                                case 0:
                                        DDRB.F0 = 1;
                                        PORTB.F0 = dataIn[1];
                                        break;
                                case 1:
                                        DDRB.F1 = 1;
                                        PORTB.F1 = dataIn[1];
                                        break;
                                case 2:
                                        DDRB.F2 = 1;
                                        PORTB.F2 = dataIn[1];
                                        break;
                                case 3:
                                        DDRB.F3 = 1;
                                        PORTB.F3 = dataIn[1];
                                        break;
                                case 4:
                                        DDRB.F4 = 1;
                                        PORTB.F4 = dataIn[1];
                                        break;
                                case 5:
                                        DDRB.F5 = 1;
                                        PORTB.F5 = dataIn[1];
                                        break;
                                case 6:
                                        DDRB.F6 = 1;
                                        PORTB.F6 = dataIn[1];
                                        break;
                                case 7:
                                        DDRB.F7 = 1;
                                        PORTB.F7 = dataIn[1];
                                        break;
                        }
                break;

                case mcuPortC:
                        switch(pinSwitch)
                        {
                                case 0:
                                        DDRC.F0 = 1;
                                        PORTC.F0 = dataIn[1];
                                        break;
                                case 1:
                                        DDRC.F1 = 1;
                                        PORTC.F1 = dataIn[1];
                                        break;
                                case 2:
                                        DDRC.F2 = 1;
                                        PORTC.F2 = dataIn[1];
                                        break;
                                case 3:
                                        DDRC.F3 = 1;
                                        PORTC.F3 = dataIn[1];
                                        break;
                                case 4:
                                        DDRC.F4 = 1;
                                        PORTC.F4 = dataIn[1];
                                        break;
                                case 5:
                                        DDRC.F5 = 1;
                                        PORTC.F5 = dataIn[1];
                                        break;
                                case 6:
                                        DDRC.F6 = 1;
                                        PORTC.F6 = dataIn[1];
                                        break;
                                case 7:
                                        DDRC.F7 = 1;
                                        PORTC.F7 = dataIn[1];
                                        break;
                        }
                break;

                case mcuPortD:
                        switch(pinSwitch)
                        {
                                case 0:
                                        DDRD.F0 = 1;
                                        PORTD.F0 = dataIn[1];
                                        break;
                                case 1:
                                        DDRD.F1 = 1;
                                        PORTD.F1 = dataIn[1];
                                        break;
                                case 2:
                                        DDRD.F2 = 1;
                                        PORTD.F2 = dataIn[1];
                                        break;
                                case 3:
                                        DDRD.F3 = 1;
                                        PORTD.F3 = dataIn[1];
                                        break;
                                case 4:
                                        DDRD.F4 = 1;
                                        PORTD.F4 = dataIn[1];
                                        break;
                                case 5:
                                        DDRD.F5 = 1;
                                        PORTD.F5 = dataIn[1];
                                        break;
                                case 6:
                                        DDRD.F6 = 1;
                                        PORTD.F6 = dataIn[1];
                                        break;
                                case 7:
                                        DDRD.F7 = 1;
                                        PORTD.F7 = dataIn[1];
                                        break;
                        }
                break;
        }

        TxPacketBytes(setDigitalOutput,
                  dataIn[0], //Port
                  dataIn[1], //State
                  0, //Blank
                  0);//Blank
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
                                case setDigitalOutput:
                                    FSetDigitalOutput(payload);
                                    break;

                                case ping:
                                    FPing();
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
