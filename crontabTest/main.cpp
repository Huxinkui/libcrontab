#include <iostream>
#include <string>
#include <unistd.h>
#include <mutex>
#include "../cronData.h"

using namespace std;


enum RUNSTATE
{
	NORMOL = 0,
	RUNNING,
	END,
};

class linkage
{
	

public:
	linkage(string name):name(name){
		runstate = 0;
	}

	void setRun(Task* p_task){
		if(runstate == NORMOL || runstate == END){
			cout <<" LINKAGE WILL RUN LINKAGE NAME : " << name << "   crontab Name : " << p_task->get_name() << endl;
			
			if(p_task->get_name() == "11111_0"){

				runstate = RUNNING;
				//base_sleep(1, 0);
			}else if(p_task->get_name() == "22222_0"){
				//base_sleep(0, 20);
			}else if(p_task->get_name() == "33333"){
				//sleep(3);
			}else if(p_task->get_name() == "44444"){
				//sleep(4);
			}
			
			
		}else{
			cout<<" LINKAGE : " << name <<  "   crontab Name : " << p_task->get_name() << "   IS RUNNING , DON'T Run again !" << endl;
			runstate = END;
		}
	}

private:
	int runstate;
	string name;

};




void update_callback(Task* _ptask, void * p_linkage)
{
	if(_ptask != NULL){
		((linkage*)p_linkage)->setRun(_ptask);
	}
}




int main(){

	linkage l1("TEST1");
	linkage l2("TEST2");
	linkage l3("TEST3");
	linkage l4("TEST4");

	CronData c;
	c.start();

	m_mutex.lock();
	c.add_schedule("11111_0","*/1 * * * * * *", update_callback, &l1);
	c.add_schedule("11111_2","*/2 * * * * * *", update_callback, &l1);
	c.add_schedule("22222_0","*/1 * * * * * *", update_callback, &l2);
	c.add_schedule("33333_0","*/1 * * * * * *", update_callback, &l3);
	c.add_schedule("44444_0","*/1 * * * * * *", update_callback, &l4);
	m_mutex.unlock();

	while(1){
		sleep(10);

		std::cout <<"###############000000000000000###############"<< std::endl;
		m_mutex.lock();
		c.remove_schedules("11111_0", "*/1 * * * * * *");
		m_mutex.unlock();
		std::cout <<"###############111111111111111###############"<< std::endl;
		sleep(10);
		std::cout <<"###############222222222222222###############"<<std::endl;
		m_mutex.lock();
		c.remove_schedules("11111");
		m_mutex.unlock();
		std::cout <<"###############333333333333333###############"<<std::endl;
		sleep(10);
		break;
	}
	c.stop();

	return 0;
}