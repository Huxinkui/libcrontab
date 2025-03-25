#ifndef _SCHEDULE_TASK_QUEUE_H_
#define _SCHEDULE_TASK_QUEUE_H_

#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <mutex>

#include "task.h"


std::mutex m_mutex;

class TaskQueue
{

    public:
        const std::vector<Task>& get_tasks() const
        {
            return c;
        }
        
        std::vector<Task>& get_tasks()
        {
            return c;
        }
        
        size_t size() const 
        {
            return c.size();
        }
        
        bool empty() const 
        {
            return c.empty();
        }
        
        void push(Task t)
        {
            c.push_back(t);
        }
        
        const Task& top() const
        {
            return c[0];
        }
        
        Task& at(const size_t i)
        {
            return c[i];
        }
        
        void sort()
        {
            std::sort(c.begin(), c.end());
        }
        
        void clear()
        {
            c.clear();
        }
        
        void remove(Task to_remove)
        {
        	std::vector<Task>::iterator iter =  c.begin();
        	for(; iter != c.end(); ){
        		if(iter->get_name() == to_remove.get_name() && iter->get_expr() == to_remove.get_expr()){
        			iter = c.erase(iter);
        		}else{
        			++iter;
        		}

        	}

        }

        void remove(std::string name, std::string expr)
        {
            std::vector<Task>::iterator iter =  c.begin();
            for(; iter != c.end(); ){
                if(iter->get_name() == name && iter->get_expr() == expr){
                    iter = c.erase(iter);
                }else{
                    ++iter;
                }
            }
            
        }


        void remove(std::string linkageID)
        {
            std::vector<Task>::iterator iter =  c.begin();
            for(; iter != c.end(); ){

                std::size_t found = iter->get_name().find_last_of("_");
                std::string linkageid_ = iter->get_name().substr(0,found);

                if(linkageid_ == linkageID){
                    iter = c.erase(iter);
                }else{
                    ++iter;
                }
            }
            
        }

        void update_top_time(time_t next_time){
        	if(c.size()> 0){
        		c[0].set_time(next_time);
        	}
        }
        
        
    private:
        std::vector<Task> c;

};
#endif
