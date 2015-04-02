#ifndef SSD1306_OLED_H
#define SSD1306_OLED_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_conf.h"
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_spi.h"


/* Definitions */
#define xLevelL         0x00
#define xLevelH         0x10
#define xLevel          ((xLevelH&0x0F)*16+xLevelL)
#define maxColumn       128
#define maxRow          64
#define oledBrightness  0xCF

class SSD1306_OLED
{
    public:
        void HWInit(void);
        void Init(void);
        void drawFrame(void);
        void drawBatt(int level, int x, int row);
        void fillScreen(unsigned char data);
        void fillBlock(unsigned char data, unsigned char startRow, unsigned char endRow, unsigned char startCol, unsigned char endCol);
        void showString(const char *dataPtr, unsigned char base, unsigned char column);
        void showFont57(char b, unsigned char c, unsigned char d);
        void sleep(unsigned char sleep);
        void setNOP(void);

    private:
        void writeCommand(uint8_t data);
        void writeData(uint8_t data);
        void setStartColumn(unsigned char d);
        void setAddressingMode(unsigned char d);
        void setColumnAddress(unsigned char a, unsigned char b);
        void setPageAddress(unsigned char a, unsigned char b);
        void setStartLine(unsigned char d);
        void setContrastControl(unsigned char d);
        void setChargePump(unsigned char d);
        void setSegmentRemap(unsigned char d);
        void setEntireDisplay(unsigned char d);
        void setInverseDisplay(unsigned char d);
        void setMultiplexRatio(unsigned char d);
        void setDisplayOnOff(unsigned char d);
        void setStartPage(unsigned char d);
        void setCommonRemap(unsigned char d);
        void setDisplayOffset(unsigned char d);
        void setDisplayClock(unsigned char d);
        void setPrechargePeriod(unsigned char d);
        void setCommonConfig(unsigned char d);
        void setVCOMH(unsigned char d);

        GPIO_InitTypeDef GPIO_InitStructure;
        SPI_HandleTypeDef SPI_HandleDef;
        /* Font Data */
        char fontData[240][5];
};

#ifdef __cplusplus
}
#endif

#endif
