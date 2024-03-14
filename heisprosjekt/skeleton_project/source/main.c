/**
 * @file
 * @brief Main file for the elevator project
 
    * This is the main file for the elevator project. It contains the main function and the while loop that runs the elevator.
*/



#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "driver/elevio.h"
#include "driver/elev_operatorscript.h"



int main(){
    elevio_init();
	printf("Pre-startup\n");
    printf("Press the stop button on the elevator panel to exit\n");

 

while(1)
{   
    //Queues for prioritizing floors and deciding to stop when passing floors
    int current_queue[4] = {0,0,0,0};
    int priority[4] = {0,0,0,0};
    int down_queue[4]={0,0,0,0};
    int up_queue[4]={0,0,0,0};
    int cabin_queue[4]={0,0,0,0};
    //Initialize to defined state
    startup();
    //Values
    int stop1 =0;
    int stop  =0;
    int super_stop = 0;
    int motor_d =0;
    int motor_dir = 0;
    int current_floor=-1;
    int current_fl = -1;
    
    while(1) 
    {
        //Add new calls
        queue_add_if_pressed(current_queue, down_queue, up_queue, cabin_queue);
        //Decide which floor to got to
        int next_floor = queue_manager(current_queue, priority, down_queue, up_queue, cabin_queue);
    
        
        //Make sure next floor returns a defined value
        if(next_floor!=-1)
        {
            stop = go_to_floor(next_floor, current_queue, &stop1, &current_fl, &motor_d, down_queue, up_queue, cabin_queue);
            stop1 = stop;
            printf("Stop: %d\n", stop1);
            printf("Direction: %d\n", motor_d);
            printf("Floor: %d\n", current_fl);
            printf("\n\n");
        }

    

        if(elevio_stopButton())// || stop)
        {
            super_stop = emergency_stop(current_queue, down_queue, up_queue, cabin_queue);
            stop = 0;
        }


            nanosleep(&(struct timespec){0, 20*1000*1000}, NULL);
    }
    }   
    return 0;
}



//to queue_manager
//go to floor sjekker vi queue i samme retning stopper og sletter den queueen
//queue_up = [] for go_to_floor and queue manager
//queue_down = [] for go_to_floor and queue manager
//queue_panel = [] for queue manager




