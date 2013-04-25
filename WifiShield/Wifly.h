#ifndef WIFLY_H
#define WIFLY_H

#include <Arduino.h>
#include "Hardware.h"
#include "Debug.h"
#include <SoftwareSerial.h>



#define NB_TRY_BEFORE_REBOOT 10
#define READ_TIMEOUT 10000
//
class WiflyClass: public SoftwareSerial {
//class WiflyClass {
public:
	WiflyClass(int,int);
	void init();
	
//WiflyClass:WiflySerial(int,int);
//		{
//			WiflySerial = new SoftwareSerial(int,int);
//		}
	//SoftwareSerial WiflySerial = new SoftwareSerial(tx,rx);
	//SoftwareSerial::SoftwareSerial WiflySerial = new SoftwareSerial::SoftwareSerial(tx,rx);
	void join(const char *ssid);
	void waitForReady(boolean dhcp);
	void setConfig(const char *ssid, const char *passphrase);
	void setConfig(const char *ssid, const char *passphrase, const char *ip, const char *mask ,const char *gateway);
	void reset();
	void closeAndExit();
	void reboot();
	bool checkConnection();
	void checkAssociated();
    bool connect(const char *host, const char *port);
    void writeToSocket(const char *data);
    char readFromSocket();
	bool canReadFromSocket();
	bool sendCommand(const char *cmd, char *expectedReturn, boolean multipart=false);
	void factoryReset();
	char Read();
	void flush();
	bool find(char *target);
	bool findUntil(char *target, char *terminate);
	void skipChar(int count);
	void print(char data);
private:
	unsigned char disconCount;
	bool _dhcp;
	uint8_t unavailConn;
	//SoftwareSerial WiflySerial;
};

//extern WiflyClass Wifly;
//extern SoftwareSerial WiflySerial(int,int);
#endif // WIFLY_H
