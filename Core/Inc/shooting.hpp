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
            mililitres currentVolume;
            bool triggerState;

        public:
            shoot(mililitres,const HAL_Ticks,const int);
            bool enoughWater();
            void setTriggerState(bool on);
            bool getTriggerState();
            HAL_Ticks getPulseTime();
    };

    class singleShot: public shoot{
        private:
            bool changeInTriggerState;
            const HAL_Ticks cooldownTime;

        public:
            singleShot(mililitres, const HAL_Ticks, const HAL_Ticks, const int);
            void gunShotonce();
    };

    class continousShots: public shoot{
        private:
            const HAL_Ticks delayBetween2Pulse;

        public:
            continousShots(mililitres, const HAL_Ticks, const HAL_Ticks, const int);
            void gunShotcontinous();
    };


}



#endif /* INC_SHOOTING_HPP_ */
