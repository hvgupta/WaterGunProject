#ifdef __cplusplus
extern "C" {
#endif

#include "infoAndStatus.hpp"

namespace WaterGun{
    /* Declaration of all the currentInfoDisplay class functions */


    currentInfoDisplay::currentInfoDisplay(){
        Volume = 100;
        Bearing = 0;
        targetBearing = 0;
        status = STATUS::OFF_STATE;
    }

    void currentInfoDisplay::displayBasic(){
        LCD_DrawString(0,318,"Water");
        LCD_DrawString(17,318,"Level");

        LCD_DrawString(0,262,"STATUS");

        LCD_DrawString( 0,198,"Current");
        LCD_DrawString(17,198,"Bearing");

        LCD_DrawString( 0,126," Target");
        LCD_DrawString(17,126,"Bearing");

        LCD_DrawString(0,54,"DIST");

        // Below this lines are drawn to make columns 

        LCD_DrawLine(34,319,34,0,0x0000); // hortizontal line for the header
                                            // The info will be between these two lines
        LCD_DrawLine(54,319,54,0,0x0000); // hortizontal line for the info 

        /*
            Below are vertical lines which divide the table into columns
        */

        LCD_DrawLine(0, 270, 54, 270, 0x0000); 
        LCD_DrawLine(0, 206, 54, 206, 0x0000);
        LCD_DrawLine(0, 134, 54, 134, 0x0000);
        LCD_DrawLine(0,  62, 54,  62, 0x0000);

        // each cell has a differnt size, but it is right of 34 (so clear from 35)

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
        /*
            The procedure will be to first clear out the entire cell and then update information
                This is to ensure no visual bugs pertain.
        */ 
        char* toPrint;
        itos(Volume,toPrint);
        LCD_Clear(35,271,18,48,0xffff);
        LCD_DrawString(35,318,toPrint);

        LCD_Clear(35,207,18,62,0xffff);
        LCD_DrawString(35, 269, STATUSToString(status));

        itos(Bearing,toPrint);
        LCD_DrawString(35,205,toPrint);

        itos(targetBearing,toPrint);
        LCD_DrawString(35,133,toPrint);

        itos(closestObject,toPrint);
        LCD_DrawString(35,61,toPrint);

    }

    /* End of the declaration of class functions */

	static uint32_t IC_Val1 = 0;
	static uint32_t IC_Val2 = 0;
	static uint32_t Difference = 0;
	static uint8_t Is_First_Captured = 0;  // is the first value captured ?
	static uint8_t Distance  = 0;

    // converts a given integer into char*
    void itos(const int a, char* buffer){
        sprintf(buffer,"%03d",a);
    }

    char* STATUSToString(const STATUS& status){
        switch (status)
        {
        case STATUS::OFF_STATE:
            return "OFF";
        case STATUS::SINGLE_SHOOT_STATE:
            return "SINGLE";
        case STATUS::CONTINIOUS_SHOOT_STATE:
            return "CONTI";
        case STATUS::RELOAD_STATE:
            return "RELOAD";
        default:
            return "error";
        }
    }

}

#ifdef __cplusplus
}
#endif