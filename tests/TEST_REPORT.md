# Smart Helmet System Test Report

## Overview

This document provides a comprehensive overview of all test cases implemented for the Smart Accident Detection Helmet system. The tests are organized by component and include detailed specifications, expected outcomes, and test coverage.

## Test Environment

- Hardware: ESP32 Development Board
- Testing Framework: ArduinoUnit
- Serial Monitor Baud Rate: 115200
- GPS Module: u-blox NEO-6M
- Accelerometer: MPU6050 (simulated in tests)

## GPS Module Tests

### 1. GPS Valid Data Parsing

**Test ID:** GPS_Valid_Data_Parsing  
**Description:** Verifies correct parsing of valid GPS NMEA data  
**Input:** Valid NMEA sentence with coordinates, speed, and course  
**Expected Output:**

- Location validity: true
- Latitude: 48.1173°N
- Longitude: 11.5167°E
- Speed: 22.4 km/h
- Course: 84.4°

### 2. GPS Invalid Data Parsing

**Test ID:** GPS_Invalid_Data_Parsing  
**Description:** Verifies handling of invalid GPS data  
**Input:** NMEA sentence with invalid status (V instead of A)  
**Expected Output:**

- Location validity: false

### 3. GPS Zero Coordinates

**Test ID:** GPS_Zero_Coordinates  
**Description:** Verifies handling of zero coordinate values  
**Input:** NMEA sentence with zero coordinates  
**Expected Output:**

- Location validity: true
- Latitude: 0.0°N
- Longitude: 0.0°E

### 4. GPS Update Rate

**Test ID:** GPS_Update_Rate  
**Description:** Verifies GPS data update timing  
**Input:** Simulated GPS updates  
**Expected Output:**

- Update interval: ≥ 1000ms

## Accident Detection Tests

### 1. Normal Movement Detection

**Test ID:** Accident_Detection_Normal_Movement  
**Description:** Verifies system behavior during normal movement  
**Input:** Accelerometer data (0.1, 0.1, 1.0)  
**Expected Output:**

- Accident detected: false
- Impact severity: 0 (No impact)

### 2. Sudden Impact Detection

**Test ID:** Accident_Detection_Sudden_Impact  
**Description:** Verifies detection of sudden impact  
**Input:** Accelerometer data (5.0, 5.0, 5.0)  
**Expected Output:**

- Accident detected: true
- Impact severity: 3 (Severe)

### 3. Minor Movement Detection

**Test ID:** Accident_Detection_Minor_Movement  
**Description:** Verifies handling of minor movements  
**Input:** Accelerometer data (0.5, 0.5, 0.5)  
**Expected Output:**

- Accident detected: false
- Impact severity: 0 (No impact)

### 4. Side Impact Detection

**Test ID:** Accident_Detection_Side_Impact  
**Description:** Verifies detection of side impacts  
**Input:** Accelerometer data (10.0, 0.0, 0.0)  
**Expected Output:**

- Accident detected: true
- Impact severity: 3 (Severe)

### 5. Front/Back Impact Detection

**Test ID:** Accident_Detection_Front_Back_Impact  
**Description:** Verifies detection of front/back impacts  
**Input:** Accelerometer data (0.0, 10.0, 0.0)  
**Expected Output:**

- Accident detected: true
- Impact severity: 3 (Severe)

### 6. Vertical Impact Detection

**Test ID:** Accident_Detection_Vertical_Impact  
**Description:** Verifies detection of vertical impacts  
**Input:** Accelerometer data (0.0, 0.0, 10.0)  
**Expected Output:**

- Accident detected: true
- Impact severity: 3 (Severe)

### 7. Threshold Boundary Testing

**Test ID:** Accident_Detection_Threshold_Boundary  
**Description:** Verifies system behavior at threshold boundaries  
**Input:**

- Below threshold: (2.9, 2.9, 2.9)
- Above threshold: (3.1, 3.1, 3.1)  
  **Expected Output:**
- Below threshold: Accident detected: false
- Above threshold: Accident detected: true

## Power Management Tests

### 1. Power Saving Mode

**Test ID:** Power_Saving_Mode  
**Description:** Verifies power saving mode configuration  
**Input:** Power settings (PSM enabled, 10s update rate)  
**Expected Output:**

- PSM enabled: true
- Update rate: 10s
- SBAS enabled: false
- Unused NMEA disabled: true
- Power consumption: < 100mA

### 2. High Performance Mode

**Test ID:** High_Performance_Mode  
**Description:** Verifies high performance mode configuration  
**Input:** Power settings (PSM disabled, 1s update rate)  
**Expected Output:**

- PSM enabled: false
- Update rate: 1s
- SBAS enabled: true
- Unused NMEA disabled: false
- Power consumption: > 100mA

### 3. Balanced Mode

**Test ID:** Balanced_Mode  
**Description:** Verifies balanced mode configuration  
**Input:** Power settings (PSM enabled, 5s update rate)  
**Expected Output:**

- PSM enabled: true
- Update rate: 5s
- SBAS enabled: false
- Unused NMEA disabled: true
- Power consumption: < 100mA

### 4. Custom Mode

**Test ID:** Custom_Mode  
**Description:** Verifies custom power mode configuration  
**Input:** Power settings (PSM disabled, 10s update rate)  
**Expected Output:**

- PSM enabled: false
- Update rate: 10s
- SBAS enabled: true
- Unused NMEA disabled: false
- Power consumption: > 100mA

### 5. Power Consumption Calculation

**Test ID:** Power_Consumption_Calculation  
**Description:** Verifies power consumption calculation accuracy  
**Input:** Power settings (PSM enabled, 10s update rate)  
**Expected Output:**

- Power consumption: > 0mA
- Power consumption: < 200mA

## Test Coverage Summary

- GPS Module: 4 test cases
- Accident Detection: 7 test cases
- Power Management: 5 test cases
- Total Test Cases: 16

## Test Execution Instructions

1. Install ArduinoUnit library in Arduino IDE
2. Upload each test file to ESP32
3. Open Serial Monitor at 115200 baud
4. Observe test results in Serial Monitor

## Notes

- All tests use mock data for simulation
- Power consumption values are approximate
- Impact severity levels: 0 (None), 1 (Light), 2 (Moderate), 3 (Severe)
- GPS update rate tests include timing verification
- Accident detection threshold: 3.0g
