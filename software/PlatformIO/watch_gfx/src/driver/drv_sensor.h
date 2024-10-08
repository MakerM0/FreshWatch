#ifndef __DRIVER_Sensor_H
#define __DRIVER_Sensor_H

#include <Arduino.h>
#include "Adafruit_SGP40.h"
#include "Adafruit_SHT31.h"
#include "Adafruit_LSM6DS3TRC.h"

extern Adafruit_LSM6DS3TRC lsm6ds3trc;
extern Adafruit_SGP40 sgp;
extern Adafruit_SHT31 sht31;

class Sensor
{
public:
    Sensor(void);
    bool begin(float temperature_offset=-2);
    void updateSteps();
    void resetSteps();
    uint16_t getSteps();



    void updateEnv();
    float getHumidity();
    float getTemperature();
    int32_t getVocIndex();

    void readAcceleration(void);

float readX(void);
float readY(void);
float readZ(void);
 

    /* data */
private:
    // lsm6d
    uint16_t _step;
    // env
    uint16_t _sraw; // voc_raw
    int32_t _voc_index;
    float _temperature;
    float _humidity;
    float _temperature_offset;

    float _acc_x,_acc_y,_acc_z;

    void env_loop();
};

extern Sensor sensor;
#endif


