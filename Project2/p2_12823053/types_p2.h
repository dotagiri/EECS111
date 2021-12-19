#ifndef __TYPES_P2_H
#define __TYPES_P2_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sys/time.h>
#include <string>
#include <vector>
#include <unistd.h>

#define EMPTY        0
#define WOMENPRESENT 1
#define MENPRESENT   2

#define MALE 		0
#define FEMALE		1

using namespace std;

class Person
{

	int gender; // 0: male 1: female
	std::string str_gender;
	struct timeval t_create;
	struct timeval t_start;
	struct timeval t_end;
	long time_to_stay_ms;


	unsigned long order;
	unsigned long use_order;

public:
	Person();

	void set_gender(int data);
	int get_gender(void);

	void set_order(unsigned long data);
	unsigned long get_order(void);

	void set_use_order(unsigned long data);
	unsigned long get_use_order(void);

	void set_time(long data);
	int ready_to_leave(void);
	long get_time(void);
	void start(void);
	void complete(void);
};


// Class for the restroom
// You may need to add more class member variables and functions
class Restroom {
	char gender_flag;
	int status;
	int occupied_count; //number of people in the restroom
	Person *rrPeople;

public:
	Restroom(){
		status = EMPTY;
	};
	// You need to use this function to print the Restroom's status
	void print_status(void);

	// Call by reference
	// This is just an example. You can implement any function you need
	void add_person(Person& p);
	bool remove_ready();
	void remove_person(void);	

	//getter functions	
	int get_status(void);
	string get_status_display(void);
	int get_men_present(void);
	int get_women_present(void);
	int get_occupied_count(void);
	
	//setter functions
	void set_people(int n);
};

#endif
