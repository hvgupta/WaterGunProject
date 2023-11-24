#ifndef __LED_HPP__
#define __LED_HPP__

#include "main.h"

namespace LED{

    struct LEDLight
    {
        int red;
        int green;
        int blue;

        void operator=(const int*);
        int& operator[](const char);
        void setRGBValue(const int, const int, const int);
    };

    class LED{
        private:
            LEDLight LEDValues[30];
            const int maxLEDNumber;
            bool dataSent_Finish;
        public:
            LED(const int);
            LEDLight& operator[](const int);
            void Set_Brightness (int);
            void WS2812B_LED_Data_Send();
            int getSize();
    };  


}

#endif