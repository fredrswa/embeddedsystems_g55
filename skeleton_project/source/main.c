#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "driver/elevio.h"



int main(){
    elevio_init();
    
    printf("Press the stop button on the elevator panel to exit\n");

    while(1){
        //Changing variables
        int floor = elevio_floorSensor();


        //STARTUP PROCEDURE
        /*
        if(floor == -1) {
            elevio_motorDirection(DIRN_DOWN);
        } else {
            elevio_motorDirection(DIRN_STOP);
            elevio_floorIndicator(floor);
        }
        */
        int current_call = elevio_callButton(3,1);
        printf("%d", current_call);

        go_to_floor(floor,1);
        floor = elevio_floorSensor();
        go_to_floor(floor,3);
        floor = elevio_floorSensor();
        go_to_floor(floor,0);
        floor = elevio_floorSensor();
        go_to_floor(floor,2);
        floor = elevio_floorSensor();
        go_to_floor(floor,3);
        floor = elevio_floorSensor();























        /*
        int floor = elevio_floorSensor();

        elevio_buttonLamp(floor,1 ,1);        int floor = elevio_floorSensor();
        if(floor == N_FLOORS-1){
            elevio_motorDirection(DIRN_DOWN);
        }


        for(int f = 0; f < N_FLOORS; f++){
            for(int b = 0; b < N_BUTTONS; b++){
                int btnPressed = elevio_callButton(f, b);
                elevio_buttonLamp(f, b, btnPressed);
            }
        }

        if(elevio_obstruction()){
            elevio_stopLamp(1);
        } else {
            elevio_stopLamp(0);
        }
        
        if(elevio_stopButton()){
            elevio_motorDirection(DIRN_STOP);
            break;
        }
        */
        if(elevio_stopButton()){
            elevio_motorDirection(DIRN_STOP);
            break;
        }


        nanosleep(&(struct timespec){0, 20*1000*1000}, NULL);
    }

    return 0;
}
