#include "cronData.h"
#include <stdlib.h>
#include <thread>
#include <iostream> 
#include <unistd.h>
using namespace std;


void * tick_thread( void * _ptr_param)
{
	CronData * pThis = (CronData * ) _ptr_param;

	while(pThis->stopflag_ == false){
		bool _call_flag = false;

		m_mutex.lock();
		_call_flag = pThis->check_queue();
		if(_call_flag){
			pThis->exec();
			m_mutex.unlock();
		}else{
			m_mutex.unlock();
			usleep(20000);
		}
	}

}

bool CronData::add_schedule(std::string name, std::string expr, dual_callback _callback, void * ptr_)
{
	Task _task(name, expr, _callback, ptr_);

	m_taskQueue.push(_task);
	m_taskQueue.sort();

}

void CronData::clear_schedules(){
	m_taskQueue.clear();
	
}

void CronData::remove_schedules(std::string name, std::string expr)
{

	m_taskQueue.remove(name, expr);
	
}

void CronData::remove_schedules(std::string linkageID){
	m_taskQueue.remove(linkageID);
}



bool CronData::check_queue(){

	if(m_taskQueue.size() > 0) {


		Task _task = m_taskQueue.top();

		time_t exec_time = _task.getExecTime();

		time_t cur_time = time(NULL);
		time_t diff_time = exec_time - cur_time;
		if(abs(diff_time) < 1) {
			return true;
		}
		else{
			return false;
		}
	}else {
		return false;
	}

	

}

void CronData::exec(){


	Task _task = m_taskQueue.top();
	_task.exec();

	time_t cur_time = time(NULL);
	time_t next_time = _task.get_nexttime();

	if(next_time - cur_time <= 0){
		std::cout <<"name : " <<_task.get_name() << "  next_time: " << next_time << "  cur_time: " << cur_time << " diff :" << next_time - cur_time << " removed" << endl;
		remove_schedules(_task.get_name(), _task.get_expr());
	}else{
		std::cout <<"name : " <<_task.get_name() << "  next_time: " << next_time << endl;

		m_taskQueue.update_top_time(next_time);
		m_taskQueue.sort();
	}
}


void CronData::start()
{
	int err;
	stopflag_ = false;

	std::thread _tick_thread(tick_thread, this);

	_tick_thread.detach();

	if(0 != err){
		std::cout << "Create tick Thread error " << std::endl;
	}
}

void CronData::stop(){
	stopflag_ = true;

}