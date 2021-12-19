#ifndef __P1_PROCESS
#define __P1_PROCESS

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <pthread.h>
#include <unistd.h>
#include <vector>
#include <cstring>
#include <string.h>
#include <math.h>
#include <sys/wait.h>
#include <cmath>
#include <iomanip>

using namespace std;

typedef struct {
	string sid;
	double grade;
} student_info;

#include "p1_threads.h"

typedef struct {
	double average;
	double median;
	double std_dev;
} stats_info;


vector<student_info> readfile(string fname);

void writefle(string fname, vector<student_info> info);

void write_statsfile(string fname, stats_info stats);

void get_statistics(std::string class_name[], int num_processes, int num_threads);


#endif
