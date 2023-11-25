/*
 * infoAndStatus.hpp
 */

#ifndef INC_INFOANDSTATUS_HPP_
#define INC_INFOANDSTATUS_HPP_
#include "main.h"
#include "lcd.h"
#include <cstdio>

#define TRIG_PIN GPIO_PIN_11
#define TRIG_PORT GPIOA

namespace WaterGun{

	enum class STATUS{
		OFF_STATE,
		RELOAD_STATE,
		SINGLE_SHOOT_STATE,
		CONTINIOUS_SHOOT_STATE
	};

	struct currentInfoDisplay{
		
		mililitres Volume;
		degrees Bearing;
		degrees targetBearing;
		STATUS status;
		meters closestObject;

		currentInfoDisplay();
		void displayBasic();
		void displayInfo();

	};

	void itos(const int,char*);
	char* STATUSToString(const STATUS&);

	void delay(const int);
}


#endif /* INC_INFOANDSTATUS_HPP_ */