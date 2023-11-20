#include "reload.hpp"

namespace reloadingProcess{

    Reload::Reload(const mililitres maxVol):maxVolume(maxVol){
        triggerState = false;
        currentVolume = 0;
    }

    bool Reload::reachedMaxAmount(){
        return currentVolume == maxVolume;
    }
    void Reload::gunReloading(){
//    	while (!reachedMaxAmount()){
//			//Turn on the reloading water pump
//			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);		//In3 on
//			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);	//In4 off
//    	}
//    	//Turn off the reloading water pump
//    	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);	//In3 off
//    	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);	//In4 off

    	//testing

    	//Turn on the reloading water pump
    	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);		//In3 on
    	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);	//In4 off
    }

}
