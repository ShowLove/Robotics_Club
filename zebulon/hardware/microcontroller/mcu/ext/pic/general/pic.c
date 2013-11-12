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
    setDigitalOutput =          0x0C,
    ping =                      0x0D,
    reportDigitalInputs =       0x82,
    reportAnalogInputs =        0x83,
    pong =                      0x84
};


void Initialize()
{
    //Initially configure all pins off and as an output
    PORTA = 0;
    TRISA = 0;
    PORTB = 0;
    TRISB = 0;
    PORTC = 0;
    TRISC = 0;
    PORTD = 0;
    TRISD = 0;
    PORTE = 0;
    TRISE = 0;

    //Configure USART
    USART_Init(BAUD);

    //Configure Analog Inputs
    TRISA.F0 = 1; //AN0
    TRISA.F1 = 1; //AN1
    TRISA.F2 = 1; //AN2
    TRISA.F3 = 1; //AN3
    TRISA.F5 = 1; //AN4
    TRISE.F0 = 1; //AN5
    TRISE.F1 = 1; //AN6
    TRISE.F2 = 1; //AN7
    ADCON1 = 0x80;

    //Configure Digital Inputs
    TRISB.F0 = 1; //GPI0
    TRISB.F1 = 1; //GPI1
    TRISB.F2 = 1; //GPI2
    TRISB.F3 = 1; //GPI3
    TRISB.F4 = 1; //GPI4
    TRISB.F5 = 1; //GPI5
    TRISB.F6 = 1; //GPI6
    TRISB.F7 = 1; //GPI7

    //Configure Digital Outputs
    TRISD.F0 = 0; //GPO0
    TRISD.F1 = 0; //GPO1
    TRISD.F2 = 0; //GPO2
    TRISD.F3 = 0; //GPO3
    TRISD.F4 = 0; //GPO4
    TRISD.F5 = 0; //GPO5
    TRISD.F6 = 0; //GPO6
    TRISD.F7 = 0; //GPO7
}


void TxPacket(char buffer[])
{
    int i;
    for (i=0; i<PACKETLENGTH; i++)
    {
        USART_Write(buffer[i]);
    }
}


void TxString(char header, char data[])
{
    char buffer[PACKETLENGTH];
    char checksum;

    checksum  = buffer[0] = STARTBYTE1;
    checksum += buffer[1] = STARTBYTE2;
    checksum += buffer[2] = header;
    checksum += buffer[3] = data[0];
    checksum += buffer[4] = data[1];
    checksum += buffer[5] = data[2];
    checksum += buffer[6] = data[3];
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

    pinState = 1 << dataIn[0];
    if (dataIn[1])
    {
        PORTD |= pinState;
    }
    else
    {
        PORTD &= (~pinState);
    }
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

    pinState = PORTB >> dataIn[0];
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


//MAIN
void main()
{
    char currentByte, previousByte;
    char header, data[NUMDATABYTES], checksum;
    char i; //Keeps track of data index
    unsigned int rxChecksumErrorCnt, rxInvalidHeaderCnt;
    enum state currentState;

    Initialize();

    currentByte = previousByte = 0;
    currentState = waitForStartBytes;
    rxChecksumErrorCnt = 0;
    rxInvalidHeaderCnt = 0;

    while (1)
    {
        previousByte = currentByte;

        if (USART_Data_Ready())
        {
            currentByte = USART_Read();

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
                        checksum += data[i] = currentByte;
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

                                case setDigitalOutput:
                                    FSetDigitalOutput(data);
                                    break;

                                case ping:
                                    FPing();
                                    break;

                                case reportDigitalInputs:
                                    FReportDigitalInputs(data);
                                    break;

                                case reportAnalogInputs:
                                    FReportAnalogInputs(data);
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
        }
    }
}
