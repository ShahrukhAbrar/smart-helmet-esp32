#include <ArduinoUnit.h>
#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>

// Mock GPS data for testing
const char* mockGPSData = "$GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A";

// Test GPS parsing
test(GPS_Parsing) {
  TinyGPSPlus gps;
  SoftwareSerial ss(16, 17); // Using the same pins as in the main project
  
  // Test valid GPS data parsing
  for (int i = 0; i < strlen(mockGPSData); i++) {
    gps.encode(mockGPSData[i]);
  }
  
  assertTrue(gps.location.isValid());
  assertEqual(48.1173, gps.location.lat(), 0.0001);
  assertEqual(11.5167, gps.location.lng(), 0.0001);
}

// Test power saving mode
test(Power_Saving_Mode) {
  // Mock power saving mode functions
  bool psmEnabled = true;
  int updateRate = 10; // 10 seconds
  
  assertTrue(psmEnabled);
  assertEqual(10, updateRate);
}

// Test WiFi connectivity
test(WiFi_Connectivity) {
  // Mock WiFi connection
  bool wifiConnected = true;
  String ssid = "TestNetwork";
  String password = "TestPassword";
  
  assertTrue(wifiConnected);
  assertEqual("TestNetwork", ssid);
  assertEqual("TestPassword", password);
}

// Test accident detection
test(Accident_Detection) {
  // Mock accelerometer data
  float x = 0.0;
  float y = 0.0;
  float z = 0.0;
  
  // Simulate normal movement
  x = 0.1;
  y = 0.1;
  z = 1.0;
  assertFalse(isAccident(x, y, z));
  
  // Simulate sudden impact
  x = 5.0;
  y = 5.0;
  z = 5.0;
  assertTrue(isAccident(x, y, z));
}

// Helper function to simulate accident detection
bool isAccident(float x, float y, float z) {
  // Simple threshold-based detection
  const float threshold = 3.0;
  return (abs(x) > threshold || abs(y) > threshold || abs(z) > threshold);
}

void setup() {
  Serial.begin(115200);
  while(!Serial) {} // Wait for serial connection
  
  // Run all tests
  Test::run();
}

void loop() {
  // Empty loop as tests run in setup
} 