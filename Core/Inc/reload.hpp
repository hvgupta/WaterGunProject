/*
 * reload.hpp
 */

#ifndef INC_RELOAD_HPP_
#define INC_RELOAD_HPP_
#include "main.h"

namespace reloadingProcess{

    class Reload{
        private:
            bool triggerState;
            mililitres currentVolume;
            const mililitres maxVolume;

        public:
            Reload(const mililitres,const mililitres);
            bool reachedMaxAmount();
            void gunReloading();
            void setTriggerState(bool on);
            bool getTriggerState();
            mililitres getCurrentVolume();
            void updateCurrentVolume(mililitres curVol);
    };

}

#endif /* INC_RELOAD_HPP_ */
