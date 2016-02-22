/*
  FrSky S.Port to UART Remote (Type B) converter class for Teensy 3.x and 328P
  based boards (e.g. Pro Mini, Nano, Uno)
  (c) Pawelsky 20150725
  Not for commercial use

  Note that only analog ports ADC3 and ADC4 are implemented, not the UART part.
*/

#include "FrSkySportSensorSp2uart.h"

FrSkySportSensorSp2uart::FrSkySportSensorSp2uart(SensorId id)
    : FrSkySportSensor(id) {}

void FrSkySportSensorSp2uart::setData(float adc3, float adc4) {
  FrSkySportSensorSp2uart::adc3 = (uint32_t)(adc3 * 100);
  FrSkySportSensorSp2uart::adc4 = (uint32_t)(adc4 * 100);
}

void FrSkySportSensorSp2uart::send(FrSkySportSingleWireSerial &serial,
                                   uint8_t id, uint32_t now) {
  if (sensorId == id) {
    switch (sensorDataIdx) {
    case 0:
      if (now > adc3Time) {
        adc3Time = now + SP2UARTB_ADC3_DATA_PERIOD;
        serial.sendData(SP2UARTB_ADC3_DATA_ID, adc3);
      } else {
        serial.sendEmpty(SP2UARTB_ADC3_DATA_ID);
      }
      break;
    case 1:
      if (now > adc4Time) {
        adc4Time = now + SP2UARTB_ADC4_DATA_PERIOD;
        serial.sendData(SP2UARTB_ADC4_DATA_ID, adc4);
      } else {
        serial.sendEmpty(SP2UARTB_ADC4_DATA_ID);
      }
      break;
    }
    sensorDataIdx++;
    if (sensorDataIdx >= SP2UARTB_DATA_COUNT)
      sensorDataIdx = 0;
  }
}
