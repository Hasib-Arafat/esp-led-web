#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>

// 🔧 Wi-Fi credentials
const char* ssid = "Shariar's Lab";          
const char* password = "************";    

// 🔧 Firebase credentials
#define FIREBASE_HOST "esp-led-demo-7797b-default-rtdb.asia-southeast1.firebasedatabase.app"
#define FIREBASE_AUTH "8PC8pTtH6xnhYXm5MF5LloICdFXNG3EGmR3ZuntE"

FirebaseConfig config;
FirebaseAuth auth;
FirebaseData fbdo;

const int ledPin = LED_BUILTIN;  // Built-in LED on ESP8266

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);  // LED OFF initially

  // Connect to Wi-Fi
  Serial.println();
  Serial.print("Connecting to Wi-Fi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ Wi-Fi Connected!");

  // Configure Firebase
  config.host = FIREBASE_HOST;
  config.signer.tokens.legacy_token = FIREBASE_AUTH;
  auth.token.uid = ""; // optional for Realtime DB
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  Serial.println("Connected to Firebase!");
}

void loop() {
  // Read "led" value from Firebase
  if (Firebase.getInt(fbdo, "/led")) {
    int ledState = fbdo.intData();
    digitalWrite(ledPin, ledState == 1 ? LOW : HIGH); // LOW=ON, HIGH=OFF
  } else {
    Serial.print("Firebase read failed: ");
    Serial.println(fbdo.errorReason());
  }

  delay(500); // check every 0.5 second
}
