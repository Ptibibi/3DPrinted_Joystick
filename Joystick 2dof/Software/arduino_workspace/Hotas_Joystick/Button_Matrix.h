/*******************************************************************************
Button_Matrix.h
V 0.02

Description:

********************************************************************************
********************************************************************************
V0.02 --> 12/04/15	Edition originale by BRB
*******************************************************************************/
#ifndef _BUTTON_MATRIX_H_
#define _BUTTON_MATRIX_H_

#include "arduino.h"
#include "Config_Joystick.h"

class CButtonMatrix {
  //Method
  public:
    CButtonMatrix(byte* pRowMap, byte* pColMap);
    ~CButtonMatrix();
    void initialize();
    void getButtonStatus(bool* pButtonStatus);
    
  private:
    void updateButtonStatus();
    void updateMatrixStatus();
    void setMatrix(int stateMatrix);
  
  //Variables
  private:
    byte rowMap[BUTTON_MATRIX_NB_ROWS];
    byte colMap[BUTTON_MATRIX_NB_COLS];
    bool buttonStatus[BUTTON_MATRIX_NB];
    int stateMatrix;
    
};

#endif /* _BUTTON_MATRIX_H_ */

