#ifdef __cplusplus
extern "C" {
#endif

#include "aimAssist.hpp"

namespace aimAssist{

    /* The defination of aimAssist memeber functions begin here */

    aimAssist::aimAssist(){
        currentBearing = 0;
        targetBearing = 0;
        d1 = 0;
        d0 = 0;
        theta = 0;
    }

    /* The end of the declaration of the aimAssist member function  */

    float sine(int deg) {
        deg %= 360; // make it less than 360
        float rad = deg * PI / 180;
        float sin = 0;

        int i;
        for(i = 0; i < TERMS; i++) { // That's Taylor series!!
            sin += power(-1, i) * power(rad, 2 * i + 1) / fact(2 * i + 1);
        }
        return sin;
    }

    float cosine(int deg) {
        deg %= 360; // make it less than 360
        float rad = deg * PI / 180;
        float cos = 0;

        int i;
        for(i = 0; i < TERMS; i++) { // That's also Taylor series!!
            cos += power(-1, i) * power(rad, 2 * i) / fact(2 * i);
        }
        return cos;
    }

    float power(float base, int exp) {
        if(exp < 0) {
            if(base == 0)
                return -0; // Error!!
            return 1 / (base * power(base, (-exp) - 1));
        }
        if(exp == 0)
            return 1;
        if(exp == 1)
            return base;
        return base * power(base, exp - 1);
    }

    int fact(int n) {
        return n <= 0 ? 1 : n * fact(n-1);
    }

}

#ifdef __cplusplus
}
#endif