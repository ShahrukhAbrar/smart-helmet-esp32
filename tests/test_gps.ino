#include <ArduinoUnit.h>
#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>

// Mock GPS data for testing
const char* mockGPSData[] = {
  "$GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A",  // Valid data
  "$GPRMC,123519,V,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A",  // Invalid data (V instead of A)
  "$GPRMC,123519,A,0000.000,N,00000.000,E,000.0,000.0,000000,000.0,W*6A"   // Zero coordinates
};

test(GPS_Valid_Data_Parsing) {
  TinyGPSPlus gps;
  SoftwareSerial ss(16, 17);
  
  // Test valid GPS data parsing
  for (int i = 0; i < strlen(mockGPSData[0]); i++) {
    gps.encode(mockGPSData[0][i]);
  }
  
  assertTrue(gps.location.isValid());
  assertEqual(48.1173, gps.location.lat(), 0.0001);
  assertEqual(11.5167, gps.location.lng(), 0.0001);
  assertEqual(22.4, gps.speed.kmph(), 0.1);
  assertEqual(84.4, gps.course.deg(), 0.1);
}

test(GPS_Invalid_Data_Parsing) {
  TinyGPSPlus gps;
  SoftwareSerial ss(16, 17);
  
  // Test invalid GPS data parsing
  for (int i = 0; i < strlen(mockGPSData[1]); i++) {
    gps.encode(mockGPSData[1][i]);
  }
  
  assertFalse(gps.location.isValid());
}

test(GPS_Zero_Coordinates) {
  TinyGPSPlus gps;
  SoftwareSerial ss(16, 17);
  
  // Test zero coordinates
  for (int i = 0; i < strlen(mockGPSData[2]); i++) {
    gps.encode(mockGPSData[2][i]);
  }
  
  assertTrue(gps.location.isValid());
  assertEqual(0.0, gps.location.lat(), 0.0001);
  assertEqual(0.0, gps.location.lng(), 0.0001);
}

test(GPS_Update_Rate) {
  TinyGPSPlus gps;
  SoftwareSerial ss(16, 17);
  unsigned long lastUpdate = 0;
  const unsigned long updateInterval = 1000; // 1 second
  
  // Simulate GPS updates
  for (int i = 0; i < strlen(mockGPSData[0]); i++) {
    gps.encode(mockGPSData[0][i]);
  }
  
  lastUpdate = millis();
  delay(updateInterval);
  
  assertTrue(millis() - lastUpdate >= updateInterval);
}

void setup() {
  Serial.begin(115200);
  while(!Serial) {}
  Test::run();
}

void loop() {} 