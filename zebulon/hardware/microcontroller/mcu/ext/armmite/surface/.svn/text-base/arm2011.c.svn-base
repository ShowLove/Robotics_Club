//UCF-ASV 2009 MCU Firmware for Coridium Armmite 
#include "coridium.h"
#include "cor_hwlib.h"
#include "cor_init.h"
#include "cor_wrflash.h"

#include "mem.h"		// not used here, included only for reference
#include "string.h"
#include "printf.h"

#define PIN_CHECK(x) (IN((x))?1:0)

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


unsigned char checksumMessage(unsigned char *message)
{
	int i;
	unsigned char sum = 0;
	sum += '#';
	sum += '%';
	for(i = 0; i < 5; i++)
	{
		sum += message[i];
	}
	
	return sum;
}

void SendMessage(unsigned char *message)
{
	int i;
	TXD0('#');
	TXD0('%');
	for(i = 0; i < 5; i++)
	{
		TXD0(message[i]);
	}
	TXD0(checksumMessage(message));
}

int ReportAnalogInputsMessage(unsigned char *message)
{
	int tempADValue = 0;
	
	if((message[1] >= 0) && (message[1] <= 7))
	{
		configAD(message[1]);
		tempADValue = AD(message[1]);
		message[2] = tempADValue%256;
		message[3] = tempADValue/256;
		message[4] = 0;
		
		SendMessage(message);
		
		return 1;
	}
	else
	{
		return 0;
	}
}

int SetDigitalOutputsMessage(unsigned char *message)
{
	if((message[1] >= 0) && (message[1] <= 15))
	{
		OUTPUT(message[1]);
		if(message[2] == 0)
		{
			LOW(message[1]);
		}
		else
		{
			HIGH(message[1]);
		}		
		SendMessage(message);
		
		return 1;
	}
	else
	{
		return 0;
	}
	
}

int ReportDigitalInputsMessage(unsigned char *message)
{
	if((message[1] >= 0) && (message[1] <= 15))
	{
		INPUT(message[1]);
		message[2] = PIN_CHECK(message[1]);
		message[3] = 0;
		message[4] = 0;
		SendMessage(message);
		
		return 1;
	}
	else
	{
		return 0;
	}
}

int QueryReportMessage()
{
	//unsigned char message[5];
	//message[0] = 0x00;
	return 1;
}

int PongMessage()
{
	unsigned char message[5];
	message[0] = pong;
	message[1] = 'P';
	message[2] = 'O';
	message[3] = 'N';
	message[4] = 'G';
	SendMessage(message);
	
	return 1;
}

int ReportMCUIdMessage()
{
	unsigned char message[5];
	message[0] = reportMCUId;
	message[1] = 'A';
	message[2] = 'R';
	message[3] = 'M';
	message[4] = '0';
	SendMessage(message);	
}


int ReceiveMessage(unsigned char *message)
{
	int returnValue = 0;
	unsigned int isum;
	//calc checksum from message
	isum=checksumMessage(message);
	//if internal and external checksum not equal, then it is bad
	if(isum!=message[5])
	{
		return returnValue;
	}
	//check the header
	switch(message[0])
	{
		case queryReport:
			returnValue = QueryReportMessage(message);
			break;
		case ping:
			returnValue = PongMessage();
			break;
		case setDigitalOutput:
			returnValue = SetDigitalOutputsMessage(message);
			break;
		case reportDigitalInputs:
			returnValue = ReportDigitalInputsMessage(message);
			break;
		case reportAnalogInputs:
			returnValue = ReportAnalogInputsMessage(message);
			break;
		case reportMCUId:
			returnValue = ReportMCUIdMessage(message);
			break;
		
		//if not a valid message return
		default:
			returnValue = 0;
		
	}
	return returnValue;
}


#define BUFFSIZE 256
int main()
{
	unsigned char buffer[BUFFSIZE];
	int pos = 0;
	int start = 0;
	int count = 0;
	unsigned char tmpchar;
	init_coridium();
	// set UART1 baudrate to 56kHz
	setbaud (0, 17);
		
	while(1)
	{
		tmpchar=RXD0();
		//check first byte
		if(start==0 && tmpchar=='#')
		{
			start++;
		}
		//check second byte
		else if(start==1 && tmpchar=='%')
		{
			start++;
			pos=0;
		}
		//store values (up to 6)
		else if(start==2 && pos<6)
		{
			buffer[pos]=tmpchar;
			pos++;
		}
		//received full packet 2 start + 6 message
		else if(pos==6)
		{
			if(ReceiveMessage(buffer)==1)
			{
				count++;
			}
			//start over regardless
			start=0;
			pos=0;
		}
		else
		{
			start=0;
			pos=0;
		}
	}
	
	return 0;
}

/* End of file */
