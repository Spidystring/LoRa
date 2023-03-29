#include <SPI.h>
#include <stdio.h>
#include <RH_RF95.h>
#define RFM95_CS 10
#define RFM95_RST 7
#define RFM95_INT 2
#define node_id "Your Node ID" //change this for the other set

#define RF95_FREQ 900.0 // Change to 434.0 or other frequency, must match RX's freq!

// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);

void setup()
{
    pinMode(RFM95_RST, OUTPUT);
    digitalWrite(RFM95_RST, HIGH);
    delay(1000);
    Serial.begin(9600);
    delay(100);
    Serial.println("Arduino LoRa SET Test!");
    // manual reset
    digitalWrite(RFM95_RST, LOW);
    delay(10);
    digitalWrite(RFM95_RST, HIGH);
    delay(10);
       
    while (!rf95.init()) {
    Serial.println("LoRa radio init failed");
    while (1);
    }

    Serial.println("LoRa radio init OK!");
    // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM

    if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
    }

 Serial.print("Set Freq to: ");
 Serial.println(RF95_FREQ);

 // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on
 // The default transmitter power is 13dBm, using PA_BOOST.
 // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then
 // you can set transmitter powers from 5 to 23 dBm:
 rf95.setTxPower(23, false);
 }

int16_t packetnum = 0; // packet counter, we increment per xmission

void loop()
{
    contrec();
    mansend();
}

void contrec(){
 if (rf95.available())
 {
    // Should be a message for us now
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    if (rf95.recv(buf, &len))
    {
      String buff = (char*)buf;
      Serial.write("\n\n---------------------------------------------------------------------------");
      Serial.write("\n\n\t\tReceived: ");
      Serial.write((char*)buf);
      Serial.write("\t\tRSSI: ");
      Serial.print(rf95.lastRssi());
      Serial.write("\n\n---------------------------------------------------------------------------");
    }

    else
    {
        Serial.println("Receive failed");
    }
  }
}


void mansend(){
 if (Serial.available() > 0){
    // read the incoming byte:
    String input = Serial.readString();
    input += "\t\tfrom: ";
    input += node_id;
    rf95.send((uint8_t*)(input.c_str()), input.length()+1);
    }
}