#include "SSD1306_OLED.h"

void SSD1306_OLED::HWInit(void) {
  // RST
  GPIO_InitStructure.Pin     = GPIO_PIN_3;
  GPIO_InitStructure.Mode    = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructure.Speed   = GPIO_SPEED_HIGH;
  GPIO_InitStructure.Pull    = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
  // DC
  GPIO_InitStructure.Pin     = GPIO_PIN_2;
  GPIO_InitStructure.Mode    = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructure.Speed   = GPIO_SPEED_HIGH;
  GPIO_InitStructure.Pull    = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

  // SPI1
  /* Configure SPI1 pins: SCK and MOSI */
  // SCK=PA5, MOSI=PA7
  GPIO_InitStructure.Pin     = GPIO_PIN_5 | GPIO_PIN_7;
  GPIO_InitStructure.Mode    = GPIO_MODE_AF_PP;
  GPIO_InitStructure.Speed   = GPIO_SPEED_MEDIUM;
  GPIO_InitStructure.Pull    = GPIO_NOPULL;
  GPIO_InitStructure.Alternate = GPIO_AF5_SPI1;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Configure CS pin */
  GPIO_InitStructure.Pin     = GPIO_PIN_4;
  GPIO_InitStructure.Mode    = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructure.Speed   = GPIO_SPEED_MEDIUM;
  GPIO_InitStructure.Pull    = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* SPI1 Periph clock enable */
  __SPI1_CLK_ENABLE();

  /* SPI1 Config */
  // Note: OLED has maximum allowable clock speed = 10 MHz (i.e. minimum Tcycle=100ns)
  SPI_HandleDef.Instance = SPI1;
  SPI_HandleDef.Init.Direction = SPI_DIRECTION_1LINE;
  SPI_HandleDef.Init.Mode = SPI_MODE_MASTER;
  SPI_HandleDef.Init.DataSize = SPI_DATASIZE_8BIT;
  SPI_HandleDef.Init.CLKPolarity = SPI_POLARITY_HIGH;
  SPI_HandleDef.Init.CLKPhase = SPI_PHASE_2EDGE;
  SPI_HandleDef.Init.NSS = SPI_NSS_SOFT;
  SPI_HandleDef.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
  SPI_HandleDef.Init.FirstBit = SPI_FIRSTBIT_MSB;
  HAL_SPI_Init(&SPI_HandleDef);
}

/* Initialization routine (call once before using OLED) */
void SSD1306_OLED::Init(void)
{
    // Toggle RST line (delay between pin SET/RESET)
    // Note: Logic low must have duration >3us
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);

    setDisplayOnOff(0x00);              // Display Off (0x00/0x01)
    setDisplayClock(0xF0);              // Max display clock frequency, divide ratio=1
    setMultiplexRatio(0x3F);            // 1/64 Duty (0x0F~0x3F)
    setDisplayOffset(0x00);             // Shift Mapping RAM Counter (0x00~0x3F)
    setStartLine(0x00);                 // Set Mapping RAM Display Start Line (0x00~0x3F)
    setChargePump(0x04);                // Enable Embedded DC/DC Converter (0x00/0x04)
    setAddressingMode(0x02);            // Set Page Addressing Mode (0x00/0x01/0x02)
    setSegmentRemap(0x01);              // Set SEG/Column Mapping (0x00/0x01)
    setCommonRemap(0x08);               // Set COM/Row Scan Direction (0x00/0x08)
    setCommonConfig(0x10);              // Set Sequential Configuration (0x00/0x10)
    setContrastControl(oledBrightness); // Set SEG Output Current
    setPrechargePeriod(0xF1);           // Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    setVCOMH(0x40);                     // Set VCOM Deselect Level
    setEntireDisplay(0x00);             // Disable Entire Display On (0x00/0x01)
    setInverseDisplay(0x00);            // Disable Inverse Display On (0x00/0x01)

    fillScreen(0x00);                   // Clear Screen

    setDisplayOnOff(0x01);              // Display On (0x00/0x01)
}

/* Write a command to the OLED */
void SSD1306_OLED::writeCommand(uint8_t data)
{
    // CS=0, DC=0
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);

    // Write data to SPI
    HAL_SPI_Transmit(&SPI_HandleDef, &data, 1, 10);
    // Wait until SPI is ready
	while(HAL_SPI_GetState(&SPI_HandleDef) == SPI_FLAG_TXE);
	while(!HAL_SPI_GetState(&SPI_HandleDef) == SPI_FLAG_BSY);
    // CS=1
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
}

/* Write data to the OLED */
void SSD1306_OLED::writeData(uint8_t data)
{
    // CS=0, DC=1
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);

    // Write data to SPI
    HAL_SPI_Transmit(&SPI_HandleDef, &data, 1, 10);
    // Wait until SPI is ready
	while(HAL_SPI_GetState(&SPI_HandleDef) == SPI_FLAG_TXE);
	while(!HAL_SPI_GetState(&SPI_HandleDef) == SPI_FLAG_BSY);
    // CS=1
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
}

/* Below are functions used to configure the OLED */
void SSD1306_OLED::setStartColumn(unsigned char d)
{
    writeCommand(0x00+d%16);    // Set Lower Column Start Address for Page Addressing Mode
                                // Default => 0x00
    writeCommand(0x10+d/16);    // Set Higher Column Start Address for Page Addressing Mode
                                // Default => 0x10
}

void SSD1306_OLED::setAddressingMode(unsigned char d)
{
    writeCommand(0x20);         // Set Memory Addressing Mode
    writeCommand(d);            // Default => 0x02
                                // 0x00 => Horizontal Addressing Mode
                                // 0x01 => Vertical Addressing Mode
                                // 0x02 => Page Addressing Mode
}

void SSD1306_OLED::setColumnAddress(unsigned char a, unsigned char b)
{
    writeCommand(0x21);         // Set Column Address
    writeCommand(a);            // Default => 0x00 (Column Start Address)
    writeCommand(b);            // Default => 0x7F (Column End Address)
}

void SSD1306_OLED::setPageAddress(unsigned char a, unsigned char b)
{
    writeCommand(0x22);         // Set Page Address
    writeCommand(a);            // Default => 0x00 (Page Start Address)
    writeCommand(b);            // Default => 0x07 (Page End Address)
}

void SSD1306_OLED::setStartLine(unsigned char d)
{
    writeCommand(0x40|d);       // Set Display Start Line
                                // Default => 0x40 (0x00)
}

void SSD1306_OLED::setContrastControl(unsigned char d)
{
    writeCommand(0x81);         // Set Contrast Control
    writeCommand(d);            // Default => 0x7F
}

void SSD1306_OLED::setChargePump(unsigned char d)
{
    writeCommand(0x8D);         // Set Charge Pump
    writeCommand(0x10|d);       // Default => 0x10
                                // 0x10 (0x00) => Disable Charge Pump
                                // 0x14 (0x04) => Enable Charge Pump
}

void SSD1306_OLED::setSegmentRemap(unsigned char d)
{
    writeCommand(0xA0|d);       // Set Segment Re-Map
                                // Default => 0xA0
                                // 0xA0 (0x00) => Column Address 0 Mapped to SEG0
                                // 0xA1 (0x01) => Column Address 0 Mapped to SEG127
}

void SSD1306_OLED::setEntireDisplay(unsigned char d)
{
    writeCommand(0xA4|d);       // Set Entire Display On / Off
                                // Default => 0xA4
                                // 0xA4 (0x00) => Normal Display
                                // 0xA5 (0x01) => Entire Display On
}

void SSD1306_OLED::setInverseDisplay(unsigned char d)
{
    writeCommand(0xA6|d);       // Set Inverse Display On/Off
                                // Default => 0xA6
                                // 0xA6 (0x00) => Normal Display
                                // 0xA7 (0x01) => Inverse Display On
}

void SSD1306_OLED::setMultiplexRatio(unsigned char d)
{
    writeCommand(0xA8);         // Set Multiplex Ratio
    writeCommand(d);            // Default => 0x3F (1/64 Duty)
}

void SSD1306_OLED::setDisplayOnOff(unsigned char d)
{
    writeCommand(0xAE|d);       // Set Display On/Off
                                // Default => 0xAE
                                // 0xAE (0x00) => Display Off
                                // 0xAF (0x01) => Display On
}

void SSD1306_OLED::setStartPage(unsigned char d)
{
    writeCommand(0xB0|d);       // Set Page Start Address for Page Addressing Mode
                                // Default => 0xB0 (0x00)
}

void SSD1306_OLED::setCommonRemap(unsigned char d)
{
    writeCommand(0xC0|d);       // Set COM Output Scan Direction
                                // Default => 0xC0
                                // 0xC0 (0x00) => Scan from COM0 to 63
                                // 0xC8 (0x08) => Scan from COM63 to 0
}

void SSD1306_OLED::setDisplayOffset(unsigned char d)
{
    writeCommand(0xD3);         // Set Display Offset
    writeCommand(d);            // Default => 0x00
}

void SSD1306_OLED::setDisplayClock(unsigned char d)
{
    writeCommand(0xD5);         // Set Display Clock Divide Ratio / Oscillator Frequency
    writeCommand(d);            // Default => 0x80
                                // D[3:0] => Display Clock Divider
                                // D[7:4] => Oscillator Frequency
}

void SSD1306_OLED::setPrechargePeriod(unsigned char d)
{
    writeCommand(0xD9);         // Set Pre-Charge Period
    writeCommand(d);            // Default => 0x22 (2 Display Clocks [Phase 2] / 2 Display Clocks [Phase 1])
                                // D[3:0] => Phase 1 Period in 1~15 Display Clocks
                                // D[7:4] => Phase 2 Period in 1~15 Display Clocks
}

void SSD1306_OLED::setCommonConfig(unsigned char d)
{
    writeCommand(0xDA);         // Set COM Pins Hardware Configuration
    writeCommand(0x02|d);       // Default => 0x12 (0x10)
                                // Alternative COM Pin Configuration
                                // Disable COM Left/Right Re-Map
}

void SSD1306_OLED::setVCOMH(unsigned char d)
{
    writeCommand(0xDB);         // Set VCOMH Deselect Level
    writeCommand(d);            // Default => 0x20 (0.77*VCC)
}

void SSD1306_OLED::setNOP()
{
    writeCommand(0xE3);         // Command for No Operation
}

/* Fill the screen with a particular data pattern */
void SSD1306_OLED::fillScreen(unsigned char data)
{
    unsigned char i,j;

    for(i=0;i<8;i++)
    {
        setStartPage(i);
        setStartColumn(0x00);

        for(j=0;j<128;j++)
            writeData(data);
    }
}

/* Fill a specified block with a particular data pattern */
void SSD1306_OLED::fillBlock(unsigned char data, unsigned char startRow, unsigned char endRow, unsigned char startCol, unsigned char endCol)
{
    unsigned char i,j;

    for(i=startRow;i<(endRow+1);i++)
    {
        setStartPage(i);
        setStartColumn(startCol);

        for(j=0;j<endCol;j++)
            writeData(data);
    }
}

/* Draw a frame (border) around the OLED */
void SSD1306_OLED::drawFrame()
{
    unsigned char i,j;

    setStartPage(0x00);
    setStartColumn(xLevel);

    for(i=0;i<maxColumn;i++)
        writeData(0x01);

    setStartPage(0x07);
    setStartColumn(xLevel);

    for(i=0;i<maxColumn;i++)
        writeData(0x80);

    for(i=0;i<8;i++)
    {
        setStartPage(i);

        for(j=0;j<maxColumn;j+=(maxColumn-1))
        {
            setStartColumn(xLevel+j);
            writeData(0xFF);
        }
    }
}

/* Draw a 30x8 battery symbol with specified level */
void SSD1306_OLED::drawBatt(int level, int x, int row)
{
    if(level>100)
        level=100;
    else if(level<0)
        level=0;

    setStartPage(row);

    /* Draw battery outline */
    for(int i=0;i<27;i++)
    {
        /* Leave 1-pixel wide gaps for 25%, 50% and 75% on outline (approximate) */
        if(i!=7 && i!=13 && i!=20)
        {
            setStartColumn(x+1+i);
            writeData(0x41);
        }
    }

    setStartColumn(x);
    writeData(0x7F);

    setStartColumn(x+28);
    writeData(0x7F);

    setStartColumn(x+29);
    writeData(0x1C);

    /* 1 pixel per 4 percentage points */
    for(int i=0;i<level/4;i++)
    {
        /* Leave 1-pixel wide gaps for 25%, 50% and 75% on outline (approximate) */
        if(i!=6 && i!=12 && i!=19)
        {
            setStartColumn(x+2+i);
            writeData(0x5D);
        }
        else
        {
            setStartColumn(x+2+i);
            writeData(0x1C);
        }
    }
}

/* Print a single character from font.cpp */
void SSD1306_OLED::showFont57(char ascii, unsigned char row, unsigned char xPos)
{
    char *srcPointer = 0;
    unsigned char i;

    srcPointer = &fontData[(ascii-32)][0];

    setStartPage(row);
    setStartColumn(xPos);

    for(i=0;i<5;i++)
    {
        writeData(*srcPointer);
        srcPointer++;
    }
    writeData(0x00);
}

/* Display a null-terminated string on the OLED */
void SSD1306_OLED::showString(const char *dataPtr, unsigned char row, unsigned char xPos)
{
    char *srcPointer;

    srcPointer = (char*)dataPtr;
    showFont57(' ',row,xPos); // NBSP must be written first before the string start

    while(1)
    {
        showFont57(*srcPointer,row,xPos);
        srcPointer++;
        xPos+=6;
        if(*srcPointer == 0) break;
    }
}

/* Put the OLED to sleep to save power */
void SSD1306_OLED::sleep(unsigned char doSleep)
{
    switch(doSleep)
    {
        case 0:
            setDisplayOnOff(0x00);
            setEntireDisplay(0x01);
            break;
        case 1:
            setEntireDisplay(0x00);
            setDisplayOnOff(0x01);
            break;
    }
}
