#include "types_p2.h"
#include "utils.h"
#include "p2_threads.h"


void Person::set_gender(int data) { gender = data; }
int Person::get_gender(void)      { return gender; }

void Person::set_order(unsigned long data) { order = data; }
unsigned long Person::get_order(void)      { return order; }

void Person::set_use_order(unsigned long data) { use_order = data; }
unsigned long Person::get_use_order(void)      { return use_order; }
long Person::get_time() { return time_to_stay_ms; }//need to obtain time from set_time
void Person::set_time(long data) { time_to_stay_ms = data; }
int Person::ready_to_leave(void) {
	struct timeval t_curr;
	gettimeofday(&t_curr, NULL);

	if (get_elasped_time(t_start, t_curr) >= time_to_stay_ms) { return 1; }
	else { return 0; }
}

void Person::start(void) {
	gettimeofday(&t_start, NULL);
//	printf("(%lu)th person enters the restroom: \n", order);
//	printf(" - (%lu) milliseconds after the creation\n", get_elasped_time(t_create, t_start));
}

void Person::complete(void) {
	gettimeofday(&t_end, NULL);
//	printf("(%lu)th person comes out of the restroom: \n", order);
//	printf(" - (%lu) milliseconds after the creation\n", get_elasped_time(t_create, t_end));
//	printf(" - (%lu) milliseconds after using the restroom\n", get_elasped_time(t_start, t_end));
}

Person::Person() {
	gettimeofday(&t_create, NULL);
}
// You need to use this function to print the Restroom's status
void Restroom::print_status(void) {
	printf("Print restroom status\n");
}

// Call by reference
// This is just an example. You can implement any function you need
void Restroom::add_person(Person &p){
	//flag: 0 if male and 1 if female
	if(status == EMPTY){//beginning of program
		p.start();
		rrPeople[occupied_count] = p;
		occupied_count++;//increment # of people
		if(p.get_gender() == MALE){
			gender_flag = 0;
			status = MENPRESENT;	
		} //fi
		else{
			gender_flag = 1;
			status = WOMENPRESENT;
		} //esle
	}//fi
	//check if person is the same gender
	else if((status == MENPRESENT && p.get_gender() == MALE) || (status == WOMENPRESENT && p.get_gender() == FEMALE)){
		p.start();
		rrPeople[occupied_count] = p;
		occupied_count++;
	}//fi
		
	if(p.get_gender() == MALE){//only men can enter if current gender is MALE
		man_enters(p);
	}//fi
	else{ //vice versa
		woman_enters(p);
	}//esle
} //end of add_Person

void Restroom::remove_person(void){
	if(status != EMPTY)
		occupied_count--;
}//end of remove_person

bool Restroom::remove_ready(){
	bool n = false;
	for(int i = 0; i < get_occupied_count(); i++){
		if(rrPeople[i].ready_to_leave() && (status != MENPRESENT || status != WOMENPRESENT)){
			rrPeople[i].complete();
			remove_person();
			if(occupied_count == 0){
				status = EMPTY;
			}//fi
			if (rrPeople[i].get_gender() == MALE){
				man_leaves(rrPeople[i]);
			}//fi
			else{
				woman_leaves(rrPeople[i]);
			}//esle
			n = true;
		}//fi
	}//rof
	return n;
} //end of remove_ready

/////////////////getter functions/////////////////
int Restroom::get_status(void){
	return status;
}

string Restroom::get_status_display(void){
	switch(status){
		case MENPRESENT:
			return "MenPresent";
		case WOMENPRESENT:
			return "WomenPresent";
		case EMPTY:
			return "Empty";
	};//end switch
}

int Restroom::get_men_present(void){
	return gender_flag ? 0 : occupied_count;
}

int Restroom::get_women_present(void){
	return gender_flag ? occupied_count : 0;
}

int Restroom::get_occupied_count(void){
	return occupied_count;
}
//////////////////////////////////////////////////

/////////////////setter functions/////////////////
void Restroom::set_people(int n){
	rrPeople = new Person[n];
}//end of set_people

