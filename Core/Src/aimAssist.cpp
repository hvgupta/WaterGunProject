#ifdef __cplusplus
extern "C" {
#endif

#include "aimAssist.hpp"

namespace aimAssist{

    /* The defination of aimAssist memeber functions begin here */

    aimAssist::aimAssist(){
        lastBearing = 0;
        currentBearing = 0;
        targetBearing = 0;
        d1 = 0;
        d2 = 0;
        theta = 0;
    }

    void aimAssist::updateD0D1(const meters d1d2[2]){
        d1 = d1d2[0];
        d2 = d1d2[1];
    }

    degrees aimAssist::predictNext(const HAL_Ticks elapsedTime){
        float d_0squrared = (float)(d1*d1 + d2*d2) - 2*d1*d2*cos(currentBearing - lastBearing);
        float cosPHI = (d1*d1 + d_0squrared - d2*d2)/(2*d1*sqrt(d_0squrared));
        float dp = (sqrt(d_0squrared)/elapsedTime)*();
    }

    
    /* The end of the declaration of the aimAssist member function  */
    void HCSR04_Read (void){
        HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_SET);  // pull the TRIG pin HIGH
        delay(10);  // wait for 10 us
        HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_RESET);  // pull the TRIG pin low

        __HAL_TIM_ENABLE_IT(&htim1, TIM_IT_CC2);
    }

}

#ifdef __cplusplus
}
#endif