#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <vector>
#include "driver/elevio.h"



int main(){
    elevio_init();
    
    printf("=== Example Program ===\n");
    printf("Press the stop button on the elevator panel to exit\n");

    elevio_motorDirection(DIRN_UP);

    while(1){
        //Variables
        int floor_call;
        int floor_current;

        //Tilstandsvektor
        int door_status;
        int floor_indicator;
        int floor_above;
        int floor_below;
        int floor_level;
        int floorbutton_up;
        int floorbutton_down;
        /*
        vector<int> stat_vector = 
        (
            door_status;
            floor_indicator;
            floor_above;
            floor_below;
            floor_level;
            floorbutton_up;
            floorbutton_down;
        );
        */
        //Matriser(Betingelse/maske)



        //Matrise -> Regel



        //Regel switch-case



        //Annet -> køsystem




        int floor = elevio_floorSensor();

        if(floor == 0){
            elevio_motorDirection(DIRN_UP);
        }

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
        
        nanosleep(&(struct timespec){0, 20*1000*1000}, NULL);
    }

    return 0;
}
