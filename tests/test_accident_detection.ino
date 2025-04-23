#include <ArduinoUnit.h>

// Mock accelerometer data structure
struct AccelerometerData {
  float x;
  float y;
  float z;
};

// Test cases for different scenarios
const AccelerometerData testCases[] = {
  {0.1, 0.1, 1.0},    // Normal movement
  {5.0, 5.0, 5.0},    // Sudden impact
  {0.5, 0.5, 0.5},    // Minor movement
  {10.0, 0.0, 0.0},   // Side impact
  {0.0, 10.0, 0.0},   // Front/back impact
  {0.0, 0.0, 10.0}    // Vertical impact
};

// Helper function to simulate accident detection
bool isAccident(float x, float y, float z) {
  const float threshold = 3.0;
  return (abs(x) > threshold || abs(y) > threshold || abs(z) > threshold);
}

// Helper function to calculate impact severity
int calculateImpactSeverity(float x, float y, float z) {
  float magnitude = sqrt(x*x + y*y + z*z);
  if (magnitude > 8.0) return 3;  // Severe
  if (magnitude > 5.0) return 2;  // Moderate
  if (magnitude > 3.0) return 1;  // Light
  return 0;  // No impact
}

test(Accident_Detection_Normal_Movement) {
  assertFalse(isAccident(testCases[0].x, testCases[0].y, testCases[0].z));
  assertEqual(0, calculateImpactSeverity(testCases[0].x, testCases[0].y, testCases[0].z));
}

test(Accident_Detection_Sudden_Impact) {
  assertTrue(isAccident(testCases[1].x, testCases[1].y, testCases[1].z));
  assertEqual(3, calculateImpactSeverity(testCases[1].x, testCases[1].y, testCases[1].z));
}

test(Accident_Detection_Minor_Movement) {
  assertFalse(isAccident(testCases[2].x, testCases[2].y, testCases[2].z));
  assertEqual(0, calculateImpactSeverity(testCases[2].x, testCases[2].y, testCases[2].z));
}

test(Accident_Detection_Side_Impact) {
  assertTrue(isAccident(testCases[3].x, testCases[3].y, testCases[3].z));
  assertEqual(3, calculateImpactSeverity(testCases[3].x, testCases[3].y, testCases[3].z));
}

test(Accident_Detection_Front_Back_Impact) {
  assertTrue(isAccident(testCases[4].x, testCases[4].y, testCases[4].z));
  assertEqual(3, calculateImpactSeverity(testCases[4].x, testCases[4].y, testCases[4].z));
}

test(Accident_Detection_Vertical_Impact) {
  assertTrue(isAccident(testCases[5].x, testCases[5].y, testCases[5].z));
  assertEqual(3, calculateImpactSeverity(testCases[5].x, testCases[5].y, testCases[5].z));
}

test(Accident_Detection_Threshold_Boundary) {
  // Test values just below and above threshold
  assertFalse(isAccident(2.9, 2.9, 2.9));
  assertTrue(isAccident(3.1, 3.1, 3.1));
}

void setup() {
  Serial.begin(115200);
  while(!Serial) {}
  Test::run();
}

void loop() {} 