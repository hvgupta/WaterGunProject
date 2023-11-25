/*
 * aimAssist.hpp
 */

#ifndef INC_AIMASSIT_HPP_
#define INC_AIMASSIT_HPP_
#include "main.h"

#define TRIG_PIN GPIO_PIN_10
#define TRIG_PORT GPIOA

namespace aimAssist{

    class aimAssist{
        private:
            degrees referenceBearing;
            degrees currentBearing;
            degrees targetBearing;
            centimeters d1;
            centimeters d2;
            HAL_Ticks measureTime;
            bool isScndCapture;

        public:
            aimAssist();
            
            void updateD1D2(const centimeters);
            void D1D2Reset();

            degrees predictNext(const HAL_Ticks);

            bool& currentFlag();

            centimeters getCurrentDist();
    };
    void HCSR04_Read (TIM_HandleTypeDef* htim);
    void delay(TIM_HandleTypeDef*,const int);
}




#endif /* INC_AIMASSIT_HPP_ */