/*
 * aimAssist.hpp
 */

#ifndef INC_AIMASSIT_HPP_
#define INC_AIMASSIT_HPP_
#include "main.h"

#define PI 3.14159
#define TERMS 7

namespace aimAssist{

    class aimAssist{
        private:
            degrees lastBearing;
            degrees currentBearing;
            degrees targetBearing;
            meters d1;
            meters d2;
            degrees theta;
        public:
            aimAssist();
            void updateD0D1(const meters*);
            degrees predictNext(const HAL_Ticks);
        
    };
}




#endif /* INC_AIMASSIT_HPP_ */
