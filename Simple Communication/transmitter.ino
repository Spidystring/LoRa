#include <SPI.h>
#include <RH_RF95.h>
#define RFM95_CS 10
#define RFM95_RST 7
#define RFM95_INT 2
#define node_id "Your Node ID" 
// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 915.0


// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);

unsigned long currentMillis = 0; // stores the value of millis() in each iteration of loop()
unsigned long previousPacket = 0; // will store last time the next packet was sent

const int nextpacket = 5000; // number of millisecs to send next packet


void setup()
{
    pinMode(RFM95_RST, OUTPUT);
    digitalWrite(RFM95_RST, HIGH);
    delay(1000);

    Serial.begin(9600);
    delay(100);

    Serial.println("Arduino LoRa TX Test!");

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

 Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);
 Serial.println("\n\n\t\tSending to rf95_server"); // Send a message to rf95_server

 // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on
 // The default transmitter power is 13dBm, using PA_BOOST.
 // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then
 // you can set transmitter powers from 5 to 23 dBm:

 rf95.setTxPower(23, false);
}

int16_t packetnum = 0; // packet counter, we increment per xmission

void loop()
{
    currentMillis = millis(); // capture the latest value of millis()
                                // this is equivalent to noting the time from a clock

    mansend();
    contsend();
    delay(10);
}

void mansend(){
 if (Serial.available() > 0) {
    // read the incoming byte:
    String input = Serial.readString();
    rf95.send((uint8_t*)(input.c_str()), input.length()+1);
    }
}


void contsend() {

    if (currentMillis - previousPacket >= nextpacket) {
    String radiopacket = "Packet #";
    radiopacket += String(packetnum++);
    radiopacket += " from ";
    radiopacket += node_id;
    Serial.println("\n\n---------------------------------------------------------------------------");
    Serial.print("\n\n\t\tThe message: ");
    Serial.println(radiopacket); //delay(10);
    rf95.send((uint8_t*)(radiopacket.c_str()), radiopacket.length()+1);

    //rf95.waitPacketSent();
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    //Serial.println("Waiting for reply...");

    if (rf95.waitAvailableTimeout(1000))
    {
        // Should be a reply message for us now
        if (rf95.recv(buf, &len))
        {
            Serial.print("\t\tReply: ");
            Serial.println((char*)buf);
            Serial.print("\t\tRSSI: ");
            Serial.println(rf95.lastRssi(), DEC);
            Serial.
            println("\n\n---------------------------------------------------------------------------");
        }

        else
            {
            Serial.println("Receive failed");
            }
    }

 else
    {
    Serial.println("No reply, is there a listener around?");
    }
    previousPacket += nextpacket;
  }
}