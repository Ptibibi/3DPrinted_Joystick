#include "Button_Matrix.h"

CButtonMatrix::CButtonMatrix(byte* pRowMap, byte* pColMap) {
  memcpy(rowMap, pRowMap, sizeof(byte) * BUTTON_MATRIX_NB_ROWS);
  memcpy(colMap, pColMap, sizeof(byte) * BUTTON_MATRIX_NB_COLS);
  int i;
  for (i = 0; i < BUTTON_MATRIX_NB; i++)
    buttonStatus[i] = false;
  stateMatrix = 0;
}

CButtonMatrix::~CButtonMatrix() {
}

void CButtonMatrix::initialize() {
  //Set ROWS
  int i;
  for (i=0; i<BUTTON_MATRIX_NB_ROWS; i++) {
    pinMode(rowMap[i], OUTPUT);
    digitalWrite(rowMap[i], LOW);
  }
  
  //Set COLS
  for (i=0; i<BUTTON_MATRIX_NB_COLS; i++) {
    pinMode(colMap[i], INPUT);
  }
  delay(100);
  
  stateMatrix = 0;
  setMatrix(stateMatrix);
}
      
void CButtonMatrix::updateButtonStatus() {
  int i;
  int indexButton;
  for (i=0; i<BUTTON_MATRIX_NB_COLS; i++) {
    indexButton = i + (stateMatrix * BUTTON_MATRIX_NB_COLS);
    buttonStatus[indexButton] = digitalRead(rowMap[i]);
  }
}
      
void CButtonMatrix::updateMatrixStatus() {
  if (stateMatrix >= BUTTON_MATRIX_NB_ROWS - 1)
    stateMatrix = 0;
  else
    stateMatrix++;
  
  setMatrix(stateMatrix);
}

void CButtonMatrix::setMatrix(int stateMatrix) {
  int i;
  for (i=0; i < BUTTON_MATRIX_NB_ROWS; i++) {
    digitalWrite(rowMap[i], LOW);
  }
  digitalWrite(rowMap[stateMatrix], HIGH);
}

void CButtonMatrix::getButtonStatus(bool* pButtonStatus) {
  updateButtonStatus();
  updateMatrixStatus();
  
  int i;
  for (i = 0; i < BUTTON_MATRIX_NB; i++)
    pButtonStatus[i] = buttonStatus[i];
}

