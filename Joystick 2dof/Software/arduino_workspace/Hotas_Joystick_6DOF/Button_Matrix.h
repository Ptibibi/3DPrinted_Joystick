#ifndef _BUTTON_MATRIX_H_
#define _BUTTON_MATRIX_H_

#include "Config_Joystick_6Dof.h"
#include "Typedef_Joystick_6Dof.h"

#include "U8g_Display.h"

class CButtonMatrix {
  //Method
  public:
    CButtonMatrix();
    CButtonMatrix(byte* pRowMap, byte* pColMap, unsigned long updateDelay, CU8gDisplay* pDisp);
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
    CU8gDisplay* display;
    unsigned long nextDelay;
    unsigned long timeNextUpdate;
    byte rowMap[BUTTON_MATRIX_NB_ROWS];
    byte colMap[BUTTON_MATRIX_NB_COLS];
    bool buttonStatus[BUTTON_MATRIX_NB];
    int stateCol;
    
};

#endif /* _BUTTON_MATRIX_H_ */

