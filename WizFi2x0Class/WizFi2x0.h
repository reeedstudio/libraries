#ifndef _WIZFI2x0_H_
#define _WIZFI2x0_H_

#include <SPI.h>

//#include <WizFiClient.h>

//extern WizFiClient myClient;

#define WizFi2x0_RST			2
#define WizFi2x0_DataReady	3
#define WizFi2x0_CS			4

#define WizFi2x0_CmdState_IDLE	0x01
#define WizFi2x0_CmdState_Sent	0x02
#define WizFi2x0_CmdState_Rcvd	0x03
#define WizFi2x0_CmdState_Ready	0x04

#define ASCII_ESC	0x1B

#define WizFi2x0_ESC_IDLE		0x01
#define WizFi2x0_ESC_STARTED	0x02
#define WizFi2x0_ESC_SOCK		0x03
#define WizFi2x0_ESC_CID		0x04
#define WizFi2x0_ESC_END_State1	0x05
#define WizFi2x0_ESC_END_State2	0x06
#define WizFi2x0_ESC_LENGTH	0x07
#define WizFi2x0_ESC_NOTIFY		0x08
#define WizFi2x0_ESC_NOTI_OK	0x09

#define MAX_SOCK_NUM			16

enum CMDOP {
	OP_AT = 1, 
	OP_ATE = 2, 
	OP_WS = 3, 
	OP_WD = 4, 
	OP_WAUTO = 5, 
	OP_NAUTO = 6, 
	OP_NDHCP = 7, 
	OP_WWEP = 8, 
	OP_WWPA = 9, 
	OP_ATA = 10, 
	OP_NSET = 11, 
	OP_ATCID = 12, 
	OP_ATO = 13, 
	OP_NCTCP = 14, 
	OP_NCLOSE = 15, 
	OP_NCLOSEALL = 16, 
	OP_CMDMODE = 17,
	OP_WA = 18,
	OP_XDUM = 19,
	OP_BDATA = 20,
	OP_DNSLOOK = 21,
	OP_NSTCP = 22,
	OP_WPAPSK = 23
};

enum CONNTYPE{
	CLIENT_MODE = 0,
	SERVER_MODE = 1
};

enum SOCKTYPE{
	PROTO_UDP = 0,
	PROTO_TCP = 1
};

enum DHCPFLAG{
	DHCP_OFF = 0,
	DHCP_ON = 1
};

enum SECUTYPE{
	NO_SECURITY = 0,
	WEP_SECURITY = 1,
	WPA_SECURITY = 2,
	WPA2PSK_SECURITY = 3
};

enum NOTITYPE{
	NO_NOTI = 0,
	DISCONNECT_NOTI = 1,
	DISASSOCIATE_NOTI = 2,
	CONNECT_NOTI = 3,
	ASSOCIATE_NOTI = 4,
	SOCKFAILURE_NOTI = 5,
	INVALID_NOTI = 6
};

class SPIChar
{
public:
	static const byte SPI_ESC_CHAR = 0xFB;
	static const byte SPI_IDLE_CHAR = 0xF5;
	static const byte SPI_XOFF_CHAR = 0xFA;
	static const byte SPI_XON_CHAR = 0xFD;
	static const byte SPI_INVALID_CHAR_ALL_ONE = 0xFF;
	static const byte SPI_INVALID_CHAR_ALL_ZERO = 0x00;
	static const byte SPI_LINK_READY = 0xF3;
};

class TimeoutClass
{
private:
	boolean IsTimerStarted;
	uint16_t TimerCount;
	uint16_t TimerValue;
	boolean IsTimeout;
public:
	void init(void);
	virtual void TimerStart(uint16_t timevalue);
	virtual void TimerStart(void);
	void TimerStop(void);
	boolean GetIsTimeout(void);	
	void CheckIsTimeout();
	void SetIsTimeout(boolean flag);
};

class WizFi2x0Class
{
public:
	byte MsgBuf[128];
	uint16_t RxIdx;
	uint8_t SendByte;
	uint8_t retryCount;
	byte RcvdBuf[128];
	boolean IsDataRcvd[MAX_SOCK_NUM];
	boolean SockAvailable[MAX_SOCK_NUM];
	uint8_t readPtr;
	uint8_t writePtr;
	uint8_t CurrentSockIndex;
	uint8_t RcvdData;
	
	
	SPIChar spichar;
	TimeoutClass ReplyCheckTimer;
	uint8_t Current_CmdState;
	uint8_t Current_Command_Code;
	uint16_t Current_Ptr;
	uint8_t Current_ESC_State;

	uint8_t SSID[33]; //= "";
	uint8_t Key[33]; //= "";
	uint8_t Connection_Type;// = 0;
	uint8_t DHCP_Flag;// = 1;
	uint8_t Socket_Type; // = 0;
	uint8_t SecurityType;
	uint8_t SrcIPAddr[16];// = "192.168.1.100";
	uint8_t SrcSubnet[16];// = "255.255.255.0";
	uint8_t SrcGateway[16];// = "192.168.1.254";
	unsigned int SrcPortNum;

	uint8_t PeerIPAddr[16];// = "192.168.1.144";
	uint16_t PeerPortNum;// = 5000;

	uint8_t DomainName[32];

	uint8_t NOTI_TYPE;
	uint8_t NOTI_CH;

private:
	boolean bAssociated;
	boolean bCommandMode;

	uint8_t CID;

	uint8_t ESC_Length_Idx;
	uint16_t ESC_Data_Idx;
	uint16_t ESC_Data_Length;
	uint16_t Current_ESC_Data_Length;

	uint8_t NotifyBuf[40];
	uint8_t NotifyIdx;

	boolean bFirstEnd;

	
public:
	uint8_t SendCommand(uint8_t command);
	void MakeCommand(uint8_t command);
	uint8_t CheckReply(uint8_t command);
	uint8_t ParseReply(byte *buf, uint8_t command);

	int StrToByte(byte *buf);
	long StrToInt(byte *buf);
	void StrToStr(byte * dst, byte * src, size_t size);

	virtual uint8_t write(byte ch);
	virtual uint8_t write(byte *buf);
	virtual uint8_t write(byte *buf, size_t size);
	virtual uint8_t read();
	virtual uint8_t read(byte *buf);
	virtual uint8_t read(byte *buf, size_t size);
	boolean ByteStuff(byte *str);
	int GetToken(byte * buf, uint8_t * Token);
	void BufClear(void);


	uint8_t ParseNotify(byte *buf);


	void SetSSID(byte *buf);
	void GetSSID(byte *buf);
	void SetKEY(byte *buf);
	void GetKEY(byte *buf);
	void SetPeerIPAddr(byte *buf);
	void GetPeerIPAddr(byte *buf);
	void SetPeerPortnum(uint16_t portnum);
	uint16_t GetPeerPortnum(void);
	void SetDHCPFlag(uint8_t flag);
	uint8_t GetDHCPFlag(void);
	void SetConnectionType(uint8_t flag);
	uint8_t GetConnectionType(void);
	void SetSocketType(uint8_t flag);
	uint8_t GetSocetType(void);
	void SetSecurityType(uint8_t type);
	uint8_t GetSecurityType(void);
	void SetDomainName(byte *buf);
	void GetDomainName(byte *buf);

	int SetIP(uint8_t* IP, byte * buf);
	
public:
	WizFi2x0Class();
	void begin(void);
	uint8_t associate(void);
	uint8_t associate(const char *ssid, const char *passphrase, SECUTYPE EncryptType, boolean isDHCP);
	uint8_t disassociate(void);
	boolean IsAssociated(void);
	uint8_t CheckSyncReply(void);
	void SendSync(void);
	
	void SetSrcIPAddr(byte *buf);
	void GetSrcIPAddr(byte *buf);
	void SetSrcSubnet(byte *buf);
	void GetSrcSubnet(byte *buf);
	void SetSrcGateway(byte *buf);
	void GetSrcGateway(byte *buf);
	void SetSrcPortnum(uint16_t portnum);
	uint16_t GetSrcPortnum(void);

	void ChangeESCState(uint8_t state);
	uint8_t RcvPacket(void);

//	WizFiClient* Client;
	friend class WizFiClient;
	friend class WizFiServer;
};

#endif
