#include "reload.hpp"

namespace reloadingProcess{

    Reload::Reload(const mililitres maxVol):maxVolume(maxVol){
        triggerState = false;
        currentVolume = 0;
    }

    bool Reload::reachedMaxAmount(){
        return currentVolume == maxVolume;
    }


}