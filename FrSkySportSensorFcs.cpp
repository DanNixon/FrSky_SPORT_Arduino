/*
  FrSky FCS-40A/FCS-150A current sensor class for Teensy 3.x and 328P based
  boards (e.g. Pro Mini, Nano, Uno)
  (c) Pawelsky 20150725
  Not for commercial use
*/

#include "FrSkySportSensorFcs.h"

FrSkySportSensorFcs::FrSkySportSensorFcs(SensorId id) : FrSkySportSensor(id) {}

void FrSkySportSensorFcs::setData(float current, float voltage) {
  FrSkySportSensorFcs::current = (uint32_t)(current * 10);
  FrSkySportSensorFcs::voltage = (uint32_t)(voltage * 100);
}

void FrSkySportSensorFcs::send(FrSkySportSingleWireSerial &serial, uint8_t id,
                               uint32_t now) {
  if (sensorId == id) {
    switch (sensorDataIdx) {
    case 0:
      if (now > currentTime) {
        currentTime = now + FCS_CURR_DATA_PERIOD;
        serial.sendData(FCS_CURR_DATA_ID, current);
      } else {
        serial.sendEmpty(FCS_CURR_DATA_ID);
      }
      break;
    case 1:
      if (now > voltageTime) {
        voltageTime = now + FCS_VOLT_DATA_PERIOD;
        serial.sendData(FCS_VOLT_DATA_ID, voltage);
      } else {
        serial.sendEmpty(FCS_VOLT_DATA_ID);
      }
      break;
    }
    sensorDataIdx++;
    if (sensorDataIdx >= FCS_DATA_COUNT)
      sensorDataIdx = 0;
  }
}
