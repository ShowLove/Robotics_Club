#define EINBUFSIZE  15
#define EOUTBUFSIZE 15

#define NUMDATABYTES 4

#define ANALOG_ADDR_WR 0x5E
#define ANALOG_ADDR_RD 0x5F

#define SEABOTIX_FUTURE_VALUE 0xFF

#define STARTBYTE1 '#'
#define STARTBYTE2 '%'

#define BAUD 57600


#use rcm33xx.lib
#use i2c.lib


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
    setMotorThrust =            0x0B,
    setDigitalOutput =          0x0C,
    ping =                      0x0D,
    setMotorAddress =           0x0E,
    reportMotorStatus =         0x80,
    //reportDepthSensor =         0x81, //Removed because it is just an analog input on channel 0
    reportDigitalInputs =       0x82,
    reportAnalogInputs =        0x83,
    pong =                      0x84,
    confirmMotorAddressChange = 0x85,
    reportMotorStatus1 =        0x86,
    reportMotorStatus2 =        0x87
};


enum motorAddresses
{
    left =       0x60,
    right =      0x70,
    vertical =   0x80,
    horizontal = 0xFE,
    all =        0xFF
};

enum DigitalIO
{
    //Digital Inputs
    ESTOP = 0x00,
    AUTONOMOUS,
    STARTBUTTON,
    SPAREIN1,
    SPAREIN2,
    HYDROI,

    //Digital Outputs
    SOLENOID1 = 0x80,
    SOLENOID2,
    ERROR1,
    ERROR2,
    HYDROOUT1,
    HYDROOUT2,
    SPAREOUT1,
    VIDEORESET,
    LEAKLED,
    RABBITSTATUS = 0xFF
};


void MsDelay(unsigned int delay)
{
    auto unsigned long done_time;

    done_time = MS_TIMER + delay;
    while((long)(MS_TIMER - done_time) < 0);
}


//Return 0 if success
int Write_I2C(char buffer[], int length)
{
    int i;
    int returnValue;

    returnValue = i2c_start_tx();
    for(i=0; i<length; i++)
    {
        returnValue += i2c_write_char(buffer[i]);
    }
    i2c_stop_tx();

    return returnValue;
}


int SetMotor(char address, char thrust)
{
    char buffer[4];
    char checksum;

    checksum  = buffer[0] = address;
    checksum += buffer[1] = thrust;
    checksum += buffer[2] = SEABOTIX_FUTURE_VALUE;
    buffer[3] = checksum;

    return Write_I2C(buffer, 4);
}


void ChangeMotorAddress(char currentAddress, char newAddress)
{
    char buffer[4];
    char checksum;

    checksum  = buffer[0] = currentAddress;
    checksum += buffer[1] = 0x06;
    checksum += buffer[2] = 0x72;
    buffer[3] = checksum;
    Write_I2C(buffer, 4);

    MsDelay(100);

    checksum  = buffer[0] = currentAddress;
    checksum += buffer[1] = newAddress;
    buffer[2] = checksum;
    Write_I2C(buffer, 3);
}


//Channel range [0, 7]
unsigned int ReadAnalogIn(char channel)
{
    unsigned int value;
    char valueHi, valueLo;
    char controlByte;
    char buffer[2];

    controlByte = 0x80;
    controlByte |= ((channel & 0x07) << 4);

    buffer[0] = ANALOG_ADDR_WR;
    buffer[1] = controlByte;
    Write_I2C(buffer, 2);

    //Read Analog over I2C
    i2c_start_tx();
    i2c_write_char(ANALOG_ADDR_RD);
    i2c_read_char(&valueHi);
    i2c_send_ack();
    i2c_read_char(&valueLo);
    i2c_send_ack();
    i2c_stop_tx();

    value = ((unsigned int) valueLo + (((unsigned int)valueHi) << 8)) >> 4;

    return value;
}


void TxPacketString(char header, char data[])
{
    char buffer[8];
    char checksum;

    checksum  = buffer[0] = STARTBYTE1;
    checksum += buffer[1] = STARTBYTE2;
    checksum += buffer[2] = header;
    checksum += buffer[3] = data[0];
    checksum += buffer[4] = data[1];
    checksum += buffer[5] = data[2];
    checksum += buffer[6] = data[3];
    buffer[7] = checksum;

    serEwrite(buffer, 8);
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

    serEwrite(buffer, 8);
}


void QueryReport()
{
    //Do something when query report is received.
}


void SetMotorThrust(char dataIn[])
{
    //dataIn[0] = Motor Address
    //dataIn[1] = Thrust
    //dataIn[2] = Thrust
    //dataIn[3] = Thrust

    if(dataIn[0] == all)
    {
        SetMotor(left, dataIn[1]);
        SetMotor(right, dataIn[2]);
        SetMotor(vertical, dataIn[3]);
    }
    else if(dataIn[0] == horizontal)
    {
        SetMotor(left, dataIn[1]);
        SetMotor(right, dataIn[2]);
    }
    else
    {
        SetMotor(dataIn[0], dataIn[1]);
    }
}


void SetDigitalOutput(char dataIn[])
{
    //dataIn[0] = Port
    //dataIn[1] = State
    //dataIn[2] = Blank
    //dataIn[3] = Blank
    char pinState;
    pinState = dataIn[1];
    if(pinState == 0)
    {
        pinState = 0;
    }
    else
    {
        pinState = 1;
    }

    switch(dataIn[0])
    {
        case SOLENOID1:                      
            BitWrPortI(PFDR, &PFDRShadow, pinState, 5);
            break;
        case SOLENOID2:
            BitWrPortI(PFDR, &PFDRShadow, pinState, 6);
            break;
        case ERROR1:
            BitWrPortI(PBDR, &PBDRShadow, pinState, 7);
            break;
        case ERROR2:
            BitWrPortI(PBDR, &PBDRShadow, pinState, 6);
            break;
        case HYDROOUT1:
            //Signal B to the logic frequency selection
            BitWrPortI(PBDR, &PBDRShadow, pinState, 4);
            break;
        case HYDROOUT2:
            //Signal A to the logic frequency selection
            BitWrPortI(PBDR, &PBDRShadow, pinState, 5);
            break;
        case SPAREOUT1:
            BitWrPortI(PFDR, &PFDRShadow, pinState, 2);
            break;
        case VIDEORESET:
            BitWrPortI(PFDR, &PFDRShadow, pinState, 3);
            break;
        case LEAKLED:
            BitWrPortI(PFDR, &PFDRShadow, pinState, 7);
            break;
        case RABBITSTATUS:
            BitWrPortI(PFDR, &PFDRShadow, pinState, 4);
            break;
    }
}


void Ping()
{
    TxPacketBytes(pong, 'P', 'O', 'N', 'G');
}


void SetMotorAddress(char dataIn[])
{
    //dataIn[0] = Current Address
    //dataIn[1] = New Address
    //dataIn[2] = Blank
    //dataIn[3] = Blank

    ChangeMotorAddress(dataIn[0], dataIn[1]);
    TxPacketBytes(confirmMotorAddressChange,
                  dataIn[1], //Current Address
                  dataIn[0], //Old Address
                  0, //Blank
                  0);//Blank
}


void ReportMotorStatus(char dataIn[])
{
    //dataIn[0] = Motor Address
    //dataIn[1] = Blank
    //dataIn[2] = Blank
    //dataIn[3] = Blank

    char motorMessageType, motorStatus, motorFault, motorCurrent, motorSpeed, motorTemperature, motorChecksum;
    char checksum;

    i2c_start_tx();
    i2c_write_char(dataIn[0] + 1);
    i2c_read_char(&motorMessageType); //Same as how the motor was addressed: data[0] + 1
    i2c_send_ack();
    i2c_read_char(&motorStatus);
    i2c_send_ack();
    i2c_read_char(&motorFault);
    i2c_send_ack();
    i2c_read_char(&motorCurrent);
    i2c_send_ack();
    i2c_read_char(&motorSpeed);
    i2c_send_ack();
    i2c_read_char(&motorTemperature);
    i2c_send_ack();
    i2c_read_char(&motorChecksum);
    i2c_send_ack();
    i2c_stop_tx();

    checksum = motorMessageType + motorStatus + motorFault + motorCurrent + motorSpeed + motorTemperature;

    if(checksum == motorChecksum) //Valid message received from motors
    {
        TxPacketBytes(reportMotorStatus1, dataIn[0], motorStatus, motorFault, motorCurrent);
        TxPacketBytes(reportMotorStatus2, dataIn[0], motorSpeed, motorTemperature, 0);
    }
}


void ReportDigitalInputs(char dataIn[])
{
    //dataIn[0] = Port
    //dataIn[1] = Blank
    //dataIn[2] = Blank
    //dataIn[3] = Blank

    //Returns two if port does not exist.

    char value;

    switch(dataIn[0])
    {
        case ESTOP:
            value = BitRdPortI(PFDR, 0);
            break;
        case AUTONOMOUS:
            value = 2;
            break;
        case STARTBUTTON:
            value = 2;
            break;
        case SPAREIN1:
            value = BitRdPortI(PFDR, 1);
            break;
        case SPAREIN2:
            value = 2;
            break;
        case HYDROI:
            value = 2;
            break;
        case SOLENOID1:
            value = BitRdPortI(PFDR, 5);
            break;
        case SOLENOID2:
            value = BitRdPortI(PFDR, 6);
            break;
        case ERROR1:
            value = BitRdPortI(PBDR, 7);
            break;
        case ERROR2:
            value = BitRdPortI(PBDR, 6);
            break;
        case HYDROOUT1:
            value = BitRdPortI(PBDR, 4);
            break;
        case HYDROOUT2:
            value = BitRdPortI(PBDR, 5);
            break;
        case SPAREOUT1:
            value = BitRdPortI(PFDR, 2);
            break;
        case VIDEORESET:
            value = BitRdPortI(PFDR, 3);
            break;
        case LEAKLED:
            value = BitRdPortI(PFDR, 7);
            break;
        case RABBITSTATUS:
            value = BitRdPortI(PFDR, 4);
            break;
        default:
            value = 2;
            break;
    }

    TxPacketBytes(reportDigitalInputs,
                  dataIn[0], //Port
                  value, //State
                  0, //Blank
                  0);//Blank
}


void ReportAnalogInputs(char dataIn[])
{
    //dataIn[0] = Channel
    //dataIn[1] = Blank
    //dataIn[2] = Blank
    //dataIn[3] = Blank

    unsigned int analogIn;
    analogIn = ReadAnalogIn(dataIn[0]);

    TxPacketBytes(reportAnalogInputs,
                  dataIn[0], //Channel
                  (unsigned char) (analogIn % 256), //Lo
                  (unsigned char) (analogIn / 256), //Hi
                  0);
}


void main()
{
    char currentByte, previousByte;
    char header, data[DATASIZE], checksum;
    char i; //Keeps track of data index
    unsigned int rxChecksumErrorCnt, rxInvalidHeaderCnt;
    enum state currentState;
    char depthSensorAddress, electronicBatteryVoltageAddress, motorBatteryVoltageAddress;
    char estopButtonCurrent, estopButtonPrevious;
    unsigned int rxPacketCnt;
    char setStatus[4];

    //brdInit(); //NOT SURE IF WE NEED THIS WHEN NOT USING DEV BOARD
    i2c_init();
    serEopen(BAUD);

    //Configure I/O Pins
/*
    Pin | Dir | Signal
    ------------------
    PF0 |  I  | ESTOP
    PF1 |  I  | SPARE IN
    PF2 |  O  | SPARE OUT
    PF3 |  O  | VIDEO_RESET
    PF4 |  O  | RABBIT_STATUS
    PF5 |  O  | SOL1
    PF6 |  O  | SOL2
    PF7 |  O  | LEAK_LED

    PB4 |  O  | HYDROOUT1
    PB5 |  O  | HYDROOUT2
    PB6 |  O  | ERROR2
    PB7 |  O  | ERROR1
*/

    WrPortI(PFDDR, &PFDDRShadow, 0xFC);  // 0b11111100, 0=Input, 1 = Output
    WrPortI(PBDDR, &PBDDRShadow, 0xF0);  // 0b11110000


    depthSensorAddress = 0x00;
    electronicBatteryVoltageAddress = 0x01;
    motorBatteryVoltageAddress = 0x02;

    estopButtonCurrent = estopButtonPrevious = 0;
    currentByte = previousByte = 0;
    currentState = waitForStartBytes;
    rxChecksumErrorCnt = 0;
    rxInvalidHeaderCnt = 0;
    rxPacketCnt = 0;
    setStatus[0] = RABBITSTATUS;
    setStatus[1] = 0;


    while(1)
    {
        loophead();
        costate
        {
            previousByte = currentByte;

            wfd currentByte = cof_serEgetc(); //yields until successfully receives a character

            if((previousByte == STARTBYTE1) && (currentByte == STARTBYTE2))
            {
                currentState = getHeader;
                checksum = previousByte + currentByte;
            }
            else
            {
                switch(currentState)
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
                        if(i == NUMDATABYTES)
                        {
                            currentState = processChecksum;
                        }
                        break;

                    case processChecksum:
                        if(checksum == currentByte) //Valid message received
                        {
                            rxPacketCnt++;

                            setStatus[1] = (char) rxPacketCnt%2;
                            SetDigitalOutput(setStatus);

                            switch(header) //Switch on header to supported messages
                            {
                                case queryReport:
                                    QueryReport();
                                    break;

                                case setMotorThrust:
                                    SetMotorThrust(data);
                                    break;

                                case setDigitalOutput:
                                    SetDigitalOutput(data);
                                    break;

                                case ping:
                                    Ping();
                                    break;

                                case setMotorAddress:
                                    SetMotorAddress(data);
                                    break;

                                case reportMotorStatus:
                                    ReportMotorStatus(data);
                                    break;

                                case reportDigitalInputs:
                                    ReportDigitalInputs(data);
                                    break;

                                case reportAnalogInputs:
                                    ReportAnalogInputs(data);
                                    break;

                                default:
                                    rxInvalidHeaderCnt++;
                                    //printf("Invalid Header\n");
                                    //Do something if count exceeds some value
                                    break;
                            }
                        }
                        else //Checksum failed, throw out message
                        {
                            rxChecksumErrorCnt++;
                            //printf("ERROR IN CHECKSUM RX: %d\n", rxChecksumErrorCnt);
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

        costate
        {
            //Report Depth Sensor as Analog input at 20Hz = every 50ms
            ReportAnalogInputs(&depthSensorAddress);
            waitfor(DelayMs(50));
        }

        costate
        {
            //Report Estop Button as Digital Input on change
            estopButtonPrevious = estopButtonCurrent;
            estopButtonCurrent = BitRdPortI(PFDR, 0);
            if(estopButtonCurrent != estopButtonPrevious)
            {
                TxPacketBytes(reportDigitalInputs, ESTOP, estopButtonCurrent, 0, 0);
            }
            waitfor(DelayMs(50));
        }

        costate
        {
            //Report Battery level voltages at 1Hz = every 1000ms
            waitfor(DelayMs(1000));
            ReportAnalogInputs(&electronicBatteryVoltageAddress);
            ReportAnalogInputs(&motorBatteryVoltageAddress);
        }
    }
    serEclose();
}


