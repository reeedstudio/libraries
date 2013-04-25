#ifndef __WIZFICLIENT_H
#define __WIZFICLIENT_H

#include <WizFi2x0.h>

extern WizFi2x0Class myWizFi;

class WizFiClient{
public:
	WizFiClient(void);
	WizFiClient(uint8_t *ip, uint16_t port);//, WizFi2x0Class WizFi);
	WizFiClient(const char* domain, uint16_t port);//, WizFi2x0Class WizFi);
	
	uint8_t connect();
	uint8_t disconnect();
	
	uint8_t async_connect();
	uint8_t async_disconnect();
	
	boolean IsConnected();
	
	void write(byte value);
	void write(byte *buf);
	void write(byte *buf, size_t size);
	
	uint8_t read(void);
	uint8_t read(byte *buf);
	uint8_t read(byte* buf, size_t size);
	
	const char *GetCIDstr(void);
	uint8_t GetCID(void);
	
	void SetIsConnected(boolean flag);
	
	boolean available();
	
	
private:
	uint8_t *SrcIP;
	uint16_t SrcPort;

	const char *DstDomain;
	uint8_t DstIP[4];
	uint16_t DstPort;

	uint8_t CID;

	boolean bOpen;
	boolean bDNSQuery;

	WizFi2x0Class* WizFi;
//	friend class WizFi2x0Class; 
};

#endif
