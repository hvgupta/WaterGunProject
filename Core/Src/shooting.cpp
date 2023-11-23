#ifdef __cplusplus
extern "C" {
#endif

#include "shooting.hpp"

namespace shootingProcess{

    shoot::shoot(const HAL_Ticks pT,const int waterOutFlow):pulseTime(pT),waterFlowRate(waterOutFlow){
        triggerState = false;
    }

    bool shoot::enoughWater(const int& waterLeft){
        return waterLeft > waterFlowRate;
    }

}

#ifdef __cplusplus
}
#endif