#include "p2_threads.h"
#include "utils.h"

using namespace std;

extern pthread_cond_t  cond;
extern pthread_mutex_t mutex;
extern struct timeval t_global_start;
extern queue<Person> man_queue;
extern queue<Person> woman_queue;
extern Restroom rr;
extern char entrance_clear;

/***************DISPLAY FUNCTIONS*****************/
void woman_wants_to_enter(Person &p)
{
	int men_size = man_queue.size();
	int women_size = woman_queue.size();
	struct timeval t_current;
	gettimeofday(&t_current, NULL);
	
	printf("[%ld ms] [Input] A person (Woman) goes into the queue.\n", 
		get_elasped_time(t_global_start, t_current));
	
} //end of woman_wants_to_enter

void man_wants_to_enter(Person &p)
{
	int men_size = man_queue.size();
	int women_size = woman_queue.size();	
	struct timeval t_current;
	gettimeofday(&t_current, NULL);
	
	printf("[%ld ms] [Input] A person (Man) goes into the queue.\n", 
		get_elasped_time(t_global_start, t_current));

}//end of man_want_to_enter

void man_enters(Person &p)
{	
	int men_size = man_queue.size();
	int women_size = woman_queue.size();	
	struct timeval t_current;
	gettimeofday(&t_current, NULL);

	printf("[%ld ms] [Queue] Send (Man) into the restroom (Stay %ld ms), Status: Total: %d (Men: %d, Women: %d)\n",
		get_elasped_time(t_global_start, t_current),
		p.get_time(),
		men_size + women_size,
		men_size,
		women_size);
	
	printf("[%ld ms] [Restroom] (Man) goes into the restroom, State is (%s): Total: %d (Men: %d, Women: %d)\n",
		get_elasped_time(t_global_start, t_current),
		rr.get_status_display().c_str(),
		rr.get_occupied_count(),
		rr.get_men_present(),
		rr.get_women_present());	
}//end of man_enters

void woman_enters(Person &p)
{	
	int men_size = man_queue.size();
	int women_size = woman_queue.size();	
	struct timeval t_current;
	gettimeofday(&t_current, NULL);

	printf("[%ld ms] [Queue] Send (Woman) into the restroom (Stay %ld ms), Status: Total: %d (Men: %d, Women: %d)\n",
		get_elasped_time(t_global_start, t_current),
		p.get_time(),
		men_size + women_size,
		men_size,
		women_size);

	printf("[%ld ms] [Restroom] (Woman) goes into the restroom, State is (%s): Total: %d (Men: %d, Women: %d)\n",
		get_elasped_time(t_global_start, t_current),
		rr.get_status_display().c_str(),
		rr.get_occupied_count(),
		rr.get_men_present(),
		rr.get_women_present());	
		
}//end of woman_enters

void woman_leaves(Person &p)
{	
	struct timeval t_current;
	gettimeofday(&t_current, NULL);
		
	printf("[%ld ms] [Restroom] (Woman) left the restroom. Status is changed, Status is (%s): Total: %d (Men: %d, Women: %d)\n",
		get_elasped_time(t_global_start, t_current),
		rr.get_status_display().c_str(), //status
		rr.get_occupied_count(), //total
		rr.get_men_present(), //Men:
		rr.get_women_present()); //Women:
	
} // end of woman_leaves
void man_leaves(Person &p)
{
	struct timeval t_current;
	gettimeofday(&t_current, NULL);
		
	printf("[%ld ms] [Restroom] (Man) left the restroom. Status is changed, Status is (%s): Total: %d (Men: %d, Women: %d)\n",
		get_elasped_time(t_global_start, t_current),
		rr.get_status_display().c_str(), //status
		rr.get_occupied_count(), //total
		rr.get_men_present(), //Men:
		rr.get_women_present()); //Women:
} //end of man_leaves
/*************************************************/

//break up threadfunc into 3 functions
//	- creation, removal, and entrance
void *create_thread(void *parm)
{	
	int status;
	int gender_count = *((int *) parm);
	int man_count = 0;
	int woman_count = 0;
	int i;
	
	for (i=0; i<2*gender_count; i++){
		Person p;
		if(man_count == gender_count){ //if men matches the total count
			p.set_gender(FEMALE); 
			p.set_order((woman_count++) + 1);
		}//fi

		else if(woman_count == gender_count){
			p.set_gender(MALE);
			p.set_order((man_count++) +1);
		} //fi
		
		else {
			p.set_gender(rand() % 2); //male:0, female:1
			if(p.get_gender() == MALE)
				p.set_order((man_count++)+1);
			else
				p.set_order((woman_count++)+1);
		} //*esle
		//people must stay in bathroom 3-10ms
		p.set_time((rand()%8)+3);

		//lock
		status = pthread_mutex_lock(&mutex);
	
		//man entering queue
		if(p.get_gender() == MALE){
			man_queue.push(p);
			man_wants_to_enter(p); //prints man into restroom
		}//fi
		else{///woman entering queue
			woman_queue.push(p);
			woman_wants_to_enter(p); //prints woman into restroom
		}//esle
		
		//release mutex
		status = pthread_mutex_unlock(&mutex);
		//wait 1-5 ms
		usleep(MSEC((rand()%5) +1));
	}//rof
	entrance_clear = 1;
} //end of create_thread

void *remove_thread(void *parm)
{
	int status;
	//remove as long as someone is still in queue
	while((rr.get_status() != EMPTY) || !entrance_clear || !man_queue.empty() || !woman_queue.empty()){
		//lock
		status = pthread_mutex_lock(&mutex);
		//remove people that are ready
		if(rr.remove_ready())
			pthread_cond_signal(&cond);
		//release lock
		status = pthread_mutex_unlock(&mutex);
	
	} //elihw

} //end of remove_thread

void *entrance_thread(void *parm)
{
	int status;
	while((rr.get_status() != EMPTY) || !entrance_clear || !man_queue.empty() || !woman_queue.empty()){
		//lock
		status = pthread_mutex_lock(&mutex);
			
		//sending a woman
		if (rr.get_status() == WOMENPRESENT){
			if(!woman_queue.empty()){//if there is someone in queue
				Person p = woman_queue.front();
				woman_queue.pop();
				rr.add_person(p);
			} //fi
		}//fi
		
		//sending a man
		else if (rr.get_status() == MENPRESENT){
			if(!man_queue.empty()){//if someone is in queue
				Person p = man_queue.front();
				man_queue.pop();
				rr.add_person(p);
			} //fi
		}//fi
		
		//empty
		else if (rr.get_status() == EMPTY){
			//check if both genders queues are empty
			if (!(man_queue.empty() && woman_queue.empty())){
				//men queue is larger, send man
				if (man_queue.size() > woman_queue.size()){
					Person p = man_queue.front();
					man_queue.pop();
					rr.add_person(p);
				}//fi
				//woman queue is larger, send woman
				else{
					Person p = woman_queue.front();
					woman_queue.pop();
					rr.add_person(p);
				}//esle
			}//fi
		}//esle
		//release lock
		status = pthread_mutex_unlock(&mutex);
		//wait 1-5ms
		usleep(MSEC((rand()%5)+1));
	}//elihw
} //end of entrance_thread

 
/*
void *threadfunc(void *parm)
{
	int status;
	printf(" [Thread] Start\n");

	printf(" [Thread] Locks\n");
	status = pthread_mutex_lock(&mutex);

    printf(" [Thread] Blocked\n");
    status = pthread_cond_wait(&cond, &mutex);

	printf(" [Thread] Starts again.\n");
	for (int i=0; i<3; i++) {
		printf(" [Thread] Complete thread after (%d) seconds\n", (3-i));
		usleep(MSEC(1000));
	}

	printf(" [Thread] Unlocks\n");
	status = pthread_mutex_unlock(&mutex);
	printf(" [Thread] Complete\n");
}*/
