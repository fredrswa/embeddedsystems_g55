#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "driver/elevio.h"



int main(){
    elevio_init();
	 printf("Pre-startup");
    printf("Press the stop button on the elevator panel to exit\n");
    

while(1){
	startup();
       	    go_to_floor(0);
	    go_to_floor(3);
	    go_to_floor(0);
	    go_to_floor(3);

        if(elevio_stopButton()){
            elevio_motorDirection(DIRN_STOP);
            break;
        }


        nanosleep(&(struct timespec){0, 20*1000*1000}, NULL);
    }

    return 0;
}





