/*
 * infoAndStatus.hpp
 */

#ifndef INC_INFOANDSTATUS_HPP_
#define INC_INFOANDSTATUS_HPP_
#include "main.h"
#include "lcd.h"

namespace WaterGun{

	enum class STATUS{
		OFF_STATE,
		RELOAD_STATE,
		SINGLE_SHOOT_STATE,
		CONTINIOUS_SHOOT_STATE
	};

	class currentInfoDisplay{
		private:
			mililitres Volume;
			degrees Bearing;
			degrees targetBearing;
			STATUS status;
			int batteryLevel;
			meters closestObject;
			
		public:
			currentInfoDisplay();
			void displayInfo();
			STATUS getStatus();
			void changeStatus(STATUS);
	};
}


#endif /* INC_INFOANDSTATUS_HPP_ */
