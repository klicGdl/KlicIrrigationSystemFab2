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

  rev   date      author        change
  1.0   1/2022      kasprzak      initial code
  2.0   1/2022      kasprzak      added touch support
  3.0   2/2023      kasprzak      modified lib to work with mech and touch

*/

#include <Arduino.h>
#include "menu_manager.h"

#include <TFT_eSPI.h>
#include <SPI.h>


EditMenu::EditMenu(TFT_eSPI *Display, bool EnableTouch)
{
  d = Display;
  // I no longer require you to choose mehcanical or touch
  // the library now supports both, I'm
  // keeping the arg to keep implementations from breaking
}

void EditMenu::init(uint16_t TextColor, uint16_t BackgroundColor,
                    uint16_t HighlightTextColor, uint16_t HighlightColor,
                    uint16_t SelectedTextColor, uint16_t SelectedColor,
                    uint16_t MenuColumn, uint16_t ItemRowHeight, uint16_t MaxRow,
                    const char *TitleText)
{ //, const GFXfont &ItemFont, const GFXfont &TitleFont) {

  i_text_color = TextColor;
  i_background_color = BackgroundColor;
  i_highlight_text_color = HighlightTextColor;
  i_highlight_color = HighlightColor;
  i_selected_text_color = SelectedTextColor;
  i_selected_color = SelectedColor;

  bcolor = TextColor;
  sbcolor = TextColor;
  title_bar_top = 0;          // title bar top
  title_bar_left = 0;          // title bar left
  title_bar_height = 40;         // title bar height
  title_bar_width = d->width(); // title bar width, default to screen width
  title_offset_x = 0;          // x pixel offset for title text to manage centering
  title_offset_y = 0;          // y pixel offset for title text to manage height centering
  isx = 10;         // where to start the menu item from left, default indent 10
  menu_margin= 5;

  isy = title_bar_top + title_bar_height + menu_margin; // where to start the menu item from top, note because array is 1 based we start drawing down a row--so back off a row, but pad 10

  i_select_bar_height= ItemRowHeight; // select bar height
  irw = title_bar_width - isx;     // select bar width, default to full width
  col = MenuColumn;
  i_max_row = MaxRow; // user has to indicate this
  offset_text_menu_bar_x = 0;      // pixels to offset text in menu bar
  offset_text_menu_bar_y = 0;      // pixels to offset text in menu bar
  // itemf = ItemFont;     // item font
  // titlef = TitleFont;     // title font
  title_text_color = TextColor;      // title text color
  title_fill_color = HighlightColor; // title fill color
  disable_text_color = MENU_C_DKGREY;
  strncpy(title_text, TitleText, MAX_CHAR_LEN);
  strncpy(exit_text, EXIT_TEXT, MAX_CHAR_LEN);
  strncpy(edit_text, EDIT_TEXT, MAX_CHAR_LEN);
  currentID = 0; // id of current highlighted or selected item (1 to ID)
  current_sel_row = 0;        // current selected row on the screen (1 to mr-1)
  totalID = 0;   // maximum number of items (where first position is 1)
  sr = 0;        // draw offset for the menu array where first postion is 0
  prev_sel_row = 1;        // previous selected rown (1 to mr - 1)
  rowselected = false;
  enablestate[0] = true;
  radius = 0;
  thick = 0;
}

int EditMenu::addNI(const char *ItemText, float Data, float LowLimit, float HighLimit, float Increment,
                    byte DecimalPlaces, const char **ItemMenuText)
{

  totalID++;
  strcpy(itemlabel[totalID], ItemText);
  data[totalID] = Data;
  low[totalID] = LowLimit;
  high[totalID] = HighLimit;
  inc[totalID] = Increment;
  dec[totalID] = DecimalPlaces;
  haslist[totalID] = false;
  value[totalID] = Data;

  if (ItemMenuText)
  {
    haslist[totalID] = true;
    itemtext[totalID] = (char **)ItemMenuText;
  }

  IconType[totalID] = ICON_NONE;
  bmp_w[totalID] = 0;
  bmp_h[totalID] = 0;

  enablestate[totalID] = true;
  return (totalID);
}

int EditMenu::addMono(const char *ItemText, float Data, float LowLimit, float HighLimit, float Increment,
                      byte DecimalPlaces, const char **ItemMenuText,
                      const unsigned char *Bitmap, uint8_t BitmapWidth, uint8_t BitmapHeight)
{

  totalID++;
  strcpy(itemlabel[totalID], ItemText);
  data[totalID] = Data;
  low[totalID] = LowLimit;
  high[totalID] = HighLimit;
  inc[totalID] = Increment;
  dec[totalID] = DecimalPlaces;
  haslist[totalID] = false;
  value[totalID] = Data;

  if (ItemMenuText)
  {
    haslist[totalID] = true;
    itemtext[totalID] = (char **)ItemMenuText;
  }

  itemBitmap[totalID] = Bitmap;
  bmp_w[totalID] = BitmapWidth;
  bmp_h[totalID] = BitmapHeight;
  this->IconType[totalID] = ICON_MONO;

  enablestate[totalID] = true;
  return (totalID);
}

int EditMenu::add565(const char *ItemText, float Data, float LowLimit, float HighLimit, float Increment,
                     byte DecimalPlaces, const char **ItemMenuText,
                     const uint16_t *Bitmap, uint8_t BitmapWidth, uint8_t BitmapHeight)
{

  totalID++;
  strcpy(itemlabel[totalID], ItemText);
  data[totalID] = Data;
  low[totalID] = LowLimit;
  high[totalID] = HighLimit;
  inc[totalID] = Increment;
  dec[totalID] = DecimalPlaces;
  haslist[totalID] = false;
  value[totalID] = Data;

  if (ItemMenuText)
  {
    haslist[totalID] = true;
    itemtext[totalID] = (char **)ItemMenuText;
  }

  item565Bitmap[totalID] = Bitmap;
  bmp_w[totalID] = BitmapWidth;
  bmp_h[totalID] = BitmapHeight;
  IconType[totalID] = ICON_565;

  enablestate[totalID] = true;
  return (totalID);
}

int EditMenu::selectRow()
{

  InputFromTouch = false;
  if (currentID == 0)
  {
    // trigger to exit out of the menu
    item = 0;
    return 0;
  }
  // otherwise this is the trigger to enable editing in the row
  rowselected = !rowselected;
  drawRow(currentID);
  item = currentID;
  return currentID;
}

void EditMenu::drawHeader(bool ShowEdit)
{

  // if current_sel_row == 0 you must draw header
  // if current_sel_row != 0 and was just draw, don't draw again
  if (current_sel_row == 0)
  {
    dh = true;
  }
  else if (dh)
  {
    dh = false;
  }
  else
  {
    return;
  }

  // d->setFont(titlef);

  if (ShowEdit)
  {
    // draw new menu bar
    d->setCursor(title_bar_left + title_offset_x, title_bar_top + title_offset_y);
    //d->fillRect(title_bar_left, title_bar_top, title_bar_width, title_bar_height, i_highlight_color);
    d->fillRectVGradient(title_bar_left,title_bar_top,title_bar_width,title_bar_height,i_highlight_color,TFT_NAVY);
    d->setTextColor(title_text_color);
    d->print(edit_text);
    // up arrow
    d->fillTriangle(title_bar_left + 10 + 5, title_bar_top + title_bar_height - 5, // bottom left
                    title_bar_left + 10 - 5, title_bar_top + title_bar_height - 5, // bottom right
                    title_bar_left + 10, title_bar_top + 5,            // center
                    title_text_color);
    // down arrow
    d->fillTriangle(title_bar_width - 10 + 5, title_bar_top + 5,  // top left
                    title_bar_width - 10 - 5, title_bar_top + 5,  // top right
                    title_bar_width - 10, title_bar_top + title_bar_height - 5, // center
                    title_text_color);
  }
  else
  {

    if (current_sel_row == 0)
    {
      // draw new menu bar
      // d->setCursor(title_bar_left + title_offset_x, title_bar_top + title_offset_y);
      //d->fillRect(title_bar_left, title_bar_top, title_bar_width, title_bar_height, i_highlight_color);
      d->fillRectVGradient(title_bar_left,title_bar_top,title_bar_width,title_bar_height,i_highlight_color,TFT_NAVY);
      d->setTextColor(title_text_color);
      // d->print(exit_text);
      d->setTextDatum(MC_DATUM);
      d->drawString(exit_text,d->width()/2,title_bar_height/2); 
    }
    else
    {
      // draw new menu bar
      d->setCursor(title_bar_left + title_offset_x, title_bar_top + title_offset_y);
      //d->fillRect(title_bar_left, title_bar_top, title_bar_width, title_bar_height, title_fill_color);
      d->fillRectVGradient(title_bar_left,title_bar_top,title_bar_width,title_bar_height,title_fill_color,TFT_CYAN);
      d->setTextColor(title_text_color);
      //d->print(title_text);
      d->setTextDatum(MC_DATUM);
      d->drawString(title_text,d->width()/2,title_bar_height/2); 
    }
    if (totalID > i_max_row)  // max rows
    {
      // up arrow
      d->fillTriangle(title_bar_left + 10 + 5, title_bar_top + title_bar_height - 5, // bottom left
                      title_bar_left + 10 - 5, title_bar_top + title_bar_height - 5, // bottom right
                      title_bar_left + 10, title_bar_top + 5,            // center
                      title_text_color);
      // down arrow
      d->fillTriangle(title_bar_width - 10 + 5, title_bar_top + 5,  // top left
                      title_bar_width - 10 - 5, title_bar_top + 5,  // top right
                      title_bar_width - 10, title_bar_top + title_bar_height - 5, // center
                      title_text_color);
    }
  }
}

int EditMenu::press(int16_t ScreenX, int16_t ScreenY)
{

  int bs;
  InputFromTouch = true;

  // check if header pressed
  if ((ScreenX > title_bar_left) && (ScreenX < (title_bar_width + title_bar_left)) && (ScreenY > title_bar_top) && (ScreenY < (title_bar_top + title_bar_height)))
  {

    if (ScreenX < (title_bar_left + 25 + 15))
    {
      // up arrow
      MoveDown();
      return -1;
    }

    if (ScreenX > (title_bar_width - 25 - 15))
    {
      // down arrow
      MoveUp();
      return -1;
    }

    // above will allow arrow up down but
    // let's not allow menu exit until they are done editing the line item
    if (rowselected)
    {
      return -1;
    }
    // not up / down and nothing selected, allow menu exit
    return 0;
  }

  dh = true;
  // clear the previous row (this may happen if user scroll to a row
  // with a mechanical device but presses another row
  drawRow(currentID);

  for (i = 1; i <= i_max_row; i++) // i_max_row = max rows
  {
    bs = icox + bmp_w[i + sr] + isx;
    itx = bs + offset_text_menu_bar_x;

    if (
        (ScreenX > bs) &&
        (ScreenY > (isy + (i_select_bar_height* (i - 1)))) &&
        (ScreenX < (irw)) &&
        (ScreenY < (isy + i_select_bar_height+ (i_select_bar_height* (i - 1)))))
    {
      if (!enablestate[i + sr])
      {
        return -1;
      }
      delay(100);
      if (rowselected)
      {
        if (i == current_sel_row)
        {
          rowselected = !rowselected;
          item = i + sr;
          current_sel_row = i;
          currentID = i + sr;
          drawHeader(false);
          return currentID;
        }
      }
      else
      {
        rowselected = !rowselected;
        currentID = i + sr;
        current_sel_row = i;
        drawHeader(true);
        return currentID;
      }
    }
  }
  return -1;
}

void EditMenu::up()
{
  current_sel_row--;
  currentID--;
  drawItems();
  if (enablestate[currentID] == false)
  {
    while (enablestate[currentID] == false)
    {
      current_sel_row--;
      currentID--;
      drawItems();
    }
  }
}

void EditMenu::down()
{

  current_sel_row++;
  currentID++;
  drawItems();
  if (enablestate[currentID] == false)
  {
    while (enablestate[currentID] == false)
    {
      current_sel_row++;
      currentID++;
      drawItems();
    }
  }
}

void EditMenu::MoveUp()
{

  if (rowselected)
  {
    incrementUp();
  }
  else
  {
    up();
  }
}

void EditMenu::MoveDown()
{

  if (rowselected)
  {
    incrementDown();
  }
  else
  {
    down();
  }
}

void EditMenu::draw()
{
  dh = true;
  drawItems();
}

void EditMenu::drawItems()
{

  int bs;

  if (i_max_row > totalID)  // i_max_row = max rows
  {
    i_max_row = totalID;
  }
  redraw = false;
  // determine if we need to pan or just increment the list

  if ((currentID > totalID) && (sr >= 0))
  {
    // up to top
    current_sel_row = 0;
    currentID = 0;
    sr = 0;
    redraw = true;
  }
  else if ((current_sel_row < 0) && (sr == 0))
  {
    //  pan whole menu to bottom
    current_sel_row = i_max_row;
    currentID = totalID;
    sr = totalID - i_max_row;
    redraw = true;
  }
  else if ((current_sel_row > i_max_row) && ((sr + current_sel_row) > totalID))
  {
    //  pan whole menu to top
    current_sel_row = 1;
    currentID = 1;
    sr = 0;
    redraw = true;
  }
  else if ((current_sel_row > i_max_row) && (sr >= 0))
  {
    // scroll whole list up one by one
    sr++;
    current_sel_row = i_max_row;
    redraw = true;
  }
  else if ((current_sel_row < 1) && (sr > 0))
  {
    // scroll whole list down one by one
    sr--;
    current_sel_row = 1;
    redraw = true;
  }

  drawHeader(false);

  // now draw the items in the rows
  for (i = 1; i <= i_max_row; i++)
  {
    // menu bar start
    bs = icox + bmp_w[i + sr] + isx;

    // text start
    itx = bs + offset_text_menu_bar_x;
    if (enablestate[i + sr])
    {
      temptColor = i_text_color;
    }
    else
    {
      temptColor = disable_text_color;
    }

    if (redraw)
    {
      // scroll so blank out every row including icon since row will get scrolled
      // d->fillRect(bs, isy - i_select_bar_height+ (i_select_bar_height* i), irw - bs, i_select_bar_height , i_background_color); // back color
      d->fillRect(icox, isy - i_select_bar_height+ (i_select_bar_height* i), irw, i_select_bar_height , i_background_color); // back color
    }

    if (i == prev_sel_row)
    {
      // maybe just row change so blank previous
      d->fillRect(bs, isy - i_select_bar_height+ (i_select_bar_height* prev_sel_row), irw - bs, i_select_bar_height , i_background_color); // back color
    }

    if ((i == current_sel_row) && (!InputFromTouch))
    {

      if (radius > 0)
      {
        d->fillRoundRect(bs, isy - i_select_bar_height+ (i_select_bar_height* i), irw - bs, i_select_bar_height , radius, bcolor);
        d->fillRoundRect(bs + thick, isy - i_select_bar_height+ (i_select_bar_height* i) + thick, irw - bs - (2 * thick), i_select_bar_height- (2 * thick), radius, i_highlight_color);
      }
      else
      {
        d->fillRect(bs, isy - i_select_bar_height+ (i_select_bar_height* i), irw - bs, i_select_bar_height , bcolor);
        d->fillRect(bs + thick, isy - i_select_bar_height+ (i_select_bar_height* i) + thick, irw - bs - (2 * thick), i_select_bar_height - (2 * thick), i_highlight_color);
      }

      temptColor = i_highlight_text_color;
    }

    // write bitmap

    if (this->IconType[i + sr] == ICON_MONO)
    {
      drawMonoBitmap(icox, icoy + isy - i_select_bar_height + (i_select_bar_height * i), this->itemBitmap[i + sr], bmp_w[i + sr], bmp_h[i + sr], i_text_color);
    }
    else if (IconType[i + sr] == ICON_565)
    {
      draw565Bitmap(icox, icoy + isy - i_select_bar_height + (i_select_bar_height * i), item565Bitmap[i + sr], bmp_w[i + sr], bmp_h[i + sr]);
    }

    // write text
    d->setTextColor(temptColor);
    d->setCursor(itx, isy - i_select_bar_height + (i_select_bar_height * i) + offset_text_menu_bar_y);
    d->print(itemlabel[i + sr]);

    // write new val
    d->setCursor(col, isy - i_select_bar_height + (i_select_bar_height * i) + offset_text_menu_bar_y);
    if (haslist[i + sr])
    {
      d->print(itemtext[i + sr][(int)data[i + sr]]);
    }
    else
    {
      d->print(data[i + sr], dec[i + sr]);
    }
  }
  InputFromTouch = false;
  prev_sel_row = current_sel_row;
}

void EditMenu::drawRow(int ID)
{

  int bs;
  int hr = ID - sr;

  uint16_t textcolor, backcolor, sbackcolor;

  // compute starting place for text
  itx = isx + icox + bmp_w[ID];
  textcolor = i_text_color;

  if (ID <= 0)
  {
    return;
  }

  bs = icox + bmp_w[hr] + isx;

  if (ID <= sr)
  {
    // item is off screen
    return;
  }

  if (ID > (sr + i_max_row))
  {
    // item is off screen
    return;
  }

  if (InputFromTouch)
  {

    if (ID == currentID)
    {
      if (!rowselected)
      {
        textcolor = i_text_color;
        backcolor = i_background_color;
        sbackcolor = i_background_color;
      }
      else
      {
        // case draw row is the selected row
        textcolor = i_selected_text_color;
        backcolor = i_selected_color;
        sbackcolor = sbcolor;
      }
    }
    else
    {
      // case draw row is not the selected row
      textcolor = i_text_color;
      backcolor = i_background_color;
      sbackcolor = i_background_color;
    }
  }
  else
  {
    if ((current_sel_row + sr) == ID)
    {

      if (!rowselected)
      {
        // case 1 draw som is draw row AND highlighted
        textcolor = i_highlight_text_color;
        backcolor = i_highlight_color;
        sbackcolor = bcolor;
      }
      else if (rowselected)
      {
        // case 2 = current row is draw row AND highlighted
        textcolor = i_selected_text_color;
        backcolor = i_selected_color;
        sbackcolor = i_background_color;
      }
    }
    else
    {
      // current row is not selected
      if (enablestate[ID])
      {
        textcolor = i_text_color;
      }
      else
      {
        textcolor = disable_text_color;
      }
      backcolor = i_background_color;
      sbackcolor = i_background_color;
    }
  }

  if (ID == 0)
  {
    if (rowselected)
    {
      // draw new menu bar
      //d->setCursor(title_bar_left + title_offset_x, title_bar_top + title_offset_y);
      //d->fillRect(title_bar_left, title_bar_top, title_bar_width, title_bar_height, i_highlight_color);
      d->fillRectVGradient(title_bar_left,title_bar_top,title_bar_width,title_bar_height,i_highlight_color,TFT_CYAN);
      d->setTextColor(title_text_color);
      //d->print(title_text);
      d->setTextDatum(MC_DATUM);
      d->drawString(title_text,d->width()/2,title_bar_height/2); 
    }
    else
    {
      // draw new menu bar
      //d->setCursor(title_bar_left + title_offset_x, title_bar_top + title_offset_y);
      //d->fillRect(title_bar_left, title_bar_top, title_bar_width, title_bar_height, title_fill_color);
      d->fillRectVGradient(title_bar_left,title_bar_top,title_bar_width,title_bar_height,title_fill_color,TFT_CYAN);
      d->setTextColor(title_text_color);
      //d->print(title_text);
      d->setTextDatum(MC_DATUM);
      d->drawString(title_text,d->width()/2,title_bar_height/2); 
    }
  }
  else
  {

    if (!InputFromTouch)
    {
      // non touch
      if (radius > 0)
      {
        d->fillRoundRect(bs, isy - i_select_bar_height + (i_select_bar_height * hr), irw - bs, i_select_bar_height , radius, sbackcolor);
        d->fillRoundRect(bs + thick, isy - i_select_bar_height + (i_select_bar_height * hr) + thick, irw - bs - (2 * thick), i_select_bar_height - (2 * thick), radius, backcolor);
      }
      else
      {
        d->fillRect(bs, isy - i_select_bar_height + (i_select_bar_height * hr), irw - bs, i_select_bar_height , sbackcolor);
        d->fillRect(bs + thick, isy - i_select_bar_height + (i_select_bar_height * hr) + thick, irw - bs - (2 * thick), i_select_bar_height - (2 * thick), backcolor);
      }
    }

    else
    {
      // touch
      if (rowselected)
      {

        if (radius > 0)
        {
          d->fillRoundRect(bs, isy - i_select_bar_height + (i_select_bar_height * hr), irw - bs, i_select_bar_height , radius, sbackcolor);
          d->fillRoundRect(bs + thick, isy - i_select_bar_height + (i_select_bar_height * hr) + thick, irw - bs - (2 * thick), i_select_bar_height - (2 * thick), radius, backcolor);
        }
        else
        {
          d->fillRect(bs, isy - i_select_bar_height + (i_select_bar_height * hr), irw - bs, i_select_bar_height , sbackcolor);
          d->fillRect(bs + thick, isy - i_select_bar_height + (i_select_bar_height * hr) + thick, irw - bs - (2 * thick), i_select_bar_height - (2 * thick), backcolor);
        }
      }
      else
      {

        d->fillRect(bs, isy - i_select_bar_height + (i_select_bar_height * hr), irw - bs, i_select_bar_height , i_background_color); // back color
      }
    }
    if (!enablestate[ID])
    {
      textcolor = disable_text_color;
    }
    // write text
    itx = bs + offset_text_menu_bar_x;
    // d->setFont(itemf);
    d->setTextColor(textcolor);
    d->setCursor(itx, isy - i_select_bar_height + (i_select_bar_height * hr) + offset_text_menu_bar_y);
    d->print(itemlabel[ID]);

    d->setCursor(col, isy - i_select_bar_height + (i_select_bar_height * (ID - sr)) + offset_text_menu_bar_y);
    if (haslist[ID])
    {
      d->print(itemtext[ID][(int)data[ID]]);
    }
    else
    {
      d->print(data[ID], dec[ID]);
    }

    // write bitmap
    if (IconType[ID] == ICON_MONO)
    {
      drawMonoBitmap(icox, icoy + isy - i_select_bar_height + (i_select_bar_height * (ID - sr)), itemBitmap[ID], bmp_w[ID], bmp_h[ID], temptColor);
    }
    else if (IconType[ID] == ICON_565)
    {
      draw565Bitmap(icox, icoy + isy - i_select_bar_height + (i_select_bar_height * (ID - sr)), item565Bitmap[ID], bmp_w[ID], bmp_h[ID]);
    }
  }
}

void EditMenu::setIncrementDelay(uint16_t Delay)
{

  incdelay = Delay;
}

void EditMenu::incrementUp()
{
  // d->setFont(itemf);
  d->setTextColor(i_selected_text_color);

  if (haslist[currentID])
  {

    if ((data[currentID] + inc[currentID]) < high[currentID])
    {
      data[currentID] += inc[currentID];
      d->fillRect(col, isy - i_select_bar_height + (i_select_bar_height * current_sel_row) + thick, irw - col - (2 * thick), i_select_bar_height - (2 * thick), i_selected_color);
      d->setCursor(col, isy - i_select_bar_height + (i_select_bar_height * current_sel_row) + offset_text_menu_bar_y);
      d->print(itemtext[currentID][(int)data[currentID]]);
    }
    else
    {
      data[currentID] = low[currentID];
      d->fillRect(col, isy - i_select_bar_height + (i_select_bar_height * current_sel_row) + thick, irw - col - (2 * thick), i_select_bar_height - (2 * thick), i_selected_color);
      d->setCursor(col, isy - i_select_bar_height + (i_select_bar_height * current_sel_row) + offset_text_menu_bar_y);
      d->print(itemtext[currentID][(int)data[currentID]]);
    }
  }
  else
  {

    data[currentID] += inc[currentID];
    if (data[currentID] > high[currentID])
    {
      data[currentID] = low[currentID];
    }
    d->fillRect(col, isy - i_select_bar_height + (i_select_bar_height * current_sel_row) + thick, irw - col - (2 * thick), i_select_bar_height - (2 * thick), i_selected_color);
    d->setCursor(col, isy - i_select_bar_height + (i_select_bar_height * current_sel_row) + offset_text_menu_bar_y);
    d->print(data[currentID], dec[currentID]);
  }

  if (IconType[currentID] == ICON_MONO)
  {
    drawMonoBitmap(icox, icoy + isy - i_select_bar_height + (i_select_bar_height * current_sel_row), itemBitmap[currentID], bmp_w[currentID], bmp_h[currentID], temptColor);
  }
  else if (IconType[currentID] == ICON_565)
  {
    draw565Bitmap(icox, icoy + isy - i_select_bar_height + (i_select_bar_height * current_sel_row), item565Bitmap[currentID], bmp_w[currentID], bmp_h[currentID]);
  }
  delay(incdelay);
  value[currentID] = data[currentID];

  item = currentID;
}

void EditMenu::incrementDown()
{

  // d->setFont(itemf);
  d->setTextColor(i_selected_text_color);

  if (haslist[currentID])
  {
    if ((data[currentID] - inc[currentID]) >= low[currentID])
    {
      data[currentID] -= inc[currentID];
      d->fillRect(col, isy - i_select_bar_height + (i_select_bar_height * current_sel_row) + thick, irw - col - (2 * thick), i_select_bar_height - (2 * thick), i_selected_color);
      d->setCursor(col, isy - i_select_bar_height + (i_select_bar_height * current_sel_row) + offset_text_menu_bar_y);
      d->print(itemtext[currentID][(int)data[currentID]]);
    }
    else
    {
      data[currentID] = high[currentID] - 1;
      d->fillRect(col, isy - i_select_bar_height + (i_select_bar_height * current_sel_row) + thick, irw - col - (2 * thick), i_select_bar_height - (2 * thick), i_selected_color);
      d->setCursor(col, isy - i_select_bar_height + (i_select_bar_height * current_sel_row) + offset_text_menu_bar_y);
      d->print(itemtext[currentID][(int)data[currentID]]);
    }
  }
  else
  {
    data[currentID] -= inc[currentID];
    if (data[currentID] < low[currentID])
    {
      data[currentID] = high[currentID];
    }
    d->fillRect(col, isy - i_select_bar_height + (i_select_bar_height * current_sel_row) + thick, irw - col - (2 * thick), i_select_bar_height - (2 * thick), i_selected_color);
    d->setCursor(col, isy - i_select_bar_height + (i_select_bar_height * current_sel_row) + offset_text_menu_bar_y);
    d->print(data[currentID], dec[currentID]);
  }

  if (IconType[currentID] == ICON_MONO)
  {
    drawMonoBitmap(icox, icoy + isy - i_select_bar_height + (i_select_bar_height * current_sel_row), itemBitmap[currentID], bmp_w[currentID], bmp_h[currentID], temptColor);
  }
  else if (IconType[currentID] == ICON_565)
  {
    draw565Bitmap(icox, icoy + isy - i_select_bar_height + (i_select_bar_height * current_sel_row), item565Bitmap[currentID], bmp_w[currentID], bmp_h[currentID]);
  }
  delay(incdelay);
  value[currentID] = data[currentID];
  item = currentID;
}

void EditMenu::setTitleColors(uint16_t TitleTextColor, uint16_t TitleFillColor)
{
  title_text_color = TitleTextColor;
  title_fill_color = TitleFillColor;
}

void EditMenu::setTitleBarSize(uint16_t TitleTop, uint16_t TitleLeft, uint16_t TitleWith, uint16_t TitleHeight)
{
  title_bar_top = TitleTop;
  title_bar_left = TitleLeft;
  title_bar_width = TitleWith;
  title_bar_height = TitleHeight;
}

void EditMenu::setTitleText(char *TitleText, char *ExitText)
{
  strncpy(title_text, TitleText, MAX_CHAR_LEN);
  strncpy(exit_text, ExitText, MAX_CHAR_LEN);
}

void EditMenu::setTitleText(char *TitleText, char *ExitText, char *EditText)
{
  strncpy(title_text, TitleText, MAX_CHAR_LEN);
  strncpy(edit_text, EditText, MAX_CHAR_LEN);
}

void EditMenu::setTitleTextMargins(uint16_t LeftMargin, uint16_t TopMargin)
{
  title_offset_x = LeftMargin; // pixels to offset text in menu bar
  title_offset_y = TopMargin;  // pixels to offset text in menu bar
}

void EditMenu::setMenuBarMargins(uint16_t LeftMargin, uint16_t Width, uint16_t BorderRadius, uint16_t BorderThickness)
{
  isx = LeftMargin;  // pixels to offset text in menu bar
  irw = Width - isx; // pixels to offset text in menu bar
  radius = BorderRadius;
  thick = BorderThickness;
}

void EditMenu::setItemColors(uint16_t DisableTextColor, uint16_t HighlightBorderColor, uint16_t SelectBorderColor)
{
  disable_text_color = DisableTextColor;
  bcolor = HighlightBorderColor;
  sbcolor = SelectBorderColor;
}

void EditMenu::SetAllColors(uint16_t TextColor, uint16_t BackgroundColor,
                            uint16_t HighlightTextColor, uint16_t HighlightColor, uint16_t HighlightBorderColor,
                            uint16_t SelectedTextColor, uint16_t SelectedColor, uint16_t SelectBorderColor,
                            uint16_t DisableTextColor, uint16_t TitleTextColor, uint16_t TitleFillColor)
{

  i_text_color = TextColor;
  i_background_color = BackgroundColor;
  i_highlight_text_color = HighlightTextColor;
  i_highlight_color = HighlightColor;
  bcolor = HighlightBorderColor;
  i_selected_text_color = SelectedTextColor;
  i_selected_color = SelectedColor;
  sbcolor = SelectBorderColor;
  title_text_color = TitleTextColor;
  title_fill_color = TitleFillColor;
  disable_text_color = DisableTextColor;
}

void EditMenu::setItemTextMargins(uint16_t LeftMargin, uint16_t TopMargin, uint16_t MenuMargin)
{
  offset_text_menu_bar_x = LeftMargin; // pixels to offset text in menu bar
  offset_text_menu_bar_y = TopMargin;  // pixels to offset text in menu bar
  menu_margin= MenuMargin;
  isy = title_bar_top + title_bar_height + menu_margin;
}

void EditMenu::setItemText(int ID, const char *ItemText)
{
  strcpy(itemlabel[ID], ItemText);
  drawRow(ID);
}

void EditMenu::setIconMargins(uint16_t LeftMargin, uint16_t TopMargin)
{
  icox = LeftMargin; // pixels to offset text in menu bar
  icoy = TopMargin;  // pixels to offset text in menu bar
}

void EditMenu::disable(int ID)
{
  enablestate[ID] = false;
}

void EditMenu::enable(int ID)
{
  enablestate[ID] = true;
}

bool EditMenu::getEnableState(int ID)
{
  return enablestate[ID];
}

void EditMenu::SetItemValue(int ID, float ItemValue)
{
  value[ID] = ItemValue;
  data[ID] = ItemValue;
}

void EditMenu::drawMonoBitmap(int16_t x, int16_t y, const unsigned char *bitmap, uint8_t w, uint8_t h, uint16_t color)
{

  uint8_t sbyte = 0;
  uint8_t byteWidth = 0;
  int jj, ii;

  byteWidth = (w + 7) / 8;

  for (jj = 0; jj < h; jj++)
  {
    for (ii = 0; ii < w; ii++)
    {
      if (ii & 7)
        sbyte <<= 1;
      else
        sbyte = pgm_read_byte(bitmap + jj * byteWidth + ii / 8);
      if (sbyte & 0x80)
        d->drawPixel(x + ii, y + jj, color);
    }
  }
}

void EditMenu::draw565Bitmap(int16_t x, int16_t y, const uint16_t *bitmap, uint8_t w, uint8_t h)
{

  uint16_t offset = 0;

  int j, i;

  for (i = 0; i < h; i++)
  {
    for (j = 0; j < w; j++)
    {
      d->drawPixel(j + x, i + y, bitmap[offset]);
      offset++;
    }
  }
}

/*

  object type to create a simple selection only menu unlike previous where selecting a line item would allow in-line editing


*/

ItemMenu::ItemMenu(TFT_eSPI *Display, bool EnableTouch)
{
  d = Display;
  // we no longer use EnableTouch, but keep it in the arg list to not break
  // implementations
}

void ItemMenu::init(uint16_t TextColor, uint16_t BackgroundColor,
                    uint16_t HighlightTextColor, uint16_t HighlightColor,
                    uint16_t ItemRowHeight, uint16_t MaxRow,
                    const char *TitleText)
{ //, const GFXfont &ItemFont, const GFXfont &TitleFont) {

  i_text_color = TextColor;
  i_background_color = BackgroundColor;
  i_highlight_text_color = HighlightTextColor;
  i_highlight_color = HighlightColor;
  bcolor = TextColor;
  title_bar_top = 0;          // title bar top
  title_bar_left = 0;          // title bar left
  title_bar_height = 20;         // title bar height
  title_bar_width = d->width(); // title bar width, default to screen width
  title_offset_x = 30;         // x pixel offset for title text to offset from left a bit
  title_offset_y = 10;         // y pixel offset for title text to manage height offset from top
  isx = 0;          // where to start the menu item from left, default indent 10
  icox = 0;
  icoy = 0;
  menu_margin= 5;
  isy = title_bar_top + title_bar_height + menu_margin; // where to start the menu item from top, note because array is 1 based we start drawing down a row--so back off a row, but pad 10
  i_select_bar_height = ItemRowHeight;  // select bar height
  irw = title_bar_width - isx;      // select bar width, default to full width
  i_max_row = MaxRow;         // user has to indicate this
  offset_text_menu_bar_x = 0;              // pixels to offset text in menu bar
  offset_text_menu_bar_y = 0;              // pixels to offset text in menu bar
  // itemf = ItemFont;     // item font
  // titlef = TitleFont;   // menu bar font, default to item font
  title_text_color = TextColor;      // title text color
  title_fill_color = HighlightColor; // title fill color
  disable_text_color = MENU_C_DKGREY;
  strncpy(title_text, TitleText, MAX_CHAR_LEN);
  strncpy(exit_text, EXIT_TEXT, 6);
  strncpy(edit_text, EDIT_TEXT, 6);
  item = 1;
  drawTitleFlag = true; // flag if we draw menu bar again
  currentID = 0;        // id of current highlighted or selected item (1 to ID)
  current_sel_row = 0;               // current selected row on the screen (1 to mr-1)
  totalID = 0;          // maximum number of items (where first position is 1)
  sr = 0;               // draw offset for the menu array where first postion is 0
  prev_sel_row = 1;               // previous selected rown (1 to mr - 1)
  rowselected = false;
  enablestate[0] = true;
  redraw = true;
  radius = 0;
  thick = 0;
}

int ItemMenu::addNI(const char *ItemLabel)
{

  totalID++;

  IconType[totalID] = ICON_NONE;
  bmp_w[totalID] = 0;
  bmp_h[totalID] = 0;

  enablestate[totalID] = true;
  strncpy(itemlabel[totalID], ItemLabel, MAX_CHAR_LEN);
  return (totalID);
}

int ItemMenu::addMono(const char *ItemLabel, const unsigned char *Bitmap, uint8_t BitmapWidth, uint8_t BitmapHeight)
{

  totalID++;

  itemBitmap[totalID] = Bitmap;
  bmp_w[totalID] = BitmapWidth;
  bmp_h[totalID] = BitmapHeight;
  IconType[totalID] = ICON_MONO;

  enablestate[totalID] = true;
  strncpy(itemlabel[totalID], ItemLabel, MAX_CHAR_LEN);
  return (totalID);
}

int ItemMenu::add565(const char *ItemLabel, const uint16_t *Bitmap, uint8_t BitmapWidth, uint8_t BitmapHeight)
{

  totalID++;

  item565Bitmap[totalID] = Bitmap;
  bmp_w[totalID] = BitmapWidth;
  bmp_h[totalID] = BitmapHeight;
  IconType[totalID] = ICON_565;

  enablestate[totalID] = true;
  strncpy(itemlabel[totalID], ItemLabel, MAX_CHAR_LEN);
  return (totalID);
}

void ItemMenu::drawHeader(bool ShowEdit)
{

  // if current_sel_row == 0 you must draw header
  // if current_sel_row != 0 and was just draw, don't draw again
  if (current_sel_row == 0)
  {
    dh = true;
  }
  else if (dh)
  {
    dh = false;
  }
  else
  {
    return;
  }

  // d->setFont(titlef);

  if (ShowEdit)
  {
    // draw new menu bar
    d->setCursor(title_bar_left + title_offset_x, title_bar_top + title_offset_y);
    //d->fillRect(title_bar_left, title_bar_top, title_bar_width, title_bar_height, i_highlight_color);
    d->fillRectVGradient(title_bar_left,title_bar_top,title_bar_width,title_bar_height,i_highlight_color,TFT_CYAN);
    d->setTextColor(title_text_color);
    d->print(edit_text);
    // up arrow
    d->fillTriangle(title_bar_left + 10 + 5, title_bar_top + title_bar_height - 5, // bottom left
                    title_bar_left + 10 - 5, title_bar_top + title_bar_height - 5, // bottom right
                    title_bar_left + 10, title_bar_top + 5,            // center
                    title_text_color);
    // down arrow
    d->fillTriangle(title_bar_width - 10 + 5, title_bar_top + 5,  // top left
                    title_bar_width - 10 - 5, title_bar_top + 5,  // top right
                    title_bar_width - 10, title_bar_top + title_bar_height - 5, // center
                    title_text_color);
  }
  else
  {

    if (current_sel_row == 0)
    {
      // draw new menu bar
      //d->setCursor(title_bar_left + title_offset_x, title_bar_top + title_offset_y);
      //d->fillRect(title_bar_left, title_bar_top, title_bar_width, title_bar_height, i_highlight_color);
      d->fillRectVGradient(title_bar_left,title_bar_top,title_bar_width,title_bar_height,i_highlight_color,TFT_NAVY);
      d->setTextColor(title_text_color);
      //d->print(exit_text);
      d->setTextDatum(MC_DATUM);
      d->drawString(exit_text,d->width()/2,title_bar_height/2); //title_bar_top + title_offset_y);
    }
    else
    {
      // draw new menu bar
      // d->setCursor(title_bar_left + title_offset_x, title_bar_top + title_offset_y);
      //d->fillRect(title_bar_left, title_bar_top, title_bar_width, title_bar_height, title_fill_color);
      d->fillRectVGradient(title_bar_left,title_bar_top,title_bar_width,title_bar_height,title_fill_color,TFT_CYAN);
      d->setTextColor(title_text_color);
      //d->print(title_text);
      d->setTextDatum(MC_DATUM);
      d->drawString(title_text,d->width()/2,title_bar_height/2); 
    }
    if (totalID > i_max_row)
    {
      // up arrow
      d->fillTriangle(title_bar_left + 10 + 5, title_bar_top + title_bar_height - 5, // bottom left
                      title_bar_left + 10 - 5, title_bar_top + title_bar_height - 5, // bottom right
                      title_bar_left + 10, title_bar_top + 10,            // center
                      title_text_color);
      // down arrow
      d->fillTriangle(title_bar_width - 10 + 5, title_bar_top + 5,  // top left
                      title_bar_width - 10 - 5, title_bar_top + 5,  // top right
                      title_bar_width - 10, title_bar_top + title_bar_height - 5, // center
                      title_text_color);
    }
  }
}

int ItemMenu::press(int16_t ScreenX, int16_t ScreenY)
{

  int bs;

  InputFromTouch = true;

  // check if header pressed
  if ((ScreenX > title_bar_left) && (ScreenX < (title_bar_width + title_bar_left)) && (ScreenY > title_bar_top) && (ScreenY < (title_bar_top + title_bar_height)))
  {

    if (ScreenX < (title_bar_left + 25 + 15))
    {
      // up arrow
      if (current_sel_row == 1)
      {
        current_sel_row = i_max_row;
        // sr++;
      }

      MoveDown();
      return -1;
    }

    if (ScreenX > (title_bar_width - 25 - 15))
    {
      // down arrow
      if (current_sel_row <= i_max_row)
      {
        current_sel_row = 0;
        // currentID = sr;
      }

      MoveUp();
      return -1;
    }

    return 0;
  }

  // clear previous row, this can happen if user scroll to a row with mechanical
  // device but presses another
  drawRow(currentID, BUTTON_NOTPRESSED);
  // now process the menu press
  for (i = 1; i <= i_max_row; i++)
  {
    bs = icox + bmp_w[i + sr] + isx;
    itx = bs + offset_text_menu_bar_x;
    if (
        (ScreenX > bs) &&
        (ScreenY > (isy + (i_select_bar_height * (i - 1)))) &&
        (ScreenX < (irw)) &&
        (ScreenY < (isy + i_select_bar_height + (i_select_bar_height * (i - 1)))))
    {
      if (!enablestate[i + sr])
      {
        return -1;
      }

      current_sel_row = i + sr;
      return current_sel_row;
    }
  }
  return -1;
}

void ItemMenu::drawRow(int ID, uint8_t style)
{

  int bs;
  int hr = ID - sr;

  if (ID == 0)
  {
    drawHeader(false);
  }

  if (ID <= 0)
  {
    return;
  }

  bs = icox + bmp_w[hr] + isx;

  if (!enablestate[hr])
  {
    return;
  }

  if (ID == 0)
  {
    if (style == BUTTON_PRESSED)
    {
      // draw new menu bar
      //d->setCursor(title_bar_left + title_offset_x, title_bar_top + title_offset_y);
      //d->fillRect(title_bar_left, title_bar_top, title_bar_width, title_bar_height, i_highlight_color);
      d->fillRectVGradient(title_bar_left,title_bar_top,title_bar_width,title_bar_height,i_highlight_color,TFT_CYAN);
      d->setTextColor(title_text_color);
      //d->print(title_text);
      d->setTextDatum(MC_DATUM);
      d->drawString(title_text,d->width()/2,title_bar_height/2); 
    }
    else
    {
      // draw new menu bar
      //d->setCursor(title_bar_left + title_offset_x, title_bar_top + title_offset_y);
      //d->fillRect(title_bar_left, title_bar_top, title_bar_width, title_bar_height, title_fill_color);
      d->fillRectVGradient(title_bar_left,title_bar_top,title_bar_width,title_bar_height,title_fill_color,TFT_CYAN);
      d->setTextColor(title_text_color);
      //d->print(title_text);
      d->setTextDatum(MC_DATUM);
      d->drawString(title_text,d->width()/2,title_bar_height/2); 
    }
  }
  else
  {

    if (style == BUTTON_PRESSED)
    {

      if (radius > 0)
      {
        d->fillRoundRect(bs, isy - i_select_bar_height + (i_select_bar_height * hr), irw - bs, i_select_bar_height , radius, bcolor);
        d->fillRoundRect(bs + thick, isy - i_select_bar_height + (i_select_bar_height * hr) + thick, irw - bs - (2 * thick), i_select_bar_height - (2 * thick), radius, i_highlight_color);
      }
      else
      {
        d->fillRect(bs, isy - i_select_bar_height + (i_select_bar_height * hr), irw - bs, i_select_bar_height , bcolor);
        d->fillRect(bs + thick, isy - i_select_bar_height + (i_select_bar_height * hr) + thick, irw - bs - (2 * thick), i_select_bar_height - (2 * thick), i_highlight_color);
      }
    }
    else
    {
      d->fillRect(bs, isy - i_select_bar_height + (i_select_bar_height * hr), irw - bs, i_select_bar_height , i_background_color); // back color
    }

    // write text
    itx = bs + offset_text_menu_bar_x;
    // d->setFont(itemf);
    d->setTextColor(i_highlight_text_color);
    d->setCursor(itx, isy - i_select_bar_height + (i_select_bar_height * hr) + offset_text_menu_bar_y);
    d->print(itemlabel[ID]);
  }
}

void ItemMenu::draw()
{

  dh = true;

  drawItems();
}

void ItemMenu::drawItems()
{

  int bs;

  if (i_max_row > totalID)
  {
    i_max_row = totalID;
  }
  redraw = false;
  // determine if we need to pan or just increment the list

  if ((currentID > totalID) && (sr >= 0))
  {
    // up to top
    current_sel_row = 0;
    currentID = 0;
    sr = 0;
    redraw = true;
  }
  else if ((current_sel_row < 0) && (sr == 0))
  {
    //  pan whole menu to bottom
    current_sel_row = i_max_row;
    currentID = totalID;
    sr = totalID - i_max_row;
    redraw = true;
  }
  else if ((current_sel_row > i_max_row) && ((sr + current_sel_row) > totalID))
  {
    //  pan whole menu to top
    current_sel_row = 1;
    currentID = 1;
    sr = 0;
    redraw = true;
  }
  else if ((current_sel_row > i_max_row) && (sr >= 0))
  {
    // scroll whole list up one by one
    sr++;
    current_sel_row = i_max_row;
    redraw = true;
  }
  else if ((current_sel_row < 1) && (sr > 0))
  {
    // scroll whole list down one by one
    sr--;
    current_sel_row = 1;
    redraw = true;
  }

  drawHeader(false);

  // determine if we need arrows
  if (current_sel_row == i_max_row)
  {
    moredown = true;
  }
  if (current_sel_row == 1)
  {
    moreup = true;
  }

  if (enablestate[currentID] == false)
  {
    return;
  }

  // d->setFont(itemf);

  // now draw the items in the rows
  for (i = 1; i <= i_max_row; i++)
  {
    // menu bar start
    // sr  - Offset for the menu array. isx start the menu item from the left
    bs = icox + bmp_w[i + sr] + isx;

    // text start
    itx = bs + offset_text_menu_bar_x;

    if (enablestate[i + sr])
    {
      temptColor = i_text_color;
    }
    else
    {
      temptColor = disable_text_color;
    }

    if (redraw)
    {
      d->fillRect(icox, isy - i_select_bar_height + (i_select_bar_height * i), irw, i_select_bar_height , i_background_color); // back color
    }

    if (i == prev_sel_row)
    {
      // maybe just row change so blank previous
      d->fillRect(bs, isy - i_select_bar_height + (i_select_bar_height * prev_sel_row), irw - bs, i_select_bar_height , i_background_color); // back color
    }

    if ((i == current_sel_row) && (!InputFromTouch))
    {

      if (radius > 0)
      {
        d->fillRoundRect(bs, isy - i_select_bar_height + (i_select_bar_height * i), irw - bs, i_select_bar_height , radius, bcolor);
        d->fillRoundRect(bs + thick, isy - i_select_bar_height + (i_select_bar_height * i) + thick, irw - bs - (2 * thick), i_select_bar_height - (2 * thick), radius, i_highlight_color);
      }
      else
      {
        d->fillRect(bs, isy - i_select_bar_height + (i_select_bar_height * i), irw - bs, i_select_bar_height , bcolor);
        d->fillRect(bs + thick, isy - i_select_bar_height + (i_select_bar_height * i) + thick, irw - bs - (2 * thick), i_select_bar_height - (2 * thick), i_highlight_color);
      }

      temptColor = i_highlight_text_color;
    }

    // write bitmap

    if (IconType[i + sr] == ICON_MONO)
    {
      drawMonoBitmap(icox, icoy + isy - i_select_bar_height + (i_select_bar_height * i), itemBitmap[i + sr], bmp_w[i + sr], bmp_h[i + sr], i_text_color);
    }
    else if (IconType[i + sr] == ICON_565)
    {
      draw565Bitmap(icox, icoy + isy - i_select_bar_height + (i_select_bar_height * i), item565Bitmap[i + sr], bmp_w[i + sr], bmp_h[i + sr]);
    }

    // write text
    d->setTextColor(temptColor);
    d->setCursor(itx, isy - i_select_bar_height + (i_select_bar_height * i) + offset_text_menu_bar_y);
    d->print(itemlabel[i + sr]);
  }

  moreup = false;
  moredown = false;
  prev_sel_row = current_sel_row;
  InputFromTouch = false;
}

void ItemMenu::MoveUp()
{

  current_sel_row--;
  currentID--;
  drawItems();

  if (enablestate[currentID] == false)
  {
    while (enablestate[currentID] == false)
    {
      current_sel_row--;
      currentID--;
      drawItems();
    }
  }
  item = currentID;
}

void ItemMenu::MoveDown()
{

  current_sel_row++;
  currentID++;
  drawItems();
  if (enablestate[currentID] == false)
  {
    while (enablestate[currentID] == false)
    {
      current_sel_row++;
      currentID++;
      drawItems();
    }
  }
  item = currentID;
}

int ItemMenu::selectRow()
{

  if (currentID == 0)
  {
    item = 0;
    current_sel_row = 0;
    sr = 0;
  }
  item = currentID;
  return currentID;
}

void ItemMenu::setTitleColors(uint16_t TitleTextColor, uint16_t TitleFillColor)
{
  title_text_color = TitleTextColor;
  title_fill_color = TitleFillColor;
}

void ItemMenu::setTitleBarSize(uint16_t TitleTop, uint16_t TitleLeft, uint16_t TitleWith, uint16_t TitleHeight)
{

  title_bar_top = TitleTop;
  title_bar_left = TitleLeft;
  title_bar_width = TitleWith;
  title_bar_height = TitleHeight;
}

void ItemMenu::setTitleText(char *TitleText, char *ExitText)
{
  strncpy(title_text, TitleText, MAX_CHAR_LEN);
  strncpy(exit_text, ExitText, MAX_CHAR_LEN);
}

void ItemMenu::setTitleText(char *TitleText, char *ExitText, char *EditText)
{
  strncpy(title_text, TitleText, MAX_CHAR_LEN);
  strncpy(exit_text, ExitText, MAX_CHAR_LEN);
  strncpy(edit_text, EditText, MAX_CHAR_LEN);
}

void ItemMenu::setTitleTextMargins(uint16_t LeftMargin, uint16_t TopMargin)
{
  title_offset_x = LeftMargin; // pixels to offset text in menu bar
  title_offset_y = TopMargin;  // pixels to offset text in menu bar
}

void ItemMenu::setMenuBarMargins(uint16_t LeftMargin, uint16_t Width, byte BorderRadius, byte BorderThickness)
{
  isx = LeftMargin;  // pixels to offset text in menu bar
  irw = Width - isx; // pixels to offset text in menu bar
  radius = BorderRadius;
  thick = BorderThickness;
}

void ItemMenu::setItemColors(uint16_t DisableTextColor, uint16_t BorderColor)
{
  disable_text_color = DisableTextColor;
  bcolor = BorderColor;
}

void ItemMenu::SetAllColors(uint16_t TextColor, uint16_t BackgroundColor, uint16_t HighlightTextColor, uint16_t HighlightColor,

                            uint16_t HighLightBorderColor, uint16_t DisableTextColor, uint16_t TitleTextColor, uint16_t TitleFillColor)
{

  i_text_color = TextColor;
  i_background_color = BackgroundColor;
  i_highlight_text_color = HighlightTextColor;
  i_highlight_color = HighlightColor;
  bcolor = HighLightBorderColor;
  title_text_color = TitleTextColor;
  title_fill_color = TitleFillColor;
  disable_text_color = DisableTextColor;
}

void ItemMenu::setItemTextMargins(uint16_t LeftMargin, uint16_t TopMargin, uint16_t MenuMargin)
{
  offset_text_menu_bar_x = LeftMargin; // pixels to offset text in menu bar
  offset_text_menu_bar_y = TopMargin;  // pixels to offset text in menu bar
  menu_margin= MenuMargin;
  isy = title_bar_top + title_bar_height + menu_margin;
}

void ItemMenu::setIconMargins(uint16_t IconOffsetX, uint16_t IconOffsetY)
{
  icox = IconOffsetX; // pixels to offset text in menu bar
  icoy = IconOffsetY; // pixels to offset text in menu bar
}

void ItemMenu::disable(int ID)
{
  this->enablestate[ID] = false;
}

void ItemMenu::enable(int ID)
{
  this->enablestate[ID] = true;
}

bool ItemMenu::getEnableState(int ID)
{
  return this->enablestate[ID];
}

void ItemMenu::drawMonoBitmap(int16_t x, int16_t y, const unsigned char *bitmap, uint8_t w, uint8_t h, uint16_t color)
{

  uint8_t sbyte = 0;
  uint8_t byteWidth = 0;
  int jj, ii;

  byteWidth = (w + 7) / 8;

  for (jj = 0; jj < h; jj++)
  {
    for (ii = 0; ii < w; ii++)
    {
      if (ii & 7)
        sbyte <<= 1;
      else
        sbyte = pgm_read_byte(bitmap + jj * byteWidth + ii / 8);
      if (sbyte & 0x80)
        d->drawPixel(x + ii, y + jj, color);
    }
  }
}

void ItemMenu::draw565Bitmap(int16_t x, int16_t y, const uint16_t *bitmap, uint8_t w, uint8_t h)
{

  uint16_t offset = 0;

  int j, i;

  for (i = 0; i < h; i++)
  {
    for (j = 0; j < w; j++)
    {
      d->drawPixel(j + x, i + y, bitmap[offset]);
      offset++;
    }
  }
}

/////////////////////////////////
// end of this menu library
/////////////////////////////////
