#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "driver/elevio.h"



int main(){
    elevio_init();
	 printf("Pre-startup");
    printf("Press the stop button on the elevator panel to exit\n");

    int current_kø[4];
    startup();
while(1){
    
    kø_add_if_pressed(current_kø);
    
    int next_floor = kø_manager(current_kø);
    
    if(next_floor!=-1){
        go_to_floor(next_floor, current_kø);
    }
            
        if(elevio_stopButton()){
            elevio_motorDirection(DIRN_STOP);
            break;
        }


        nanosleep(&(struct timespec){0, 20*1000*1000}, NULL);
    }

    return 0;
}





