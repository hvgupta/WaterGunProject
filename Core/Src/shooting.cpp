#ifdef __cplusplus
extern "C" {
#endif

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
    mililitres shoot::getCurrentVolume(){
    	return this->currentVolume;
    }
    void shoot::updateCurrentVolume(mililitres curVol){
    	this->currentVolume = curVol;
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

				//LED strip animation
				//LED shoot out motion
				for (int i=0; i<10+3; i++)
				{
				  if (i<10){
					  //Turn on head of leftmost LED
					  Set_LED(i, 138, 43, 226);			//Blue-purple
					  //Turn on head of rightmost LED
					  Set_LED(19-i, 138, 43, 226);		//Blue-purple
				  }
				  if ( (i-3) >= 0 ){
					  //Turn off tail of leftmost LED
					  Set_LED(i-3, 0, 0, 0);
					  //Turn off tail of rightmost LED
					  Set_LED(19-i+3, 0, 0, 0);
				  }
				  Set_Brightness(70);
				  WS2812B_LED_Data_Send();
				  HAL_Delay (getPulseTime()/13);
				}

				//pump for getPulseTime()
				//Update currentVolume
				int Vafter = this->currentVolume - waterFlowRate/(60*(1000/getPulseTime()));
				this->currentVolume = (Vafter < 0 )? 0 : Vafter;

				//Maybe update something LCD screen
				/*add here*/
				char str_vol[4];
				sprintf(str_vol, "%d", getCurrentVolume());
				if(getCurrentVolume()/10 == 0){
					str_vol[1]='_';
					str_vol[2]='_';
				}
				else if(getCurrentVolume()/100 == 0){
					str_vol[2]='_';
				}
				LCD_DrawString(200,250, str_vol);		//test

				//Turn off the shooting water pump
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);		//In1 off
				HAL_Delay(this->cooldownTime);
    		}
    		else{
    			//LED warning motion (Breathing)
				for (int i=0; i<20; i++)
				{
					Set_LED(i, 255, 20, 40);			//RED
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
				LCD_DrawString(200,250, str_vol);		//test
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

				//LED strip animation
				//LED shoot out motion
				for (int i=0; i<10+3; i++)
				{
				  if (i<10){
					  //Turn on head of leftmost LED
					  Set_LED(i, 255, 105, 100);			//Pink
					  //Turn on head of rightmost LED
					  Set_LED(19-i, 255, 105, 100);			//Pink
				  }
				  if ( (i-3) >= 0 ){
					  //Turn off tail of leftmost LED
					  Set_LED(i-3, 0, 0, 0);
					  //Turn off tail of rightmost LED
					  Set_LED(19-i+3, 0, 0, 0);
				  }
				  Set_Brightness(70);
				  WS2812B_LED_Data_Send();
				  HAL_Delay (getPulseTime()/13);
				}

				//pump for getPulseTime()
				//Update currentVolume
				int Vafter = this->currentVolume - waterFlowRate/(60*(1000/getPulseTime()));
				this->currentVolume = (Vafter < 0 )? 0 : Vafter;
				//Maybe update something LCD screen
				/*add here*/
				char str_vol[4];
				sprintf(str_vol, "%d", this->getCurrentVolume());
				if(getCurrentVolume()/10 == 0){
					str_vol[1]='_';
					str_vol[2]='_';
				}
				else if(getCurrentVolume()/100 == 0){
					str_vol[2]='_';
				}
				LCD_DrawString(200,50, str_vol);		//test
				//Turn off the shooting water pump
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);		//In3 off
				HAL_Delay(this->delayBetween2Pulse);
    		}
    		else{
    			//Remind to refill water (LED set to red)
    			//LED warning motion (Breathing)
				for (int i=0; i<20; i++)
				{
					Set_LED(i, 255, 20, 40);			//RED
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
    }
}


#ifdef __cplusplus
}
#endif