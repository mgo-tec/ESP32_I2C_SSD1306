# ESP32_I2C_SSD1306
This is Arduino core for the ESP32 library ( I2C ).  
Beta ver 1.20  
  
Use Wire.h library  
  
# Change log
(1.20)  
Fixed because it misinterpreted Co bit of control byte.  
  
(1.10)  
By font size up function, it is possible to display different sizes of 8 x 16 font vertically and horizontally.
Also added scroll function argument to reset scroll.  

(1.01)  
Minor fixed.  
  
(1.0)  
New Release  
  
  
【更新履歴】(Japanese)  
(1.20)  
コントロールバイトのCo bitの解釈を間違えていたので修正。  
  
(1.10)  
8x16 フォントの縦と横で異なるサイズアップができるようになりました。  
また、スクロール関数で、スクロールをリセットする引数を追加しました。  
  
- SizeUp_8x16_Font_DisplayOut(uint8_t H_size, uint8_t V_size, uint8_t txtMax, uint8_t x0, uint8_t p0, uint8_t Fnt[][16]);  
- boolean Scroller_Font8x16_PageReplace(boolean *Scl_Reset, uint8_t txtMax, uint8_t num, uint8_t Zen_or_Han, uint8_t font_buf1[][16], uint8_t scl_buff1[][16]);  
  
(1.01)  
軽微な修正をしました。  
  
(1.0)  
New Release  
東雲フォントを倍角、４倍角表示できるようになりました  
  
  
The MIT License (MIT)  
  
MITライセンス  
Copyright (c) 2017 Mgo-tec  
  
My Blog:  
https://www.mgo-tec.com