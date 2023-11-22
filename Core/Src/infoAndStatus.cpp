#include "infoAndStatus.hpp"

namespace WaterGun{
    /* Declaration of all the currentInfoDisplay class functions */

    currentInfoDisplay::currentInfoDisplay(){
        Volume = 0;
        Bearing = 0;
        targetBearing = 0;
        status = STATUS::OFF_STATE;
        batteryLevel = 100;
    }

    /*
        The Following function will be used to display the following quantities on the screen
        - water level
        - status
        - battery level (if possible)
        - bearing
        - target bearing(only show if in tracking mode)
        - the distance of the closest object
        other things to consider, maybe making the board display in landscape instead of potrait
    */
    void currentInfoDisplay::displayInfo(){


    }
    STATUS currentInfoDisplay::getStatus(){
    	return this->status;
    }
    void currentInfoDisplay::changeStatus(STATUS a){
    	this->status = a;
    }

}
