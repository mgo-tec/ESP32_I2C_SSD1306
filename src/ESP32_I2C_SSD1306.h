/*
  ESP32_I2C_SSD1306.h - for Arduino core for the ESP32 ( Use I2C library ).
  Beta version 1.10
  
The MIT License (MIT)

Copyright (c) 2017 Mgo-tec. All rights reserved.
Blog URL ---> https://www.mgo-tec.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

Wire.h (I2C library for ESP32)
Copyright (c) 2006 Nicholas Zambetti.  All right reserved.
License : LGPL ver 2.1
*/

#ifndef _ESP32_I2C_SSD1306_H_INCLUDED
#define _ESP32_I2C_SSD1306_H_INCLUDED

#include <Arduino.h>
#include <Wire.h>

class ESP32_I2C_SSD1306
{
private:
  uint8_t _Addres;
  int _sda;
  int _scl;
  uint32_t _freq;
  uint8_t _MaxHpix, _MaxVpix, _MaxX, _MaxY, _MaxPage, _TotalPage, _TotalHbytes;
  uint8_t _Zen_or_Han_cnt2[4] = {};
  uint8_t _scl_cnt2[4] = {};

public:
  ESP32_I2C_SSD1306(const uint8_t Addres, const int sda, const int scl, const uint8_t MaxHpix, const uint8_t MaxVpix);

  void SSD1306_Init();
  void SSD1306_Init(uint32_t frequency);
  void Brightness(uint8_t brightness);
  void Display_Clear_All();
  void Display_Clear_Page(uint8_t x0, uint8_t p0, uint8_t x1, uint8_t p1);
  void Font_8x8_DisplayOut(int16_t Rotation, uint8_t Direction, uint8_t txtMax, uint8_t x0, uint8_t p0, uint8_t Fnt[][8]);
  void SizeUp_8x8_Font_DisplayOut(uint8_t size, uint8_t txtMax, uint8_t x0, uint8_t p0, uint8_t Fnt[][8]);

  void SizeUp_8x16_Font_DisplayOut(uint8_t size, uint8_t txtMax, uint8_t x0, uint8_t p0, uint8_t Fnt[][16]);
  void SizeUp_8x16_Font_DisplayOut(uint8_t H_size, uint8_t V_size, uint8_t txtMax, uint8_t x0, uint8_t p0, uint8_t Fnt[][16]);

  void Font8x16_1line_DisplayOut(uint8_t txtMax, uint8_t x0, uint8_t p0, uint8_t Fnt[][16]);
  void Font8x16_1line_DisplayOut(int16_t Rotation, uint8_t txtMax, uint8_t x0, uint8_t p0, uint8_t Fnt[][16]);
  void Font8x16_1line_Page_DisplayOut(uint8_t txtMax, uint8_t x0, uint8_t p0, uint8_t Fnt[][16]);

  boolean Scroller_Font8x16_PageReplace(boolean *Scl_Reset, uint8_t txtMax, uint8_t num, uint8_t Zen_or_Han, uint8_t font_buf1[][16], uint8_t scl_buff1[][16]);
  boolean Scroller_Font8x16_PageReplace(uint8_t txtMax, uint8_t num, uint8_t Zen_or_Han, uint8_t font_buf1[][16], uint8_t scl_buff1[][16]);

  void Font8x16pix_Rotation(int16_t Rotation, uint8_t Fnt[16], uint8_t p1_8pix[8], uint8_t p2_8pix[8]);
  void GDDRAM_Horizontal_Scroll(uint8_t RL, uint8_t start_page, uint8_t interval, uint8_t end_page);
  void GDDRAM_Vertical_and_Horizontal_Scroll(uint8_t RL, uint8_t start_page, uint8_t interval, uint8_t end_page, uint8_t v_offset, uint8_t row0, uint8_t row1);
  void GDDRAM_Scroll_Start();
  void GDDRAM_Scroll_Stop();
  void Column_Page_Set(uint8_t x0, uint8_t x1, uint8_t page);
  void Data_Send(uint8_t data);
};

#endif
