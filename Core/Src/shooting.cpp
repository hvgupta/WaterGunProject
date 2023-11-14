#include "shooting.hpp"

namespace shootingProcess{

    shoot::shoot(const HAL_Ticks pT,const int waterOutFlow):pulseTime(pT),waterFlowRate(waterOutFlow){
        triggerState = false;
    }

    bool shoot::enoughWater(const int& waterLeft){
        return waterLeft > waterFlowRate;
    }

}