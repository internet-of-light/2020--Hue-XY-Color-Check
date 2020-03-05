/*
    Tab 2 - Hue API Funcion headers and bodies
*/

//Push parameters to group
void changeGroup(byte groupNum, byte transitiontime, String parameter, String newValue, String parameter2 = "",
                 String newValue2 = "", String parameter3 = "", String newValue3 = "",
                 String parameter4 = "", String newValue4 = "");


//Push parameters to individual light
void changeLight(byte lightNum, byte transitiontime, String parameter, String newValue, String parameter2 = "",
                 String newValue2 = "", String parameter3 = "", String newValue3 = "",
                 String parameter4 = "", String newValue4 = "");


/*  checkLightStatus
    returns true or false (1 or 0)
    checks if light is on or off
    parameter: # of light
*/
bool checkLightStatus(byte lightNum);


/*  toggleLight
    Simply toggle the on/off status of a light
    transitiontime must be specified
    parameter: # of light
*/
void toggleLight(byte lightNum, byte transitiontime) {
  bool newStatus = !checkLightStatus(lightNum);
  changeLight(lightNum, transitiontime, "on", newStatus ? "true" : "false");
}





void changeGroup(byte groupNum, byte transitiontime, String parameter, String newValue, String parameter2,
                 String newValue2, String parameter3, String newValue3,
                 String parameter4, String newValue4) {

  String req_string = "http://" + ip + "/api/" + api_token + "/groups/" + groupNum + "/action";
  HTTPClient http;
  http.begin(req_string);

  String put_string = "{\"" + parameter + "\":" + newValue + ", \"transitiontime\": " +
                      transitiontime;
  if (!parameter2.equals("")) put_string += + ", \"" + parameter2 + "\": " + newValue2;
  if (!parameter3.equals("")) put_string += ", \"" + parameter3 + "\" : " + newValue3;
  if (!parameter4.equals("")) put_string += ", \"" + parameter4 + "\" : " + newValue4;
  put_string +=  + "}";

  Serial.println("Attempting PUT: " + put_string + " for GROUP: " + String(groupNum));

  int httpResponseCode = http.PUT(put_string);
  if (httpResponseCode == 200) {
    String response = http.getString();
    Serial.println("Response: " + response);
  } else {
    Serial.println("Error on sending PUT Request: ");
    Serial.println(String(httpResponseCode));
  }
  http.end();
}

void changeLight(byte lightNum, byte transitiontime, String parameter, String newValue, String parameter2,
                 String newValue2, String parameter3, String newValue3,
                 String parameter4, String newValue4) {

  String req_string = "http://" + ip + "/api/" + api_token + "/lights/" + lightNum + "/state";
  HTTPClient http;
  http.begin(req_string);

  String put_string = "{\"" + parameter + "\":" + newValue + ", \"transitiontime\":" + transitiontime;
  if (!parameter2.equals("")) put_string += + ", \"" + parameter2 + "\": " + newValue2;
  if (!parameter3.equals("")) put_string += ", \"" + parameter3 + "\" : " + newValue3;
  if (!parameter4.equals("")) put_string += ", \"" + parameter4 + "\" : " + newValue4;
  put_string +=  + "}";

  Serial.println("Attempting PUT: " + put_string + " for LIGHT: " + String(lightNum));


  int httpResponseCode = http.PUT(put_string);
  if (httpResponseCode == 200) {
    String response = http.getString();
    Serial.println("Response: " + response);
  } else {
    Serial.println("Error on sending PUT Request: ");
    Serial.println(String(httpResponseCode));
  }
  http.end();
}

bool checkLightStatus(byte lightNum) {
  Serial.println("Checking status of light " + String(lightNum));
  String req_string = "http://" + ip + "/api/" + api_token + "/lights/" + lightNum;
  HTTPClient http;
  http.begin(req_string);

  //char* get_string = "{\"on\": true}";
  int httpResponseCode = http.GET();

  if (httpResponseCode == 200) {
    DynamicJsonBuffer jsonBuffer;
    String payload = http.getString();
    http.end();
    JsonObject& root = jsonBuffer.parse(payload);
    //Serial.println("Payload: " + payload);
    String lightStatus = (const char*)root["state"]["on"];
    Serial.println("ON Status of light " + String(lightNum) + ": " + lightStatus);
    return lightStatus.equals("true");
  } else {
    http.end();
    Serial.println("Error on sending GET Request: ");
    Serial.println(String(httpResponseCode));
    return false;
  }
}
