#include "libs/task.h"
#include "libs/data_struct/list.h"
#include <stdio.h>

void task_func1()
{
	printf("func1!!");
}
void task_func2()
{
	printf("func2!!");
}
struct list tasks;
struct list* my_list;
int main()
{
	
	printf("my name is ");printf("hi\n");
	printf("!!");
	my_list = &tasks;
	printf("%p\n", my_list);
	list_init(my_list);
	printf("%p\n", my_list);
	task_t* task1 = init_task(task_func1, 100, 100, "TASK1");
	task_t* task2 = init_task(task_func2, 500, 500, "TASK2");
	printf("init process is done\n");
	insert_back_task(my_list, task1);
	insert_back_task(my_list, task2);

	printf("insert is done!!\n");
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
