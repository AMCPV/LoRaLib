/*
   LoRaLib Receive with Inerrupts Example

   This example listens for LoRa transmissions and tries to
   receive them. Once a packet is received, an interrupt is
   triggered. To successfully receive data, the following
   settings have to be the same on both transmitter 
   and receiver:
    - carrier frequency
    - bandwidth
    - spreading factor
    - coding rate
    - sync word

   For more detailed information, see the LoRaLib Wiki
   https://github.com/jgromes/LoRaLib/wiki
*/

// include the library
#include <LoRaLib.h>

// create instance of LoRa class using SX1278 module
// this pinout corresponds to LoRenz shield: 
// https://github.com/jgromes/LoRenz
// NSS pin:   7
// DIO0 pin:  2
// DIO1 pin:  3
// IMPORTANT: because this example uses external 
//            interrupts, DIO0 MUST be connected
//            to Arduino pin 2 or 3. DIO1 can be
//            connected to any free pin.
SX1278 lora = new LoRa;

void setup() {
  Serial.begin(9600);

  // initialize SX1278 with default settings
  Serial.print(F("Initializing ... "));
  // carrier frequency:                   434.0 MHz
  // bandwidth:                           125.0 kHz
  // spreading factor:                    9
  // coding rate:                         7
  // sync word:                           0x12
  // output power:                        17 dBm
  int state = lora.begin();
  if (state == ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true);
  }

  // set the function that will be called 
  // when new packet is received
  lora.onReceive(handlePacket);
  
  // start listening for LoRa packets
  Serial.print(F("Starting to listen ... "));
  state = lora.listen();
  if (state == ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true);
  }

  // the 'listen' mode can be disabled by calling:
  // lora.standby()
  // lora.sleep()
  // lora.transmit();
  // lora.receive();
}

// this function is called when a complete packet
// is received by the module
// IMPORTANT: this function MUST be 'void' type
//            and MUST NOT have any arguments!
void handlePacket(void) {
  // you can receive data as an Arduino String
  String str;
  int state = lora.readData(str);

  // you can also receive data as byte array
  /*
    byte byteArr[8];
    int state = lora.receive(byteArr, 8);
  */
  
  if (state == ERR_NONE) {
    // packet was successfully received
    Serial.println("Received packet!");

    // print data of the packet
    Serial.print("Data:\t\t");
    Serial.println(str);

    // print RSSI (Received Signal Strength Indicator) 
    // of the last received packet
    Serial.print("RSSI:\t\t");
    Serial.print(lora.lastPacketRSSI);
    Serial.println(" dBm");

    // print SNR (Signal-to-Noise Ratio) 
    // of the last received packet
    Serial.print("SNR:\t\t");
    Serial.print(lora.lastPacketSNR);
    Serial.println(" dBm");

  } else if (state == ERR_CRC_MISMATCH) {
    // packet was received, but is malformed
    Serial.println("CRC error!");

  }
}

void loop() {
  // nothing here

}