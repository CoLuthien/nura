#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "../libs/data_struct/list.h"
#include "../libs/comports/debug.h"

#define TASK_MAGIC 0x1234


typedef void (*task_func)(void);
typedef struct _task_t
{
    task_func task;
    struct list_elem t_elem;
    char task_name[16];
    uint16_t magic;

    uint16_t interval; // in ms
    uint16_t limit; // in ms

    unsigned int sec;
    unsigned int msec;

    bool is_skipped;
}task_t;

typedef bool (*task_cond_func)(task_t* target, task_t* input);


task_t* init_task(task_func func, uint16_t _interval,
                  uint16_t _limit, const char* task_name);
void insert_back_task(struct list* tasks, task_t* task);
void insert_task(task_t* before, task_t* task);
/*
before insert     
        +------+        +-------+     +-------+ ...  +------+
    <---| head |<--->...|   1   |<--->|before | <--->| tail |<--->...
        +------+        +-------+     +-------+      +------+

after insert
        +------+        +-------+     +-------+ ...  +------+
    <---| head |<--->...|  task |<--->|before | <--->| tail |
        +------+        +-------+     +-------+      +------+
 */
void remove_task(task_t* task); // remove from task_list
void destroy_task(task_t* task);
bool is_task(task_t* task);

/*
    simple Round Robin scheduler
 */
task_t* fetch_task(struct list* tasks);
/*
    if list is null then return null
 */
void schedule(struct list* tasks, task_t* cur);// currently RR is used 