#ifndef _SCHEDULE_TASK_H_
#define _SCHEDULE_TASK_H_

#include <string>
#include <time.h>
#include <string.h>
#include "ccronexpr.h"
#include <iostream>


using namespace std;

class Task;

typedef void(*dual_callback)(Task *, void *);


int gettimezone() {
	time_t time_utc = 0;
	struct tm p_tm_time;
	int time_zone = 0;
	localtime_r(&time_utc, &p_tm_time);
	time_zone = ( p_tm_time.tm_hour > 12 ) ? ( p_tm_time.tm_hour-= 24 ) : p_tm_time.tm_hour;
	return time_zone * 3600;
}


class Task
{
public:

	Task(std::string name, std::string expr, dual_callback _callback, void * ptr_):m_name(name) {
	
		m_func = _callback;
		user_ptr_ = ptr_;
		m_expr = expr;

		const char* err = NULL;
		memset(&m_cron_expr, 0, sizeof(expr));

		cron_parse_expr(m_expr.c_str(), &m_cron_expr, &err);

		if (err) {
			std::cout<< "cron_parse_expr error name: "  << name <<  "  msg: " << err << std::endl;
		}
		time_t cur_utc = time(NULL);
		time_t cur = cur_utc + gettimezone();
		m_time = cron_next(&m_cron_expr, cur) - gettimezone();
	}

	~Task(){

	}


	Task(const Task& other) 
	{
		m_time = other.m_time;
		m_name = other.m_name;
		m_expr = other.m_expr;
		m_func = other.m_func;
		user_ptr_ = other.user_ptr_;
		m_cron_expr = other.m_cron_expr;
	}

	// Task& operator= (const Task& other) const
	// {
	//     this->m_time = other.m_time;
	// 	this->m_name = other.m_name;
	// 	this->m_expr = other.m_expr;
	// 	this->m_func = other.m_func;
	// 	this->user_ptr_ = other.user_ptr_;
	// 	this->m_cron_expr = other.m_cron_expr;
	// 	return *this;
	// }

	bool operator>(const Task& other) const
	{
	    return m_time > other.m_time;
	}

	bool operator<(const Task& other) const
	{
	    return m_time < other.m_time;
	}

	time_t getExecTime(){
		return m_time;
	}

	void exec(){
		
		m_func(this, user_ptr_);
		
	}

	std::string get_name(){
		return m_name;
	}

	std::string get_expr(){
		return m_expr;
	}

	time_t get_nexttime(){

		time_t cur = m_time + gettimezone();
		time_t _time = cron_next(&m_cron_expr, cur) - gettimezone();
		return _time;
	}

	void set_time(time_t _time){

		m_time = _time;
	}


private:
	time_t      m_time; //执行时间
	std::string m_name; //task名称，使用uuid_num
	std::string m_expr; //crontab 表达式
	dual_callback m_func; //回调函数
	void 		* user_ptr_; 
	cron_expr   m_cron_expr;
};
#endif