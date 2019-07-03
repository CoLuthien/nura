#include <stdlib.h>
#include "task.h"

bool is_task(task_t * task)
{
    return task->magic == TASK_MAGIC;
}
static inline double get_time()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);

    double sec = (double)ts.tv_sec;
    double msec = (double)(ts.tv_nsec / 1000000);
    return sec + msec;
}

task_t* init_task(task_func func, uint16_t _interval, uint16_t _limit)
{
    task_t* t = (task_t*)malloc(sizeof(task_t));

    if (NULL == func)
    {
        free(t);
        return NULL;
    }

    t->interval = _interval;
    t->limit = _limit;
    t->magic = TASK_MAGIC;
    t->is_skipped = false;
    t->task = func;
    t->next_run = get_time() + (double)_interval;
    
    return t;
}

void insert_task(task_t* before, task_t* task)
{
    ASSERT(task != NULL);
    ASSERT(NULL != before);

    list_insert(before, &task->t_elem);
}

void insert_back_task(struct list* tasks, task_t* task)
{
    ASSERT(NULL != task);
    list_push_back(tasks, &task->t_elem);
}

void remove_task(task_t* task)
{
    ASSERT(NULL != task);
    ASSERT(is_task(task));

    list_remove(&task->t_elem);
    free(task);
    task = NULL;
}

task_t* fetch_task(struct list* tasks)
{
    if(list_empty(tasks))
    {
        return NULL;
    }
    struct list_elem* next_elem = list_pop_front(tasks);
    task_t* next = list_entry(next_elem, task_t, t_elem);
    ASSERT(is_task(next));

    return next;
}

void schedule(struct list* tasks, task_t* cur)
{
    ASSERT(is_task(cur));
    list_push_back(tasks, &cur->t_elem);
}