#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sys/time.h>
#include <string>
#include <vector>
#include <unistd.h>
#include <pthread.h>
#include "types_p2.h"
#include "p2_threads.h"
#include "utils.h"

using namespace std;

pthread_cond_t  cond  = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
struct timeval t_global_start;
queue<Person> man_queue;
queue<Person> woman_queue;
Restroom rr;
char entrance_clear;

int main(int argc, char** argv)
{
	int gender_count;
	int total;
	entrance_clear = 0;
	//print error for not 1 argument
	if(argc != 2 || !(atoi(argv[1]) > 0)){
		cout << "[ERROR] Expected 1 argument, but got " << argc - 1 << "." << endl;
		cout << "[USAGE] p2_exec <number>" << endl;
		return -1;
	}

	
	// This is to set the global start time
	gettimeofday(&t_global_start, NULL);

	srand((unsigned) time(NULL));
	pthread_t       tid[3];
	int             status = 0;
	
	gender_count = atoi(argv[1]);
	total = gender_count * 2;
	rr.set_people(total);	
	
	//split the threadfunc here
	// 1. create
	// 2. remove
	// 3. entrance
	// no need to lock threads here (already locked inside p2_threads.cpp)
	pthread_create(&tid[0], NULL, create_thread, &gender_count);
	pthread_create(&tid[1], NULL, remove_thread, NULL);
	pthread_create(&tid[2], NULL, entrance_thread, NULL);

	//join threads
	for(int i =0; i<3; i++)
		pthread_join(tid[i], NULL);
	
	return 0;
	// Example code for sleep and class usage
	/*Person p1;
	p1.set_order(1);

	usleep(MSEC(200));
	p1.start();


	usleep(MSEC(150));
	p1.complete(); */
	///////////////////////////////////////////
/*
	if(pthread_create(&tid, NULL, threadfunc, NULL)) {
		fprintf(stderr, "Error creating thread\n");		
	}
	usleep(MSEC(10));

	for (int i=0; i<5; i++) {
		printf("Wake up thread after (%d) seconds\n", (5-i));
		usleep(MSEC(1000));
	}

	printf("Wake up thread\n");
	status = pthread_cond_signal(&cond);
	


	 wait for the second thread to finish 
	if(pthread_join(tid, NULL)) {
		fprintf(stderr, "Error joining thread\n");	
	}
*/
}

