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
        measureTime = 0;
    }

    void aimAssist::updateD0D1(const meters d1d2[2]){
        d1 = d1d2[0];
        d2 = d1d2[1];
    }

    degrees aimAssist::predictNext(const HAL_Ticks elapsedTime){
        float theta = (currentBearing - lastBearing)*M_PI/180.0;
        float d0 = sqrt((float)(d1*d1 + d2*d2) - 2*d1*d2*cos(theta));
        float phi = acos(((d1*d1 + d0*d0 - d2*d2)/(2*d1*d0)));
        float multiple = (d0/measureTime)*(elapsedTime);
        float b = 2.0*multiple*d2*cos(theta+phi);
        float d3 = (-0.5)*(b+sqrt(b*b-4*(1-multiple)*(-d2*d2))); // quadratic
        targetBearing = acos((d3*d3 + d2*d2 - (multiple*d3)*(multiple*d3))/(2*d3*d2))*M_PI/180.0;
    }

    
    /* The end of the declaration of the aimAssist member function  */
    void aimAssist::HCSR04_Read (TIM_HandleTypeDef* htim){
        HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_SET);  // pull the TRIG pin HIGH
        delay(htim,10);  // wait for 10 us
        HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_RESET);  // pull the TRIG pin low

        __HAL_TIM_ENABLE_IT(htim, TIM_IT_CC2);
    }

    void delay(TIM_HandleTypeDef* htim,const int time){
        __HAL_TIM_SET_COUNTER(htim,0);
        while(__HAL_TIM_GET_COUNTER (htim) < time);
    }

}

#ifdef __cplusplus
}
#endif