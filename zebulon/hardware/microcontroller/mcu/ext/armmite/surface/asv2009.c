//UCF-ASV 2009 MCU Firmware for Coridium Armmite 
#include "coridium.h"
#include "cor_hwlib.h"
#include "cor_init.h"
#include "cor_wrflash.h"

#include "mem.h"		// not used here, included only for reference
#include "string.h"
#include "printf.h"

#define PIN_CHECK(x) (IN((x))?1:0)

#define DEBOUNCE_TIME 5
#define START_BUTTON_PIN 15
#define BATTERY_CHECK_PIN 0
#define E_STOP_CHECK_PIN 1
#define TRANSISTOR_LIGHT 14
#define TRANSISTOR_HORN 12

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

/*int BatteryCheck()
{
}

void EStopStatus()
{
}

void StartButtonCheck()
{	
	//startButton = PIN_CHECK(IN(START_BUTTON_PIN));
}

void TransistorLight()
{
}

void TransisotrHorn()
{
}*/

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
	// Start button check
	// Battery Check
	// E-Stop status
	// Nothing
	return 1;
}

int PongMessage()
{
	unsigned char message[5];
	message[0] = 0x84;
	message[1] = 'P';
	message[2] = 'O';
	message[3] = 'N';
	message[4] = 'G';
	SendMessage(message);
	
	return 1;
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
		case 0x00:
			returnValue = QueryReportMessage(message);
			break;
		case 0x0D:
			returnValue = PongMessage(message);
			break;
		case 0x0C:
			returnValue = SetDigitalOutputsMessage(message);
			break;
		case 0x82:
			returnValue = ReportDigitalInputsMessage(message);
			break;
		case 0x83:
			returnValue = ReportAnalogInputsMessage(message);
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
	
	// inputs
	// Estop, Auto, Start, CompLogic, 2X10, 2X5
	INPUT(9); // Auto
	INPUT(11); // estop
	INPUT(10); // Start
	
	// Outputs
	// Horn, Greenlight, redlight
	OUTPUT(8); // Armdir
	LOW(0);
	OUTPUT(13); // Horn
	LOW(0);
	OUTPUT(14); // greenlight
	LOW(0);
	OUTPUT(15); // redlight
	LOW(0);
	
	
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
