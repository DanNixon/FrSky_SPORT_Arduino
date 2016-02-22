/*
  FrSky FLVSS LiPo voltage sensor class for Teensy 3.x and 328P based boards
  (e.g. Pro Mini, Nano, Uno)
  (c) Pawelsky 20150725
  Not for commercial use
*/

#include "FrSkySportSensorFlvss.h"

FrSkySportSensorFlvss::FrSkySportSensorFlvss(SensorId id)
    : FrSkySportSensor(id) {}

uint32_t FrSkySportSensorFlvss::setCellData(uint8_t cellNum,
                                            uint8_t firstCellNo, float cell1,
                                            float cell2) {
  uint16_t cell1Data = cell1 * 1000 / 2;
  uint16_t cell2Data = cell2 * 1000 / 2;
  uint32_t cellData = 0;

  cellData = cell2Data & 0x0FFF;
  cellData <<= 12;
  cellData |= cell1Data & 0x0FFF;
  cellData <<= 4;
  cellData |= cellNum & 0x0F;
  cellData <<= 4;
  cellData |= firstCellNo & 0x0F;

  return cellData;
}

void FrSkySportSensorFlvss::setData(float cell1, float cell2, float cell3,
                                    float cell4, float cell5, float cell6) {
  uint8_t numCells = 1; // We assume at least one cell
  if (cell2 > 0) {
    numCells++;
    if (cell3 > 0) {
      numCells++;
      if (cell4 > 0) {
        numCells++;
        if (cell5 > 0) {
          numCells++;
          if (cell6 > 0) {
            numCells++;
          }
        }
      }
    }
  }
  cellData1 = setCellData(numCells, 0, cell1, cell2);
  if (numCells > 2)
    cellData2 = setCellData(numCells, 2, cell3, cell4);
  else
    cellData2 = 0;
  if (numCells > 4)
    cellData3 = setCellData(numCells, 4, cell5, cell6);
  else
    cellData3 = 0;
}

void FrSkySportSensorFlvss::send(FrSkySportSingleWireSerial &serial, uint8_t id,
                                 uint32_t now) {
  if (sensorId == id) {
    if (now > cellDataTime) {
      cellDataTime = now + FLVSS_CELL_DATA_PERIOD;
      switch (sensorDataIdx) {
      case 0:
        serial.sendData(FLVSS_CELL_DATA_ID, cellData1);
        if (cellData2 != 0)
          sensorDataIdx = 1;
        else
          sensorDataIdx = 0;
        break;
      case 1:
        serial.sendData(FLVSS_CELL_DATA_ID, cellData2);
        if (cellData3 != 0)
          sensorDataIdx = 2;
        else
          sensorDataIdx = 0;
        break;
      case 2:
        serial.sendData(FLVSS_CELL_DATA_ID, cellData3);
        sensorDataIdx = 0;
        break;
      }
    } else {
      serial.sendEmpty(FLVSS_CELL_DATA_ID);
    }
  }
}
