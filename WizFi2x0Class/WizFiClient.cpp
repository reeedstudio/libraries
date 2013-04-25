#include <WizFiClient.h>

/////////////////////////////////////////////////////////
// WIZnet WizFi Shield library is Copyright(c) 2012 by WIZnet, Inc.
// and is licensed under the LGPL.
// 
// Author : James YS Kim

WizFiClient::WizFiClient(void)
{
	WizFi = &myWizFi;
	bOpen = false;
	bDNSQuery = false;

	CID = 0xFF;
}

WizFiClient::WizFiClient(uint8_t *ip, uint16_t port)//, WizFi2x0Class WizFi)
{
	memcpy(DstIP, ip, 4);
	DstPort = port;
	DstDomain = NULL;
	
       WizFi = &myWizFi;
	   
	bOpen = false;
	bDNSQuery = false;

	CID = 0xFF;
}

WizFiClient::WizFiClient(const char* domain, uint16_t port)//, WizFi2x0Class WizFi)
{
	memset(DstIP, 0, 4);
	DstPort = port;
	DstDomain = domain;

       WizFi = &myWizFi;
	   
	bOpen = false;
	bDNSQuery = true;

	CID = 0xFF;
}

boolean WizFiClient::available()
{
	byte IPAddr[16];
	byte tmpstr[20];
	
	if(myWizFi.NOTI_TYPE == DISCONNECT_NOTI)
	{
		if(myWizFi.NOTI_CH == CID)
		{
			myWizFi.NOTI_TYPE = NO_NOTI;
	 		myWizFi.ChangeESCState(WizFi2x0_ESC_IDLE);
			
			Serial.print("CID[");
			Serial.print((char)CID);
			Serial.println("] Disconnected!");
			
	 		bOpen = false;
			CID = 0xFF;
//			memset(DstIP, 0, 4);
//			DstPort = 0;
		}
	}else if(myWizFi.NOTI_TYPE == CONNECT_NOTI)
	{
		if(CID == 0xFF)
		{
			CID = myWizFi.NOTI_CH;
			myWizFi.NOTI_TYPE = NO_NOTI;
			myWizFi.ChangeESCState(WizFi2x0_ESC_IDLE);
			bOpen = true;
			myWizFi.GetPeerIPAddr(IPAddr);
			myWizFi.SetIP(DstIP, IPAddr);
			DstPort = myWizFi.GetPeerPortnum();
			
			Serial.print("CID[");
			Serial.print((char)CID);
			Serial.println("] Connected!");
			Serial.print("Peer IP: ");
			sprintf((char *)tmpstr, "%d.%d.%d.%d", DstIP[0], DstIP[1], DstIP[2], DstIP[3]);
			Serial.println((char *)tmpstr);
			Serial.print("Peer Portnum: ");
			sprintf((char *)tmpstr, "%u", DstPort);
			Serial.println((char *)tmpstr);
		}
	}else if(myWizFi.NOTI_TYPE == SOCKFAILURE_NOTI)
	{
		myWizFi.NOTI_TYPE = NO_NOTI;
		myWizFi.ChangeESCState(WizFi2x0_ESC_IDLE);
	}else if(myWizFi.NOTI_TYPE == INVALID_NOTI)
	{
		myWizFi.NOTI_TYPE = NO_NOTI;
		myWizFi.ChangeESCState(WizFi2x0_ESC_IDLE);
	}

	return bOpen;
	
}

uint8_t WizFiClient::connect()
{
	uint8_t retval;
	byte tmpstr[20];
	byte tmpIP[16];

	if(bDNSQuery)
	{
		WizFi->SetDomainName((byte *)DstDomain);
		WizFi->Current_Command_Code = OP_DNSLOOK;
		while(1)
		{
			retval = WizFi->SendCommand(WizFi->Current_Command_Code);
			if(retval == 1)
			{
				WizFi->GetPeerIPAddr(tmpIP);
				WizFi->SetIP(DstIP, tmpIP);
				Serial.println("DNS Query Success");
				break;
			}
			else if(retval == 2 || retval == 3)
			{
				Serial.println("DNS Query Failed");
				return 0; //DNS Query Failed
			}
		}
	}
	
	WizFi->Current_Command_Code = OP_NCTCP;
	memset(tmpstr, 0, sizeof(tmpstr));
	sprintf((char *)tmpstr, "%d.%d.%d.%d", DstIP[0], DstIP[1], DstIP[2], DstIP[3]);
	Serial.println((char *)tmpstr);
	WizFi->SetPeerIPAddr(tmpstr);
	WizFi->SetPeerPortnum(DstPort);
	while(1)
	{
		retval = WizFi->SendCommand(WizFi->Current_Command_Code);

		if(retval == 1)
		{
			bOpen = true;
			WizFi->bCommandMode = false;
			CID = WizFi->CID;
			Serial.print("CID[");
			Serial.print((char)CID);
			Serial.println("] Connected!");
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

uint8_t WizFiClient::disconnect()
{
	uint8_t retval;

	WizFi->Current_Command_Code = OP_NCLOSE;
	WizFi->CID = CID;
	
	while(1)
	{
		retval = WizFi->SendCommand(WizFi->Current_Command_Code);

		if(retval == 1)
		{
			Serial.print("CID[");
			Serial.print((char)CID);
			Serial.println("] Disconnected!");
			
	 		bOpen = false;
			CID = 0xFF;
//			memset(DstIP, 0, 4);
//			DstPort = 0;
			
			return 1;
		}else
		{			
			if(retval == 2 || retval == 3)
			{
				Serial.println("Disconnection Failed");
				bOpen = false;
				CID = 0xFF;
//				memset(DstIP, 0, 4);
//				DstPort = 0;
				
				return 0;
			}
		}
	}
}

uint8_t WizFiClient::async_connect()
{
	uint8_t retval;
/*
	if(!WizFi->IsCommandMode())
	{
		retval = WizFi->EnterCommandMode();
		if(retval != 1)
			return retval;
		else
		{
			WizFi->Current_Command_Code = OP_NCTCP;
			WizFi->SetPeerIPAddr((byte *)"192.168.123.119");
			WizFi->SetPeerPortnum(5000);
			return 4;
		}
	}
	else
	{
		if(WizFi->Current_Command_Code == OP_AT)
		{
			WizFi->Current_Command_Code = OP_NCTCP;
			WizFi->SetPeerIPAddr((byte *)"192.168.123.119");
			WizFi->SetPeerPortnum(5000);
			return 4;
		}
		retval = WizFi->SendCommand(WizFi->Current_Command_Code);

		if(retval == 1)
		{
			WizFi->Current_Command_Code = OP_AT;
			bOpen = true;
			WizFi->bCommandMode = false;
			CID = WizFi->CID;
			Serial.println((char *)WizFi->MsgBuf);
//			Serial.print("My Client CID: ");
//			Serial.println((char)CID);
			return 1;
		}else
		{
			return retval;
		}
	}
*/
	return 0;
}

uint8_t WizFiClient::async_disconnect()
{
	uint8_t retval;

/*
	if(!WizFi->IsCommandMode())
	{
		retval = WizFi->EnterCommandMode();
		if(retval != 1)
			return retval;
		else
		{
			WizFi->Current_Command_Code = OP_NCLOSEALL;
			return 4;
		}
	}
	else
	{
		retval = WizFi->SendCommand(WizFi->Current_Command_Code);

		if(retval == 1)
		{
			WizFi->Current_Command_Code = OP_AT;
			bOpen = false;
			return 1;
		}else
		{
			return retval;
		}
	}
*/
	return 0;
}


boolean WizFiClient::IsConnected()
{
	return bOpen;
}

void WizFiClient::SetIsConnected(boolean flag)
{
	bOpen = flag;
}

uint8_t WizFiClient::GetCID(void)
{
	return CID;
}

const char *WizFiClient::GetCIDstr(void)
{
	byte* strCID;

	sprintf((char *)strCID, "%d", CID);

	return (const char *)strCID;
}

void WizFiClient::write(byte value)
{
//	sprintf(tmpstr, "%cS%c%c%cE", ASCII_ESC, CID, value, ASCII_ESC);
	WizFi->write(ASCII_ESC);
	WizFi->write('S');
	WizFi->write(CID);
	WizFi->write(value);
	WizFi->write(ASCII_ESC);
	WizFi->write('E');

}

void WizFiClient::write(byte *buf)
{
	WizFi->write(ASCII_ESC);
	WizFi->write('S');
	WizFi->write(CID);
	WizFi->write(buf);
	WizFi->write(ASCII_ESC);
	WizFi->write('E');

}

void WizFiClient::write(byte *buf, size_t size)
{
	WizFi->write(ASCII_ESC);
	WizFi->write('S');
	WizFi->write(CID);
	WizFi->write(buf, size);
	WizFi->write(ASCII_ESC);
	WizFi->write('E');
}

uint8_t WizFiClient::read(void)
{
	return WizFi->read();
}

uint8_t WizFiClient::read(byte *buf)
{
	uint8_t index, i=0;
	byte DBG_Buf[10];

	index = CID - '0';


//	WizFi->RcvPacket();

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

uint8_t WizFiClient::read(byte *buf, size_t size)
{
	uint8_t index, retval, i=0;

	index = CID - '0';

	retval = 0;
	
//	WizFi->RcvPacket();
	
	if(WizFi->IsDataRcvd[index])
	{
		while(WizFi->readPtr != WizFi->writePtr)
		{
			buf[i++] = WizFi->RcvdBuf[WizFi->readPtr++];
		}
		WizFi->IsDataRcvd[index] = false;

		return i;
	}
/*	
	if(WizFi->IsDataRcvd[index])
	{
		memset(buf, 0, 33);
		memcpy(buf, WizFi->RcvdBuf[index], WizFi->RxPtr[index]);
		memset(WizFi->RcvdBuf[index], 0, 32);
		retval = WizFi->RxPtr[index];
		WizFi->RxPtr[index] = 0;
		WizFi->IsDataRcvd[index] = false;
	}
*/	
	return retval;
}

