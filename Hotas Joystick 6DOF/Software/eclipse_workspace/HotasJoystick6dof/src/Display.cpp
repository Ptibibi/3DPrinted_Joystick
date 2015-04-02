#include "Display.h"

CDisplay::CDisplay() {
	//SSD1306_OLED* ssd1306 = new SSD1306_OLED();
	ssd1306.HWInit();
	ssd1306.Init();
	lineOffset = 15;
}

/*CDisplay::CDisplay(uint16_t clkPin, uint16_t mosiPin, uint16_t csPin, uint16_t dcPin) {

	lineOffset = 15;
}*/

CDisplay::~CDisplay() {
}

void CDisplay::initialize() {
	lineOffset = 15;
}

void CDisplay::print(const char* txt, ...) {
	// Liste des arguments //
	va_list args;
	// Initialisation, à partir du dernier paramètre connu //
	va_start(args, txt);

	char ret[80];
	uint16_t result = vsprintf(ret, txt, args);
	if (result > 0){
		uint16_t linePos = 0;
		uint16_t offset = 0;
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
				ssd1306.showString(str, linePos, 0);
			}
			else {
				linePos += lineOffset;
				ssd1306.showString((char*)(ptxt + offset), linePos, 0);
				break;
			}
			//Update offset for next step
			offset = (pch + 1) - ptxt;
		} while(pch != NULL);
	}
	else {

	}
	// Fermeture //
	va_end(args);
}
