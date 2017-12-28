#include <Ultrasonic.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "ESP8266Ifttt.h"

const char* ssid = "o.d_guest";
const char* password = "Osak1_f@rm";

int count = 0;
boolean hasSentIfttt = false;

Ultrasonic ultrasonic(14);
WiFiServer server(80);
WiFiClient client;

void setup() {
    Serial.begin(115200);

    // Connect to WiFi network
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");

    // Start the server
    server.begin();
    Serial.println("Server started");

    // Print the IP address
    Serial.println(WiFi.localIP());
}

void loop() {
    long RangeInCentimeters;
    RangeInCentimeters = ultrasonic.MeasureInCentimeters();
    Serial.print("distance: ");
    Serial.print(RangeInCentimeters);
    Serial.println(" cm");

    if (RangeInCentimeters > 17) {
        count++;
        Serial.print("***** Increment count: current count is ");
        Serial.print(count);
        Serial.println(" *****");
        checkCount();
    } else {
        Serial.println("***** Reset count *****");
        count = 0;
    }

    delay(1000);
}

void checkCount() {
    if (count > 10) {
        checkSentStatus();
    }
}

void checkSentStatus() {
    if (!hasSentIfttt) {
        Serial.println("***** Send webhook *****");
        IFTTT.trigger("vending-machine", "dxbFXTF7i97rkQ1ZFLQxq-");
        hasSentIfttt = true;
    } else {
        Serial.println("***** Already sent IFTTT *****");
    }
}

