#include "U8g_Display.h"

CU8gDisplay::CU8gDisplay() {
}
  
CU8gDisplay::CU8gDisplay(byte clkPin, byte mosiPin, byte csPin, byte dcPin) {
  pu8g = new U8GLIB_SSD1306_128X64(clkPin, mosiPin, csPin, dcPin);
}

CU8gDisplay::~CU8gDisplay() {
}

void CU8gDisplay::initialize() {
  if ( pu8g->getMode() == U8G_MODE_R3G3B2 ) {
    pu8g->setColorIndex(255);     // white
  }
  else if ( pu8g->getMode() == U8G_MODE_GRAY2BIT ) {
    pu8g->setColorIndex(3);         // max intensity
  }
  else if ( pu8g->getMode() == U8G_MODE_BW ) {
    pu8g->setColorIndex(1);         // pixel on
  }
  else if ( pu8g->getMode() == U8G_MODE_HICOLOR ) {
    pu8g->setHiColorByRGB(255,255,255);
  }
  
  lineOffset = 15;
}

void CU8gDisplay::print(const char* txt, ...) {
  // Liste des arguments //
  va_list args;
  // Initialisation, Ã  partir du dernier paramÃ¨tre connu //
  va_start(args, txt);
  
  char ret[80];
  int result = vsprintf(ret, txt, args);
  if (result > 0){
    pu8g->firstPage();  
    do {
      pu8g->setFont(u8g_font_unifont);
      int linePos = 0;
      int offset = 0;
      char* pch;
      char* ptxt = ret;
      do {
        //Search special characters
        pch = strchr(ptxt + offset, '\n');
        if (pch != NULL) {
          linePos += lineOffset;
          char str[80];
          memset(str, 0, sizeof(str));
          memcpy(str, ptxt + offset, pch - (ptxt + offset));
          
          pu8g->setPrintPos(0, linePos);
          pu8g->print(str);
        }
        else {
          linePos += lineOffset;
          pu8g->setPrintPos(0, linePos);
          pu8g->print((char*)(ptxt + offset));
          break;
        }
        //Update offset for next step
        offset = (pch + 1) - ptxt;
        
      } while(pch != NULL);
    } while(pu8g->nextPage());
  }
  else {
  }
  // Fermeture //
  va_end(args);
}

