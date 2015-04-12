/*******************************************************************************
Button_Matrix.cpp
V 0.02

Description:

********************************************************************************
********************************************************************************
V0.02 --> 12/04/15	Edition originale by BRB
*******************************************************************************/
#include "Button_Matrix.h"

CButtonMatrix::CButtonMatrix(byte* pRowMap, byte* pColMap) {
  int i;
  for (i = 0; i < BUTTON_MATRIX_NB_ROWS; i++)
    rowMap[i] = pRowMap[i];
    
  for (i = 0; i < BUTTON_MATRIX_NB_COLS; i++)
    colMap[i] = pColMap[i];
    
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
    pinMode(rowMap[i], INPUT);
  }
  
  //Set COLS
  for (i=0; i<BUTTON_MATRIX_NB_COLS; i++) {
    pinMode(colMap[i], OUTPUT);
    digitalWrite(colMap[i], LOW);
  }
  delay(100);
  
  stateMatrix = 0;
  setMatrix(stateMatrix);
}
      
void CButtonMatrix::updateButtonStatus() {
  int i;
  int indexButton;
  int offset = stateMatrix * BUTTON_MATRIX_NB_ROWS;
  for (i=0; i<BUTTON_MATRIX_NB_ROWS; i++) {
    indexButton = i + offset;
    buttonStatus[indexButton] = digitalRead(rowMap[i]);
  }
}
      
void CButtonMatrix::updateMatrixStatus() {
  if (stateMatrix >= BUTTON_MATRIX_NB_COLS - 1)
    stateMatrix = 0;
  else
    stateMatrix++;
  
  setMatrix(stateMatrix);
}

void CButtonMatrix::setMatrix(int stateMatrix) {
  int i;
  for (i=0; i < BUTTON_MATRIX_NB_COLS; i++) {
    digitalWrite(colMap[i], LOW);
  }
  digitalWrite(colMap[stateMatrix], HIGH);
}

void CButtonMatrix::getButtonStatus(bool* pButtonStatus) {
  updateButtonStatus();
  updateMatrixStatus();
  
  int i;
  for (i = 0; i < BUTTON_MATRIX_NB; i++)
    *(pButtonStatus + i) = buttonStatus[i];
}

