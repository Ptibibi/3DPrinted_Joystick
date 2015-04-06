#ifndef _BUTTON_MATRIX_H_
#define _BUTTON_MATRIX_H_

#include "arduino.h"
#include "Config_Joystick.h"

class CButtonMatrix {
  //Method
  public:
    CButtonMatrix();
    CButtonMatrix(byte* pRowMap, byte* pColMap);
    ~CButtonMatrix();
    void initialize();
    void getButtonStatus(bool* pButtonStatus);
    
  private:
    void setMatrix(int stateCol);
    bool bitRightShiftAndMask(int bitmap, int shift, int mask);
    void nextButtonMatrixState();
    void updateButtonStatus();
  
  //Variables
  private:
    byte rowMap[BUTTON_MATRIX_NB_ROWS];
    byte colMap[BUTTON_MATRIX_NB_COLS];
    bool buttonStatus[BUTTON_MATRIX_NB];
    int stateCol;
    
};

#endif /* _BUTTON_MATRIX_H_ */
