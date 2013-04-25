#include <Arduino.h>
#include <SoftwareSerial.h>
#include <WiFly.h>
#include "HTTPClient.h"

#define SSID      "SEEED-MKT"
#define KEY       "seeed0728"
#define AUTH      WIFLY_AUTH_WPA2_PSK

#define HTTP_POST_URL_LIGHT "http://api.yeelink.net/v1.0/device/2227/sensor/2903/datapoints"
#define HTTP_POST_URL_TEMP  "http://api.yeelink.net/v1.0/device/2227/sensor/2930/datapoints"
#define HTTP_POST_HEADERS "U-ApiKey: 61edb2da67b94b575acd53b83e72c852\r\n"

// Pins' connection
// Arduino       WiFly
//  2    <---->    TX
//  3    <---->    RX
WiFly wifly(2, 3);
HTTPClient http;
char get;

int getTemp()
{

    int sum = 0;
    for(int i = 0; i<32; i++)
    {
        sum += analogRead(0);
    }
    sum = sum>>5;
    float resistance=(float)(1023-sum)*10000/sum;           //get the resistance of the sensor;
    return 1/(log(resistance/10000)/3975+1/298.15)-273.15;  //convert to temperature via datasheet ;

}

void setup() {
    Serial.begin(9600);
    Serial.println("------- WIFLY HTTP --------");

    // Wait WiFly to init
    // delay(3000)

    wifly.reset();

    while (1) {
        Serial.println("Try to join " SSID );
        if (wifly.join(SSID, KEY, AUTH)) {
            Serial.println("Succeed to join " SSID);
            wifly.clear();
            break;
        } else {
            Serial.println("Failed to join " SSID);
            Serial.println("Wait 1 second and try again...");
            delay(1000);
        }
    }
}

unsigned long start_millis = 0;
char post_data_buf[32];
unsigned long cnt = 0;

void loop() {

    if ((millis() - start_millis) < 11000) {
        return;
    } else {
        start_millis = millis();
        cnt++;
    }

    if(cnt %2 == 0)    // send light value
    {
        Serial.println("\r\n\r\nTry to post data to url - " HTTP_POST_URL_LIGHT);
        Serial.println("-------------------------------");

        snprintf(post_data_buf, sizeof(post_data_buf), "{\"value\": %d}\r\n", analogRead(A5));
        while (http.post(HTTP_POST_URL_LIGHT, HTTP_POST_HEADERS, post_data_buf, 10000) < 0) {
        }

        char r;
        while (wifly.receive((uint8_t *)&r, 1, 1000) == 1) {
            Serial.print(r);
        }
    }
    else                // send temperature
    {
        Serial.println("\r\n\r\nTry to post data to url - " HTTP_POST_URL_LIGHT);
        Serial.println("-------------------------------");

        snprintf(post_data_buf, sizeof(post_data_buf), "{\"value\": %d}\r\n", getTemp());
        while (http.post(HTTP_POST_URL_TEMP, HTTP_POST_HEADERS, post_data_buf, 10000) < 0) {
        }

        char r;
        while (wifly.receive((uint8_t *)&r, 1, 1000) == 1) {
            Serial.print(r);
        }
    }
}
