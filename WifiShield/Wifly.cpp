#include "Wifly.h"

void WiflyClass::init() {
	
	SoftwareSerial::begin(9600);

	unavailConn = 0;
}

void WiflyClass::waitForReady(boolean dhcp) {
	_dhcp = dhcp;
	while(!find("Associated!"));
	if(_dhcp) {
				  
		while(!find("DHCP in"));
		
	} else {
		while(!find("Using Static IP"));
		
	}
	find("Listen on 2000");
}

void WiflyClass::setConfig(const char *ssid, const char *passphrase) {
	sendCommand("$$$", "CMD");//enter the setting model
	sendCommand("set w join 0\r","AOK");//set it not to join automatic
	sendCommand("set comm remote 0\r", "AOK");//set the message(*OPEN*) when connected the remote host/client
	sendCommand("set w linkmon 5\r", "AOK");//set it connect the route per 5 sec
	sendCommand("set wlan auth 4\r","AOK");//set the wifly's insecure model
	sendCommand("set comm idle 0\r", "AOK");//set it when is connected will not disconnect
	sendCommand("set w ssid ","");//set the name of the wifi you want to join in
	sendCommand(ssid, "", true);
	sendCommand("\r", "AOK", true);
	if(passphrase != NULL) {
		sendCommand("set w phrase ","");//set the phrase of the wifi
		sendCommand(passphrase, "", true);
		sendCommand("\r", "AOK", true);
	}
	sendCommand("set ip dhcp 1\r","AOK");//open the DHCP,so it will get the ip automatic
	sendCommand("save\r", "Storing in config");//save it
	sendCommand("exit\r", "EXIT");//exit the setting model
	reboot();
}

void WiflyClass::setConfig(const char *ssid, const char *passphrase, const char *ip, const char *mask, const char *gateway) {
	sendCommand("$$$", "CMD");
	sendCommand("set w join 1\r", "AOK");
	sendCommand("set comm remote 0\r", "AOK");
	sendCommand("set w linkmon 5\r", "AOK");
	sendCommand("set comm idle 0\r", "AOK");
	sendCommand("set wlan auth 4\r","AOK");//set the wifly's insecure model
	sendCommand("set dns address 192.168.1.2\r", "AOK");
	sendCommand("set w ssid ","");
	sendCommand(ssid, "", true);
	sendCommand("\r", "AOK", true);
	if(passphrase != NULL) {
		sendCommand("set w phrase ","");
		sendCommand(passphrase, "", true);
		sendCommand("\r", "AOK", true);
	}
	sendCommand("set ip dhcp 0\r","AOK");
	sendCommand("set ip address ", "");
	sendCommand(ip, "", true);
	sendCommand("\r", "AOK", true);
	sendCommand("set ip gateway ", "");
	sendCommand(gateway, "", true);
	sendCommand("\r", "AOK", true);
	sendCommand("set ip netmask ", "");
	sendCommand(mask, "", true);
	sendCommand("\r", "AOK", true);
	sendCommand("save\r", "Storing in config");
	sendCommand("exit\r", "EXIT");
	reboot();
}

void WiflyClass::checkAssociated() {
	while(!find("Associated!"));
	while(!find("DHCP in"));
	find("Listen on 2000");

}

void WiflyClass::reboot() {
	sendCommand("$$$", "CMD");
	sendCommand("reboot\r","*Reboot*");
	while(!find("*READY*"));
}

void WiflyClass::join(const char *ssid)
{
	sendCommand("$$$", "CMD");
	sendCommand("join ","" );
	sendCommand(ssid,"");
	sendCommand("\r","");
}

void WiflyClass::reset() {
	digitalWrite(WIFLY_RST, LOW);
	delay(1);
	digitalWrite(WIFLY_RST, HIGH);
	//while(!find("*READY*"));
}

bool WiflyClass::checkConnection() {
	unsigned long _start = millis();
	while((_start + 1000) > millis()) {
		int i = digitalRead(WIFLY_GPIO6);
		if(i == 1) {
			unavailConn++;
			if(unavailConn == 10) {
				reboot();
				waitForReady(_dhcp);
				unavailConn = 0;
			}
			return false;
		}
	}
	return true;
}

bool WiflyClass::connect(const char *host, const char *port) {
	sendCommand("$$$", "CMD");
	sendCommand("open ", "");
	sendCommand(host, "", true);
	sendCommand(" ", "", true);
	sendCommand(port, "", true);
	boolean ret = sendCommand("\r", "*OPEN*", true);
	if(!ret) {
		sendCommand("exit\r", "EXIT");
		return false;
	}
	return true;
}

void WiflyClass::closeAndExit() {
	sendCommand("close\r","");
	sendCommand("exit\r", "EXIT");
}

void WiflyClass::writeToSocket(const char *data) {
	sendCommand(data, "");
}

void WiflyClass::print(char data) {
	SoftwareSerial::print(data);
	Serial.print(data);
}

char WiflyClass::readFromSocket() {
	return Read();
}

bool WiflyClass::canReadFromSocket() {
	return (SoftwareSerial::available() > 0 ? true:false);
	//return (WiflySerial::available() > 0 ? true:false);
}

void WiflyClass::factoryReset() {
	sendCommand("$$$", "CMD");
	sendCommand("factory RESET\r", "");
	sendCommand("exit\r", "EXIT");
	reboot();	
}

bool WiflyClass::sendCommand(const char *cmd, char *expectedReturn, boolean multipart) {
	SoftwareSerial::print(cmd);
#if 1// debug
	Serial.print(cmd);
#endif
	//WiflySerial::print(cmd);
	return find(expectedReturn); 
}

char WiflyClass::Read() {
	long start = millis();
	char c;
	int temp;
	while(millis() < (start + READ_TIMEOUT)) {
		
		if (SoftwareSerial::available() > 0) {
			char c = SoftwareSerial::read();
			Serial.print(c);
			return c;
		}
	} 
	return NULL;
}

bool WiflyClass::find(char *target) {
    return findUntil(target, NULL);
}

// as above but search ends if the terminate string is found
bool WiflyClass::findUntil(char *target, char *terminate)
{
    unsigned char targetLen = strlen(target); 
    unsigned char index = 0;  // maximum target string length is 255 unsigned chars
    unsigned char termIndex = 0;
    unsigned char termLen = (terminate == NULL ? 0 : strlen(terminate));
    char c;
	
    if( *target == 0)
		return true;   // return true if target is a null string 
    while( (c = Read()) != 0){
		if( c == target[index]){        
			if(++index >= targetLen){ // return true if all chars in the target match
				return true;    
			}
		}
		else{        
			index = 0;  // reset index if any char does not match   
		}
		if(termLen > 0 && c == terminate[termIndex]){
			if(++termIndex >= termLen)  
				return false;       // return false if terminate string found before target string
		}
		else
			termIndex = 0;
    }
    return false;
}

void WiflyClass::skipChar(int count) {
	int i=0;
	for(i=0;i<count;i++)
		read();
}

void WiflyClass::flush() {
	SoftwareSerial::flush();
	//WiflySerial::flush();
}

WiflyClass::WiflyClass(int rx,int tx):SoftwareSerial(rx,tx)
{
	//int a = 5;
	//PenClass pen;
	//pen = new PenClass(blue);
	//SoftwareSerial WiflySerial = new SoftwareSerial(tx,rx)
}
