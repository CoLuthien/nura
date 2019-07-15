#include "task.h"
#include "list.h"
#include <stdio.h>

void task_func1()
{
	printf("func1!!\n");
}
void task_func2()
{
	printf("func2!!\n");
}
struct list tasks;
struct list* my_list;
int main()
{
	
	my_list = &tasks;
	list_init(my_list);
	task_t* task1 = init_task(task_func1, 100, 100, "TASK1");
	task_t* task2 = init_task(task_func2, 500, 500, "TASK2");
	insert_back_task(my_list, task1);
	insert_back_task(my_list, task2);

	while(1)
	{
		task_t* fetched = fetch_task(my_list);
		if (fetched == NULL)
		{
			continue;
		}
		fetched->task();
		schedule(my_list, fetched);
	}
}
