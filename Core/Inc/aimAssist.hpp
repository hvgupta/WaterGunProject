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
            degrees currentBearing;
            degrees targetBearing;
            meters d1;
            meters d0;
            degrees theta;
        public:
            aimAssist();
            degrees predictNext();
            void calculateD0andD1();
    };
}




#endif /* INC_AIMASSIT_HPP_ */
