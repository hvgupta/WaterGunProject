#include "shooting.hpp"

namespace shootingProcess{

    shoot::shoot(mililitres curVol,const HAL_Ticks pT,const int waterOutFlow):pulseTime(pT),waterFlowRate(waterOutFlow),currentVolume(curVol){
        triggerState = false;
    }

    bool shoot::enoughWater(){
    	GPIO_PinState pinUpperBottle = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_10);
		GPIO_PinState pinLowerBottle = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_11);
		if (pinUpperBottle == GPIO_PIN_SET && pinLowerBottle == GPIO_PIN_SET){
			this->currentVolume = 0;
			return false;
		}
		return true;
    }

    void shoot::setTriggerState(bool on){
    	if (on){
    		this->triggerState = true;
    	}
    	else{
    		this->triggerState = false;
    	}
    }
    bool shoot::getTriggerState(){
    	return triggerState==true;
    }
    HAL_Ticks shoot::getPulseTime(){
		return this->pulseTime;
	}

    singleShot::singleShot(mililitres curVol, const HAL_Ticks pT, const HAL_Ticks cd, const int waterOutFlow):shoot(curVol,pT,waterOutFlow),cooldownTime(cd){
    }
    void singleShot::gunShotonce(){
    	if(getTriggerState()){
    		if(enoughWater()){
    			//Turn off the reloading water pump
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);		//In3 off
				//Turn on the shooting water pump
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);			//In1 on

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

				HAL_Delay(getPulseTime());		//0.05s pump 0.1 second
				//Turn off the shooting water pump
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);		//In1 off
				HAL_Delay(this->cooldownTime);
    		}
    		else{
    			//Remind to refill water (LED set to red), etc
    		}
    	}
    }

    continousShots::continousShots(mililitres curVol, const HAL_Ticks pT, const HAL_Ticks delay, const int waterOutFlow):shoot(curVol,pT,waterOutFlow),delayBetween2Pulse(delay){
    }

    void continousShots::gunShotcontinous(){
    	while(getTriggerState()){
    		if(enoughWater()){
    			//Turn off the reloading water pump
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);		//In3 off
				//Turn on the shooting water pump
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);			//In1 on

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

				HAL_Delay(getPulseTime());		//0.05s pump 0.1 second
				//Turn off the shooting water pump
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);		//In3 off
				HAL_Delay(this->delayBetween2Pulse);
    		}
    		else{
    			//Remind to refill water (LED set to red), etc
    		}
    	}
    }
}
