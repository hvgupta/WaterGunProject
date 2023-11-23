#ifdef __cplusplus
extern "C" {
#endif

#include "reload.hpp"

namespace reloadingProcess{

    Reload::Reload(const mililitres maxVol):maxVolume(maxVol){
        triggerState = false;
        currentVolume = 0;
    }

    bool Reload::reachedMaxAmount(){
    	GPIO_PinState pinUpperBottle = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_10);
    	GPIO_PinState pinLowerBottle = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_11);
    	if (pinUpperBottle == GPIO_PIN_RESET && pinLowerBottle == GPIO_PIN_RESET){
    		currentVolume = maxVolume;
    		return true;
        }
    	return false;
    }
    void Reload::gunReloading(){
    	while (!reachedMaxAmount() && triggerState){
			//Turn on the reloading water pump
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);		//In3 on
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);	//In2, In4 off

			//Maybe update something LCD screen
			/*add here*/

			//LED strip animation
			/*add here*/

			//Update currentVolume
			/*add here*/
			//Base on actual testing
			//If flow_rate = 1000ml/min, then
			//currentVolume += flow_rate/(60*(1000/50));
			//if currentVolume > maxVolume, then currentVolume = maxVolume
			HAL_Delay(50);		//0.05s pump 0.1 second
		}

    	//Turn off the reloading water pump
    	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);		//In3 off
    	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);	//In2, In4 off
    }
    void Reload::setTriggerState(bool on){
    	if (on){
    		this->triggerState = true;
    	}
    	else{
    		this->triggerState = false;
    	}
    }
    bool Reload::getTriggerState(){
    	return triggerState==true;
    }

}

#ifdef __cplusplus
}
#endif