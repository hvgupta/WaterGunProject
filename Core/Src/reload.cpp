#ifdef __cplusplus
extern "C" {
#endif

#include "reload.hpp"

namespace reloadingProcess{

    Reload::Reload(const mililitres maxVol, mililitres curVol):currentVolume(curVol),maxVolume(maxVol){
        triggerState = false;
    }

    bool Reload::reachedMaxAmount(){
    	GPIO_PinState pinUpperBottle = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_10);
    	GPIO_PinState pinLowerBottle = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_11);
    	if (pinUpperBottle == GPIO_PIN_RESET && pinLowerBottle == GPIO_PIN_RESET){
    		this->currentVolume = this->maxVolume;
    		return true;
        }
    	return false;
    }
    void Reload::gunReloading(){
    	while (!reachedMaxAmount() && triggerState){
			//Turn on the reloading water pump
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);		//In3 on
			//Turn off the shooting water pump
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);	//In1 off

			//LED strip animation
			//LED reload motion
			for (int i=0; i<10+3; i++)
			{
			  if (i<10){
				  //Turn on head of middleleft LED
				  Set_LED(9-i, 42, 254, 183);			//Cyan
				  //Turn on head of middleright LED
				  Set_LED(10+i, 42, 254, 183);			//Cyan
			  }
			  if ( (i-3) >= 0 ){
				  //Turn on tail of middleleft LED
				  Set_LED(9-(i-3), 0, 0, 0);
				  //Turn off tail of middleright LED
				  Set_LED(10+(i-3), 0, 0, 0);
			  }
			  Set_Brightness(60);
			  WS2812B_LED_Data_Send();
			  HAL_Delay (19);
			}
			//pump for 19*13 = 247 ms

			//Update currentVolume
			int inputflow_rate = 1800; 			//ml/min	come from testing
			this->currentVolume += inputflow_rate/(60*(1000/247));
			if (this->currentVolume > maxVolume){
				this->currentVolume = maxVolume;
			}
			//Maybe update something LCD screen
			/*add here*/
			char str_vol[4];
			sprintf(str_vol, "%d", this->currentVolume);
			if(this->currentVolume/10 == 0){
				str_vol[1]='_';
				str_vol[2]='_';
			}
			else if(this->currentVolume/100 == 0){
				str_vol[2]='_';
			}
			LCD_DrawString(50,50, str_vol);		//test

		}
    	//Turn off the reloading water pump
    	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);		//In3 off
    	if (reachedMaxAmount()){
    		//Remind that water is full (LED set to green)
			//LED warning motion (Breathing)
			for (int i=0; i<20; i++)
			{
				Set_LED(i, 20, 255, 40);			//RED
			}
			for (int i=0; i<20; i++)
			{
			  Set_Brightness(3*i);
			  WS2812B_LED_Data_Send();
			  HAL_Delay (25);
			}
			for (int i=19; i>=0; i--)
			{
			  Set_Brightness(3*i);
			  WS2812B_LED_Data_Send();
			  HAL_Delay (25);
			}
			//Maybe update something LCD screen
			/*add here*/
			char str_vol[4];
			sprintf(str_vol, "%d", this->currentVolume);
			if(this->currentVolume/10 == 0){
				str_vol[1]='_';
				str_vol[2]='_';
			}
			else if(this->currentVolume/100 == 0){
				str_vol[2]='_';
			}
			LCD_DrawString(200,50, str_vol);		//test
    	}
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
    mililitres Reload::getCurrentVolume(){
    	return this->currentVolume;
    }
    void Reload::updateCurrentVolume(mililitres curVol){
    	this->currentVolume = curVol;
    }
}

#ifdef __cplusplus
}
#endif