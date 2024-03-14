/**
 * @file
*/

#include "elevio.h"
#include "elev_operatorscript.h"





int go_to_floor(int new_floor, int queue[], int *stop, int *c_f2, int *m_d, int down_queue[], int up_queue[], int cabin_queue[])
{   
    int current_floor = elevio_floorSensor();
    int super_stop = 0;
    int floor_mem = 0;
    //int motor_dir = 0;
    int current_floor1 = -1;
    
    while(1){
        if (elevio_stopButton())
        {   
            *stop = 1;
            return 1;
        }
        if(!*stop){
            current_floor1 = elevio_floorSensor();
        }



        if((current_floor1 != -1) && (!*stop)){
            floor_mem = current_floor1;
            current_floor = current_floor1;
            elevio_floorIndicator(current_floor);

        }
        if(*stop == 1){
            if(*m_d ==1){
                if(*c_f2 != new_floor){
                    current_floor = *c_f2;
                }else{
                    current_floor = *c_f2 +2;
                }
            }
            if(*m_d == -1){
                if(*c_f2 != new_floor){
                    current_floor = *c_f2;
                }else{
                    current_floor = *c_f2 -2;
                }
            }
        }
        *c_f2 = current_floor;
        *stop = 0;

        //Implementasjon for å stoppe underveis hvis noen har trykket riktig reting imellom etager.
        if((current_floor1 != -1) && (current_floor != new_floor)){
        if(*m_d==-1){
            for(int i = 0; i < 4; i++){
                queue_add_if_pressed(queue, down_queue, up_queue, cabin_queue); 
                if(down_queue[i]==1 || cabin_queue[i]==1){
                    if(current_floor==i){
                        elevio_motorDirection(DIRN_STOP);
                        if (elevio_stopButton())
                            {
                                *stop = 1;
                                return 1;
                            }
                        super_stop = open_door(queue, down_queue, up_queue, cabin_queue);
                        queue_add_if_pressed(queue, down_queue, up_queue, cabin_queue);
                        queue_del_when_completed(current_floor,queue, down_queue, up_queue, cabin_queue);
                        break;
                    }
                }
            }
        }else if(*m_d==1){
            for(int i = 0; i < 4; i++){
                queue_add_if_pressed(queue, down_queue, up_queue, cabin_queue);
                if(up_queue[i]==1 || cabin_queue[i]==1){
                    if(current_floor==i){
                        elevio_motorDirection(DIRN_STOP);
                        if (elevio_stopButton())
                            {
                                *stop = 1;
                                return 1;
                            }
                        super_stop = open_door(queue, down_queue, up_queue, cabin_queue);
                        queue_add_if_pressed(queue, down_queue, up_queue, cabin_queue);
                        queue_del_when_completed(current_floor,queue, down_queue, up_queue, cabin_queue);
                        break;
                    }
                }
            }
        }
        }
        //Implementasjon slutt.


        if(current_floor > new_floor) {
            elevio_motorDirection(DIRN_DOWN);
            *m_d = -1;
        }else if(current_floor < new_floor)
        {
            *m_d = 1;
            elevio_motorDirection(DIRN_UP);
        }
        if(current_floor == new_floor) {
            elevio_motorDirection(DIRN_STOP);
        if (elevio_stopButton())
        {
            *stop = 1;
            return 1;
        }
            super_stop = open_door(queue, down_queue, up_queue, cabin_queue);
            queue_add_if_pressed(queue, down_queue, up_queue, cabin_queue);
            queue_del_when_completed(new_floor,queue, down_queue, up_queue, cabin_queue);
            return super_stop;

        }
        queue_add_if_pressed(queue, down_queue, up_queue, cabin_queue);
        


        if(elevio_stopButton()){
            emergency_stop(queue, down_queue, up_queue, cabin_queue);
            super_stop = 1;
            *stop  =1;
            return super_stop;
            

        }
        
    }

    return super_stop;

}



int open_door(int queue[], int down_queue[], int up_queue[], int cabin_queue[]){
    if (elevio_stopButton()) {
        return 1;
    }
    if(between_floors()){
        printf("The elevator is unable to open the dor in this state");
    }else{
        elevio_doorOpenLamp(1);
        int i = 0;
        for(int i = 0; i < 250; i++)
        {
            queue_add_if_pressed(queue, down_queue, up_queue, cabin_queue);
            if (elevio_stopButton()) 
            {
                return 1;
            }
            usleep(10000);
        }
        if(elevio_obstruction())
        {/*
            while(elevio_obstruction())
            {
                for(int i = 0; i < 50; i++)
                {
                    queue_add_if_pressed(queue, down_queue, up_queue, cabin_queue);
                    if (elevio_stopButton()) 
                    {
                        return 1;
                    }
                    usleep(10000);
                }
            }



        for(int i = 0; i < 250; i++){
        queue_add_if_pressed(queue, down_queue, up_queue, cabin_queue);
        if (elevio_stopButton()) {
            return 1;
        }
        usleep(10000);
        }
        */


            int i = 0; 
            while(i<250) {
                queue_add_if_pressed(queue, down_queue, up_queue, cabin_queue);
                
                if(elevio_obstruction()){
                    i = 0;
                }
                if (elevio_stopButton()) 
                    {
                        return 1;
                    }
                usleep(10000);
                i++;
            }

        }
        elevio_doorOpenLamp(0);
    }
    return 0;
}
int between_floors(){
    int floor = elevio_floorSensor();
    if(floor==-1)
    {
        return 1;
    } else
    {
        return 0;
    }
}
void startup(){
	while(1)
    {
        int floor = elevio_floorSensor();
        if(floor == -1) 
        {
            elevio_motorDirection(DIRN_DOWN);
        } else
        {
            elevio_motorDirection(DIRN_STOP);
            elevio_floorIndicator(floor);
	        break;
        }
    }
}

int is_queue_empty(int queue[]){
    for(int i = 0; i < 4; i++)
    {
        if(queue[i]==1)
        {
            return 0;
        }
    }
    return 1;
}

void queue_add_if_pressed(int queue[], int down_queue[], int up_queue[], int cabin_queue[]){
    
    
    if(elevio_callButton(0, 0) || elevio_callButton(0, 1) || elevio_callButton(0, 2))
    {
        queue[0]=1;

        if(elevio_callButton(0, 0))
        {
            elevio_buttonLamp(0, 0, 1);
            up_queue[0]=1;
        }else if(elevio_callButton(0, 1))
        {
            elevio_buttonLamp(0, 1, 1);
            down_queue[0]=1;
        }else if(elevio_callButton(0, 2))
        {
            elevio_buttonLamp(0, 2, 1);
            cabin_queue[0]=1;}
    }
    if(elevio_callButton(1, 0) || elevio_callButton(1, 1) || elevio_callButton(1, 2))
    {
        queue[1]=1;
        if(elevio_callButton(1, 0)){
            elevio_buttonLamp(1, 0, 1);
            up_queue[1]=1;
        }else if(elevio_callButton(1, 1)){
            elevio_buttonLamp(1, 1, 1);
            down_queue[1]=1;
        }else if(elevio_callButton(1, 2)){
            elevio_buttonLamp(1, 2, 1);
            cabin_queue[1]=1;}
        
    }
    if(elevio_callButton(2, 0) || elevio_callButton(2, 1) || elevio_callButton(2, 2)){
        queue[2]=1;

        if(elevio_callButton(2, 0)){
            elevio_buttonLamp(2, 0, 1);
            up_queue[2]=1;
        }else if(elevio_callButton(2, 1)){
            elevio_buttonLamp(2, 1, 1);
            down_queue[2]=1;
        }else if(elevio_callButton(2, 2)){
            elevio_buttonLamp(2, 2, 1);
            cabin_queue[2]=1;}
    }
    if(elevio_callButton(3, 0) || elevio_callButton(3, 1) || elevio_callButton(3, 2)){
        queue[3]=1;
        if(elevio_callButton(3, 0)){
            elevio_buttonLamp(3, 0, 1);
            up_queue[3]=1;
        }else if(elevio_callButton(3, 1)){
            elevio_buttonLamp(3, 1, 1);
            down_queue[3]=1;
        }else if(elevio_callButton(3, 2)){
            elevio_buttonLamp(3, 2, 1);
            cabin_queue[3]=1;}
    }
}
int queue_manager(int queue[], int pri[], int down_queue[], int up_queue[], int cabin_queue[]){
    int call = 0;
    int b = 0;
    for(int i = 0; i < 4; i++){
        queue_add_if_pressed(queue, down_queue, up_queue, cabin_queue);
        b+=queue[i];
        if((queue[i] &&  pri[i] > pri[call])){
            call = i;
        }
    }

    for(int i = 0; i < 4; i++) {
        if (queue[i]) {
            pri[i]++;
        }
    }

    pri[call] = 0;

    if(!b || (queue[call] == 0)){
        return -1;
    }
    return call;
}
void queue_del_when_completed(int floor ,int queue[], int down_queue[], int up_queue[], int cabin_queue[]){
    queue[floor]=0;
    down_queue[floor]=0;
    up_queue[floor]=0;
    cabin_queue[floor]=0;
    if(floor == 0){
        elevio_buttonLamp(0, 0, 0);
        elevio_buttonLamp(0, 1, 0);
        elevio_buttonLamp(0, 2, 0);
    }else if(floor == 1){
        elevio_buttonLamp(1, 0, 0);
        elevio_buttonLamp(1, 1, 0);
        elevio_buttonLamp(1, 2, 0);
    }else if(floor == 2){
        elevio_buttonLamp(2, 0, 0);
        elevio_buttonLamp(2, 1, 0);
        elevio_buttonLamp(2, 2, 0);
    }else{
        elevio_buttonLamp(3, 0, 0);
        elevio_buttonLamp(3, 1, 0);
        elevio_buttonLamp(3, 2, 0);
    }
}
int emergency_stop(int queue[], int down_queue[], int up_queue[], int cabin_queue[]) {
    printf("Emergency Stop\n");
    elevio_motorDirection(DIRN_STOP);
    elevio_stopLamp(1);
    
    int i = 0;
    int super_stop = 0;
    if (elevio_floorSensor() != -1) {
        elevio_doorOpenLamp(1);
    }
    while(elevio_stopButton())
    { 
        usleep(10000);
        i++;
        if (i == 500) {
            super_stop = 1;
            break;
        }
              
    }
    elevio_stopLamp(0);      
    if (elevio_floorSensor() > -1)
    {
        elevio_doorOpenLamp(1);
        int i  =0;
        while(i<200) {
                usleep(10000);
                i++;
                if(elevio_obstruction()){
                    break;
                }
            }
        
        if(elevio_obstruction()) {    
            int i = 0; 
            while(i<200) {
                usleep(10000);
                i++;
                if(elevio_obstruction()){
                    i = 0;
                }
            }
            
        }
        
        elevio_doorOpenLamp(0);
    }
    for (int floor = 0; floor < 4; floor++) {
        queue_del_when_completed(floor, queue, down_queue, up_queue, cabin_queue);
    }
    
    return super_stop;
}