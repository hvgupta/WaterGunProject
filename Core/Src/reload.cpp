#ifdef __cplusplus
extern "C" {
#endif

#include "reload.hpp"
#include "main.h"
#include "stdio.h"
#include "infoAndStatus.hpp"

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
    	if (this->currentVolume >= 400)		//Added
    		return true;					//Added
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
			/*Test: Not favourable but allow dynamically changing the water level*/
			char toPrint[4]; //This line is added by zlashc
	        WaterGun::itos(this->currentVolume*100/400,toPrint);
	        LCD_Clear(35,271,18,48,0xffff);
	        LCD_DrawString(35,318,toPrint);
			/*End*/

		}
    	//Turn off the reloading water pump
    	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);		//In3 off

    	if (reachedMaxAmount()){
    		//Remind that water is full (LED set to green)
			//LED warning motion (Breathing)
			for (int i=0; i<20; i++)
			{
				Set_LED(i, 20, 255, 40);			//Green
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
