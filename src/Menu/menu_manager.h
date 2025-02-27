#pragma once
/*
    This is a modification of the library created by Kris Kasprazak to use the ST7735 display
    Original Kris project is here https://github.com/KrisKasprzak/ILI9341_t3_Menu
*/
/*
  The MIT License (MIT)

  library writen by Kris Kasprzak
  
  Permission is hereby granted, free of charge, to any person obtaining a copy of
  this software and associated documentation files (the "Software"), to deal in
  the Software without restriction, including without limitation the rights to
  use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
  the Software, and to permit persons to whom the Software is furnished to do so,
  subject to the following conditions:
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
  FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
  COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
  IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

  On a personal note, if you develop an application or product using this library 
  and make millions of dollars, I'm happy for you!

	rev		date			author				change
	1.0		1/2022			kasprzak			initial code
	2.0   1/2022      kasprzak      added touch support
 	4.0   8/21/2024   kasprzak      fixed a nast bug if number of menu items = MAX_OPT in the .h

	// Website for generating icons
	// https://javl.github.io/image2cpp/


*/

#pragma once

#include <TFT_eSPI.h>
#include <SPI.h>

#undef TFT_MISO
#undef TFT_MOSI
#undef TFT_SCLK

#undef TFT_CS  
#undef TFT_DC  
#undef TFT_RST 

#define TFT_MISO  19  // Automatically assigned with ESP8266 if not defined
#define TFT_MOSI  23  // Automatically assigned with ESP8266 if not defined
#define TFT_SCLK  18  // Automatically assigned with ESP8266 if not defined

#define TFT_CS    5  // Chip select control pin D8
#define TFT_DC    19  // Data Command control pin
#define TFT_RST  -1     // Set TFT_RST to -1 if the display RESET is connected to NodeMCU RST or 3.3V

#define CS_PIN 5
#define DC_PIN 19
#define MOSI 23
#define CLK 18

#define MAX_OPT_1 15				// max elements in a menu, increase as needed
#define MAX_CHAR_LEN 30			// max chars in menus, increase as needed
#define TRIANGLE_H 3.7
#define TRIANGLE_W 2.5
#define MENU_C_DKGREY 0x4A49	// used for disable color, method to change
#define EXIT_TEXT "Exit"		// default text for menu exit text, change here or use a method
#define EDIT_TEXT "Edit"		// default text for menu exit text, change here or use a method

#define ICON_NONE 0
#define ICON_MONO 1
#define ICON_565  2

#define  BUTTON_PRESSED 1
#define  BUTTON_NOTPRESSED 0

class  EditMenu {
		
public:

	EditMenu(TFT_eSPI *Display,bool EnableTouch = false);

	void init(uint16_t TextColor, uint16_t BackgroundColor, 
		uint16_t HighlightTextColor, uint16_t HighlightColor,
		uint16_t SelectedTextColor, uint16_t SelectedColor,
		uint16_t MenuColumn, uint16_t ItemRowHeight,uint16_t MaxRow,
		const char *TitleText); //, const GFXfont &ItemFont, const GFXfont &TitleFont);

	int addNI(const char *ItemText, float Data, float LowLimit, float HighLimit, 
		float Increment, byte DecimalPlaces = 0, const char **ItemMenuText = NULL);

	int addMono(const char *ItemText, float Data, float LowLimit, float HighLimit, 
		float Increment, byte DecimalPlaces = 0,const char **ItemMenuText = NULL, 
		const unsigned char *Bitmap = nullptr, uint8_t BitmapWidth = 0, uint8_t BitmapHeight = 0);

	int add565(const char *ItemText, float Data, float LowLimit, float HighLimit, 
		float Increment, byte DecimalPlaces = 0, const char **ItemMenuText = NULL, 
		const uint16_t *Bitmap = nullptr, uint8_t BitmapWidth = 0, uint8_t BitmapHeight = 0);
	
	int selectRow();

	void draw();

	void MoveUp();

	void MoveDown();

	void setTitleColors(uint16_t TitleTextColor, uint16_t TitleFillColor);

	void setTitleBarSize(uint16_t TitleTop, uint16_t TitleLeft, uint16_t TitleWith, uint16_t TitleHeight);

	void setTitleText( char *TitleText,  char *ExitText);
	
	void setTitleText( char *TitleText,  char *ExitText,  char *EditText);

	void setTitleTextMargins(uint16_t LeftMargin, uint16_t TopMargin);

	void setIncrementDelay(uint16_t Delay);
	
	void setMenuBarMargins(uint16_t LeftMargin, uint16_t Width, uint16_t BorderRadius, uint16_t BorderThickness);

	void setItemColors( uint16_t DisableTextColor, uint16_t BorderColor, uint16_t EditModeBorderColor = 0);

	void setItemTextMargins(uint16_t LeftMargin, uint16_t TopMargin, uint16_t MenuMargin);

	void setItemText(int ItemID, const char *ItemText);

	void setIconMargins(uint16_t LeftMargin, uint16_t TopMargin);

	void SetItemValue(int ItemID, float ItemValue);

	void SetAllColors(uint16_t TextColor, uint16_t BackgroundColor, 
							uint16_t HighlightTextColor, uint16_t HighlightColor, uint16_t HighlightBorderColor,
							uint16_t SelectedTextColor, uint16_t SelectedColor, uint16_t SelectBorderColor,
							uint16_t DisableTextColor ,	uint16_t TitleTextColor, uint16_t TitleFillColor);

	void disable(int ID);

	void enable(int ID);

	bool getEnableState(int ID);
	
	int press(int16_t ScreenX, int16_t ScreenY);

	void drawRow(int ID);
		
	float value[MAX_OPT_1];

	int item;

private:

	void drawHeader(bool ShowEdit);
	
	void up();

	void down();

	void incrementUp();
	
	void incrementDown();

	void drawItems();

	void drawMonoBitmap(int16_t x, int16_t y, const unsigned char *bitmap, uint8_t w, uint8_t h, uint16_t color);

	void draw565Bitmap(int16_t x, int16_t y, const uint16_t *bitmap, uint8_t w, uint8_t h);
	
	TFT_eSPI *d;
	char itemlabel[MAX_OPT_1][MAX_CHAR_LEN];
	char title_text[MAX_CHAR_LEN];
	char exit_text[MAX_CHAR_LEN]; 
	char edit_text[MAX_CHAR_LEN];
	GFXfont itemf;
	GFXfont titlef;
	uint16_t i_text_color = 0, i_background_color = 0, i_highlight_text_color = 0, i_highlight_color = 0, i_selected_text_color = 0, i_selected_color = 0;	// item variables
	uint16_t title_bar_left = 0, title_bar_top = 0, title_bar_width = 0, title_bar_height = 0, title_text_color = 0, title_fill_color = 0, title_offset_x = 0, title_offset_y = 0;	// title variables
	// margins
	uint16_t i_max_row = 0, isx = 0, itx = 0, isy = 0, i_select_bar_height = 0, irw = 0, offset_text_menu_bar_y = 0, offset_text_menu_bar_x = 0, menu_margin= 0, icox = 0, icoy = 0;
	int i = 0;
	int totalID = 0;
	int MaxRow = 0;
	int currentID = 0;
	int current_sel_row = 0;
	byte debounce = 0;
	int sr = 0, prev_sel_row= 0;
	uint16_t col = 0;
	float data[MAX_OPT_1] = {};
	float low[MAX_OPT_1] = {};
	float high[MAX_OPT_1]= {};
	float inc[MAX_OPT_1] = {};
	byte dec[MAX_OPT_1] = {};
	
	char **itemtext[MAX_OPT_1];
	
	bool rowselected = false;
	bool haslist[MAX_OPT_1];
	bool enablestate[MAX_OPT_1];
	bool drawTitleFlag = true;
	bool redraw = false;
	uint16_t disable_text_color = 0;
	uint16_t temptColor = 0, bcolor, sbcolor;
	const unsigned char	*itemBitmap[MAX_OPT_1];
	const uint16_t *item565Bitmap[MAX_OPT_1];
	uint8_t bmp_w[MAX_OPT_1] = {0};
	uint8_t bmp_h[MAX_OPT_1] = {0};
	byte IconType[MAX_OPT_1];
	uint16_t  radius = 0;
	uint16_t thick = 0;
	uint16_t incdelay = 50;
	bool InputFromTouch = false;
	bool dh = true; 
};


class  ItemMenu {

	
public:
	ItemMenu(TFT_eSPI *Display, bool EnableTouch = false);
	
	void init(uint16_t TextColor, uint16_t BackgroundColor,
		uint16_t HighlightTextColor, uint16_t HighlightColor, 
		uint16_t ItemRowHeight,uint16_t MaxRow,
		const char *TitleText); //, const GFXfont &ItemFont, const GFXfont &TitleFont);
		
	int addNI(const char *ItemLabel);

	int addMono(const char *ItemLabel, const unsigned char *Bitmap, uint8_t BitmapWidth, uint8_t BitmapHeight );

	int add565(const char *ItemLabel, const uint16_t *Bitmap, uint8_t BitmapWidth, uint8_t BitmapHeight);

	void draw();
	
	void MoveUp();

	void MoveDown();

	int selectRow();

	void setTitleColors(uint16_t TitleTextColor, uint16_t TitleFillColor);

	void setTitleBarSize(uint16_t TitleTop, uint16_t TitleLeft, uint16_t TitleWith, uint16_t TitleHeight);

	void setTitleText( char *TitleText,  char *ExitText);
	
	void setTitleText(char *TitleText, char *ExitText, char *EditText);
	
	void setTitleTextMargins(uint16_t LeftMargin, uint16_t TopMargin);

	void setMenuBarMargins(uint16_t LeftMargin, uint16_t Width, byte BorderRadius, byte BorderThickness);

	void setItemColors( uint16_t DisableTextColor, uint16_t BorderColor);

	void setItemTextMargins(uint16_t LeftMargin, uint16_t TopMargin, uint16_t MenuMargin);

	void setItemText(int ItemID, const char *ItemText);

	void setIconMargins(uint16_t LeftMargin, uint16_t TopMargin);

	void SetAllColors(uint16_t TextColor, uint16_t BackgroundColor, uint16_t HighlightTextColor, uint16_t HighlightColor, 
		uint16_t HighLightBorderColor, uint16_t DisableTextColor, uint16_t TitleTextColor, uint16_t TitleFillColor);

	void disable(int ID);

	void enable(int ID);

	bool getEnableState(int ID);

	int press(int16_t ScreenX, int16_t ScreenY);

	void drawRow(int ID, uint8_t style);

	float value[MAX_OPT_1];

	int item;

private:

	void drawHeader(bool ShowEdit);

	void drawItems();
	
	void drawMonoBitmap(int16_t x, int16_t y, const unsigned char *bitmap, uint8_t w, uint8_t h, uint16_t color);

	void draw565Bitmap(int16_t x, int16_t y, const uint16_t *Bitmap , uint8_t w, uint8_t h);

	TFT_eSPI *d;
	//bool enabletouch;
	char itemlabel[MAX_OPT_1][MAX_CHAR_LEN];
	char title_text[MAX_CHAR_LEN];
	char exit_text[MAX_CHAR_LEN];
	char edit_text[MAX_CHAR_LEN];
	GFXfont itemf;
	GFXfont titlef;
	uint16_t bkgr = 0, isx = 0, itx = 0, isy = 0;
	uint16_t i_select_bar_height = 0, i_text_color = 0, i_background_color = 0, i_highlight_color = 0, i_highlight_text_color = 0;
	uint16_t isc = 0, i_max_row = 0, irw = 0, offset_text_menu_bar_y = 0, offset_text_menu_bar_x = 0;	// item variables
	uint16_t title_bar_left = 0, title_bar_top = 0, title_bar_width = 0, title_bar_height = 0, title_text_color = 0, title_fill_color = 0;
	uint16_t title_offset_x = 0, title_offset_y = 0, icox = 0, icoy = 0, di = 0, menu_margin= 0;	// title variables
	uint16_t disable_text_color = 0, difc = 0, temptColor = 0, bcolor = 0;
	bool hasIcon = false, moreup = false, moredown = false;
	int i = 0;
	int totalID = 0;
	int MaxRow = 0;
	int currentID = 0;
	int current_sel_row = 0;
	byte debounce = 0;
	int sr = 0, prev_sel_row = 0;
	const unsigned char *itemBitmap[MAX_OPT_1];
	const uint16_t *item565Bitmap[MAX_OPT_1];
	bool rowselected = false;
	bool drawTitleFlag = true;
	bool redraw = false;
	bool enablestate[MAX_OPT_1];
	uint8_t bmp_w[MAX_OPT_1] = {0};
uint8_t bmp_h[MAX_OPT_1] = {0};
	byte IconType[MAX_OPT_1];
	byte radius, thick;
	bool InputFromTouch = false;
	bool dh = true; // draw header
	
};

