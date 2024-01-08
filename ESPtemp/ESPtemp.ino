#include <OneWire.h>
#include <ESP8266WiFi.h>
#include "dbconnector.hpp"
#include "dstemp.hpp"
#include "config.hpp"

OneWire ds(2);                //initialize one-wire protocol on GPIO2.
unsigned char tempRom[8];     //storage for the one-wire roms and data.
unsigned char tempData[12];

void setup(void) {
  Serial.begin(9600);         //initialize serail interface for debugging.
  Serial.println("\nRESET");

  WiFi.begin(STASSID, STAPSK);  //connect to the Wifi network.
}

void loop(void) {
  unsigned char i;
  float tempval;
  unsigned char failCnt = 0;

  //check if the module is connected to the network, if it doesn't change for 20 retries,
  //we concluded that the network is not available
  while(WiFi.status() != WL_CONNECTED) {
    failCnt++;
    delay(1000);
    if(failCnt > 20) {
      Serial.println("Not connected to WiFi.");
      break;
    }
  }

  //Searching for devices.We only need one, just check if there is any available device.
  if(!ds.search(tempRom)) Serial.println("No devices.");
  Serial.print("ROM: ");

  for(i = 0; i < 8; i++) {
    Serial.print(tempRom[i], HEX);
  }

  Serial.println();

  //Check the CRC.
  if(OneWire::crc8(tempRom, 7) != tempRom[7]) {
    Serial.println("Wrong CRC!");
  }
  else {
    //if the detected device is a DS18B20 then read the temperature
    if(tempRom[0] == 0x28) {
      tempval = getTemp(ds, tempRom);
      Serial.print("Temp: ");
      Serial.println(tempval);
      
      //if the temperature is a reasonable value and the network is available
      if(tempval < 400 && failCnt <= 20) {
        if(sendToDB(String(tempval, 3))) {
          Serial.println("Value sent.");
        }
      }
      Serial.println();
    }
    else {
      Serial.println("Not DS18B20 sensor.");
    }
  }

  ds.reset_search();
  delay(300000);      //wait for 5 minutes.
}
