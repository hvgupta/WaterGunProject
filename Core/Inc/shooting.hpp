/*
 * shooting.hpp
 */

#ifndef INC_SHOOTING_HPP_
#define INC_SHOOTING_HPP_
#include "main.h"

namespace shootingProcess{

    class shoot{
        private:
            const HAL_Ticks pulseTime;
            const int waterFlowRate;

            bool triggerState;

        public:
            shoot(const HAL_Ticks,const int);
            bool enoughWater(const int&);
    };

    class singleShot: public shoot{
        private:
            bool changeInTriggerState;
            const HAL_Ticks cooldownTime;

        public:
            singleShot(const HAL_Ticks, const HAL_Ticks, const int);
    };

    class continousShots: public shoot{
        private:
            const HAL_Ticks delayBetween2Pulse;

        public:
            continousShots(const HAL_Ticks, const HAL_Ticks, const int);
    };


}



#endif /* INC_SHOOTING_HPP_ */
