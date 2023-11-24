#ifdef __cplusplus
extern "C" {
#endif

#include "LED.hpp"

namespace LED{
    
    void LEDLight::operator=(const int rgb[3]){
        red = rgb[0];
        green = rgb[1];
        blue = rgb[2];
    }

    int& LEDLight::operator[](const char colour){
        switch (colour)
        {
        case 'r':
            return red;
        case 'g':
            return green;
        case 'b':
            return blue;
        }
    }

    void LEDLight::setRGBValue(const int r, const int g, const int b){
        red = r; green = g; blue = b;
    }

    LED::LED(const int size):maxLEDNumber(size){}

    LEDLight& LED::operator[](const int index){
        return LEDValues[index];
    }

    void LED::Set_Brightness(const int brightness){
        //Make sure value is from 0 to 100
        int newBrightness = (brightness < 0) ? 0 : (brightness > 100) ? 100 : brightness;

        //Tangent for linear scaling
        int brightness_45 = brightness*45/100;	// 0-45
        float angle = 90-brightness_45;  	// in degree

        for (int i=0; i<maxLEDNumber; i++){
            for (int j=0; j<3; j++){
                LEDValues[i][j] = LEDValues[i][j] / tangent(angle*PI / 180);
            }
        }
    }

    void LED::WS2812B_LED_Data_Send(){
        uint32_t indx = 0;
        uint32_t colorData;			//use 32 bits as need transmit 24 bits RGB.
        uint16_t pwmData[24*maxLEDNumber + 45];

        for (int i = 0; i < maxLEDNumber ; i++){
            //24 bit, G7 G6 G5 G4 G3 G2 G1 G0 R7 R6 R5 R4 R3 R2 R1 R0 B7 B6 B5 B4 B3 B2 B1 B0
            colorData = (LEDValues[i][0] << 16) |
                            (LEDValues[i][1] << 8) |
                                        LEDValues[i][2];

            //High bit is sent first, follow data sheet
            for (int j = 23; j >= 0; j--)
                pwmData[indx++] = (colorData & (1 << j)) ? 60 : 30;	// 1s: ~64% of 90, 0s: ~32% of 90
        }
        //Reset code which is above 50 us of 0s, period of clock is 1.25us
        for (int i = 0; i < 45; i++)
            pwmData[indx++] = 0;

        HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1, (uint32_t *)pwmData, indx);

        // while (!dataSent_Finish){
        // 	// Wait for data to be sent
        // }
        // // Reset the flag
        // dataSent_Finish = 0;
    }

    int LED::getSize(){
        return maxLEDNumber;
    }

}




#ifdef __cplusplus
}
#endif