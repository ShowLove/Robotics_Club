//General Purpose MCU Firmware for Microchip PIC16F877A

/*
 - Analog Inputs
    + AN0: RA0 = PIN 2
    + AN1: RA1 = PIN 3
    + AN2: RA2 = PIN 4
    + AN3: RA3 = PIN 5
    + AN4: RA5 = PIN 7
    + AN5: RE0 = PIN 8
    + AN6: RE1 = PIN 9
    + AN7: RE2 = PIN 10

 - Digital Inputs - PORTB
    + GPI0: RB0 = PIN 33
    + GPI1: RB1 = PIN 34
    + GPI2: RB2 = PIN 35
    + GPI3: RB3 = PIN 36
    + GPI4: RB4 = PIN 37
    + GPI5: RB5 = PIN 38
    + GPI6: RB6 = PIN 39
    + GPI7: RB7 = PIN 40

 - Digital Outputs - PORTD
    + GPO0: RD0 = PIN 19
    + GPO1: RD1 = PIN 20
    + GPO2: RD2 = PIN 21
    + GPO3: RD3 = PIN 22
    + GPO4: RD4 = PIN 27
    + GPO5: RD5 = PIN 28
    + GPO6: RD6 = PIN 29
    + GPO7: RD7 = PIN 30

*/

//#include <built_in.h>

#define NUMDATABYTES 4
#define PACKETLENGTH NUMDATABYTES+4

#define MOTORFUTUREGROWTH 0x64

#define STARTBYTE1 '#'
#define STARTBYTE2 '%'

#define BAUD 57600
//#define BAUD 19200

/*char gServo0_OpenPos = 30;
char gServo0_ClosePos = 30;
char gServo1_OpenPos = 30;
char gServo1_ClosePos = 30;*/

enum state
{
    waitForStartBytes = 0,
    getHeader,
    getData,
    processChecksum
};


enum commandTypes
{
    queryReport =               0x00,
    //setMotorThrust =            0x0A,
    setServoOutputs =           0x0B,
    setMotorAddress =           0x0A,
    setDigitalOutput =          0x0C,
    ping =                      PINB >> temp; 0x0D,
    reportMotorStatus =         0x81,
    reportDigitalInputs =       0x82,
    reportAnalogInputs =        0x83,
    pong =                      0x84
};

enum motorAddresses
{
    /*left =       0x60,
    right =      0x70,
    vertical =   0x80,
    horizontal = 0xFE,
    all =        0xFF*/

    axialRight =                0x50,
    axialLeft =                 0x52,
    lateralTop =                0x60,
    lateralBottom =             0x62,
    verticalFront =             0x70,
    verticalBack =              0x72,
    axial =                     0xFC,
    lateral =                   0xFD,
    vertical =                  0xFE,
    all =                       0xFF
};

enum DigitalIO
{
    // Digital Inputs
    START = 0x00,

    // Digital Outputs
    POWERSUPPLYELEC = 0x10,
    POWERSUPPLYMOTOR,
    THINKLED

};

enum AnalogInputs
{
    // Digital Inputs
    POWERON = 0x20,
    ESTOP,
    BATTVOLTAGEELEC,
    BATTVOLTAGEMOTOR,
    SHOREVOLTAGEELEC,
    SHOREVOLTAGEMOTOR

};

enum SwitchStatesElec
{
     CheckShoreElec = 0,
     TimerElec,
     SwitchPowerElec,
     CheckDisconnectElec
};

enum SwitchStatesMotor
{
     CheckShoreMotor = 0,
     TimerMotor,
     SwitchPowerMotor,
     CheckDisconnectMotor
};

enum ADCStates
    {
         AD1 = 1,
         AD2,
         AD3,
         AD4
    };

// Software I2C connections
sbit Soft_I2C_Scl_Output    at PORTC0_bit;
sbit Soft_I2C_Sda_Output    at PORTC1_bit;
sbit Soft_I2C_Scl_Input     at PINC0_bit;
sbit Soft_I2C_Sda_Input     at PINC1_bit;
sbit Soft_I2C_Scl_Direction at DDC0_bit;
sbit Soft_I2C_Sda_Direction at DDC1_bit;
// End Software I2C connections

void Initialize()
{
    char temp;
    //Initially configure all pins off and as an output
    //PORTA = 0;
    //DDRA  = 0xFF;           // Set direction to be output
    DDRB = 0xFF;           // Set direction to be output
    PORTB = 0;


    //PORTC = 0;
    //DDRC = 0xFF;           // Set direction to be output

    //DDRD = 0xFF;          // Set direction to be output
    //PORTD = 0;



    //Configure USART
    UART1_Init(BAUD);
    Delay_ms(100);

    //UART_Set_Active(&UART1_Read, &UART1_Write, &UART1_Data_Ready, &UART1_Tx_Idle);
    //Delay_ms(100);

    // Initialize I2C
    //TWI_Init(100000);
    Soft_I2C_Init();
    Delay_ms(100);

    //Configure Analog Inputs
    /*
    DDRA.F0 = 0; //AN0
    DDRA.F1 = 0; //AN1
    DDRA.F2 = 0; //AN2
    DDRA.F3 = 0; //AN3
    DDRA.F4 = 0; //AN4
    DDRA.F5 = 0; //AN5
    DDRA.F6 = 0; //AN6
    DDRA.F7 = 0; //AN7
    */
    //Configure Digital Inputs
    /*DDRB.F0 = 0; //GPI0
    DDRB.F1 = 1; //GPI1
    DDRB.F2 = 1; //GPI2
    DDRB.F3 = 0; //GPI3
    DDRB.F4 = 0; //GPI4
    DDRB.F5 = 0; //GPI5
    DDRB.F6 = 0; //GPI6
    DDRB.F7 = 0; //GPI7

    //Configure Digital Outputs
    DDRC.F0 = 1; //GPO0
    DDRC.F1 = 1; //GPO1
    DDRC.F2 = 1; //GPO2
    DDRC.F3 = 1; //GPO3
    DDRC.F4 = 1; //GPO4
    DDRC.F5 = 1; //GPO5
    DDRC.F6 = 1; //GPO6
    DDRC.F7 = 1; //GPO7

    temp = 0;

    PORTD.F2 = temp.F0;
    PORTD.F3 = temp.F1;
    PORTD.F4 = temp.F2;
    PORTD.F5 = temp.F3;

    DDRD.F6 = 0; //Set to input for reading if shutter is open*/

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


/*//COMMANDS
void FQueryReport()
{
    //Do something when query report is received.
} */

char SetCharBit()

void FSetDigitalOutput(char dataIn[])
{
    char pinState;
    char temp = 0;
    
    if(dataIn[0] == 0)
    {
     pinState = 0;
    }
    else if(dataIn[0] <= 8)
    {
     temp = dataIn[0] - 1;
     DDRB |= (1 << temp);
     PORTB = (PORTB & ~(1 << temp)) | (dataIn[1] << temp);
    }
    else if(dataIn[0] <= 21)
    {
     temp = dataIn[0] - 14;
     DDRD |= (1 << temp);
     PORTD = (PORTD & ~(1 << temp)) | (dataIn[1] << temp);
    }
    else if(dataIn[0] <= 29)
    {
     temp = dataIn[0] - 22;
     DDRC |= (1 << temp);
     PORTC = (PORTC & ~(1 << temp)) | (dataIn[1] << temp);
    }
    else
    {
     pinState = 0;
    }
}



void FPing()
{
    //PORTB.F0 = 1;
    TxPacketBytes(pong, 'P', 'O', 'N', 'G');
    //PORTB.F0 = 0;
}


void FReportDigitalInputs(char dataIn[])
{
    //dataIn[0] = Pin
    //dataIn[1] = Blank
    //dataIn[2] = Blank
    //dataIn[3] = Blank
    char pinState;
    char temp = 0;
    
    if(dataIn[0] == 0)
    {
     pinState = 0;
    }
    else if(dataIn[0] <= 8)
    {
     temp = dataIn[0] - 1;
     DDRB &= ~(1 << temp);
     pinState = PINB >> temp;
     pinState &= 0x01;
    }
    else if(dataIn[0] <= 21)
    {
     temp = dataIn[0] - 14;
     DDRD &= ~(1 << temp);
     pinState = PIND >> temp;
     pinState &= 0x01;
    }
    else if(dataIn[0] <= 29)
    {
     temp = dataIn[0] - 22;
     DDRC &= ~(1 << temp);
     pinState = PINC >> temp;
     pinState &= 0x01;
    }
    else
    {
     pinState = 0;
    }

    TxPacketBytes(reportDigitalInputs,
                  dataIn[0], //Port
                  pinState, //State
                  0, //Blank
                  0);//Blank
}


void FReportAnalogInputs(char dataIn[])
{
    //dataIn[0] = Channel
    //dataIn[1] = Blank
    //dataIn[2] = Blank
    //dataIn[3] = Blank

    unsigned analogIn;
    analogIn = ADC_Read(dataIn[0]);

    TxPacketBytes(reportAnalogInputs,
                  dataIn[0], //Channel
                  (unsigned char)(analogIn % 256),  //Lo
                  (unsigned char)(analogIn / 256),  //Hi
                  0);
}

void FReportMotorStatus(char dataIn[])
{
     char message[8];
     //PORTB.F2 = 1;
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

     //PORTB.F2 = 0;
}

/*void SetServo(char channel, char value)
{
    int servoValue;
    char servoValueH, servoValueL;

    servoValue = value;
    servoValue = servoValue*2 + 235 - 5;

    servoValueH = (char)((servoValue >> 8) & 0x00FF);
    servoValueL = (char)(servoValue & 0x00FF);

    switch (channel)
    {
        case 0:
            OCR1AH = servoValueH;
            OCR1AL = servoValueL;
            break;

        case 1:
            OCR1BH = servoValueH;
            OCR1BL = servoValueL;
            break;

        case 2:
            //OCR1CH = valueH;
            //OCR1CL = valueL;
            break;

        default:
            //invalid channel received
            break;
    }
}*/

void FCommandMotors(char address, char command)
{
     //PORTB.F1 = 1;
     //char checkSum = 0;
    // Send START
     Soft_I2C_Start();

      // Write to motors: Address
      Soft_I2C_Write(address);

      // Write to motors: Speed
      Soft_I2C_Write(command);

      // Write to motors: Info
      Soft_I2C_Write(MOTORFUTUREGROWTH);

      // Write to motors: Checksum
      //checkSum = address + command + MOTORFUTUREGROWTH;
      Soft_I2C_Write(address + command + MOTORFUTUREGROWTH);

      // Stop
      Soft_I2C_Stop();

     //PORTB.F1 = 0;
}

void FSetServoOutputs(char dataIn[])
{
    /*if (dataIn[0] == axial)
    {
        FCommandMotors(axialRight, dataIn[1]);
        FCommandMotors(axialLeft, dataIn[2]);
    }
    else if (dataIn[0] == lateral)
    {
        FCommandMotors(lateralTop, dataIn[1]);
        FCommandMotors(lateralBottom, dataIn[2]);
    }
    else if (dataIn[0] == vertical)
    {
        FCommandMotors(verticalFront, dataIn[1]);
        FCommandMotors(verticalBack, dataIn[2]);
    }*/
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
    else if (dataIn[0] == lateralBottom)
    {
        FCommandMotors(lateralBottom, dataIn[1]);
    }
    else if (dataIn[0] == verticalFront)
    {
        FCommandMotors(verticalFront, dataIn[1]);
    }
    else if (dataIn[0] == verticalBack)
    {
        FCommandMotors(verticalBack, dataIn[1]);
    }
}

void FSetMotorAddress(char dataIn[])
{
      //PORTB.F4 = 1;

      //char checkSum = 0;
      // I2C stuff in here
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
      //checkSum = 0;

      Soft_I2C_Start();
      // Write to motors: Address
      Soft_I2C_Write(dataIn[0]);
      // Write to motors: Speed
      Soft_I2C_Write(dataIn[1]);
      // Write to motors: Checksum
      //checkSum = dataIn[0] + dataIn[1];
      //Soft_I2C_Write(0xC2);
      Soft_I2C_Write(dataIn[0] + dataIn[1]);
      //Soft_I2C_Write(dataIn[0]+dataIn[0]+dataIn[1]+0x06+0x72 + dataIn[1]);
      // Stop
      Soft_I2C_Stop();
      //DDRB.F3 = 0x00;
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
    //int count = 0;


    //int countSwitch = 0;
    //int mSwitchRelayElec = 0;

    // Main Variables
    unsigned int shoreElecVal;
    unsigned int shoreMotorVal;
    unsigned int BatteryElecVal;
    unsigned int BatteryMotorVal;
    int switchStateElec = CheckShoreElec;
    int countTimerElec;

    int switchStateMotor = CheckShoreMotor;
    int countTimerMotor;

    int ADCState = AD1;
    int countAD = 1;
    int slowADCount = 0;



    Initialize();
    //DDRB.F6 = 1;
    //PORTB.F6 = 1;



    currentByte = previousByte = 0;
    currentState = waitForStartBytes;
    rxChecksumErrorCnt = 0;
    rxInvalidHeaderCnt = 0;
    openFlag = 0;
    closeCounter = 0;

    while (1)
    {
                       /*if(count == 0)
                        {
                           DDRB.F7 = 1;
                           PORTB.F7 = 1;
                           count = 1;
                        }
                        else
                        {
                            DDRB.F7 = 1;
                            PORTB.F7 = 0;
                            //PORTB.F7 = 0;
                            count = 0;
                        }*/
                        //Delay_ms(100);

        //previousByte = currentByte;
        if (UART1_Data_Ready())
        {
            //DDRB.F5 = 1;
            //PORTB.F5 = 0;
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
                                /*case queryReport:
                                    FQueryReport();
                                    break;*/

                                case setServoOutputs:
                                    FSetServoOutputs(payload);
                                    break;

                                case setDigitalOutput:
                                    FSetDigitalOutput(payload);
                                    break;

                                case ping:
                                    FPing();
                                    break;

                                case reportDigitalInputs:
                                    FReportDigitalInputs(payload);
                                    break;

                                case reportAnalogInputs:
                                    FReportAnalogInputs(payload);
                                    break;

                                case reportMotorStatus:
                                    FReportMotorStatus(payload);
                                    break;

                                case setMotorAddress:
                                    FSetMotorAddress(payload);
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
                        /*if(count == 0)
                        {
                           PORTB.F7 = 1;
                           count = 1;
                        }
                        else
                        {
                            PORTB.F7 = 0;
                            count = 0;
                        } */


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
            //Delay_ms(1000);

            // Switching control for AUV Statemachine
            // Check the voltages of shore electronics/ shore motors
            // If one has been on for X amount of time, switch from battery to
            // shore

            /*shoreElecVal = ADC_Read(4);
            shoreMotorVal = ADC_Read(3);
            BatteryElecVal = ADC_Read(5);
            BatteryMotorVal = ADC_Read(2);*/



            if(slowADCount >= 50)
            {
                slowADCount = 0;
                ADCState = countAD%4;
                
                switch(ADCState)
                {
                      case AD1:
                           shoreElecVal = ADC_Read(4);
                           break;
                      case AD2:
                           shoreMotorVal = ADC_Read(3);
                           break;
                      case AD3:
                           BatteryElecVal = ADC_Read(5);
                           break;
                      case AD4:
                           BatteryMotorVal = ADC_Read(2);
                           countAD = 1;
                           break;
                };
                
                countAD++;
            }
            slowADCount++;



            switch(switchStateElec)
            {
                  case CheckShoreElec:
                       // Check if shore electronics is connected
                       if(shoreElecVal > 0xFF)
                       {
                           switchStateElec = TimerElec;
                       }
                       else
                       {
                           switchStateElec = CheckShoreElec;
                       }
                       break;
                  case TimerElec:
                       if(countTimerElec > 70000)
                       {
                           switchStateElec = SwitchPowerElec;
                       }
                       else if(shoreElecVal < 0xFF)
                       {
                            countTimerElec = 0;
                            switchStateElec = CheckShoreElec;
                       }
                       else
                       {
                           countTimerElec++;
                       }
                       break;
                  case SwitchPowerElec:
                       countTimerElec = 0;
                       DDRD.F7 = 1;
                       PORTD.F7 = 1;
                       switchStateElec = CheckDisconnectElec;
                       break;
                  case CheckDisconnectElec:
                       if(shoreElecVal < 0xFF)
                       {
                           DDRD.F7 = 1;
                           PORTD.F7 = 0;
                           switchStateElec = CheckShoreElec;
                       }
                       else
                       {
                           switchStateElec = CheckDisconnectElec;
                       }
                       break;
                  //default:
                      // break;

            }

            /*switch(switchStateMotor)
            {
                  case CheckShoreMotor:
                       // Check if shore electronics is connected
                       if(shoreMotorVal > 0xFF)
                       {
                           switchStateMotor = TimerMotor;
                       }
                       else
                       {
                           switchStateMotor = CheckShoreMotor;
                       }
                       break;
                  case TimerMotor:
                       if(countTimerMotor > 30000)
                       {
                           switchStateMotor = SwitchPowerMotor;
                       }
                       else if(shoreMotorVal < 0xFF)
                       {
                            countTimerMotor = 0;
                            switchStateMotor = CheckShoreMotor;
                       }
                       else
                       {
                           countTimerMotor++;
                       }
                       break;
                  case SwitchPowerMotor:
                       countTimerMotor = 0;
                       DDRC.F7 = 1;
                       PORTC.F7 = 1;
                       switchStateMotor = CheckDisconnectMotor;
                       break;
                  case CheckDisconnectMotor:
                       if(shoreMotorVal < 0xFF)
                       {
                          // DDRC.F7 = 1;
                           PORTC.F7 = 0;
                           switchStateMotor = CheckShoreMotor;
                       }
                       else
                       {
                           switchStateMotor = CheckDisconnectMotor;
                       }
                       break;
                  //default:
                  //     break;
            }*/



        }

    }
}
