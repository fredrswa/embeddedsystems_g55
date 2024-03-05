#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "driver/elevio.h"



int main(){
    elevio_init();
	printf("Pre-startup\n");
    printf("Press the stop button on the elevator panel to exit\n");

 

while(1){
    int current_kø[4] = {0,0,0,0};
    int priority[4] = {0,0,0,0};
    startup();
    int stop =0;
    int super_stop = 0;
    int motor_dir =0;
    int current_floor=-1;
    
    while(1) 
    {
        kø_add_if_pressed(current_kø);
        int next_floor = kø_manager(current_kø, priority);
    
        
        
        if(next_floor!=-1){
        stop, motor_dir, current_floor = go_to_floor(next_floor, current_kø, stop, motor_dir, current_floor);
        }

    

        if(elevio_stopButton() || stop)
        {
            super_stop = emergency_stop(current_kø);
        }


            nanosleep(&(struct timespec){0, 20*1000*1000}, NULL);
    }


    if(super_stop == 1) {
        printf("Application ended");
        break;
    }
    }   
    return 0;
}





