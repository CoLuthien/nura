#include <stdlib.h>
#include "task.h"

//target is prior to 'to'
bool is_prior(task_t* to, task_t* target)// 'to' is try to insert, target is task in list
{
    if(to->sec > target->sec)
    {
        return true;
    }
    else if(to->msec > target->msec)
    {
        return true;
    }

    return false;
}

inline void update_time(task_t* task)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    time_t sec = ts.tv_sec;
    time_t msec = ts.tv_nsec / 1000000;

    msec += (time_t)task->interval;
    if(msec >= 1000)
    {
        sec += 1;
        msec -= 1000;
    }
    task->sec = sec;
    task->msec = msec;
}
//find task which met the condition.
task_t* find_task(struct list* tasks, task_t* task, task_cond_func cond)
{
    struct list_elem* prev = NULL;
    task_t* suffice_task = NULL;
    struct list_elem* last_elem = list_back(tasks);
    // now for O(N) search algorithm
    /* ToDO
        implement balanced tree algorithm
     */
    for (struct list_elem* cur = list_front(tasks); 
         cur != last_elem;
         cur = list_next(prev))
    {
        task_t* draw_task = list_entry(cur, task_t, t_elem);
        if(cond(task, draw_task))
        {
            suffice_task = draw_task;
            break;
        }
        prev = cur;
    }

   return suffice_task; 
}

inline bool is_task(task_t * task)
{
    return task->magic == TASK_MAGIC;
}

task_t* init_task(task_func func, uint16_t _interval, uint16_t _limit,
                  const char* task_name)
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
    memcpy(t->task_name, task_name, 16);

    update_time(t);
    
    return t;
}

void insert_task(task_t* before, task_t* task)
{
    ASSERT(task != NULL);
    ASSERT(NULL != before);

    list_insert(&before->t_elem, &task->t_elem);
}

void insert_back_task(struct list* tasks, task_t* task)
{
    ASSERT(NULL != task);
    list_push_back(tasks, &task->t_elem);
}

void remove_task(task_t* task)
{
    ASSERT(NULL != task);

    list_remove(&task->t_elem);
}

void destroy_task(task_t* task)
{
    ASSERT(NULL != task);
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
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    time_t sec = ts.tv_sec, msec = ts.tv_nsec / 1000000;
    struct list_elem* cur = list_pop_front(tasks);
    task_t* fetched = list_entry(cur, task_t, t_elem);
    if(fetched->sec < sec)
    {  
        return fetched;
    }
    else if(fetched->sec == sec)
    {
        if(fetched->msec <= msec)
        {
            return fetched;
        }            
    }

    list_push_back(tasks, cur);
    return NULL;
}

void schedule(struct list* tasks, task_t* cur)
{
    ASSERT(is_task(cur));

    update_time(cur);
    list_push_back(tasks, &cur->t_elem);
}