#ifndef __P2_THREADS_H
#define __P2_THREADS_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sys/time.h>
#include <string>
#include <vector>
#include <unistd.h>
#include "types_p2.h"
#include <queue>

//send people to restroom
void *create_thread(void *parm);

//removing people from room
void *remove_thread(void *parm);

//people entering restroom if stalls are available
void *entrance_thread(void *parm);

//Required functions
void man_wants_to_enter(Person &p);
void woman_wants_to_enter(Person &p);
void man_enters(Person &p);
void woman_enters(Person &p);
void man_leaves(Person &p);
void woman_leaves(Person &p);




#endif
