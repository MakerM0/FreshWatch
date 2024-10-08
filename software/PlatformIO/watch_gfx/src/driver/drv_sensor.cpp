/*** 
 * @Author: MakerM0
 * @Date: 2024-09-05 15:20:36
 * @LastEditTime: 2024-10-03 19:51:25
 * @FilePath: \jdi_watch_gfx\src\driver\drv_sensor.cpp
 * @Description: 
 * @https://github.com/MakerM0/
 * @Copyright (c) 2024 by MakerM0, All Rights Reserved. 
 */
#include "drv_sensor.h"
#include "drv_disp.h"
SemaphoreHandle_t wire_mux;

Adafruit_LSM6DS3TRC lsm6ds3trc;

Adafruit_SGP40 sgp;
Adafruit_SHT31 sht31;

static void lsm6d_setup(void)
{

    Serial.println("Adafruit LSM6DS3TR-C test!");

    if (!lsm6ds3trc.begin_I2C())
    {
        // if (!lsm6ds3trc.begin_SPI(LSM_CS)) {
        // if (!lsm6ds3trc.begin_SPI(LSM_CS, LSM_SCK, LSM_MISO, LSM_MOSI)) {
        Serial.println("Failed to find LSM6DS3TR-C chip");
        while (1)
        {
            delay(10);
        }
    }

    Serial.println("LSM6DS3TR-C Found!");

    lsm6ds3trc.setAccelRange(LSM6DS_ACCEL_RANGE_2_G);
    // Serial.print("Accelerometer range set to: ");
    // switch (lsm6ds3trc.getAccelRange())
    // {
    // case LSM6DS_ACCEL_RANGE_2_G:
    //     Serial.println("+-2G");
    //     break;
    // case LSM6DS_ACCEL_RANGE_4_G:
    //     Serial.println("+-4G");
    //     break;
    // case LSM6DS_ACCEL_RANGE_8_G:
    //     Serial.println("+-8G");
    //     break;
    // case LSM6DS_ACCEL_RANGE_16_G:
    //     Serial.println("+-16G");
    //     break;
    // }

    // lsm6ds3trc.setGyroRange(LSM6DS_GYRO_RANGE_250_DPS);
    // Serial.print("Gyro range set to: ");
    // switch (lsm6ds3trc.getGyroRange())
    // {
    // case LSM6DS_GYRO_RANGE_125_DPS:
    //     Serial.println("125 degrees/s");
    //     break;
    // case LSM6DS_GYRO_RANGE_250_DPS:
    //     Serial.println("250 degrees/s");
    //     break;
    // case LSM6DS_GYRO_RANGE_500_DPS:
    //     Serial.println("500 degrees/s");
    //     break;
    // case LSM6DS_GYRO_RANGE_1000_DPS:
    //     Serial.println("1000 degrees/s");
    //     break;
    // case LSM6DS_GYRO_RANGE_2000_DPS:
    //     Serial.println("2000 degrees/s");
    //     break;
    // case ISM330DHCX_GYRO_RANGE_4000_DPS:
    //     break; // unsupported range for the DS33
    // }

    lsm6ds3trc.setAccelDataRate(LSM6DS_RATE_52_HZ);
    // Serial.print("Accelerometer data rate set to: ");
    // switch (lsm6ds3trc.getAccelDataRate())
    // {
    // case LSM6DS_RATE_SHUTDOWN:
    //     Serial.println("0 Hz");
    //     break;
    // case LSM6DS_RATE_12_5_HZ:
    //     Serial.println("12.5 Hz");
    //     break;
    // case LSM6DS_RATE_26_HZ:
    //     Serial.println("26 Hz");
    //     break;
    // case LSM6DS_RATE_52_HZ:
    //     Serial.println("52 Hz");
    //     break;
    // case LSM6DS_RATE_104_HZ:
    //     Serial.println("104 Hz");
    //     break;
    // case LSM6DS_RATE_208_HZ:
    //     Serial.println("208 Hz");
    //     break;
    // case LSM6DS_RATE_416_HZ:
    //     Serial.println("416 Hz");
    //     break;
    // case LSM6DS_RATE_833_HZ:
    //     Serial.println("833 Hz");
    //     break;
    // case LSM6DS_RATE_1_66K_HZ:
    //     Serial.println("1.66 KHz");
    //     break;
    // case LSM6DS_RATE_3_33K_HZ:
    //     Serial.println("3.33 KHz");
    //     break;
    // case LSM6DS_RATE_6_66K_HZ:
    //     Serial.println("6.66 KHz");
    //     break;
    // }

    lsm6ds3trc.setGyroDataRate(LSM6DS_RATE_SHUTDOWN);
    // Serial.print("Gyro data rate set to: ");
    // switch (lsm6ds3trc.getGyroDataRate())
    // {
    // case LSM6DS_RATE_SHUTDOWN:
    //     Serial.println("0 Hz");
    //     break;
    // case LSM6DS_RATE_12_5_HZ:
    //     Serial.println("12.5 Hz");
    //     break;
    // case LSM6DS_RATE_26_HZ:
    //     Serial.println("26 Hz");
    //     break;
    // case LSM6DS_RATE_52_HZ:
    //     Serial.println("52 Hz");
    //     break;
    // case LSM6DS_RATE_104_HZ:
    //     Serial.println("104 Hz");
    //     break;
    // case LSM6DS_RATE_208_HZ:
    //     Serial.println("208 Hz");
    //     break;
    // case LSM6DS_RATE_416_HZ:
    //     Serial.println("416 Hz");
    //     break;
    // case LSM6DS_RATE_833_HZ:
    //     Serial.println("833 Hz");
    //     break;
    // case LSM6DS_RATE_1_66K_HZ:
    //     Serial.println("1.66 KHz");
    //     break;
    // case LSM6DS_RATE_3_33K_HZ:
    //     Serial.println("3.33 KHz");
    //     break;
    // case LSM6DS_RATE_6_66K_HZ:
    //     Serial.println("6.66 KHz");
    //     break;
    // }

    // lsm6ds3trc.configInt1(false, false, true); // accelerometer DRDY on INT1
    // lsm6ds3trc.configInt2(false, true, false); // gyro DRDY on INT2

    lsm6ds3trc.enablePedometer(true);
}

static void env_setup()
{
    if (!sgp.begin())
    {
        disp.println("SGP40 Sensor not found :(");disp.refresh();
        while (1)
            ;
    }

    if (!sht31.begin(0x44))
    { // Set to 0x45 for alternate i2c addr
        disp.println("Couldn't find SHT31");disp.refresh();
        while (1)
            ;
    }
}

bool wire_lock(uint32_t timeout_ms)
{

    const TickType_t timeout_ticks = (timeout_ms == 0) ? portMAX_DELAY : pdMS_TO_TICKS(timeout_ms);
    return xSemaphoreTakeRecursive(wire_mux, timeout_ticks) == pdTRUE;
}

void wire_unlock(void)
{

    xSemaphoreGiveRecursive(wire_mux);
}


void wire_begin()
{
    NRF_TWIM0->ENABLE=(TWIM_ENABLE_ENABLE_Enabled << TWIM_ENABLE_ENABLE_Pos);
}
void wire_end()
{
    NRF_TWIM0->ENABLE=(TWIM_ENABLE_ENABLE_Disabled << TWIM_ENABLE_ENABLE_Pos);
}

Sensor::Sensor(void)
{
    _step = 0;
    // env
    _sraw = 0; // voc_raw
    _voc_index = 0;
    _temperature = 0;
    _humidity = 0;
}
void Sensor::readAcceleration(void)
{
    wire_lock(0);
    wire_begin();
    lsm6ds3trc.readAcceleration(_acc_x, _acc_y, _acc_z);
    wire_end();
    wire_unlock();
}

float Sensor::readX(void)
{
    return _acc_x;
}

float Sensor::readY(void)
{
    return _acc_y;
}

float Sensor::readZ(void)
{
    return _acc_z;
}

void Sensor::updateSteps()
{
    wire_lock(0);
    wire_begin();
    _step = lsm6ds3trc.readPedometer();
    wire_end();
    wire_unlock();
}

void Sensor::resetSteps()
{
    wire_lock(0);
    wire_begin();
    lsm6ds3trc.resetPedometer();
    wire_end();
    wire_unlock();
}

uint16_t Sensor::getSteps()
{
    return _step;
}

void Sensor::updateEnv()
{
    wire_lock(0);
    wire_begin();
    float t = sht31.readTemperature() + _temperature_offset;
    // Serial.print("Temp *C = "); Serial.print(t); Serial.print("\t\t");
    float h = sht31.readHumidity();
    // Serial.print("Hum. % = "); Serial.println(h);
    _temperature = (t);
    _humidity = (h);
    _sraw = sgp.measureRaw(t, h);
    // Serial.print("Raw measurement: ");
    // Serial.println(sraw);

    _voc_index = sgp.measureVocIndex(t, h);
    wire_end();
    wire_unlock();
}
float Sensor::getHumidity()
{
    return _humidity;
}
float Sensor::getTemperature()
{
    return _temperature;
}
int32_t Sensor::getVocIndex()
{
    return _voc_index;
}

bool Sensor::begin(float temperature_offset)
{
    wire_mux = xSemaphoreCreateRecursiveMutex();

    _temperature_offset = temperature_offset;
    env_setup();
    lsm6d_setup();
    Wire.setClock(250000);

    return true;
}



Sensor sensor;
