#ifdef __cplusplus
extern "C" {
#endif

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

    void currentInfoDisplay::displayBasic(){
        LCD_DrawString(0,318,"Water");
        LCD_DrawString(17,318,"Level");

        LCD_DrawString(0,262,"STATUS");

        LCD_DrawString( 0,198,"Current");
        LCD_DrawString(17,198,"Bearing");

        LCD_DrawString( 0,126," Target");
        LCD_DrawString(17,126,"Bearing");

        LCD_DrawString(0,54,"DIST");

        LCD_DrawLine(34,319,34,0,0x0000);

        LCD_DrawLine(54,319,54,0,0x0000);

        LCD_DrawLine(0, 270, 54, 270, 0x0000);
        LCD_DrawLine(0, 206, 54, 206, 0x0000);
        LCD_DrawLine(0, 134, 54, 134, 0x0000);
        LCD_DrawLine(0,  62, 54,  62, 0x0000);
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
//        LCD_Clear(35,319,16,47,0xFFFF);
////        LCD_DrawString(35,319,itos(Volume));
//
//        LCD_Clear(35,271,17,51,0xFFFF);
//        LCD_DrawString(35,271,STATUSToString(status));

//        LCD_Clear()
    }

    /* End of the declaration of class functions */

//    char* itos(const int a){
//        char buffer[10];
//        sprintf(buffer,"%d",a);
//        return buffer;
//    }

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
