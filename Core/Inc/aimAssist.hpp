/*
 * aimAssist.hpp
 */

#ifndef INC_AIMASSIT_HPP_
#define INC_AIMASSIT_HPP_
#include "main.h"

#define TRIG_PIN GPIO_PIN_10
#define TRIG_PORT GPIOA

#define HMC5883L_Addr 0x1E
namespace aimAssist{

    class aimAssist{
        private:
            degrees referenceBearing;
            degrees currentBearing;
            degrees targetBearing;
            centimeters d1;
            centimeters d2;
            float measureTime;
            bool isScndCapture;

        public:
            aimAssist();
            
            void updateD1D2(const centimeters);
            void D1D2Reset();
            void measureAngle(I2C_HandleTypeDef*);
            void measureAimAssist(TIM_HandleTypeDef*,I2C_HandleTypeDef*);
            void measureNormal(TIM_HandleTypeDef*,I2C_HandleTypeDef*);

            degrees predictNext(const HAL_Ticks);

            bool& currentFlag();

            void getCurrentinfo(int*);
    };

    int twosHexToDec(uint16_t hex);
    void HCSR04_Read (TIM_HandleTypeDef* htim);
    void delay(TIM_HandleTypeDef*,const int);
}




#endif /* INC_AIMASSIT_HPP_ */
