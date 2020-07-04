/*
 Name:		RelayDriverAndRFReceiver.ino
 Created:	6/29/2020 8:03:35 PM
 Author:	Ewan Drever-Smith
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


#define CE_PIN   7
#define CSN_PIN 8

const byte thisSlaveAddress[5] = { 'R','x','A','A','A' };

RF24 radio(CE_PIN, CSN_PIN);

char dataReceived[10];
bool newData = false;


void setup() 
{
    pinMode(3,OUTPUT);
    Serial.begin(9600);
    Serial.println("Starting...");
    radio.begin();
    radio.setDataRate(RF24_250KBPS);
    radio.openReadingPipe(1, thisSlaveAddress);
    radio.startListening();
}

void loop() 
{
    getData();
    updateRelayState();
}


void getData() 
{
    if (radio.available()) 
    {
        radio.read(&dataReceived, sizeof(dataReceived));
        newData = true;
    }
}

void updateRelayState() 
{
    if (newData == true) 
    {
      Serial.println("Data received");
      Serial.println(dataReceived[8]);
        if(dataReceived[8] == '1')
        {
          digitalWrite(3,HIGH);
        }
        else if(dataReceived[8] == '2')
        {
          digitalWrite(3,LOW);
        }
        newData = false;
    }
}
