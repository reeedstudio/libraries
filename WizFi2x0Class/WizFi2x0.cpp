#include <stdio.h>
#include <string.h>
#include "WizFi2x0.h"

/////////////////////////////////////////////////////////
// WIZnet WizFi Shield library is Copyright(c) 2012 by WIZnet, Inc.
// and is licensed under the LGPL.
// 
// Author : James YS Kim

WizFi2x0Class::WizFi2x0Class()
{
}

void WizFi2x0Class::begin(void)
{
	uint8_t i;
	
	// Set IO Pins Direction
	pinMode(WizFi2x0_DataReady, INPUT);
	pinMode(WizFi2x0_CS, OUTPUT);
	pinMode(WizFi2x0_RST, OUTPUT);

	// SPI Init
	SPI.setClockDivider(SPI_CLOCK_DIV16); // Max Clock Frequency
	SPI.begin();

	//Reset Wifi2x0
	digitalWrite(WizFi2x0_CS, HIGH);
	digitalWrite(WizFi2x0_RST, HIGH);

	digitalWrite(WizFi2x0_RST, LOW);
	delay(500);

	digitalWrite(WizFi2x0_RST, HIGH);
	delay(4000);

	ReplyCheckTimer.init();

	retryCount = 0;

	CID = 0xFF;

	Current_CmdState = WizFi2x0_CmdState_IDLE;
	Current_ESC_State = WizFi2x0_ESC_IDLE;
	Current_Command_Code = OP_AT;
	bCommandMode = true;
	bAssociated = false;

	readPtr = writePtr = 0;
	
	for(i=0; i<MAX_SOCK_NUM; i++)
	{
		IsDataRcvd[i] = false;
		SockAvailable[i] = false;
	}

	NOTI_TYPE = NO_NOTI;

	bFirstEnd = false;
	

//	Client = &myClient;

}

uint8_t WizFi2x0Class::associate(void)
{
	uint8_t retval;
	
	Current_Command_Code = OP_WA;
	Current_CmdState = WizFi2x0_CmdState_IDLE;
	
	while(1)
	{
		retval = SendCommand(Current_Command_Code);

		if(retval == 1)
		{
			bAssociated = true;
			return 1;
		}else
		{
			return retval;
		}
	}
}

uint8_t WizFi2x0Class::associate(const char *ssid, const char *passphrase, SECUTYPE EncryptType, boolean isDHCP)
{
	uint8_t retval;

	Current_Command_Code = OP_AT;
	Current_CmdState = WizFi2x0_CmdState_IDLE;
	
	while(1)
	{
		retval = SendCommand(Current_Command_Code);

		if(retval == 1)
		{
			switch(Current_Command_Code)
			{
			case OP_AT:
				Current_Command_Code = OP_ATE;
				SetSSID((byte *)ssid);
				SetKEY((byte *)passphrase);
				SetDHCPFlag((uint8_t)isDHCP);
				SetSecurityType(EncryptType);
				break;
			case OP_ATE:
				Current_Command_Code = OP_XDUM;
				break;
			case OP_XDUM:
				Current_Command_Code = OP_BDATA;
				break;
			case OP_BDATA:
				Current_Command_Code = OP_WD;
				break;
			case OP_WD:
				if(EncryptType == NO_SECURITY)
				{
					if(DHCP_Flag == 1)
						Current_Command_Code = OP_NDHCP;
					else
						Current_Command_Code = OP_NSET;
				}
				else
				{	
					if(EncryptType == WEP_SECURITY)
						Current_Command_Code = OP_WWEP;
					else if(EncryptType == WPA_SECURITY)
						Current_Command_Code = OP_WWPA;
					else if(EncryptType == WPA2PSK_SECURITY)
						Current_Command_Code = OP_WPAPSK;
				}
				break;
			case OP_WWEP:
			case OP_WWPA:
			case OP_WPAPSK:
				if(DHCP_Flag == 1)
					Current_Command_Code = OP_NDHCP;
				else
					Current_Command_Code = OP_NSET;
				break;
			case OP_NDHCP:
				Current_Command_Code = OP_WA;
				break;
			case OP_NSET:
				Current_Command_Code = OP_WA;
				break;
			case OP_WA:
				bAssociated = true;
				return 1;
			}
		}else if(retval == 2 || retval == 3)
			return 0; // association failed
	}
}

uint8_t WizFi2x0Class::disassociate(void)
{
	uint8_t retval;

	Current_Command_Code = OP_WD;
	Current_CmdState = WizFi2x0_CmdState_IDLE;
	
	while(1)
	{
		retval = SendCommand(Current_Command_Code);

		if(retval == 1)
		{
			bAssociated = false;
			return 1;
		}else if(retval == 2 || retval == 3)
		{
			return 0; //Disassociation Failed
		}
	}
	
}

boolean WizFi2x0Class::IsAssociated(void)
{
	return bAssociated;
}



uint8_t WizFi2x0Class::write(byte ch)
{
	if(ByteStuff(&ch))
	{
		digitalWrite(WizFi2x0_CS, LOW);
		SPI.transfer(spichar.SPI_ESC_CHAR);
		digitalWrite(WizFi2x0_CS, HIGH);
	}

	digitalWrite(WizFi2x0_CS, LOW);
	SPI.transfer(ch);
	digitalWrite(WizFi2x0_CS, HIGH);

//	Serial.println((char *)"WizFi2x0Class::write() called");
	return 1;
}

uint8_t WizFi2x0Class::write(byte *buf)
{
	int i = 0;

//	Serial.println((char *)buf);
	while(*(buf + i) != '\0')
	{
		write(*(buf + i));
		//delay(10);
		i++;
	}

	BufClear();
	
	return (uint8_t)i;
}

uint8_t WizFi2x0Class::write(byte *buf, size_t size)
{
	int i = 0;
	
	while(i < size)
	{
		write(*(buf + i));
		//delay(10);
		i++;
	}

	BufClear();
	
	return (uint8_t)i;
}


uint8_t WizFi2x0Class::read(void)
{
	uint8_t result;

	digitalWrite(WizFi2x0_CS, LOW);
	result = SPI.transfer(0);
	digitalWrite(WizFi2x0_CS, HIGH);

//	Serial.println(result);
	return result;
}

uint8_t WizFi2x0Class::read(byte *buf)
{
	int idx = 0;
	byte tmp;
	byte chk = 0;
	
	while(digitalRead(WizFi2x0_DataReady) == HIGH)
	{
		tmp = read();
		if((tmp != spichar.SPI_IDLE_CHAR) && (tmp != spichar.SPI_INVALID_CHAR_ALL_ZERO) && (tmp != spichar.SPI_INVALID_CHAR_ALL_ONE))
		{
			if(tmp == spichar.SPI_ESC_CHAR)
				chk = 1;
			else
			{
				if(chk == 1)
				{
					chk = 0;
					tmp ^= 0x20;
				}			
				buf[idx++] = tmp;
			}
		}
	}

	return idx;
	
}

uint8_t WizFi2x0Class::read(byte *buf, size_t size)
{
	int idx = 0;
	byte tmp;
	byte chk = 0;
	
	while(digitalRead(WizFi2x0_DataReady) == HIGH)
	{
		tmp = read();
		if((tmp != spichar.SPI_IDLE_CHAR) && (tmp != spichar.SPI_INVALID_CHAR_ALL_ZERO) && (tmp != spichar.SPI_INVALID_CHAR_ALL_ONE))
		{
			if(tmp == spichar.SPI_ESC_CHAR)
				chk = 1;
			else
			{
				if(chk == 1)
				{
					chk = 0;
					tmp ^= 0x20;
				}
				if(idx < size)
					buf[idx++] = tmp;
			}
		}
	}

	return idx;
	
}

void WizFi2x0Class::BufClear(void)
{
	memset((char *)MsgBuf, 0, sizeof(MsgBuf));
}


uint8_t WizFi2x0Class::SendCommand(uint8_t command)
{
	uint8_t retval;
	
	if(Current_CmdState == WizFi2x0_CmdState_IDLE)
	{
		memset((char *)MsgBuf, 0, sizeof(MsgBuf));
		MakeCommand(command);
		Current_CmdState = WizFi2x0_CmdState_Ready;
//		Serial.println("WizFi2x0_CmdState_Ready");
//		Serial.println((char *)MsgBuf);
		return 5;
	}else if(Current_CmdState == WizFi2x0_CmdState_Ready)
	{
		ReplyCheckTimer.TimerStart(20000);
		write(MsgBuf);
		
		RxIdx = 0;
		Current_CmdState = WizFi2x0_CmdState_Sent;
//		Serial.println("WizFi2x0_CmdState_Sent");
	}else if(Current_CmdState == WizFi2x0_CmdState_Sent)
	{
		retval = CheckReply(command);
//		Serial.print(".");
		if(retval == 1)
		{
			Current_CmdState = WizFi2x0_CmdState_Rcvd;
			ReplyCheckTimer.TimerStop();
//			Serial.println("WizFi2x0_CmdState_Rcvd");
		}else if(retval == 2)
		{
			ReplyCheckTimer.TimerStop();
			Current_CmdState = WizFi2x0_CmdState_IDLE;
//			Serial.println("Timeout  with ERROR reply");
			return 2; //Timeout  with ERROR reply
		}

		if(ReplyCheckTimer.GetIsTimeout())
		{
			ReplyCheckTimer.TimerStop();
			Current_CmdState = WizFi2x0_CmdState_IDLE;
//			Serial.println("Timeout with no reply");
			return 3; //Timeout with no reply
		}
	}else if(Current_CmdState == WizFi2x0_CmdState_Rcvd)
	{
			Current_CmdState = WizFi2x0_CmdState_IDLE;
//			Serial.println("OK reply");
			return 1; // OK reply, Setup completed
	}
	return 0;
}

void WizFi2x0Class::MakeCommand(uint8_t command)
{
	
	if(command == OP_AT)
	{
		sprintf((char *)MsgBuf, "AT\r\n");
	}else if(command == OP_ATE)
	{
		sprintf((char *)MsgBuf, "ATE0\r\n");
	}else if(command == OP_WS)
	{
		sprintf((char *)MsgBuf, "AT+WS=%s\r\n", SSID);
	}else if(command == OP_WA)
	{
		sprintf((char *)MsgBuf, "AT+WA=%s\r\n", SSID);
	}else if(command == OP_XDUM)
	{
		sprintf((char *)MsgBuf, "AT+XDUM=0\r\n");
	}else if(command == OP_WD)
	{
		sprintf((char *)MsgBuf, "AT+WD\r\n");
	}else if(command == OP_DNSLOOK)
	{
		sprintf((char *)MsgBuf, "AT+DNSLOOKUP=%s,3,5\r\n", DomainName);
	}else if(command == OP_WAUTO)
	{
		sprintf((char *)MsgBuf, "AT+WAUTO=0,%s\r\n", SSID);
	}else if(command == OP_NAUTO)
	{
		sprintf((char *)MsgBuf, "AT+NAUTO=%d,%d,%s,%d\r\n", Connection_Type, Socket_Type, PeerIPAddr, PeerPortNum);
	}else if(command == OP_NDHCP)
	{
		sprintf((char *)MsgBuf, "AT+NDHCP=%d\r\n", DHCP_Flag);
	}else if(command == OP_WWEP)
	{
		sprintf((char *)MsgBuf, "AT+WWEP1=%s\r\n", Key);
	}else if(command == OP_WWPA)
	{
		sprintf((char *)MsgBuf, "AT+WWPA=%s\r\n", Key);
	}else if(command == OP_WPAPSK)
	{
		sprintf((char *)MsgBuf, "AT+WPAPSK=%s,%s\r\n", SSID, Key);
	}else if(command == OP_BDATA)
	{
		sprintf((char *)MsgBuf, "AT+BDATA=1\r\n");
	}else if(command == OP_ATA)
	{
		sprintf((char *)MsgBuf, "ATA\r");
	}else if(command == OP_NSET)
	{
		sprintf((char *)MsgBuf, "AT+NSET=%s,%s,%s\r\n", SrcIPAddr, SrcSubnet, SrcGateway);
	}else if(command == OP_ATCID)
	{
		sprintf((char *)MsgBuf, "AT+CID=?\r\n");
	}else if(command == OP_ATO)
	{
		sprintf((char *)MsgBuf, "AT\r");
	}else if(command == OP_NCTCP)
	{
		sprintf((char *)MsgBuf, "AT+NCTCP=%s,%u\r\n", PeerIPAddr, PeerPortNum);
	}else if(command == OP_NSTCP)
	{
		sprintf((char *)MsgBuf, "AT+NSTCP=%u\r\n", SrcPortNum);
	}else if(command == OP_NCLOSE)
	{
		sprintf((char *)MsgBuf, "AT+NCLOSE=%c\r\n", CID);
	}else if(command == OP_NCLOSEALL)
	{
		sprintf((char *)MsgBuf, "AT+NCLOSEALL\r\n");
	}else if(command == OP_CMDMODE)
	{
		sprintf((char *)MsgBuf, "+++");
	}
}

void WizFi2x0Class::ChangeESCState(uint8_t state)
{
	Current_ESC_State = state;
}

uint8_t WizFi2x0Class::RcvPacket(void)
{
	byte tmp;
	byte DBG_Buf[10];
	
/*	
		memset(DBG_Buf, 0, 10);
		sprintf((char *)DBG_Buf, "%02X ", tmp);
		Serial.print((char *)DBG_Buf);
*/		
	switch(Current_ESC_State)
	{
	case WizFi2x0_ESC_IDLE:
		tmp = read();
		if((tmp != spichar.SPI_IDLE_CHAR) && (tmp != spichar.SPI_INVALID_CHAR_ALL_ZERO) && (tmp != spichar.SPI_INVALID_CHAR_ALL_ONE))
		{
			if(tmp == ASCII_ESC)
			{
				Current_ESC_State = WizFi2x0_ESC_STARTED;
//				Serial.println("<ESC>");
			}else if(tmp == '[')
			{
				Current_ESC_State = WizFi2x0_ESC_NOTIFY;
				memset(NotifyBuf, 0, 40);
				NotifyIdx = 0;
//				Serial.println("NOTIFY Message was started");
			}
		}
		break;
	case WizFi2x0_ESC_STARTED:
		tmp = read();
		if((tmp != spichar.SPI_IDLE_CHAR) && (tmp != spichar.SPI_INVALID_CHAR_ALL_ZERO) && (tmp != spichar.SPI_INVALID_CHAR_ALL_ONE))
		{
			if(tmp == 'Z')
			{
				Current_ESC_State = WizFi2x0_ESC_SOCK;
//				Serial.println("Z");
			}else
			{
				Current_ESC_State = WizFi2x0_ESC_IDLE;
			}
		}
		break;
	case WizFi2x0_ESC_SOCK:
		tmp = read();
		if((tmp != spichar.SPI_IDLE_CHAR) && (tmp != spichar.SPI_INVALID_CHAR_ALL_ZERO) && (tmp != spichar.SPI_INVALID_CHAR_ALL_ONE))
		{
			if(((tmp >= '0') && (tmp <= '9')) || ((tmp >= 'a') && (tmp <= 'f')))
			{
				Current_ESC_State = WizFi2x0_ESC_CID;
				CurrentSockIndex = tmp - '0';
				memset(RcvdBuf, 0, 128);
				ESC_Length_Idx = 0;
				ESC_Data_Length = 0;
//				Serial.println((char)tmp);
			}else
			{
				Current_ESC_State = WizFi2x0_ESC_IDLE;
			}
		}
		break;
	case WizFi2x0_ESC_CID:
		tmp = read();
		if((tmp != spichar.SPI_IDLE_CHAR) && (tmp != spichar.SPI_INVALID_CHAR_ALL_ZERO) && (tmp != spichar.SPI_INVALID_CHAR_ALL_ONE))
		{
			if((tmp >= '0') && (tmp <= '9'))
			{
				ESC_Data_Length *= 10;
				ESC_Data_Length += (uint16_t)(tmp - '0');
				
				if(++ESC_Length_Idx == 4)
				{
					Current_ESC_State = WizFi2x0_ESC_LENGTH;
					ESC_Data_Idx = 0;
					Current_ESC_Data_Length = 0;
	//				Serial.print("Data Length: ");
	//				sprintf((char *)DBG_Buf, "%d ", ESC_Data_Length);
	//				Serial.println((char *)DBG_Buf);
					
				}
//				Serial.println((char)tmp);
			}else
			{
				Current_ESC_State = WizFi2x0_ESC_IDLE;
			}
		}
		break;
	case WizFi2x0_ESC_LENGTH:
		tmp = read();
		if((tmp != spichar.SPI_IDLE_CHAR) && (tmp != spichar.SPI_INVALID_CHAR_ALL_ZERO) && (tmp != spichar.SPI_INVALID_CHAR_ALL_ONE))
		{
			RcvdBuf[writePtr++] = tmp;
			Current_ESC_Data_Length++;
//			IsDataRcvd[CurrentSockIndex] = true;
			if(writePtr == 128)
				writePtr = 0;
			
//			Serial.print((char)tmp);
			ESC_Data_Idx++;

			if(ESC_Data_Length > 128)
			{
				if(Current_ESC_Data_Length == 128)
					IsDataRcvd[CurrentSockIndex] = true;
			}
			
			if(ESC_Data_Idx == ESC_Data_Length)
			{
//				Serial.println("All Data Received");
				
//				while(digitalRead(WizFi2x0_DataReady) == HIGH)
//				{
//					tmp = read();
//				}
				if((Current_ESC_Data_Length > 0) && (IsDataRcvd[CurrentSockIndex] == false))
					IsDataRcvd[CurrentSockIndex] = true;
				
				Current_ESC_State = WizFi2x0_ESC_IDLE;
			}
		}
		break;
	case WizFi2x0_ESC_NOTIFY:
		tmp = read();
		if((tmp != spichar.SPI_IDLE_CHAR) && (tmp != spichar.SPI_INVALID_CHAR_ALL_ZERO) && (tmp != spichar.SPI_INVALID_CHAR_ALL_ONE))
		{
			if(tmp != ']')
				NotifyBuf[NotifyIdx++] = tmp;
			else
			{
				NotifyBuf[NotifyIdx++] = tmp;
				RxIdx = NotifyIdx;
//				Serial.println("Enter ParseNotify: ");
//				Serial.println((char *)NotifyBuf);
				ParseNotify(NotifyBuf);
				Current_ESC_State = WizFi2x0_ESC_IDLE;
//				Current_ESC_State = WizFi2x0_ESC_NOTI_OK;
//				Serial.println("NOTIFY Message was ended");

			}
		}
		break;
	case WizFi2x0_ESC_NOTI_OK:
		return 0;
	}

	return 0; // Reply is in progress
}

uint8_t WizFi2x0Class::ParseNotify(byte *buf)
{
	int retval;
	long retval32;
	uint8_t Token[33];
	
	Current_Ptr = 0;
	retval = GetToken(buf, Token);
//	Serial.println((char *)Token);
	if(!strcmp((char const*)Token, "DISCONNECT"))
	{
		retval = GetToken(buf, Token);
//		Serial.println((char *)Token);
		if(retval == -1)
		{
			return -1;
		}else
		{
			if(Token[0] >= '0' && Token[0] <= '9')						
				NOTI_CH = Token[0];
			else if(Token[0] >= 'a' && Token[0] <= 'f')
				NOTI_CH = Token[0];
			NOTI_TYPE = DISCONNECT_NOTI;
			
			return 1;
		}
	}else if(!strcmp((char const*)Token, "CONNECT"))
	{
		retval = GetToken(buf, Token);
		if(retval == -1)
			return -1;
//		Serial.println((char *)Token);
		retval = GetToken(buf, Token);
		if(retval == -1)
			return -1;
		if(Token[0] >= '0' && Token[0] <= '9')
		{
			NOTI_CH = Token[0];
			if(Token[1] == ']')
				return 1;
		}else if(Token[0] >= 'a' && Token[0] <= 'f')
		{
			NOTI_CH = Token[0];
			if(Token[1] == ']')
				return 1;
		}else
			return -1;

//		Serial.println((char *)Token);
		retval = GetToken(buf, Token);
		if(retval == -1)
			return -1;
		SetPeerIPAddr((byte *)Token);
//		Serial.println((char *)Token);
		retval = GetToken(buf, Token);
		if(retval == -1)
			return -1;
//		Serial.println((char *)Token);
		retval32 = StrToInt(Token);
		if(retval32 == -1)
			return -1;
		SetPeerPortnum(retval32);
//		Serial.println((char *)Token);

		NOTI_TYPE = CONNECT_NOTI;

		return 1;
		
	}else if(!strcmp((char const*)Token, "ERROR"))
	{
		NOTI_TYPE = SOCKFAILURE_NOTI;
		
		return 1;
	}else
	{
		NOTI_TYPE = INVALID_NOTI;
//		Current_ESC_State = WizFi2x0_ESC_IDLE;

		return 1;
	}
}

uint8_t WizFi2x0Class::CheckReply(uint8_t command)
{
	byte tmp;

	tmp = read();
	if((tmp != spichar.SPI_IDLE_CHAR) && (tmp != spichar.SPI_INVALID_CHAR_ALL_ZERO) && (tmp != spichar.SPI_INVALID_CHAR_ALL_ONE))
	{
		MsgBuf[RxIdx++] = tmp;
	}

	if(tmp == ']')
	{
		if(command == OP_NSTCP)
		{
			if(bFirstEnd == false)
			{
				bFirstEnd = true;
				return 0;
			}else
				bFirstEnd = false;
		}else
			bFirstEnd = false;
		
		/////////////////////////////////////////	
		//Flush rx data
		while(digitalRead(WizFi2x0_DataReady) == HIGH)
		{
			tmp = read();
		}
		//
		////////////////////////////////////////
//		Serial.println((char *)MsgBuf);
		if(ParseReply(MsgBuf, command))
			return 1; // Reply is OK
		else
			return 2; // Reply is FAILED
	}

	return 0; // Reply is in progress
	
}

uint8_t WizFi2x0Class::ParseReply(byte * buf, uint8_t command)
{
	int retval;
	uint8_t Token[33];
	uint8_t tmpToken[33];
	uint8_t APListIndex;
	
	switch(command)
	{
	case OP_AT: 
	case OP_ATE:
	case OP_WD:
	case OP_WWEP:
	case OP_WWPA:
	case OP_WPAPSK:
	case OP_NDHCP:
	case OP_WAUTO:
	case OP_NAUTO:
	case OP_NSET:
	case OP_ATO:
	case OP_NCLOSE:
	case OP_NCLOSEALL:
	case OP_BDATA:
		Current_Ptr = 0;
		retval = GetToken(buf, Token);
//		Serial.println((char *)Token);

		while(1)
		{
			if(!strcmp((char const *)Token, "[OK]"))
				return 1;
			else
			{
				retval = GetToken(buf, Token);
//				Serial.println((char *)Token);
//				printf("\r\n Token Length: %d, Token : %s", retval, Token);
				if(!strcmp((char const *)Token, "[OK]"))
					return 1;
				if(retval == -1)
					return 0;
			}
		}
	case OP_DNSLOOK:
		Current_Ptr = 0;
		retval = GetToken(buf, Token);
//		Serial.println((char *)Token);

		while(1)
		{
			if(!strcmp((char const *)Token, "IP"))
			{
				retval = GetToken(buf, Token);
//				Serial.println((char *)Token);
				if(retval > 0)
					SetPeerIPAddr(Token);
				else if(retval == -1)
					return 0;

				while(1)
				{
					retval = GetToken(buf, Token);
					if(!strcmp((char const *)Token, "[OK]"))
						return 1;
					if(retval == -1)
						return 0;
				}
			}
			else
			{
				retval = GetToken(buf, Token);
//				Serial.println((char *)Token);
				if(retval == -1)
					return 0;
			}
		}
	case OP_XDUM:
		Current_Ptr = 0;
		retval = GetToken(buf, Token);
//		Serial.println((char *)Token);

		while(1)
		{
			if(!strcmp((char const *)Token, "[OK]"))
				return 1;
			else
			{
				retval = GetToken(buf, Token);
//				Serial.println((char *)Token);
				if(retval == -1)
					return 0;
			}
		}
		
	case OP_NCTCP:
		Current_Ptr = 0;
		retval = GetToken(buf, Token);

		while(1)
		{
//			if(retval != 0)
//				Serial.println((char *)Token);
			if(!strcmp((char const*)Token, "[CONNECT"))
			{
				retval = GetToken(buf, Token);
				if(retval!=0)
				{
//					Serial.println((char *)Token);
					if(Token[0] >= '0' && Token[0] <= '9')						
						CID = Token[0];
					else if(Token[0] >= 'a' && Token[0] <= 'f')
						CID = Token[0];

					return 1;
				}
				
			}else
			{
				retval = GetToken(buf, Token);
				if(retval == -1)
					return 0;
			}
		}
	case OP_NSTCP:
		Current_Ptr = 0;
		retval = GetToken(buf, Token);
//		Serial.println("NSTCP");
//		Serial.println((char *)Token);

		while(1)
		{			
			if(!strcmp((char const*)Token, "[CONNECT"))
			{
				retval = GetToken(buf, Token);
				if(retval<=0)
				{
					return 0;
				}
//				Serial.println((char *)Token);
				if(Token[0] >= '0' && Token[0] <= '9')						
					CID = Token[0];
				else if(Token[0] >= 'a' && Token[0] <= 'f')
					CID = Token[0];

				while(1)
				{
					retval = GetToken(buf, Token);
//					Serial.println((char *)Token);
					if(!strcmp((char const*)Token, "[OK]"))
						return 1;
					if(retval == -1)
						return 0;
				}
			}else
			{
				retval = GetToken(buf, Token);
				if(retval == -1)
					return 0;
			}
		}
	case OP_WA:
		Current_Ptr = 0;
		retval = GetToken(buf, Token);
		
		while(1)
		{
//			if(retval != 0)
//				Serial.println((char *)Token);
			if(!strcmp((char const *)Token, "Gateway"))
			{
				while(GetToken(buf, Token) == 0);
//				Serial.print("SrcIPAddr: ");
//				Serial.println((char *)Token);
				SetSrcIPAddr(Token);
				while(GetToken(buf, Token) == 0);
//				Serial.print("SrcSubnet: ");
//				Serial.println((char *)Token);
				SetSrcSubnet(Token);
				while(GetToken(buf, Token) == 0);
//				Serial.print("SrcGateway: ");
//				Serial.println((char *)Token);
				SetSrcGateway(Token);
//				Serial.println((char *)Token);

				while(1)
				{
					retval = GetToken(buf, Token);

					if(retval == -1)
						return 0;
					
					if(!strcmp((char const *)Token, "[OK]"))
						return 1;
				}
			}else
			{
				retval = GetToken(buf, Token);
				if(retval == -1)
					return 0;
			}
		}
	case OP_WS:

		Current_Ptr = 0;
		retval = GetToken(buf, Token);

		return 1;
#if 0
		while(1)
		{
			if(!strcmp((char const *)Token, "BSSIDSSIDChannelTypeRSSISecurity"))
			{
				retval = GetToken(Token);
//				printf("\r\n Token Length: %d, Token : %s", retval, Token);
				memset(tmpToken, 0, 33);
				memcpy(tmpToken, Token, 13);
				APListIndex = WIZFI210_GetAPListCount();
				while(strcmp((char const *)tmpToken, "No.OfAPFound:")!=0)
				{
//					printf("\r\nBSSI: %s", Token);
					WIZFI210_SetBSSID(APListIndex, Token);
					
					retval = GetToken(Token);
//					printf("\tSSID: %s", Token);
					WIZFI210_SetSSID(APListIndex, Token);
					
					retval = GetToken(Token);
//					printf("\t\tChannel: %s", Token);
					WIZFI210_SetChannel(APListIndex, Token);
					
					retval = GetToken(Token);
//					printf("\tType: %s", Token);
					WIZFI210_SetType(APListIndex, Token);

					retval = GetToken(Token);
//					printf("\tRSSI: %s", Token);
					WIZFI210_SetRSSI(APListIndex, Token);

					retval = GetToken(Token);
//					printf("\t\tSecurity: %s", Token);
					WIZFI210_SetSecurity(APListIndex, Token);

					APListIndex++;
					WIZFI210_SetAPListCount(APListIndex);
					
					retval = GetToken(Token);
					memset(tmpToken, 0, 33);
					memcpy(tmpToken, Token, 13);
				}
				retval = GetToken(Token);
//				printf("\r\n %s", Token);
				if(!strcmp((char const *)Token, "[OK]"))
					return 1;
				else
					return 0;
			}else
			{
				retval = GetToken(Token);
				if(retval == -1)
					return 0;
			}
		}
#endif
	case OP_ATA:
		Current_Ptr = 0;
		retval = GetToken(buf, Token);
//		printf("\r\n Token Length: %d, Token : %s", retval, Token);

		while(1)
		{
			if(!strcmp((char const *)Token, "[OK]"))
			{
				return 1;
			}
			else
			{
				memset(tmpToken, 0, 33);
				memcpy(tmpToken, Token, 7);
				if(!strcmp((char const *)tmpToken, "[ERROR]"))
					return 0;
				retval = GetToken(buf, Token);
//				printf("\r\n Token Length: %d, Token : %s", retval, Token);
				if(!strcmp((char const *)Token, "[OK]"))
				{
					return 1;
				}
				if(retval == -1)
					return 0;
			}
		}
	case OP_ATCID:
		Current_Ptr = 0;
		retval = GetToken(buf, Token);

		while(1)
		{
			if(!strcmp((char const *)Token, "NovalidCids"))
			{				
				return 1;
			}
			else if(!strcmp((char const *)Token, "CIDTYPEMODELOCALPORTREMOTEPORTREMOTEIP"))
			{
				return 1;
			}
			retval = GetToken(buf, Token);
//			printf("\r\n Token Length: %d, Token : %s", retval, Token);			
			if(retval == -1)
				return 0;
		}
	}
	return 0;
}

int WizFi2x0Class::GetToken(byte * buf, uint8_t * Token)
{
	uint8_t i = 0;
	memset(Token, 0, 33);

	while(Current_Ptr <= RxIdx)
	{
//		Serial.print(i);
//		Serial.print(MsgBuf[Current_Ptr]);
		
		if(buf[Current_Ptr] != ',' && buf[Current_Ptr] != 0x0D && buf[Current_Ptr] != 0x0A && buf[Current_Ptr] != '\0' 
			&& buf[Current_Ptr] != ':' && buf[Current_Ptr] != ' ' && buf[Current_Ptr] != '\t')
		{
			Token[i++] = buf[Current_Ptr++];
		}
		else
		{
			if((buf[Current_Ptr] == ',') ||(buf[Current_Ptr] == '\0')||(buf[Current_Ptr] == ':')
				||(buf[Current_Ptr] == ' ') ||(buf[Current_Ptr] == '\t'))
			{
				Current_Ptr++;
				if(i > 0)
					return i;
			}else if(buf[Current_Ptr] == 0x0D)
			{
				if(buf[Current_Ptr + 1] == 0x0A)
				{
					Current_Ptr += 2;
					if(i > 0)
						return i;
				}else
				{
					Current_Ptr++;
					if(i > 0)
						return i;
				}
			}else if(buf[Current_Ptr] == 0x0A)
			{
				if(buf[Current_Ptr + 1] == 0x0D)
				{
					Current_Ptr += 2;
					if(i > 0)
						return i;
				}else
				{
					Current_Ptr++;
					if(i > 0)
						return i;
				}
			}
		}
	}

	return -1;
}

uint8_t WizFi2x0Class::CheckSyncReply(void)
{
	byte key;

	key = read();
	if((key != spichar.SPI_IDLE_CHAR) && ( key != spichar.SPI_INVALID_CHAR_ALL_ZERO) && (key != spichar.SPI_INVALID_CHAR_ALL_ONE))
	{
		;
	}

	if(digitalRead(WizFi2x0_DataReady) == LOW)
		return 1;

	delay(10); // wait during 10ms
	
	return 0;
}

void WizFi2x0Class::SendSync(void)
{
	write(spichar.SPI_IDLE_CHAR);	
}

boolean WizFi2x0Class::ByteStuff(byte *ch)
{
	if((*ch == spichar.SPI_ESC_CHAR) ||
		(*ch == spichar.SPI_XON_CHAR) ||
		(*ch == spichar.SPI_XOFF_CHAR) ||
		(*ch == spichar.SPI_IDLE_CHAR) ||
		(*ch == spichar.SPI_INVALID_CHAR_ALL_ONE) ||
		(*ch == spichar.SPI_INVALID_CHAR_ALL_ZERO) ||
		(*ch == spichar.SPI_LINK_READY) )
	{
		*ch = *ch^0x20;
		return true;
	}else
		return false;
}

void WizFi2x0Class::StrToStr(byte * dst, byte * src, size_t size)
{
	memset((char *)dst, 0, sizeof(size));
	sprintf((char *)dst, (char *)src);
}

void WizFi2x0Class::SetSSID(byte *buf)
{
//	memset((char *)SSID, 0, sizeof(SSID));
//	sprintf((char *)SSID, (char *)buf);
	StrToStr(SSID, buf, sizeof(SSID));
}

void WizFi2x0Class::GetSSID(byte * buf)
{
//	memset((char *)buf, 0, sizeof(SSID));
//	sprintf((char *)buf, (char *)SSID);
	StrToStr(buf, SSID, sizeof(SSID));
}

void WizFi2x0Class::SetKEY(byte *buf)
{
//	memset((char *)Key, 0, sizeof(Key));
//	sprintf((char *)Key, (char *)buf);
	StrToStr(Key, buf, sizeof(Key));
}

void WizFi2x0Class::GetKEY(byte *buf)
{
//	memset((char *)buf, 0, sizeof(Key));
//	sprintf((char *)buf, (char *)Key);
	StrToStr(buf, Key, sizeof(Key));
}

void WizFi2x0Class::SetSrcIPAddr(byte *buf)
{
//	memset((char *)SrcIPAddr, 0, sizeof(SrcIPAddr));
//	sprintf((char *)SrcIPAddr, (char *)buf);
	StrToStr(SrcIPAddr, buf, sizeof(SrcIPAddr));
}

void WizFi2x0Class::GetSrcIPAddr(byte *buf)
{
//	memset((char *)buf, 0, sizeof(SrcIPAddr));
//	sprintf((char *)buf, (char *)SrcIPAddr);
	StrToStr(buf, SrcIPAddr, sizeof(SrcIPAddr));
}

void WizFi2x0Class::SetSrcSubnet(byte *buf)
{
//	memset((char *)SrcSubnet, 0, sizeof(SrcSubnet));
//	sprintf((char *)SrcSubnet, (char *)buf);
	StrToStr(SrcSubnet, buf, sizeof(SrcSubnet));
}

void WizFi2x0Class::GetSrcSubnet(byte *buf)
{
//	memset((char *)buf, 0, sizeof(SrcSubnet));
//    sprintf((char *)buf, (char *)SrcSubnet);
	StrToStr(buf, SrcSubnet, sizeof(SrcSubnet));
}

void WizFi2x0Class::SetSrcGateway(byte *buf)
{
//	memset((char *)SrcGateway, 0, sizeof(SrcGateway));
//	sprintf((char *)SrcGateway, (char *)buf);
	StrToStr(SrcGateway, buf, sizeof(SrcGateway));
}

void WizFi2x0Class::GetSrcGateway(byte * buf)
{
//	memset((char *)buf, 0, sizeof(SrcGateway));
//	sprintf((char *)buf, (char *)SrcGateway);
	StrToStr(buf, SrcGateway, sizeof(SrcGateway));
}

void WizFi2x0Class::SetDomainName(byte *buf)
{
//	memset((char *)DomainName, 0, sizeof(DomainName));
//	sprintf((char *)DomainName, (char *)buf);
	StrToStr(DomainName, buf, sizeof(DomainName));
}

void WizFi2x0Class::GetDomainName(byte *buf)
{
//	memset((char *)buf, 0, sizeof(DomainName));
//	sprintf((char *)buf, (char *)DomainName);
	StrToStr(buf, DomainName, sizeof(DomainName));
}

void WizFi2x0Class::SetSrcPortnum(unsigned int portnum)
{
	uint8_t tmpstr[20];
	
	SrcPortNum = portnum;

	sprintf((char *)tmpstr, "SPort: %u", SrcPortNum);	
	Serial.println("SerSrcPortnum");
	Serial.println((char *)tmpstr);
}

uint16_t WizFi2x0Class::GetSrcPortnum(void)
{
	return SrcPortNum;
}

void WizFi2x0Class::SetPeerIPAddr(byte *buf)
{
	memset((char *)PeerIPAddr, 0, sizeof(PeerIPAddr));
	sprintf((char *)PeerIPAddr, (char *)buf);
}

void WizFi2x0Class::GetPeerIPAddr(byte *buf)
{
	memset((char *)buf, 0, sizeof(PeerIPAddr));
	sprintf((char *)buf, (char *)PeerIPAddr);
}

void WizFi2x0Class::SetPeerPortnum(uint16_t portnum)
{
	PeerPortNum = portnum;
}

uint16_t WizFi2x0Class::GetPeerPortnum(void)
{
	return PeerPortNum;
}

void WizFi2x0Class::SetDHCPFlag(uint8_t flag)
{
	DHCP_Flag = flag;
}

uint8_t WizFi2x0Class::GetDHCPFlag(void)
{
	return DHCP_Flag;
}

void WizFi2x0Class::SetConnectionType(uint8_t flag)
{
	Connection_Type = flag;
}

uint8_t WizFi2x0Class::GetConnectionType(void)
{
	return Connection_Type;
}

void WizFi2x0Class::SetSocketType(uint8_t flag)
{
	Socket_Type = flag;
}

uint8_t WizFi2x0Class::GetSocetType(void)
{
	return Socket_Type;
}

void WizFi2x0Class::SetSecurityType(uint8_t type)
{
	SecurityType = type;
}

uint8_t WizFi2x0Class::GetSecurityType(void)
{
	return SecurityType;
}

int WizFi2x0Class::SetIP(uint8_t* IP, byte * buf)
{
	uint8_t i = 0, index=0, pos=0;
	byte tmpBuf[5];
	int retval;
	
	memset(tmpBuf, 0, 5);
//	Serial.print("GetPeerIPAddr() retval: ");
//	Serial.println((char *)buf);

	while(1)
	{
//		Serial.print(i);
//		Serial.print(MsgBuf[Current_Ptr]);
//		Serial.print(buf[index]);
//		Serial.print("\t\t\t");
//		Serial.println((char *)tmpBuf);
		if(buf[index] != '.' && buf[index] != '\0' )
		{
			tmpBuf[i++] = buf[index++];
		}
		else
		{
			if(buf[index] == '.' )
			{
				index++;
				tmpBuf[i] = '\0';
//				Serial.println((char *)tmpBuf);
				retval = StrToByte(tmpBuf);
				memset(tmpBuf, 0, 5);
				i = 0;
				if(retval == -1)
					return retval;
				IP[pos++] = (uint8_t)retval;
			}else if(buf[index] == '\0' )
			{
				tmpBuf[i] = '\0';
				retval = StrToByte(tmpBuf);
				if(retval == -1)
					return retval;
				IP[pos] = (uint8_t)retval;
				return 1;
			}
		}
	}

	return -1;	
}
int WizFi2x0Class::StrToByte(byte *buf)
{
	uint8_t i = 0;
	int retval = 0;

	while(1)
	{
		if(i > 3)
			return -1;
		if(buf[i] >= '0' && buf[i] <= '9')
		{
			retval *= 10;
			retval += buf[i] - '0';
		}else if(buf[i] == '\0')
		{
			if(retval >= 0 && retval <= 255)
				return retval;
			else
				return -1;
		}else
			return -1;
		i++;
	}
}

long WizFi2x0Class::StrToInt(byte *buf)
{
	uint8_t i = 0;
	byte tmpstr[20];
	
	long retval = 0;

	while(1)
	{
//		sprintf((char *)tmpstr, "retval: %u", retval);
//		Serial.println((char *)tmpstr);
		
		if(i > 5)
			return -1;
		if(buf[i] >= '0' && buf[i] <= '9')
		{
			retval *= 10;
			retval += buf[i] - '0';
		}else if(buf[i] == '\0' || buf[i] == ']')
		{
			if(retval >= 0 && retval <= 65535)
				return retval;
			else
				return -1;
		}else
			return -1;
		i++;
	}
}

void TimeoutClass::init(void)
{
	TimerValue = 0;
	IsTimeout = false;
	TimerCount = 0;
	IsTimerStarted = false;
}


void TimeoutClass::TimerStart(uint16_t timevalue)
{
	TimerValue = timevalue;
	TimerStart();
}

void TimeoutClass::TimerStart(void)
{
	IsTimeout = false;
	TimerCount = 0;
	IsTimerStarted = true;
}

void TimeoutClass::TimerStop(void)
{
	IsTimerStarted = false;
}

boolean TimeoutClass::GetIsTimeout(void)
{
	return IsTimeout;
}

void TimeoutClass::SetIsTimeout(boolean flag)
{
	IsTimeout = flag;
}

void TimeoutClass::CheckIsTimeout(void)
{
	if(IsTimerStarted)
	{
		if(TimerCount++ >= TimerValue)
		{
			IsTimeout = true;
			TimerStop();
		}
	}
}


