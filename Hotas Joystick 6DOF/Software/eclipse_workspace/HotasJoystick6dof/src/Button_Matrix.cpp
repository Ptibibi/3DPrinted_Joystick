#include "Button_Matrix.h"

CButtonMatrix::CButtonMatrix(_gpioxConfig* pRowMap, _gpioxConfig* pColMap, uint64_t updateDelay, CDisplay* pDisp) {
  display = pDisp;
  memcpy(rowMap, pRowMap, sizeof(_gpioxConfig) * BUTTON_MATRIX_NB_ROWS);
  memcpy(colMap, pColMap, sizeof(_gpioxConfig) * BUTTON_MATRIX_NB_COLS);
  nextDelay = updateDelay;

  timeNextUpdate = 0;
  stateCol = 0;

  timer.start();
}

CButtonMatrix::~CButtonMatrix() {
}

void CButtonMatrix::initialize() {
  //Set ROWS
  uint16_t i;
  for (i=0; i<BUTTON_MATRIX_NB_ROWS; i++) {
      setConfigGPIO(rowMap[i]);
  }

  //Set COLS
  for (i=0; i<BUTTON_MATRIX_NB_COLS; i++) {
      setConfigGPIO(colMap[i]);
  }
  timer.sleep(100);

  stateCol=1;
  setMatrix(stateCol);
  timeNextUpdate = /*millis()*/ + nextDelay;
}

void CButtonMatrix::setMatrix(uint16_t stateCol) {
  uint16_t i;
  for (i=0; i<BUTTON_MATRIX_NB_COLS; i++) {
    GPIO_PinState outputState = (GPIO_PinState)((stateCol >> i) & 0x0001);
    HAL_GPIO_WritePin(colMap[i].gpiox, colMap[i].Pin, outputState);
  }
}

void CButtonMatrix::nextButtonMatrixState() {
  bool resetCol = (stateCol >> BUTTON_MATRIX_NB_COLS) & 0x0001;
  if (resetCol)
    stateCol=1;
  else
    stateCol=stateCol<<1;
  setMatrix(stateCol);
}

void CButtonMatrix::updateButtonStatus() {
  uint16_t i;
  uint16_t offsetCol=0;

  for (i=0; i<BUTTON_MATRIX_NB_COLS; i++) {
    if ((stateCol >> i) & 0x0001) {
	offsetCol = i * BUTTON_MATRIX_NB_ROWS;
	break;
    }
  }

  for (i=0; i<BUTTON_MATRIX_NB_ROWS; i++)
    buttonStatus[i + offsetCol] = HAL_GPIO_ReadPin(rowMap[i].gpiox, rowMap[i].Pin);
}

void CButtonMatrix::getButtonStatus(GPIO_PinState* pButtonStatus) {
  if (timeNextUpdate <= 0 ) {//millis()) {
    timeNextUpdate = /*millis() + */BUTTON_MATRIX_DELAY_UPDATE;
    updateButtonStatus();
    nextButtonMatrixState();
  }
  memcpy(pButtonStatus, buttonStatus, sizeof(buttonStatus));
}
