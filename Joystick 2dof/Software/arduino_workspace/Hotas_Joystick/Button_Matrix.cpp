#include "Button_Matrix.h"

CButtonMatrix::CButtonMatrix(byte* pRowMap, byte* pColMap) {
  memcpy(rowMap, pRowMap, sizeof(byte) * BUTTON_MATRIX_NB_ROWS);
  memcpy(colMap, pColMap, sizeof(byte) * BUTTON_MATRIX_NB_COLS);
}

CButtonMatrix::~CButtonMatrix() {
}

void CButtonMatrix::initialize() {
  //Set ROWS
  int i;
  for (i=0; i<BUTTON_MATRIX_NB_ROWS; i++) {
    pinMode(rowMap[i], INPUT);
  }
  
  //Set COLS
  for (i=0; i<BUTTON_MATRIX_NB_COLS; i++) {
    pinMode(colMap[i], OUTPUT);
  }
  delay(100);
  
  stateCol=1;
  setMatrix(stateCol);
}

void CButtonMatrix::setMatrix(int stateCol) {
  int i;
  for (i=0; i<BUTTON_MATRIX_NB_COLS; i++) {
    bool outputState = bitRightShiftAndMask(stateCol, i, 0x0001);
    digitalWrite(colMap[i], outputState);
  }
}

bool CButtonMatrix::bitRightShiftAndMask(int bitmap, int shift, int mask) {
  return ((bitmap >> shift) & mask);
}
      
void CButtonMatrix::nextButtonMatrixState() {
  bool resetCol = bitRightShiftAndMask(stateCol, BUTTON_MATRIX_NB_COLS, 0x0001);
  if (resetCol) stateCol=1;
  else stateCol=stateCol<<1;
  setMatrix(stateCol);
}
      
void CButtonMatrix::updateButtonStatus() {
  int i;
  int offsetCol=0;
  
  for (i=0; i<BUTTON_MATRIX_NB_COLS; i++) {
    bool outputState = bitRightShiftAndMask(stateCol, i, 0x0001);
    if (bitRightShiftAndMask(stateCol, i, 0x0001)) {
      offsetCol = i * BUTTON_MATRIX_NB_ROWS;
      break;
    }
  }
  
  for (i=0; i<BUTTON_MATRIX_NB_ROWS; i++) {
    buttonStatus[i + offsetCol] = digitalRead(rowMap[i]);
  }
}

void CButtonMatrix::getButtonStatus(bool* pButtonStatus) {
  updateButtonStatus();
  nextButtonMatrixState();
  memcpy(pButtonStatus, buttonStatus, sizeof(buttonStatus));
}

