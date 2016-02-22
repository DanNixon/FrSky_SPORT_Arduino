/*
  FrSky sensor base class for Teensy 3.x and 328P based boards (e.g. Pro Mini,
  Nano, Uno)
  (c) Pawelsky 20150725
  Not for commercial use
*/

#include "FrSkySportSensor.h"

FrSkySportSensor::FrSkySportSensor(SensorId id)
    : sensorId(id), sensorDataIdx(0) {}

void FrSkySportSensor::send(FrSkySportSingleWireSerial &serial, uint8_t id,
                            uint32_t now) {}
