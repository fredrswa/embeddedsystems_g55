#include <assert.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <pthread.h>

#include "elevio.h"
#include "con_load.h"

static int sockfd;
static pthread_mutex_t sockmtx;

void elevio_init(void){
    char ip[16] = "localhost";
    char port[8] = "15657";
    con_load("elevio.con",
        con_val("com_ip",   ip,   "%s")
        con_val("com_port", port, "%s")
    )
    
    pthread_mutex_init(&sockmtx, NULL);
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    assert(sockfd != -1 && "Unable to set up socket");
    
    struct addrinfo hints = {
        .ai_family      = AF_INET, 
        .ai_socktype    = SOCK_STREAM, 
        .ai_protocol    = IPPROTO_TCP,
    };
    struct addrinfo* res;
    getaddrinfo(ip, port, &hints, &res);
    
    int fail = connect(sockfd, res->ai_addr, res->ai_addrlen);
    assert(fail == 0 && "Unable to connect to elevator server");
    
    freeaddrinfo(res);
    
    send(sockfd, (char[4]){0}, 4, 0);
}




void elevio_motorDirection(MotorDirection dirn){
    pthread_mutex_lock(&sockmtx);
    send(sockfd, (char[4]){1, dirn}, 4, 0);
    pthread_mutex_unlock(&sockmtx);
}


void elevio_buttonLamp(int floor, ButtonType button, int value){
    assert(floor >= 0);
    assert(floor < N_FLOORS);
    assert(button >= 0);
    assert(button < N_BUTTONS);

    pthread_mutex_lock(&sockmtx);
    send(sockfd, (char[4]){2, button, floor, value}, 4, 0);
    pthread_mutex_unlock(&sockmtx);
}


void elevio_floorIndicator(int floor){
    assert(floor >= 0);
    assert(floor < N_FLOORS);

    pthread_mutex_lock(&sockmtx);
    send(sockfd, (char[4]){3, floor}, 4, 0);
    pthread_mutex_unlock(&sockmtx);
}


void elevio_doorOpenLamp(int value){
    pthread_mutex_lock(&sockmtx);
    send(sockfd, (char[4]){4, value}, 4, 0);
    pthread_mutex_unlock(&sockmtx);
}


void elevio_stopLamp(int value){
    pthread_mutex_lock(&sockmtx);
    send(sockfd, (char[4]){5, value}, 4, 0);
    pthread_mutex_unlock(&sockmtx);
}




int elevio_callButton(int floor, ButtonType button){
    pthread_mutex_lock(&sockmtx);
    send(sockfd, (char[4]){6, button, floor}, 4, 0);
    char buf[4];
    recv(sockfd, buf, 4, 0);
    pthread_mutex_unlock(&sockmtx);
    return buf[1];
}


int elevio_floorSensor(void){
    pthread_mutex_lock(&sockmtx);
    send(sockfd, (char[4]){7}, 4, 0);
    char buf[4];
    recv(sockfd, buf, 4, 0);
    pthread_mutex_unlock(&sockmtx);
    return buf[1] ? buf[2] : -1;
}


int elevio_stopButton(void){
    pthread_mutex_lock(&sockmtx);
    send(sockfd, (char[4]){8}, 4, 0);
    char buf[4];
    recv(sockfd, buf, 4, 0);
    pthread_mutex_unlock(&sockmtx);
    return buf[1];
}


int elevio_obstruction(void){
    pthread_mutex_lock(&sockmtx);
    send(sockfd, (char[4]){9}, 4, 0);
    char buf[4];
    recv(sockfd, buf, 4, 0);
    pthread_mutex_unlock(&sockmtx);
    return buf[1];
}


//Created for project

int go_to_floor(int new_floor, int kø[], int *stop, int *c_f2, int *m_d, int down_kø[], int up_kø[], int cabin_kø[])
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
                kø_add_if_pressed(kø, down_kø, up_kø, cabin_kø); 
                if(down_kø[i]==1 || cabin_kø[i]==1){
                    if(current_floor==i){
                        elevio_motorDirection(DIRN_STOP);
                        if (elevio_stopButton())
                            {
                                *stop = 1;
                                return 1;
                            }
                        super_stop = open_door(kø, down_kø, up_kø, cabin_kø);
                        kø_add_if_pressed(kø, down_kø, up_kø, cabin_kø);
                        kø_del_when_completed(current_floor,kø, down_kø, up_kø, cabin_kø);
                        break;
                    }
                }
            }
        }else if(*m_d==1){
            for(int i = 0; i < 4; i++){
                kø_add_if_pressed(kø, down_kø, up_kø, cabin_kø);
                if(up_kø[i]==1 || cabin_kø[i]==1){
                    if(current_floor==i){
                        elevio_motorDirection(DIRN_STOP);
                        if (elevio_stopButton())
                            {
                                *stop = 1;
                                return 1;
                            }
                        super_stop = open_door(kø, down_kø, up_kø, cabin_kø);
                        kø_add_if_pressed(kø, down_kø, up_kø, cabin_kø);
                        kø_del_when_completed(current_floor,kø, down_kø, up_kø, cabin_kø);
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
            super_stop = open_door(kø, down_kø, up_kø, cabin_kø);
            kø_add_if_pressed(kø, down_kø, up_kø, cabin_kø);
            kø_del_when_completed(new_floor,kø, down_kø, up_kø, cabin_kø);
            return super_stop;

        }
        kø_add_if_pressed(kø, down_kø, up_kø, cabin_kø);
        


        if(elevio_stopButton()){
            emergency_stop(kø, down_kø, up_kø, cabin_kø);
            super_stop = 1;
            *stop  =1;
            return super_stop;
            

        }
        
    }

    return super_stop;

}

int open_door(int kø[], int down_kø[], int up_kø[], int cabin_kø[]){
    if (elevio_stopButton()) {
        return 1;
    }
    if(between_floors()){
        printf("The elevator is unable to open the dor in this state");
    }else{
        elevio_doorOpenLamp(1);
        int i = 0;
        for(int i = 0; i < 200; i++){
        kø_add_if_pressed(kø, down_kø, up_kø, cabin_kø);
        if (elevio_stopButton()) {
            return 1;
        }
        usleep(10000);
        }
        if(elevio_obstruction()){
        while(elevio_obstruction()){
            for(int i = 0; i < 50; i++){
        kø_add_if_pressed(kø, down_kø, up_kø, cabin_kø);
        if (elevio_stopButton()) {
            return 1;
        }
        usleep(10000);
        }

        }
        for(int i = 0; i < 200; i++){
        kø_add_if_pressed(kø, down_kø, up_kø, cabin_kø);
        if (elevio_stopButton()) {
            return 1;
        }
        usleep(10000);
        }
        }
        elevio_doorOpenLamp(0);
    }
    return 0;
}
int between_floors(){
    int floor = elevio_floorSensor();
    if(floor==-1){
        return 1;
    }else{
        return 0;
    }
}
void startup(){
    
	
	while(1){
    int floor = elevio_floorSensor();
    if(floor == -1) {
            elevio_motorDirection(DIRN_DOWN);
        }else{
        elevio_motorDirection(DIRN_STOP);
        elevio_floorIndicator(floor);
	    break;
}
}
}

int is_kø_empty(int kø[]){
    for(int i = 0; i < 4; i++){
        if(kø[i]==1){
            return 0;
        }
    }
    return 1;
}

void kø_add_if_pressed(int kø[], int down_kø[], int up_kø[], int cabin_kø[]){
    
    
    if(elevio_callButton(0, 0) || elevio_callButton(0, 1) || elevio_callButton(0, 2)){
        kø[0]=1;

        if(elevio_callButton(0, 0)){
            elevio_buttonLamp(0, 0, 1);
            up_kø[0]=1;
        }else if(elevio_callButton(0, 1)){
            elevio_buttonLamp(0, 1, 1);
            down_kø[0]=1;
        }else if(elevio_callButton(0, 2)){
            elevio_buttonLamp(0, 2, 1);
            cabin_kø[0]=1;}
        }
    if(elevio_callButton(1, 0) || elevio_callButton(1, 1) || elevio_callButton(1, 2)){
        kø[1]=1;
        if(elevio_callButton(1, 0)){
            elevio_buttonLamp(1, 0, 1);
            up_kø[1]=1;
        }else if(elevio_callButton(1, 1)){
            elevio_buttonLamp(1, 1, 1);
            down_kø[1]=1;
        }else if(elevio_callButton(1, 2)){
            elevio_buttonLamp(1, 2, 1);
            cabin_kø[1]=1;}
        
    }
    if(elevio_callButton(2, 0) || elevio_callButton(2, 1) || elevio_callButton(2, 2)){
        kø[2]=1;

        if(elevio_callButton(2, 0)){
            elevio_buttonLamp(2, 0, 1);
            up_kø[2]=1;
        }else if(elevio_callButton(2, 1)){
            elevio_buttonLamp(2, 1, 1);
            down_kø[2]=1;
        }else if(elevio_callButton(2, 2)){
            elevio_buttonLamp(2, 2, 1);
            cabin_kø[2]=1;}
    }
    if(elevio_callButton(3, 0) || elevio_callButton(3, 1) || elevio_callButton(3, 2)){
        kø[3]=1;
        if(elevio_callButton(3, 0)){
            elevio_buttonLamp(3, 0, 1);
            up_kø[3]=1;
        }else if(elevio_callButton(3, 1)){
            elevio_buttonLamp(3, 1, 1);
            down_kø[3]=1;
        }else if(elevio_callButton(3, 2)){
            elevio_buttonLamp(3, 2, 1);
            cabin_kø[3]=1;}
    }
}
int kø_manager(int kø[], int pri[], int down_kø[], int up_kø[], int cabin_kø[]){
    int call = 0;
    int b = 0;
    for(int i = 0; i < 4; i++){
        kø_add_if_pressed(kø, down_kø, up_kø, cabin_kø);
        b+=kø[i];
        if((kø[i] &&  pri[i] > pri[call])){
            call = i;
        }
    }

    for(int i = 0; i < 4; i++) {
        if (kø[i]) {
            pri[i]++;
        }
    }

    pri[call] = 0;

    if(!b || (kø[call] == 0)){
        return -1;
    }
    return call;
}
void kø_del_when_completed(int floor ,int kø[], int down_kø[], int up_kø[], int cabin_kø[]){
    kø[floor]=0;
    down_kø[floor]=0;
    up_kø[floor]=0;
    cabin_kø[floor]=0;
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
int emergency_stop(int kø[], int down_kø[], int up_kø[], int cabin_kø[]) {
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
        kø_del_when_completed(floor, kø, down_kø, up_kø, cabin_kø);
    }
    
    return super_stop;
}