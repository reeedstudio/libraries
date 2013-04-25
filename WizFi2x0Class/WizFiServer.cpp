#include <WizFiServer.h>

WizFiServer::WizFiServer(uint16_t port)
{
//	uint8_t tmpstr[20];

	src_port= port;

//	sprintf((char *)tmpstr, "SPort: %d", src_port);	
//	Serial.println((char *)tmpstr);
	
       WizFi = &myWizFi;
	   
	bOpen = false;
}

uint8_t WizFiServer::begin()
{
	uint8_t retval;
	byte tmpstr[20];
	byte tmpIP[16];
	
	WizFi->Current_Command_Code = OP_NSTCP;
	WizFi->SetSrcPortnum(src_port);
	while(1)
	{
		retval = WizFi->SendCommand(WizFi->Current_Command_Code);

		if(retval == 1)
		{
			WizFi->Current_Command_Code = OP_AT;
			bOpen = true;
			WizFi->bCommandMode = false;
			CID = WizFi->CID;
//			Serial.println((char *)WizFi->MsgBuf);
//			Serial.print("My Client CID: ");
//			Serial.println((char)CID);
			return 1;
		}else
		{
			if(retval == 2)
			{
//				Serial.println("Error reply");
				return 0; // Connection faiied
			}
			else if(retval == 3)
			{
//				Serial.println("Timeout");
				return 0; // Connection faiied
			}
		}
	}
}

/*
boolean WizFiServer::IsConnected()
{
	return bOpen;
}

void WizFiServer::SetIsConnected(boolean flag)
{
	bOpen = flag;
}
*/

uint8_t WizFiServer::GetCID(void)
{
	return CID;
}

const char *WizFiServer::GetCIDstr(void)
{
	byte* strCID;

	sprintf((char *)strCID, "%d", CID);

	return (const char *)strCID;
}

/*
void WizFiServer::write(byte value)
{
//	sprintf(tmpstr, "%cS%c%c%cE", ASCII_ESC, CID, value, ASCII_ESC);
	WizFi->write(ASCII_ESC);
	WizFi->write('S');
	WizFi->write(CID);
	WizFi->write(value);
	WizFi->write(ASCII_ESC);
	WizFi->write('E');

}

void WizFiServer::write(byte *buf)
{
	WizFi->write(ASCII_ESC);
	WizFi->write('S');
	WizFi->write(CID);
	WizFi->write(buf);
	WizFi->write(ASCII_ESC);
	WizFi->write('E');

}

void WizFiServer::write(byte *buf, size_t size)
{
	WizFi->write(ASCII_ESC);
	WizFi->write('S');
	WizFi->write(CID);
	WizFi->write(buf, size);
	WizFi->write(ASCII_ESC);
	WizFi->write('E');

}

uint8_t WizFiServer::read(void)
{
	return WizFi->read();
}

uint8_t WizFiServer::read(byte *buf)
{
	uint8_t index, i=0;

	index = CID - '0';


	WizFi->RcvPacket();

	if(WizFi->IsDataRcvd[index])
	{
		while(WizFi->Current_ESC_Data_Length > 0)
		{
			buf[i++] = WizFi->RcvdBuf[WizFi->readPtr++];
			if(WizFi->readPtr == 128)
				WizFi->readPtr = 0;
			WizFi->Current_ESC_Data_Length--;
		}
		WizFi->IsDataRcvd[index] = false;

		return i;
	}
	return 0;
}

uint8_t WizFiServer::read(byte *buf, size_t size)
{
	uint8_t index, retval, i=0;

	index = CID - '0';

	retval = 0;
	
	WizFi->RcvPacket();
	
	if(WizFi->IsDataRcvd[index])
	{
		while(WizFi->readPtr != WizFi->writePtr)
		{
			buf[i++] = WizFi->RcvdBuf[WizFi->readPtr++];
		}
		WizFi->IsDataRcvd[index] = false;

		return i;
	}
	return retval;
}

*/
