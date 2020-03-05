#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

WiFiClient espClient;

const char* ssid = "University of Washington"; // Wifi network SSID
const char* password = ""; // Wifi network password

//String ip = "172.28.219.179"; // Sieg Master IP
//String api_token = "rARKEpLebwXuW01cNVvQbnDEkd2bd56Nj-hpTETB"; // Sieg Master API Token


String ip = "172.28.219.177"; // Red IP
String api_token = "Lht3HgITYDN-96UYm5mkJ4CEjKj20d3siYidcSq-"; // Red API Token

//Sieg lower floor
int lowerLights[] = {10, 23, 11, 15, 7, 14, 22, 21, 16};
int upperLights[] = {18, 20, 12, 25, 26, 5, 8, 19, 13, 24, 9, 17};
int redLights[] = {1};

typedef struct {
  String col;
  String pot;
} randomValues;

randomValues rv;

//BUTTON SETTINGS
const int buttonPin = 16;
int buttonState = 0;

unsigned long currentTime, lastPattern, timerOne, timerTwo; //timing

String readString;
boolean x;

typedef struct {
  String x;
  String y;
} xy;

xy color;

void setup() {
  Serial.begin(115200);
  pinMode(buttonPin, INPUT);
  setup_wifi();
  color.x = .6400;
  color.y = .3300;
  x = true;
}

void loop() {
  if (x == true) {
    setLights()
  }

  while (Serial.available()) {
    char c = Serial.read();  //gets one byte from serial buffer
    readString += c; //makes the string readString
    delay(2);  //slow looping to allow buffer to fill with next character
  }

  if (readString.length() > 0) {
    if (x == true) {
      color.x = readString;
      Serial.println("x: " + readString); //so you can see the captured string
    } else {
      color.y = readString;
      Serial.println("y: " + readString + "\n"); //so you can see the captured string
    }
    x = !x;
    readString = "";
  }
}

void setLights() {
  changeLight(redLights[0], 0, "on", "true", "bri", "254", "xy", "[.6400,.3300]", "sat", "254");
  //  for(int i = 0; i < 9; i++) {
  //        changeLight(lowerLights[i], 0, "on", "true", "bri", "254", "hue", rv.col, "sat", "254");
  //  }
}

void buttonCheck() {
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) {
    Serial.println("ON");
    setLights();
    timerOne = currentTime;
  } else {
    Serial.println("OFF");
  }
}

void setup_wifi() {
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println(".");
  }
  Serial.println("Connected to the WiFi network");
}
