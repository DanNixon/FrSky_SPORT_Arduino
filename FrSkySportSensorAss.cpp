/*
  FrSky ASS-70/ASS-100 airspeed sensor class for Teensy 3.x and 328P based
  boards (e.g. Pro Mini, Nano, Uno)
  (c) Pawelsky 20150921
  Not for commercial use

  Note that FrSky does not yet provide this sensor so it is based on the
  specification only and may change in the future
*/

#include "FrSkySportSensorAss.h"

FrSkySportSensorAss::FrSkySportSensorAss(SensorId id) : FrSkySportSensor(id) {}

void FrSkySportSensorAss::setData(float speed) {
  FrSkySportSensorAss::speed = (uint32_t)(speed * 10.0);
}

void FrSkySportSensorAss::send(FrSkySportSingleWireSerial &serial, uint8_t id,
                               uint32_t now) {
  if (sensorId == id) {
    if (now > speedTime) {
      speedTime = now + ASS_SPEED_DATA_PERIOD;
      serial.sendData(ASS_SPEED_DATA_ID, speed);
    } else {
      serial.sendEmpty(ASS_SPEED_DATA_ID);
    }
  }
}
