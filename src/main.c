#include <unistd.h>
#include "task.h"
#include "log.h"

extern bool interrupt = false;
static struct list main_task;

void run_task()
{
    while(!interrupt)
    {
        task_t* cur = fetch_task(&main_task);
        cur->task();
        schedule(&main_task, cur);
    }
}

void init_main()
{
    list_init(&main_task);
    // insert tasks
    //initialize devices
}

int main()
{
    return 0;
}