#include "p1_process.h"

#define NUM_OF_FILES 	5
#define BUFFER_SIZE 	128

vector<student_info> readfile(string fname){
	double grade = 0.0;
	char buff[BUFFER_SIZE];
	FILE *fp = NULL;
	fp = fopen(fname.c_str(), "r");
	vector<student_info> info;
	char *token = NULL;
	student_info current;

	if(fp != NULL){ //make sure file was read
		fgets(buff, BUFFER_SIZE, fp); 	
		while(fgets(buff, BUFFER_SIZE, fp) != NULL){
			token = strtok( buff, ","); //tokenize with commas in between each info
			if(token != NULL){//as long as token exist
				string temp(token);
				current.sid = temp;
				token = strtok(NULL, ",");
				if (token != NULL){
					grade = atof(token);
					current.grade = grade;
				} //fi
			} //fi
			info.push_back(current);
		}// elihw 		
		fclose(fp);
	} //fi
		
	return info;
	
} //end of file reading

void writefile(string fname, vector<student_info> info){ //os-sorted.csv content
	int rank = 1; //first student rank starts from 1

	ofstream wfile;
	wfile.open(fname.c_str());
	wfile << "Rank,Student ID,Grade\n";
	for(int i = info.size() - 1; i >= 0; i--){
		wfile << rank++ << "," << info[i].sid << "," << setprecision(12) << info[i].grade << "\n";
	} //rof
	wfile.close();
} //end of writefile

void write_statsfile(string fname, stats_info stats){ //os-stats.csv content
	ofstream wsfile;
	wsfile.open(fname.c_str());
	wsfile << "Average,Median,Std. Dev\n"; 
	wsfile << setprecision(5) << stats.average << "," << stats.median << "," << stats.std_dev << "\n";
	wsfile.close();
} //end of write_statsfile

//new function to calculate stats
stats_info calculate(vector<student_info> info){
	stats_info stats;
	vector<student_info>::iterator ptr;

	//average
	double sum = 0;//initialize sum
	for (ptr = info.begin(); ptr != info.end(); ptr++){//iterate from the beginning of vector struct
		sum += (*ptr).grade;
	} //rof
	stats.average = sum / info.size();//set average in struct stats
		
	//median
	//if the # of students is even, do special method
	if (info.size() == 0){
		; //nothing 
		} //fi
	else {
		if (info.size() % 2 ==0)//checks if # of students is even
			stats.median = (info[(info.size()/2)-1].grade + info[info.size()/2].grade)/2;
		else //# of students is odd, simply select middle
			stats.median = info[info.size()/2].grade;
	} //esle

	//standard deviation
	sum = 0;//reset sum
	for(ptr = info.begin(); ptr != info.end(); ptr++){
		sum += pow(((*ptr).grade - stats.average),2);
	}//rof

	stats.std_dev = sqrt(sum/info.size());

	return stats;

} //end of calculate

void get_statistics(std::string class_name[], int num_processes, int num_threads) {

	// You need to create multiple processes here (each process
	// handles at least one file.)
					
	// Each process should use the sort function which you have defined  		
	// in the p1_threads.cpp for multithread sorting of the data. 
	
	// Your code should be able to get the statistics and generate
	// the required outputs for any number of child processes and threads.
	
	int count = 0; //keep track of # of children
	pid_t pid, wpid;

	//each process gets 1 file
	if(num_processes > NUM_OF_FILES){ //input directory has 5 csv files
		num_processes = NUM_OF_FILES;
	}
		
	//start creating processes
	for(int i = 0; i<num_processes; i++) {
		//begin child creation
		pid = fork(); 
		if (pid > 0){//parent process
			printf("Child process is created. (pid: %d)\n", pid);
			count++;
		}

		else if (pid == 0){ //child process
			//loop through all file
			int completed = 0;
			int current_file;
			while((current_file = i+(num_processes*completed))<NUM_OF_FILES){
				//read .csv files from input director
				vector<student_info> info = readfile("input/" + class_name[current_file] + ".csv");
				merge_sort(&info, num_threads);//conduct mergesort with defined  number of threads
					
				//write sorted files
				writefile("output/" + class_name[current_file] + "_sorted.csv", info);
				
				//write stats files
				stats_info stats = calculate(info);
				write_statsfile("output/" + class_name[current_file] + "_stats.csv", stats);
				
				//increment # of completely written files
				completed++;
				} //elihw
			exit(0);
		} //fi esle		
			
	} // end of process creation
	
	printf("...\n");

	//start terminating processes
	
	//status method
/*	int status;	
	while((wpid = wait(&status)) >0){
		pid = wait(NULL);	
		printf("Child process is terminated. (pid: %d)\n", pid);
		} //end terminating 	*/

	//counter method
		while(count>0){
		pid = wait(NULL);
		printf("Child process is terminated. (pid: %d)\n", pid);
		count--;
		} //end terminating 	
	

} //end of get_statistics
