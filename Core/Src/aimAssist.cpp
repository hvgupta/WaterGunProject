#ifdef __cplusplus
extern "C" {
#endif

#include "aimAssist.hpp"

namespace aimAssist{

    /* The defination of aimAssist memeber functions begin here */

    aimAssist::aimAssist(){
        referenceBearing = 0;
        currentAngle = 0;
        targetAngle = 0;
        d1 = 0;
        d2 = 0;
        measureTime = 0;
        isScndCapture = false;
    }

    void aimAssist::measureAimAssist(TIM_HandleTypeDef* htim,I2C_HandleTypeDef* hi2c){
        HAL_Ticks initialTime = HAL_GetTick();
        HCSR04_Read(htim);
        HAL_Delay(100);
        degrees initialBearing = measureAngle(hi2c); // get the angle at 
        HCSR04_Read(htim);
        HAL_Delay(25);

        HCSR04_Read(htim);
        HAL_Delay(100);
        degrees targetBearing = measureAngle(hi2c);
        HCSR04_Read(htim);
        HAL_Delay(25);

        referenceBearing = targetBearing;
        currentAngle = targetBearing - initialBearing; // basically theta
        measureTime = (HAL_GetTick() - initialTime)/1000;
    }

    void aimAssist::measureNormal(TIM_HandleTypeDef* htim,I2C_HandleTypeDef* hi2c){
        HCSR04_Read(htim);
        HAL_Delay(50);
        degrees curBearing = measureAngle(hi2c);
        HCSR04_Read(htim);
        HAL_Delay(50);

        currentAngle = (360 + (curBearing - referenceBearing)%360)%360;
    }

    void aimAssist::updateD1D2(const centimeters d1d2){
        switch (d1){
            
            case 0:
                d1 = d1d2;
                break;
            default:
                d2 = d1d2;
                break;
        }
    }

    void aimAssist::D1D2Reset(){
        d1 = d2 = 0;
    }

    bool& aimAssist::currentFlag(){
        return isScndCapture;
    }

    degrees aimAssist::predictNext(const HAL_Ticks elapsedTime){
        float theta = currentAngle*M_PI/180.0;                      // conversion to radians
        float d0 = sqrt((float)(d1*d1 + d2*d2) - 2*d1*d2*cos(theta));
        float phi = acos(((d1*d1 + d0*d0 - d2*d2)/(2*d1*d0)));
        float multiple = (d0/measureTime)*(elapsedTime)/1000;
        float b = 2.0*multiple*d2*cos(theta+phi);
        float d3 = (-0.5)*(b+sqrt(b*b-4*(1-multiple)*(d2*d2))); // quadratic
        targetAngle = acos((d3*d3 + d2*d2 - (multiple*d3)*(multiple*d3))/(2*d3*d2))*M_PI/180.0;
    }
    
    void aimAssist::getCurrentinfo(int arrayOutput[4]){
        arrayOutput[0] = d1;
        arrayOutput[1] = currentAngle;
        arrayOutput[2] = targetAngle;
    }

    degrees aimAssist::measureAngle(I2C_HandleTypeDef*hi2c){
        //Measurement start
        uint8_t DataX[2];
        uint16_t X_ax;
        uint8_t DataY[2];
        uint16_t Y_ax;
        
        HAL_Delay(10);
        //Read x y from register
        HAL_I2C_Mem_Read(hi2c,0x3D,0x03,1,&DataX[0],1,100);	//DataX[0] store MSB of X
        HAL_I2C_Mem_Read(hi2c,0x3D,0x04,1,&DataX[1],1,100);	//DataX[1] store LSB of X
        HAL_I2C_Mem_Read(hi2c,0x3D,0x07,1,&DataY[0],1,100);	//DataX[0] store MSB of Y
        HAL_I2C_Mem_Read(hi2c,0x3D,0x08,1,&DataY[1],1,100);	//DataX[1] store LSB of Y
        //Convert x y from hex to dec
        X_ax = (DataX[0]<<8) | DataX[1];
        Y_ax = (DataY[0]<<8) | DataY[1];
        int X = twosHexToDec(X_ax);
        int Y = twosHexToDec(Y_ax);
        //Calculate the angle
        double angle_rad = atan2f(Y,X);
        return 180.0*angle_rad/M_PI;
    }

    /* end of the aimAssist class defining */

    void delay(TIM_HandleTypeDef* htim,const int time){
        __HAL_TIM_SET_COUNTER(htim,0);
        while(__HAL_TIM_GET_COUNTER (htim) < time);
    }

    void HCSR04_Read (TIM_HandleTypeDef* htim){
        HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_SET);  // pull the TRIG pin HIGH
        delay(htim,10);  // wait for 10 us
        HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_RESET);  // pull the TRIG pin low

        __HAL_TIM_ENABLE_IT(htim, TIM_IT_CC2);
    }

    int twosHexToDec(uint16_t hex) {
        int decimal;
        int sign = 1;
        //If the number is negative
        if ((hex & 0x8000) != 0) {
            sign = -1;
            // Flip the bits for negative number
            hex = ~hex + 1;
        }
        decimal = hex;
        return sign * decimal;
    }


}

#ifdef __cplusplus
}
#endif
