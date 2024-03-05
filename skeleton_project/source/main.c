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
    int stop1 =0;
    int stop  =0;
    int super_stop = 0;
    int motor_d =0;
    int motor_dir = 0;
    int current_floor=-1;
    int current_fl = -1;
    
    while(1) 
    {
        kø_add_if_pressed(current_kø);
        int next_floor = kø_manager(current_kø, priority);
    
        
        
        if(next_floor!=-1){
        stop = go_to_floor(next_floor, current_kø, &stop1, &current_fl, &motor_d);
        stop1 = stop;
        printf("Stop: %d\n", stop1);
        printf("Direction: %d\n", motor_d);
        printf("Floor: %d\n", current_fl);
        printf("\n\n");
        }

    

        if(elevio_stopButton())// || stop)
        {
            super_stop = emergency_stop(current_kø);
            stop = 0;
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



//to kø_manager
//go to floor sjekker vi kø i samme retning stopper og sletter den køen
//queue_up = [] for go_to_floor and queue manager
//queue_down = [] for go_to_floor and queue manager
//queue_panel = [] for queue manager




