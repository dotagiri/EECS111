#include "p1_threads.h"

// This file includes functions that actually perform the
// computation. You can create a thread with these function
// or directly call this function

void merge(INFO_V &info, int low, int mid, int high){
	int i, j;
	int left = mid - low + 1;
	int right = high - mid;
	int k = low;
	INFO_V left_v, right_v;
	
	//store elements for left part
	for(i = 0; i < left; i++){
		left_v.push_back(info[low+i]);
	}//rof
	
	//store elements for right part
	for(j = 0; j < right; j++){
		right_v.push_back(info[mid+j+1]);
	} //rof	
	
	i = j = 0;//reset i and j indicies
	//merge in ascending order
	while(i < left && j < right){
		if(left_v[i].grade >= right_v[j].grade)	
			info[k++] = right_v[j++];
		else	
			info[k++] = left_v[i++];
	} //elihw	
	
	//insert remaining values from left side	
	while(i < left)
		info[k++] = left_v[i++];
	
	//insert remaining values from right side
	while(j < right)
		info[k++] = right_v[j++];
	
} //end of merge

void merge_mthreads(INFO_V &info, merge_list *args, int start, int end){
	
	int mid;

	if((end-start) == 1){ //two segments
		merge(info, args[start].start, args[start].end, args[end].end);
	}//fi

	else if((end-start) == 2){ //three segments
		merge_mthreads(info, args, start, start+1);
		merge(info, args[start].start, args[start+1].end, args[end].end);
	} //fi esle

	else {//more than 3 
		mid = mean(start,end);
		merge_mthreads(info, args, start, mid);
		merge_mthreads(info, args, mid+1, end);
		merge(info, args[start].start, args[mid].end, args[end].end);
	} /*esle*/
} //end of merge_sort_mthreading	

void *merge_sort_mthreads(void *argp){
	int mid;

	merge_list *args = (merge_list *) argp;
	if (args->start < args->end){
		mid = mean(args->start, args->end);
		
		merge_list first_arg, second_arg;
		first_arg.info = second_arg.info = args->info;

		//first half is from beginning to middle of the list
		first_arg.start = args->start;
		first_arg.end = mid;
		
		//second half is from middle+1 to end
		second_arg.start = mid+1;
		second_arg.end = args->end;
		
		merge_sort_mthreads((void *) &first_arg);
		merge_sort_mthreads((void *) &second_arg);
		merge(*args->info, args->start, mid, args->end);
	} //fi
}//end of merge_sort_mthreads

int merge_sort(INFO_V *info, int mxthreads){
	if(mxthreads < 1){//no threads
		return -1;//end function
	} //fi

	int i;
	int num_threads;
	int l =info->size();

	if(mxthreads > 1)
		num_threads = l;
	else
		num_threads = mxthreads;

	int temp_array_size = l / num_threads;
	pthread_t threads[num_threads];

	merge_list args[num_threads];

	for(i = 0; i < num_threads; i++){
		args[i].info = info;
		} //rof

	args[0].start = 0;//initialize the start
	args[0].end = temp_array_size - 1;//eliminate the null operator with -1

	if(num_threads > 1){
		for(i = 1; i< num_threads - 1; i++){
			args[i].start = args[i-1].end++;
			args[i].end = (temp_array_size + args[i].start)-1;
		} //rof
		args[num_threads-1].start = args[num_threads-2].end++;
		args[num_threads-1].end = l--;
	} //fi

	for(i=0; i< num_threads; i++){//create threads
		pthread_create(&threads[i], NULL, merge_sort_mthreads, (void *) &args[i]);
	}//rof
	
	for(i=0; i < num_threads; i++){//join threads
		pthread_join(threads[i], NULL);
	}//rof

	if(num_threads>1){
		merge_mthreads(*info, args, 0, num_threads - 1);
	}//fi

	return 0;
} //end of merge_sort

