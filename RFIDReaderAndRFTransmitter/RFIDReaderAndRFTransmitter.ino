/*
 Name:		RFIDReaderAndRFTransmitter.ino
 Created:	6/29/2020 8:03:35 PM
 Author:	Ewan Drever-Smith
*/


#include <SPI.h>
#include <MFRC522.h>
#include <nRF24L01.h>
#include <RF24.h>

//RF24 Pins
#define CE_PIN  7
#define CSN_PIN 8

//RC522 Pins
#define RST_PIN 9
#define SS_PIN  10        

MFRC522 reader(SS_PIN, RST_PIN);  // Create MFRC522 instance
RF24 radio(CE_PIN, CSN_PIN);      // Create RF24 instance

const byte slaveAddress[5] = { 'R','x','A','A','A' };
char dataToSend[10] = "Message 0";
enum Status {entering = '1', exiting = '2'} status;

// the setup function runs once when you press reset or power the board
void setup() 
{
    Serial.begin(9600);		// Initialize serial communications with the PC

    SPI.begin();			// Init SPI bus

    reader.PCD_Init();		// Init MFRC522

    //reader.PCD_DumpVersionToSerial();	// Show details of PCD - MFRC522 Card Reader details
    radio.begin();
    radio.setDataRate(RF24_250KBPS);
    radio.setRetries(3, 5); // delay, count
    radio.openWritingPipe(slaveAddress);

    status = entering;
}

// the loop function runs over and over again until power down or reset
void loop() 
{
    // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
    if (!reader.PICC_IsNewCardPresent())
        return;

    // Select one of the cards
    if (!reader.PICC_ReadCardSerial())
        return;

    if (reader.uid.uidByte[0] == 0x3d && reader.uid.uidByte[1] == 0x4a && reader.uid.uidByte[2] == 0x64 && reader.uid.uidByte[3] == 0x9d) //Ewan's Uni card
    {
        if (status == entering)
            status = exiting;
        else
            status = entering;
        Serial.write(status);
        Serial.println();
        send();
    }
    delay(500);
}

void send() 
{
    dataToSend[8] = status;
    bool result;
    result = radio.write(&dataToSend, sizeof(dataToSend));

    Serial.print("Data Sent ");
    Serial.print(dataToSend);
    if (result)
        Serial.println("  Acknowledge received");
    else 
        Serial.println("  Tx failed"); //implement error light
}
