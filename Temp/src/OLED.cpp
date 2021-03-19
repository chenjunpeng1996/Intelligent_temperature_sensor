/*
 * @Author: your name
 * @Date: 2021-03-17 10:04:00
 * @LastEditTime: 2021-03-17 13:59:42
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /Temp/lib/mserial.CPP
 */
#include <OLED.h>
#include <Arduino.h>

#include <U8g2lib.h>
#include <Wire.h>
#include <img.h>
U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

void OLED::LED_Init()
{
    u8g2.setI2CAddress(0x3C*2);
    u8g2.begin();
    u8g2.enableUTF8Print();
}
void OLED::LED_Print(int RSSI,float Temp,bool Bty,int Power)
{
    u8g2.firstPage();
    do
    {
        u8g2.setFont(u8g2_font_timR24_tf);
        u8g2.setFontPosTop();
        u8g2.setCursor(30,28);
        u8g2.print(Temp);

        switch (RSSI)
        {
        case 0:
            u8g2.drawXBMP(0, 0, 16, 16, WiFi_Img0);
            break;
        case 1:
            u8g2.drawXBMP(0, 0, 16, 16, WiFi_Img1);
            break;
        case 2:
            u8g2.drawXBMP(0, 0, 16, 16, WiFi_Img2);
            break;
        case 3:
            u8g2.drawXBMP(0, 0, 16, 16, WiFi_Img3);
            break;
        }
        
        switch (Power)
        {
        case 0:
            u8g2.drawXBMP(95, 0, 32, 16, Power_Img0);
            break;
        case 1:
            u8g2.drawXBMP(95, 0, 32, 16, Power_Img1);
            break;
        case 2:
            u8g2.drawXBMP(95, 0, 32, 16, Power_Img2);
            break;
        case 3:
            u8g2.drawXBMP(95, 0, 32, 16, Power_Img3);
            break;
        case 4:
            u8g2.drawXBMP(95, 0, 32, 16, Power_Img4);
            break;
        }

        switch (Bty)
        {
        case 0:
            u8g2.drawXBMP(114, 24, 14, 14, SheShiDu_Img);
            break;
        case 1:
            u8g2.drawXBMP(114, 24, 14, 14, HuaShiDu_Img);
            break;
        }
    }while(u8g2.nextPage());
    
}