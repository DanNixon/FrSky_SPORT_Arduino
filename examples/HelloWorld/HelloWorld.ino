/*
  FrSky S-Port Telemetry library example
  (c) Pawelsky 20150921
  Not for commercial use
  
  Note that you need Teensy 3.x or 328P based (e.g. Pro Mini, Nano, Uno) board and FrSkySportTelemetry library for this example to work
*/

#include "FrSkySportSensor.h"
#include "FrSkySportSensorAss.h"
#include "FrSkySportSensorFcs.h"
#include "FrSkySportSensorFlvss.h"
#include "FrSkySportSensorGps.h"
#include "FrSkySportSensorRpm.h"
#include "FrSkySportSensorSp2uart.h"
#include "FrSkySportSensorVario.h"
#include "FrSkySportSingleWireSerial.h"
#include "FrSkySportTelemetry.h"
#if !defined(__MK20DX128__) && !defined(__MK20DX256__)
#include "SoftwareSerial.h"
#endif

FrSkySportSensorAss ass;                               // Create ASS sensor with default ID
FrSkySportSensorFcs fcs;                               // Create FCS sensor with default ID
FrSkySportSensorFlvss flvss1;                          // Create FLVSS sensor with default ID
FrSkySportSensorFlvss flvss2(FrSkySportSensor::ID15);  // Create FLVSS sensor with given ID
FrSkySportSensorGps gps;                               // Create GPS sensor with default ID
FrSkySportSensorRpm rpm;                               // Create RPM sensor with default ID
FrSkySportSensorSp2uart sp2uart;                       // Create SP2UART Type B sensor with default ID
FrSkySportSensorVario vario;                           // Create Variometer sensor with default ID
FrSkySportTelemetry telemetry;                         // Create Variometer telemetry object

void setup()
{
  // Configure the telemetry serial port and sensors (remember to use & to specify a pointer to sensor)
#if defined(__MK20DX128__) || defined(__MK20DX256__)
  telemetry.begin(FrSkySportSingleWireSerial::SERIAL_3, &ass, &fcs, &flvss1, &flvss2, &gps, &sp2uart, &rpm, &vario);
#else
  telemetry.begin(FrSkySportSingleWireSerial::SOFT_SERIAL_PIN_12, &ass, &fcs, &flvss1, &flvss2, &gps, &sp2uart, &rpm, &vario);
#endif
}

void loop()
{
  // Set airspeed sensor (ASS) data
  ass.setData(76.5);  // Airspeed in km/h

  // Set current/voltage sensor (FCS) data
  // (set Voltage source to FAS in menu to use this data for battery voltage,
  //  set Current source to FAS in menu to use this data for current readins)
  fcs.setData(25.3,   // Current consumption in amps
              12.6);  // Battery voltage in volts

  // Set LiPo voltage sensor (FLVSS) data (we use two sensors to simulate 8S battery 
  // (set Voltage source to Cells in menu to use this data for battery voltage)
  flvss1.setData(4.07, 4.08, 4.09, 4.10, 4.11, 4.12);  // Cell voltages in volts (cells 1-6)
  flvss2.setData(4.13, 4.14);                          // Cell voltages in volts (cells 7-8)

  // Set GPS data
  gps.setData(48.858289, 2.294502,   // Latitude and longitude in degrees decimal (positive for N/E, negative for S/W)
              245.5,                 // Altitude in m (can be nevative)
              100.0,                 // Speed in m/s
              90.23,                 // Course over ground in degrees
              14, 9, 14,             // Date (year - 2000, month, day)
              12, 00, 00);           // Time (hour, minute, second) - will be affected by timezone setings in your radio

  // Set RPM/temperature sensor data
  // (set number of blades to 2 in telemetry menu to get correct rpm value)
  rpm.setData(200,    // Rotations per minute
              25.6,   // Temperature #1 in degrees Celsuis (can be negative)
              -7.8);  // Temperature #1 in degrees Celsuis (can be negative)

  // Set SP2UART sensor data
  // (values from 0.0 to 3.3 are accepted)
  sp2uart.setData(1.5,    // ADC3 voltage in volts
                  3.3);   // ADC4 voltage in volts

 // Set variometer data
  // (set Variometer source to VSpd in menu to use the vertical speed data from this sensor for variometer).
  vario.setData(250.5,  // Altitude in Meters (can be negative)
                -1.5);  // Vertical speed in m/s (positive - up, negative - down)

  // Send the telemetry data, note that the data will only be sent for sensors
  // that are being polled at given moment
  telemetry.send();
}