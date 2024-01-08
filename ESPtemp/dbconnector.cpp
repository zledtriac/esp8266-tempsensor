#include <ESP8266HTTPClient.h>
#include "dbconnector.hpp"
#include "config.hpp"

unsigned char sendToDB(String value) {
  WiFiClient client;
  HTTPClient http;

  http.begin(client, "http://" DBIP ":" DBPORT "/api/v2/write?bucket=" BUCKET "&org=" ORGID);
  http.addHeader("Authorization", "Token " TOKEN);
  http.addHeader("Content-Type", "text/plain");
  http.addHeader("Accept", "application/json");

  int httpCode = http.POST(MEASUREMENT ",unit=" MUNIT " tempC=" + value);

  http.end();

  if(httpCode > 0 && httpCode == HTTP_CODE_NO_CONTENT) return 1;
  
  return 0;     //We only care if the request was succesful or not.
}