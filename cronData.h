#ifndef _CRON_DATA_H_
#define _CRON_DATA_H_

#include <vector>
#include "taskQueue.h"


class CronData
{
public:

	~CronData(){
		stopflag_ = true;
	}
	//name 联动linkageID_$index linkageID为uuid，理论上唯一 ; expr ,crontab 表达式， _ballback 回调函数， ptr_ 对象指针 
	bool add_schedule(std::string name, std::string expr, dual_callback _callback, void * ptr_);

	void clear_schedules();

	// name : linkageID_$index  
	// 根据linkageID_$index  和expr 删除队列中的单条task记录，在第一条执行完成后，
	// 判断联动下次某个时间点是否需要再次执行，不需要从task中删除
	void remove_schedules(std::string name, std::string expr);

	// name : linkageID  
	// 根据linkageID删除队列中的task记录name字符串中包含linkageID的记录
	// 在联动发布转编辑时，需要将现有队列中，该linkageID的所有task都删除
	void remove_schedules(std::string linkageID);

	void start();

	void stop();

	void exec();
	bool check_queue();

	bool stopflag_;

private:

	TaskQueue  m_taskQueue;

	pthread_t tick_thread_id;


};

#endif