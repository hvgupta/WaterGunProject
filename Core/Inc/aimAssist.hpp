/*
 * aimAssist.hpp
 */

#ifndef INC_AIMASSIT_HPP_
#define INC_AIMASSIT_HPP_
#include "main.h"

#define TRIG_PIN GPIO_PIN_11
#define TRIG_PORT GPIOA

namespace aimAssist{

    class aimAssist{
        private:
            degrees lastBearing;
            degrees currentBearing;
            degrees targetBearing;
            meters d1;
            meters d2;
            HAL_Ticks measureTime;

        public:
            aimAssist();
            void updateD0D1(const meters*);
            degrees predictNext(const HAL_Ticks);
            void HCSR04_Read (void);
    };
}




#endif /* INC_AIMASSIT_HPP_ */