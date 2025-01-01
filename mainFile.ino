#include <Wire.h>
#include <esp_sleep.h>


//WIFI Configuration
const char* ssid = "hotispoti";
const char* password = "hello1234";
String phoneNumber = "+923405389020";
String apiKey = "2686523";

//Accelerometer
const int MPU_ADDR = 0x68;
int16_t AcX, AcY, AcZ;
int16_t prevAcX, prevAcY, prevAcZ;
const int THRESHOLD = 20000;


//FSR
int FSRsensorPin = 36;
int FSRValue = 0;

//button
const int buttonPin = 15;
int buttonState = HIGH;
long startTime = 0;



void setup() {
  //accelerometer
  Serial.begin(115200);
  gpsSerial.begin(9600, SERIAL_8N1, 16, 17);

  Wire.begin(21, 22);  // ESP32 default SDA, SCL pins
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // Wake up the MPU-6050
  Wire.endTransmission(true);

  //Button
  pinMode(buttonPin, INPUT_PULLUP);

  //GPS
  enablePowerSavingMode();
  disableSBAS();
  setUltraLowUpdateRate();
  disableUnnecessaryNMEASentences();

  //wifi
  disconnectWiFi();
}

void loop() {

  FSRValue = fsr(FSRsensorPin);
  Serial.println(FSRValue);

  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 6, true);

  AcX = (Wire.read() << 8) | Wire.read();
  AcY = (Wire.read() << 8) | Wire.read();
  AcZ = (Wire.read() << 8) | Wire.read();


  // Check for sudden motion
  if (abs(AcX - prevAcX) > THRESHOLD || abs(AcY - prevAcY) > THRESHOLD || abs(AcZ - prevAcZ) > THRESHOLD || FSRValue > 100) {

    Serial.println("Sudden motion detected!");
    tone(4, 854);          // Start the buzzer
    startTime = millis();  // Record start time

    while (millis() - startTime < 30000) {  // Run for a max of 30 seconds
      buttonState = digitalRead(buttonPin);
      if (buttonState == LOW) {
        //Serial.println("Button Pressed! Stopping buzzer...");
        noTone(4);
        //Serial.println("Buzzer Stopped!");
        Serial.println("[+] : Action Cancelled!");
        prevAcX = AcX;
        prevAcY = AcY;
        prevAcZ = AcZ;
        return;
      }
    }

    noTone(4);
    readGPSDataOnce();
    Serial.print("[+] : (⌐■_■)   [̲̅$̲̅(̲̅ιο̲̅̅)̲̅$̲̅]");
    String info = "Crash Detected! Location: https://www.google.com/maps?q=" + String(Prevlatitude, 6) + "," + String(Prevlongtitude, 6);
    if (connectWiFi()) {
      sendMessage(info);
    } else {
      Serial.println("[-] : Failed to Connect to WiFi!");
    }
    disconnectWiFi();
  }

  prevAcX = AcX;
  prevAcY = AcY;
  prevAcZ = AcZ;

  Serial.println(AcX);
  Serial.println(AcY);
  Serial.println(AcZ);
}

int fsr(int sensorPin) {
  int digitalValue = analogRead(sensorPin);
  return digitalValue;
}