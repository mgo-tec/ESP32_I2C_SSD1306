/*
  ESP32_I2C_SSD1306.cpp - for Arduino core for the ESP32 ( Use I2C library ).
  Beta version 1.0
  
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

#include "ESP32_I2C_SSD1306.h"

ESP32_I2C_SSD1306::ESP32_I2C_SSD1306(const uint8_t Addres, const int sda, const int scl, const uint8_t MaxHpix, const uint8_t MaxVpix)
  : _Addres(Addres)
  , _sda(sda)
  , _scl(scl)
  , _MaxHpix(MaxHpix)
  , _MaxVpix(MaxVpix)
{}

//************ Initialize SSD1306 ****************************************
void ESP32_I2C_SSD1306::SSD1306_Init(){
  ESP32_I2C_SSD1306::SSD1306_Init(400000);
}

void ESP32_I2C_SSD1306::SSD1306_Init(uint32_t frequency){
  Wire.begin(_sda, _scl);
  Wire.setClock(frequency);
  delay(100);
  
  _MaxX = _MaxHpix-1;
  _MaxY = _MaxVpix-1;
  _TotalPage = _MaxVpix/8;
  _MaxPage = _TotalPage-1;
  _TotalHbytes = _MaxHpix/8;

  Wire.beginTransmission(_Addres); // Up to 31 bytes following this.
    Wire.write(0b00000000); //control byte, Co bit = 0 (continue), D/C# = 0 (command)
      Wire.write(0xAE); //display off
    Wire.write(0b00000000);
      Wire.write(0xA8); //Set Multiplex Ratio  0xA8, 0x3F
        Wire.write(0b00111111); //64MUX
    Wire.write(0b00000000);    
      Wire.write(0xD3); //Set Display Offset 0xD3, 0x00
        Wire.write(0x00);
    Wire.write(0b00000000);
      Wire.write(0x40); //Set Display Start Line 0x40
    Wire.write(0b00000000);
      Wire.write(0xA1); //Set Segment re-map 0xA0/0xA1
    Wire.write(0b00000000);
      Wire.write(0xC8); //Set COM Output Scan Direction 0xC0,/0xC8
    Wire.write(0b00000000);
      Wire.write(0xDA); //Set COM Pins hardware configuration 0xDA, 0x02
        Wire.write(0b00010010);
    Wire.write(0b00000000);
      Wire.write(0x81); //Set Contrast Control 0x81, default=0x7F
        Wire.write(255); //0-255
    Wire.write(0b00000000);
      Wire.write(0xA4); //Disable Entire Display On
    Wire.write(0b00000000);
      Wire.write(0xA6); //Set Normal Display 0xA6, Inverse display 0xA7
    Wire.write(0b00000000);
      Wire.write(0xD5); //Set Display Clock Divide Ratio/Oscillator Frequency 0xD5, 0x80
        Wire.write(0b10000000);
    Wire.write(0b00000000);
      Wire.write(0x20); //Set Memory Addressing Mode
        Wire.write(0x00); //Page addressing mode
  Wire.endTransmission();
  Wire.beginTransmission(_Addres);
    Wire.write(0b00000000);
      Wire.write(0x22); //Set Page Address
        Wire.write(0); //Start page set
        Wire.write(_MaxPage); //End page set
    Wire.write(0b00000000);
      Wire.write(0x21); //set Column Address
        Wire.write(0); //Column Start Address
        Wire.write(_MaxX); //Column Stop Address
    Wire.write(0b00000000);
      Wire.write(0x8D); //Set Enable charge pump regulator 0x8D, 0x14
        Wire.write(0x14);
  Wire.endTransmission();

  ESP32_I2C_SSD1306::Display_Clear_All();
  delay(1000);

  Wire.beginTransmission(_Addres);
    Wire.write(0b00000000);
      Wire.write(0xAF); //Display On 0xAF
  Wire.endTransmission();
}
//*************************************************
void ESP32_I2C_SSD1306::Brightness(uint8_t brightness){
  Wire.beginTransmission(_Addres);
    Wire.write(0b00000000); //control byte, Co bit = 1 (One command only), D/C# = 0 (command)
      Wire.write(0x81); //Set Contrast Control 0x81
        Wire.write(brightness); //0-255, default=0x7F
  Wire.endTransmission();
}
//**************************************************
void ESP32_I2C_SSD1306::Display_Clear_All(){
  uint8_t i, j, k;
  ESP32_I2C_SSD1306::Column_Page_Set(0, _MaxX, 0);
  for(i=0; i<_TotalPage; i++){
    for(j = 0; j < 8; j++){//column = 8byte x 16
      Wire.beginTransmission(_Addres);
      Wire.write(0b01000000); //control byte, Co bit = 0 (continue), D/C# = 1 (data)
      for(k = 0; k < 16; k++){ //continue to 31byte
        Wire.write(0x00);
      }
      Wire.endTransmission();
    }
  }
}
//**************************************************
void ESP32_I2C_SSD1306::Display_Clear_Page(uint8_t x0, uint8_t p0, uint8_t x1, uint8_t p1){
  uint8_t i, j, k;

  for(i = p0; i < p1; i++){//Page(0-7)
    ESP32_I2C_SSD1306::Column_Page_Set(x0, x1, i);

    for(j = 0; j < _TotalHbytes; j++){//column = 8byte x 16
      Wire.beginTransmission(_Addres); // Up to 31 bytes following this.
      Wire.write(0b01000000); //control byte, Co bit = 0 (continue), D/C# = 1 (data)
      for(k = 0; k < 8; k++){ //continue to 31byte
        Wire.write(0x00);
      }
      Wire.endTransmission();
    }
    //delay(1);
  }
}
//********* OLED 8x8フォント出力 ********************
void ESP32_I2C_SSD1306::Font_8x8_DisplayOut(int16_t Rotation, uint8_t Direction, uint8_t txtMax, uint8_t x0, uint8_t p0, uint8_t Fnt[][8]){
  int i,j,k;
  uint8_t b = 0, dummy = 0;

  if(txtMax > 16) txtMax = 16;

  ESP32_I2C_SSD1306::Column_Page_Set(x0, ((txtMax)*8-x0-1), p0);

  for(i=0; i<txtMax; i++){//column = 8byte x 16
    Wire.beginTransmission(_Addres); // Up to 31 bytes following this.
    Wire.write(0b01000000); //control byte, Co bit = 0 (continue), D/C# = 1 (data)

    switch(Rotation){
      case 90:
        break;
      case -90:
        for(j=7; j>=0; j--){
          for(k=7; k>=0; k--){
            dummy = (Fnt[i][j] >> k) & 0x01;
            if(dummy > 0){
              b = b | (dummy<<(7-k));
            }
          }
          Wire.write(b);
          b = 0;
        }
        break;
      case 180:
        break;
      default:
        for(j=7; j>=0; j--){
          for(k=0; k<8; k++){
            dummy = ( Fnt[i][k] >> j ) & 0x01;
            if(dummy > 0){
              b = b | (dummy << k);
            }
          }
          Wire.write(b);
          b = 0;
        }
        break;
    }

    Wire.endTransmission();
  }
}
//********* OLED 4倍角フォント出力 ********************
void ESP32_I2C_SSD1306::SizeUp_8x8_Font_DisplayOut(uint8_t size, uint8_t txtMax, uint8_t x0, uint8_t p0, uint8_t Fnt[][8]){
  int i, j, k, m, n;
  uint8_t b = 0, dummy = 0;
  uint8_t dot_cnt_v = 0;
  uint8_t dot_cnt_v2 = 0;

  if(size > 8) size = 8;
  if(txtMax > (16/size)) txtMax = 16/size;

  for(m=0; m<size; m++){
    ESP32_I2C_SSD1306::Column_Page_Set(x0, (x0 + (txtMax*size)*8 - 1), (p0+m));

    for(i=0; i<txtMax; i++){//column = 8byte x 16
      for(j=7; j>=0; j--){
        Wire.beginTransmission(_Addres); // Up to 31 bytes following this.
          Wire.write(0b01000000); //control byte, Co bit = 0 (continue), D/C# = 1 (data)

        for(k=dot_cnt_v; k<(dot_cnt_v + 8/size); k++){
          dummy = ( Fnt[i][k] >> j ) & 0x01;
          for(n=0; n<size; n++){
            b = b | (dummy << (dot_cnt_v2 + n));
          }
          dot_cnt_v2 = dot_cnt_v2 + size;
        }
        dot_cnt_v2 = 0;

        for(n=0; n<size; n++){
          Wire.write(b);
        }

        b = 0;
        Wire.endTransmission();
      }
      delay(1);
    }
    dot_cnt_v = dot_cnt_v + 8/size;
  }
}

//********* OLED 16x16フォントサイズアップ ********************
void ESP32_I2C_SSD1306::SizeUp_8x16_Font_DisplayOut(uint8_t size, uint8_t txtMax, uint8_t x0, uint8_t p0, uint8_t Fnt[][16]){
  //size must 2 or 4
  int i, n;
  uint8_t b = 0;
  uint8_t fnt_cnv[txtMax][size][size][16] = {};
  int8_t FpageV = 0;
  uint8_t FpageH = 0;
  uint8_t shift_bit_cnt = 0;
  uint8_t byt_read_cnt = 0;
  uint8_t byt_write_cnt = 0;
  //uint8_t set_x_max = x0 + (txtMax*size)*8 - 1;
  uint8_t set_x_max = 127;
  uint8_t page_max = size*2;
  uint8_t map_bit = 0b11000000; //case by size=2
  if(size == 4){
    map_bit = 0b11110000;
  }

  for(n=0; n<txtMax; n++){
    for(byt_read_cnt=0; byt_read_cnt<16; byt_read_cnt++){
      if(byt_write_cnt > 15){
        byt_write_cnt = 0;
        FpageH++;
      }

      for(int8_t bit_read_cnt=7; bit_read_cnt>=0; bit_read_cnt--){
        b = (Fnt[n][byt_read_cnt] >> bit_read_cnt) & 0x01; //bitRead
        if(shift_bit_cnt > 7){
          shift_bit_cnt = 0;
          FpageV++;
        }

        if(b>0){ //bitWrite
          fnt_cnv[n][FpageH][FpageV][byt_write_cnt] = fnt_cnv[n][FpageH][FpageV][byt_write_cnt] | (map_bit >> shift_bit_cnt);
        }
        shift_bit_cnt = shift_bit_cnt + size;
      }

      shift_bit_cnt = 0;

      for(FpageV=0; FpageV<size; FpageV++){
        for(i=1; i<size; i++){
          fnt_cnv[n][FpageH][FpageV][byt_write_cnt+i] = fnt_cnv[n][FpageH][FpageV][byt_write_cnt];
        }
      }

      FpageV = 0;
      byt_write_cnt = byt_write_cnt + size;
    }

    FpageH = 0;
    byt_write_cnt = 0;
  }

  uint8_t Fpage_max = size-1;

  FpageV = Fpage_max;
  for(i=0; i<page_max; i++){
    ESP32_I2C_SSD1306::Column_Page_Set(x0, set_x_max, p0+i);
    if(i<size){
      n=1; //奇数値8x16フォントpage割り当て
    }else{
      n=0; //偶数値8x16フォントpage割り当て
    }
    while(n<txtMax){
      for(FpageH=0; FpageH<size; FpageH++){
        Wire.beginTransmission(_Addres);
          Wire.write(0b01000000);
          Wire.write(fnt_cnv[n][FpageH][FpageV], 16);
        Wire.endTransmission();
      }
      n = n + 2;
    }
    FpageV--;
    if(FpageV < 0){
      FpageV = Fpage_max;
    }
  }
}
//********* OLED 8x16フォント bytes 1 line 出力 ********************
void ESP32_I2C_SSD1306::Font8x16_1line_DisplayOut(uint8_t txtMax, uint8_t x0, uint8_t p0, uint8_t Fnt[][16]){
  ESP32_I2C_SSD1306::Font8x16_1line_DisplayOut(-90, txtMax, x0, p0, Fnt);
}
//********* OLED 8x16フォント bytes 1 line 出力 ********************
void ESP32_I2C_SSD1306::Font8x16_1line_DisplayOut(int16_t Rotation, uint8_t txtMax, uint8_t x0, uint8_t p0, uint8_t Fnt[][16]){
  int i;

  if(txtMax > 16) txtMax = 16;
  uint8_t DispMax = txtMax * 8;
  uint8_t p1_8pix[txtMax][8]={};
  uint8_t p2_8pix[txtMax][8]={};
  
  for(i=0; i<txtMax; i++){
    ESP32_I2C_SSD1306::Font8x16pix_Rotation(Rotation, Fnt[i], p1_8pix[i], p2_8pix[i]);
  }

  ESP32_I2C_SSD1306::Column_Page_Set(x0, x0+DispMax-1, p0);
  for(i=0; i<txtMax; i++){
    Wire.beginTransmission(_Addres); // Up to 31 bytes following this.
      Wire.write(0b01000000); //control byte, Co bit = 0 (continue), D/C# = 1 (data)
        Wire.write(p1_8pix[i++], 8); //2byte連続出力。beginTransmissionとコントロールバイト含めて32byteしか連続出力できない
        Wire.write(p1_8pix[i], 8);
    Wire.endTransmission();
  }

  ESP32_I2C_SSD1306::Column_Page_Set(x0, x0+DispMax-1, p0+1);
  for(i=0; i<txtMax; i++){
    Wire.beginTransmission(_Addres); // Up to 31 bytes following this.
      Wire.write(0b01000000); //control byte, Co bit = 0 (continue), D/C# = 1 (data)
        Wire.write(p2_8pix[i++], 8); //2byte連続出力。beginTransmissionとコントロールバイト含めて32byteしか連続出力できない
        Wire.write(p2_8pix[i], 8);
    Wire.endTransmission();
  }
}

//********* OLED 8x16フォント bytes 1 line 出力 ********************
void ESP32_I2C_SSD1306::Font8x16_1line_Page_DisplayOut(uint8_t txtMax, uint8_t x0, uint8_t p0, uint8_t Fnt[][16]){
  int i;

  if(txtMax > 16) txtMax = 16;
  uint8_t DispMax = txtMax * 8;

  ESP32_I2C_SSD1306::Column_Page_Set(x0, x0+DispMax-1, p0);
  for(i=1; i<txtMax; i++){
    Wire.beginTransmission(_Addres); // Up to 31 bytes following this.
      Wire.write(0b01000000); //control byte, Co bit = 0 (continue), D/C# = 1 (data)
        Wire.write(Fnt[i++], 16); //2byte連続出力。beginTransmissionとコントロールバイト含めて32byteしか連続出力できない
    Wire.endTransmission();
  }

  //ESP32_I2C_SSD1306::Column_Page_Set(x0, x0+DispMax-1, p0+1);
  for(i=0; i<txtMax; i++){
    Wire.beginTransmission(_Addres); // Up to 31 bytes following this.
      Wire.write(0b01000000); //control byte, Co bit = 0 (continue), D/C# = 1 (data)
        Wire.write(Fnt[i++], 16); //2byte連続出力。beginTransmissionとコントロールバイト含めて32byteしか連続出力できない
    Wire.endTransmission();
  }
}
//*********電光掲示板風スクロール関数 8x16 Page単位 16文字用********************
boolean ESP32_I2C_SSD1306::Scroller_Font8x16_PageReplace(uint8_t txtMax, uint8_t num, uint8_t Zen_or_Han, uint8_t font_buf1[][16], uint8_t scl_buff1[][16]){
  int8_t i, j;
  boolean fnt_read_ok = false;
  uint8_t v_scl_cnt = _scl_cnt2[num] + _Zen_or_Han_cnt2[num]*8;
  uint8_t max2 = txtMax-2;

  for(j=0; j<max2; j++){
    for(i=0; i<15; i++){
      scl_buff1[j][i] = scl_buff1[j][i+1];
    }
    scl_buff1[j][15] = scl_buff1[j+2][0];
  }
  for(j=max2; j<txtMax; j++){
    for(i=0; i<15; i++){
      scl_buff1[j][i] = scl_buff1[j][i+1];
    }
    scl_buff1[j][15] = font_buf1[j-max2][v_scl_cnt];
  }

  _scl_cnt2[num]++;

  if(_scl_cnt2[num] == 8){
    _scl_cnt2[num] = 0;
    _Zen_or_Han_cnt2[num]++;
    if(_Zen_or_Han_cnt2[num] == Zen_or_Han){
      _Zen_or_Han_cnt2[num] = 0;
      fnt_read_ok = true;
    }
  }

  return fnt_read_ok;
}

//*******************************************
void ESP32_I2C_SSD1306::Font8x16pix_Rotation(int16_t Rotation, uint8_t Fnt[16], uint8_t p1_8pix[8], uint8_t p2_8pix[8]){
  int i, j, j_rev=0;
  uint8_t dummy = 0;

  switch(Rotation){
    case 0:
      for(i=0; i<8; i++){
        p1_8pix[i] = Fnt[i];
      }
      for(i=8; i<16; i++){
        p2_8pix[i-8] = Fnt[i];
      }
      break;
    case 90:
      break;
    case -90:
      for(j=0; j<8; j++){ //Page1 8x8pixel mapping (Rotation 90)
        j_rev = 7 - j;
        for(i=0; i<8; i++){ 
          dummy = ( Fnt[i] >> j_rev ) & 0x01;
          if(dummy > 0){
            p1_8pix[j] = p1_8pix[j] | (dummy << i);
          }
        }
      }

      for(j=0; j<8; j++){ //Page2 8x8pixel mapping (Rotation 90)
        j_rev = 7 - j;
        for(i=0; i<8; i++){
          dummy = ( Fnt[i+8] >> j_rev ) & 0x01;
          if(dummy > 0){
            p2_8pix[j] = p2_8pix[j] | (dummy << i);
          }
        }
      }
      break;
  }
}
//*******************************************
void ESP32_I2C_SSD1306::GDDRAM_Horizontal_Scroll(uint8_t RL, uint8_t start_page, uint8_t interval, uint8_t end_page){
  Wire.beginTransmission(_Addres); // Up to 31 bytes following this.
    Wire.write(0b00000000); //control byte, Co bit = 0 (continue), D/C# = 0 (command)
      Wire.write(0x26 | RL); //Continuous Horizontal Scroll Setup 0x26:Right 0x27;Left
        Wire.write(0x00); //dummy
        Wire.write(start_page); //Column Start Address←水平開始位置はここで決める(0～127)
        Wire.write(interval); //0-7
        Wire.write(end_page); //0-7
        Wire.write(0x00); //dummy
        Wire.write(0xFF); //dummy
  Wire.endTransmission();
}
//*******************************************
void ESP32_I2C_SSD1306::GDDRAM_Vertical_and_Horizontal_Scroll(uint8_t RL, uint8_t start_page, uint8_t interval, uint8_t end_page, uint8_t v_offset, uint8_t row0, uint8_t row1){
  Wire.beginTransmission(_Addres); // Up to 31 bytes following this.
    Wire.write(0b00000000); //control byte, Co bit = 0 (continue), D/C# = 0 (command)
      Wire.write(0x29 | RL); //Continuous Horizontal Scroll Setup 0x26:Right 0x27;Left
        Wire.write(0x00); //dummy
        Wire.write(start_page); //Column Start Address←水平開始位置はここで決める(0～127)
        Wire.write(interval); //0-7
        Wire.write(end_page); //0-7
        Wire.write(v_offset); //Vertical Offset 0-63
    Wire.write(0b00000000);
      Wire.write(0xA3); //Set Vertical Scroll Area
        Wire.write(row0); //Set No. of rows in top fixed area.
        Wire.write(row1); //Set No. of rows in scroll area.
  Wire.endTransmission();
}
//*******************************************
void ESP32_I2C_SSD1306::GDDRAM_Scroll_Start(){
  Wire.beginTransmission(_Addres);
    Wire.write(0b00000000); //control byte, Co bit = 0 (continue), D/C# = 0 (command)
      Wire.write(0x2F); //Activate scroll
  Wire.endTransmission();
}
//*******************************************
void ESP32_I2C_SSD1306::GDDRAM_Scroll_Stop(){
  Wire.beginTransmission(_Addres);
    Wire.write(0b00000000); //control byte, Co bit = 0 (continue), D/C# = 0 (command)
      Wire.write(0x2E); //Deactivate scroll
  Wire.endTransmission();
}
//******************************************
void ESP32_I2C_SSD1306::Column_Page_Set(uint8_t x0, uint8_t x1, uint8_t page){
  Wire.beginTransmission(_Addres); // Up to 31 bytes following this.
    Wire.write(0b00000000); //control byte, Co bit = 0 (continue), D/C# = 0 (command)
      Wire.write(0xB0 | page ); //set page start address(B0～B7)
    Wire.write(0b00000000);
      Wire.write(0x21); //set Column Address
        Wire.write(x0); //Column Start Address(0-127)
        Wire.write(x1); //Column Stop Address(0-127)
  Wire.endTransmission();
}
//*****************************************
void ESP32_I2C_SSD1306::Data_Send(uint8_t data){
  Wire.beginTransmission(_Addres);
    Wire.write(0b01000000);
      Wire.write(data);
  Wire.endTransmission();
}