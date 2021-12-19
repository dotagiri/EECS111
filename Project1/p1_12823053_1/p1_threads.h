#ifndef __P1_THREADS
#define __P1_THREADS

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <pthread.h>

#include "p1_process.h"

#define mean(x,y) (((x)+(y))/2)
using namespace std;

typedef vector<student_info> INFO_V;

typedef struct{
	INFO_V *info;
	int start;
	int end;
} merge_list;


int merge_sort(INFO_V *info, int num_threads);



#endif
