/*
 * @Author: your name
 * @Date: 2021-03-16 18:45:54
 * @LastEditTime: 2021-03-17 12:07:24
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /Temp/include/mserial.h
 */
#ifndef OLED_H_
#define OLED_H_

class OLED
{
    public:
        void LED_Init();
        void LED_Print(int RSSI,float Temp,bool Bty,int Power);
    private:

};//注意这里的分号不要漏掉

#endif