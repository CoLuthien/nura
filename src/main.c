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
#include <bcm2835.h>
#include <pthread.h>

#define IN  0
#define OUT 1

#define LOW  0
#define HIGH 1

#define PWM_PIN 12
#define PWM_CHANNEL 0
#define RANGE 1024

#define deploy_in_msec 1090


#define PIN  24 /* P1-18 */
#define POUT 4  /* P1-07 */

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

void deploy_para()
{
    for(int i = -RANGE; i < RANGE; i++)
    {
        bcm2835_pwm_set_data(PWM_CHANNEL, i);
        usleep(1 * 1000);
    }
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
        
    sprintf(gps_log, "%d,%d  %f,%f,%f,%f,%d,%d,%f,%f,%f,%f,%f,%f,%f,%f\n",
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
    static bool is_deploy = false;
    float p_diff = baro->ref_press - baro->pressure;
    static struct timespec deploy_time;
    
    if(p_diff <= 0 )
    {
        if(imu->accel[2] >= 1.6)
        {
            is_deploy = true;
            clock_gettime(CLOCK_MONOTONIC, &deploy_time);
            time_t sec = deploy_in_msec / 1000;
            time_t msec = deploy_in_msec - sec * 1000;
            deploy_time.tv_sec += sec;
            time_t nmsec = deploy_time.tv_nsec / 1000000;
            nmsec += msec;
            if(nmsec >= 1000)
            {
                deploy_time.tv_sec += 1;
                nmsec -= 1000;
            }
            deploy_time.tv_nsec = nmsec * 1000000;
        }
    }
    if(is_deploy)
    {
        struct timespec cur;
        clock_gettime(CLOCK_MONOTONIC, &cur);
        if(deploy_time.tv_sec < cur.tv_sec)
        {
            deploy_para();           
        }
        else if(deploy_time.tv_sec == cur.tv_sec)
        {
            if(deploy_time.tv_nsec <= cur.tv_nsec)
            {
                deploy_para();
            }            
        }
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
    if(!bcm2835_init())
    {
        char* debug = "bcm2835 init failed!!\n";
        push_log(flight_log, debug);
        flush_log(flight_log);
        exit(-1);
    }

    bcm2835_gpio_fsel(PWM_PIN, BCM2835_GPIO_FSEL_ALT5);
    bcm2835_pwm_set_clock(BCM2835_PWM_CLOCK_DIVIDER_64);
    bcm2835_pwm_set_mode(PWM_CHANNEL, 1, 1);
    bcm2835_pwm_set_range(PWM_CHANNEL, RANGE);
    bcm2835_pwm_set_data(PWM_CHANNEL, RANGE * -1);// zero init

    insert_back_task(&main_task, init_task(update_gps, 100, 150, "gps_update"));
    insert_back_task(&main_task,init_task(update_imu, 1000 / imu->super.rate, 20, "imu_update"));
    insert_back_task(&main_task,init_task(update_pressure, 1000 / baro->super.rate, 100, "barometer"));
    insert_back_task(&main_task, init_task(check_status, 10, 10, "status"));
    insert_back_task(&main_task,init_task(write_data_log, 20, 100, "data_log"));

    export_gpio(POUT);
    set_gpio_direction(POUT, OUT);
    for(int i = 0; i < 10; i++)
    {
        write_gpio(POUT, i % 2);
        usleep(100*1000);
    }
    unexport_gpio(POUT);
}

int main(int argc, char* argv)
{
    init_main();
    run_task();
    return 0;
}