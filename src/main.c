#include <unistd.h>
#include <signal.h>
#include <math.h>
#include "task.h"
#include "mpu9250.h"
#include "gps.h"
#include "list.h"
#include "gpio.h"
#include "serial.h"
#include "log.h"
#include "lps25.h"

bool interrupt = false;
bool status = false;
static struct list main_task;
static i2c_dev_t* i2c = NULL;
static serial_dev_t* port1 = NULL;
static serial_dev_t* port2 = NULL;
static gps_t* gps = NULL;
static logger_t* flight_log = NULL;
static mpu9250_t* imu = NULL;
static lps25_t* baro = NULL;
struct timespec ts;

void update_imu()
{
    update_data(imu);
    calculate_accel();
}
void sig_handle(int signum)
{
    fclose(flight_log->fp);
    exit(0);
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

    char* gps_log = malloc(sizeof(char) * 1024);
    clock_gettime(CLOCK_MONOTONIC, &ts);
        
    sprintf(gps_log, "%d,%d  %.2f,%.2f,%.2f,%.2f,%d,%d,%.3f,%.3f,%.3f,%.2f,%.2f,%.2f,%2f,%2f\n",
        ts.tv_sec, ts.tv_nsec / 1000000,
        info->lat, info->lon, info->elv, info->speed, info->sig, info->fix,
        imu->accel[0], imu->accel[1], imu->accel[2],
        imu->gyro[0], imu->gyro[1], imu->gyro[2], baro->pressure, baro->temp
    );
    push_log(flight_log, gps_log);
}
void calculate_accel()
{
    float a_sqrd = 0.0;
    for (int i = 0; i < 3; i++)
    {
        a_sqrd += imu->accel[i] * imu->accel[i];
    }
    float a_sqrt = sqrt(a_sqrd);
    imu->mean_acc = a_sqrt;
}

void update_gps()
{
    gps_receive(gps);
}

void check_status()
{
    if(imu->mean_acc <= 0.1)
    {
        printf("!!");
        /*
            todo: 
            if the condition met then do the things
         */
    }
}
void update_pressure()
{
    update_baro(baro);
}

void run_task()
{
    while(!interrupt)
    {
        task_t* cur = fetch_task(&main_task);
        if(cur == NULL)
        {
            struct list_elem* cur = list_pop_front(&main_task);
            list_push_back(&main_task, cur);//To Do: change schedule algorithm
            continue;// you can do what ever you want. but you have to come back until the dead line
        }
        cur->task();
        schedule(&main_task, cur);
    }
}
void init_main()
{
    list_init(&main_task);
    i2c = init_i2c("/dev/i2c-1");
    port1 = init_serial("/dev/ttyAMA0", 9600);
//    port2 = init_serial("/dev/,", 115200);
    gps = init_gps(port1);
    flight_log = init_logger("flight_data", 64);
    imu = init_mpu9250(i2c, 50, 16, 500);
    baro = init_baro(i2c, 25);

    insert_back_task(&main_task, init_task(update_gps, 100, 150, "gps_update"));
    insert_back_task(&main_task,init_task(update_imu, 1000 / imu->super.rate, 20, "imu_update"));
    insert_back_task(&main_task,init_task(update_pressure, 1000 / baro->super.rate, 100, "barometer"));
    insert_back_task(&main_task, init_task(check_status, 10, 10, "status"));
    insert_back_task(&main_task,init_task(write_data_log, 10, 100, "data_log"));
}

int main(int argc, char* argv)
{
    init_main();
    run_task();
    return 0;
}