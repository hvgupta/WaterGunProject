/*
 * aimAssist.hpp
 */

#ifndef INC_AIMASSIT_HPP_
#define INC_AIMASSIT_HPP_
#include "main.h"

namespace aimAssist{

    class aimAssist{
        private:
            degrees currentBearing;
            degrees targetBearing;
            meters d1;
            meters d0;
            degrees theta;
        public:
            aimAssist();
            degrees predictNext();
    };

}




#endif /* INC_AIMASSIT_HPP_ */
