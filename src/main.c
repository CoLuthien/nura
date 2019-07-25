#include <unistd.h>
#include "task.h"
#include "mpu9250.h"
#include "gps.h"
#include "list.h"
#include "gpio.h"
#include "serial.h"
#include "log.h"
#include "lps25.h"

extern bool interrupt = false;
static struct list main_task;
static i2c_dev_t* i2c = NULL;
static serial_dev_t* port1 = NULL;
static serial_dev_t* port2 = NULL;
static gps_t* gps = NULL;
static logger_t* log = NULL;
static logger_t* debug = NULL;
static mpu9250_t* imu = NULL;
static lps25_t* baro = NULL;

void update_imu()
{
    update_data(imu);
}

void write_data_log()
{
    nmeaINFO* info = &gps->cur_info;
    double gps_info[] = {
        info->lat, info->lon, info->elv, info->speed, info->sig, info->fix
    };
    double imu_info[] = {
        imu->accel[0], imu->accel[1], imu->accel[2],
        imu->gyro[0], imu->gyro[1], imu->gyro[2],
        imu->accel_res, imu->gyro_res
    };

    char* gps_log = malloc(sizeof(char) * 512);
    int next = 0;
    for(int i = 0; i < 6; i++)
    {
        next = sprintf(gps_log + next, "%.2f, ", gps_info[i]);
    }
    push_log(log, gps_log);
    
    char* imu_log = malloc(sizeof(char)* 256);
    next = 0;
    for(int i = 0; i < 8; i++)
    {
        next = sprintf(imu_log + next, "%.2f, ", imu_info[i]);
    }
    push_log(log, imu_log);

}
void update_gps_baro()
{
    if(gps_receive(gps))
    {
      // show the infos
    }
    update_baro(baro);
}

void run_task()
{
    while(!interrupt)
    {
        task_t* cur = fetch_task(&main_task);
        if(cur == NULL)
        {
            continue;// you can do what ever you want. but you have to come back until the dead line
        }
        cur->task();
        schedule(&main_task, cur);
    }
}

void init_main()
{
    printf("hi!!\n");
    list_init(&main_task);
    i2c = init_i2c("/dev/i2c-1");
    port1 = init_serial("/dev/ttyAMA0", 9600);
    port2 = init_serial("/dev/,", 115200);
    gps = init_gps(port1);
    log = init_logger("name..", 64);
    imu = init_mpu9250(i2c, 50, 16, 500);
    baro = init_baro(i2c);

    init_task(update_gps_baro, 100, 150, "gps_update");
    init_task(update_imu, 20, 20, "imu_update");
    init_task(write_data_log, 100, 100, "data_log");
    
}

int main()
{
    init_main();
    run_task();
    return 0;
}