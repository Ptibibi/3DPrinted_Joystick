#include "Button_Matrix.h"

CButtonMatrix::CButtonMatrix(_gpioxConfig* pRowMap, _gpioxConfig* pColMap, uint64_t updateDelay, CDisplay* pDisp) {
  display = pDisp;
  memcpy(rowMap, pRowMap, sizeof(_gpioxConfig) * BUTTON_MATRIX_NB_ROWS);
  memcpy(colMap, pColMap, sizeof(_gpioxConfig) * BUTTON_MATRIX_NB_COLS);
  nextDelay = updateDelay;

  timeNextUpdate = 0;
  stateCol = 0;
}

CButtonMatrix::~CButtonMatrix() {
}

void CButtonMatrix::initialize() {
  //RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
  __GPIOA_CLK_ENABLE();

  //Set ROWS
  uint16_t i;
  for (i=0; i<BUTTON_MATRIX_NB_ROWS; i++) {
    GPIO_InitTypeDef GPIO_InitDef;
    GPIO_InitDef.Pin = rowMap[i].Pin;
    GPIO_InitDef.Mode = rowMap[i].Mode;
    GPIO_InitDef.Pull = rowMap[i].Pull;
    GPIO_InitDef.Speed = rowMap[i].Speed;
    GPIO_InitDef.Alternate = rowMap[i].Alternate;
    HAL_GPIO_Init(rowMap[i].gpiox, &GPIO_InitDef);
  }

  //Set COLS
  for (i=0; i<BUTTON_MATRIX_NB_COLS; i++) {
    GPIO_InitTypeDef GPIO_InitDef;
    GPIO_InitDef.Pin = colMap[i].Pin;
    GPIO_InitDef.Mode = colMap[i].Mode;
    GPIO_InitDef.Pull = colMap[i].Pull;
    GPIO_InitDef.Speed = colMap[i].Speed;
    GPIO_InitDef.Alternate = colMap[i].Alternate;
    HAL_GPIO_Init(colMap[i].gpiox, &GPIO_InitDef);
  }
  HAL_Delay(100);

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
