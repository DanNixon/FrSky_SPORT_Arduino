/*
  FrSky RPM sensor class for Teensy 3.x and 328P based boards (e.g. Pro Mini,
  Nano, Uno)
  (c) Pawelsky 20150725
  Not for commercial use
*/

#include "FrSkySportSensorRpm.h"

FrSkySportSensorRpm::FrSkySportSensorRpm(SensorId id) : FrSkySportSensor(id) {}

void FrSkySportSensorRpm::setData(float rpm, float t1, float t2) {
  FrSkySportSensorRpm::rpm = (uint32_t)(rpm * 2);
  FrSkySportSensorRpm::t1 = (int32_t)t1;
  FrSkySportSensorRpm::t2 = (int32_t)t2;
}

void FrSkySportSensorRpm::send(FrSkySportSingleWireSerial &serial, uint8_t id,
                               uint32_t now) {
  if (sensorId == id) {
    switch (sensorDataIdx) {
    case 0:
      if (now > t1Time) {
        t1Time = now + RPM_T1_DATA_PERIOD;
        serial.sendData(RPM_T1_DATA_ID, t1);
      } else {
        serial.sendEmpty(RPM_T1_DATA_ID);
      }
      break;
    case 1:
      if (now > t2Time) {
        t2Time = now + RPM_T2_DATA_PERIOD;
        serial.sendData(RPM_T2_DATA_ID, t2);
      } else {
        serial.sendEmpty(RPM_T2_DATA_ID);
      }
      break;
    case 2:
      if (now > rpmTime) {
        rpmTime = now + RPM_ROT_DATA_PERIOD;
        serial.sendData(RPM_ROT_DATA_ID, rpm);
      } else {
        serial.sendEmpty(RPM_ROT_DATA_ID);
      }
      break;
    }
    sensorDataIdx++;
    if (sensorDataIdx >= RPM_DATA_COUNT)
      sensorDataIdx = 0;
  }
}
