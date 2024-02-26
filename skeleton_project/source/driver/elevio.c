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

void go_to_floor(int new_floor, int kø[]) 
{   
    int current_floor = elevio_floorSensor();
    while(1){
        int current_floor1 = elevio_floorSensor();
        if(current_floor1 != -1){
            current_floor = current_floor1;
            elevio_floorIndicator(current_floor);

        }
        if(current_floor > new_floor) {

            elevio_motorDirection(DIRN_DOWN);
        }else if(current_floor < new_floor)
        {
            elevio_motorDirection(DIRN_UP);
        }
        if(current_floor == new_floor) {
            elevio_motorDirection(DIRN_STOP);
            open_door(kø);
            kø_add_if_pressed(kø);
            kø_del_when_completed(new_floor,kø);
            break;

        }
        kø_add_if_pressed(kø);
        


        if(elevio_stopButton()){
            elevio_motorDirection(DIRN_STOP);
            break;
        }
        
    }

}
void open_door(int kø[]){
    if(between_floors()){
        printf("The elevator is unable to open the dor in this state");
    }else{
        elevio_doorOpenLamp(1);
        int i = 0;
        for(int i = 0; i < 200; i++){
        kø_add_if_pressed(kø);
        usleep(10000);
        }
        while(elevio_obstruction()){
            for(int i = 0; i < 50; i++){
        kø_add_if_pressed(kø);
        usleep(10000);
        }

        }
        elevio_doorOpenLamp(0);
    }
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

void kø_add_if_pressed(int kø[]){
    
    if(elevio_callButton(0, 0) || elevio_callButton(0, 1) || elevio_callButton(0, 2)){
        kø[0]=1;
    }
    if(elevio_callButton(1, 0) || elevio_callButton(1, 1) || elevio_callButton(1, 2)){
        kø[1]=1;
    }
    if(elevio_callButton(2, 0) || elevio_callButton(2, 1) || elevio_callButton(2, 2)){
        kø[2]=1;
    }
    if(elevio_callButton(3, 0) || elevio_callButton(3, 1) || elevio_callButton(3, 2)){
        kø[3]=1;
    }
}
int kø_manager(int kø[]){
    for(int i = 0; i < 4; i++){
        kø_add_if_pressed(kø);
        if(kø[i]==1){
            return i;
        }
    }
    return -1;
}
void kø_del_when_completed(int floor ,int kø[]){
    kø[floor]=0;
}
