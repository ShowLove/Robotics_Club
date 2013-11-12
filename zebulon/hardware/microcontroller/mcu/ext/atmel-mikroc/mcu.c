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

#define NUMDATABYTES 4
#define PACKETLENGTH NUMDATABYTES+4

#define STARTBYTE1 '#'
#define STARTBYTE2 '%'

#define BAUD 57600

char gServo0_OpenPos = 30;
char gServo0_ClosePos = 30;
char gServo1_OpenPos = 30;
char gServo1_ClosePos = 30;

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
    setServoOutputs =           0x0B,
    setDigitalOutput =          0x0C,
    ping =                      0x0D,
    reportDigitalInputs =       0x82,
    reportAnalogInputs =        0x83,
    pong =                      0x84,
    setShutter =                0x20,
    openShutter =               0x21,
    closeShutter =              0x22
};


void Initialize()
{
    char temp;
    //Initially configure all pins off and as an output
    //PORTA = 0;
    //DDRA  = 0xFF;           // Set direction to be output
    
    PORTB = 0;
    DDRB = 0xFF;           // Set direction to be output

    PORTC = 0;
    DDRC = 0xFF;           // Set direction to be output

    PORTD = 0;
    DDRD = 0xFF;          // Set direction to be output

    //Configure USART
    UART1_Init(BAUD);

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
    DDRB.F0 = 0; //GPI0
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
    
    DDRD.F6 = 0; //Set to input for reading if shutter is open

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


//COMMANDS
void FQueryReport()
{
    //Do something when query report is received.
}


void FSetDigitalOutput(char dataIn[])
{
    //dataIn[0] = Pin
    //dataIn[1] = State
    //dataIn[2] = Blank
    //dataIn[3] = Blank
    char pinState;

    switch(dataIn[0])
    {
        case 0:
            break;

        case 1:   //BCD Port Command
            PORTD.F2 = dataIn[1].F0;
            PORTD.F3 = dataIn[1].F1;
            PORTD.F4 = dataIn[1].F2;
            PORTD.F5 = dataIn[1].F3;
            break;
            
        default:  //Invalid PORT Received
            break;
    }
    /*
    pinState = 1 << dataIn[0];
    if (dataIn[1])
    {
        PORTC |= pinState;
    }
    else
    {
        PORTC &= (~pinState);
    }
    */
}


void FPing()
{
    TxPacketBytes(pong, 'P', 'O', 'N', 'G');
}


void FReportDigitalInputs(char dataIn[])
{
    //dataIn[0] = Pin
    //dataIn[1] = Blank
    //dataIn[2] = Blank
    //dataIn[3] = Blank
    char pinState;

    pinState = PINB >> dataIn[0];
    pinState &= 0x01;

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


void SetServo(char channel, char value)
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
}


void FSetServoOutputs(char dataIn[])
{
    SetServo(dataIn[0], dataIn[1]);
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

    Initialize();

    currentByte = previousByte = 0;
    currentState = waitForStartBytes;
    rxChecksumErrorCnt = 0;
    rxInvalidHeaderCnt = 0;
    openFlag = 0;
    closeCounter = 0;

    TCCR1A = 0xA0;
    TCCR1B = 0x12;
    TCNT1H = 0x00;
    TCNT1L = 0x00;
    CS00_bit = 0;
    CS01_bit = 1;
    CS02_bit = 0;
    ICR1H = 0x27;
    ICR1L = 0x10;

    SetServo(0, gServo0_ClosePos);
    SetServo(1, gServo1_ClosePos);

    while (1)
    {
        previousByte = currentByte;
        if (UART1_Data_Ready())
        {
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
                        if (i == NUMDATABYTES)
                        {
                            currentState = processChecksum;
                        }
                        break;

                    case processChecksum:
                        if (checksum == currentByte) //Valid message received
                        {
                            switch (header) //Switch on header to supported messages
                            {
                                case queryReport:
                                    FQueryReport();
                                    break;

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
                                    
                                case setShutter:
                                    gServo0_OpenPos = payload[0];
                                    gServo0_ClosePos = payload[1];
                                    gServo1_OpenPos = payload[2];
                                    gServo1_ClosePos = payload[3];
                                    break;
                                    
                                case openShutter:
                                    SetServo(0, gServo0_OpenPos);
                                    SetServo(1, gServo1_OpenPos);
                                    break;
                                    
                                case closeShutter:
                                    SetServo(0, gServo0_ClosePos);
                                    SetServo(1, gServo1_ClosePos);
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
            //Do other stuff when not reading serial
            if(PIND.F6 == 1)
            {
                closeCounter = 0;
                if (openFlag == 1)
                {
                    openFlag = 0;
                    TxPacketBytes(reportDigitalInputs,
                                  1, //Port for Shutter
                                  1, //State
                                  0, //Blank
                                  0);//Blank
                }
            }
            else if (PIND.F6 == 0)
            {
                if(closeCounter > 2048)
                {
                    openFlag = 1;
                }
                else
                {
                    closeCounter++;
                }
            }
        }

    }
}
