#pragma once


#define N_FLOORS 4

typedef enum { 
    DIRN_DOWN   = -1,
    DIRN_STOP   = 0,
    DIRN_UP     = 1
} MotorDirection;


#define N_BUTTONS 3

typedef enum { 
    BUTTON_HALL_UP      = 0,
    BUTTON_HALL_DOWN    = 1,
    BUTTON_CAB          = 2
} ButtonType;

#define m_second 1000


void elevio_init(void);

void elevio_motorDirection(MotorDirection dirn);
void elevio_buttonLamp(int floor, ButtonType button, int value);
void elevio_floorIndicator(int floor);
void elevio_doorOpenLamp(int value);
void elevio_stopLamp(int value);

int elevio_callButton(int floor, ButtonType button);
int elevio_floorSensor(void);
int elevio_stopButton(void);
int elevio_obstruction(void);


//Created for project

int go_to_floor(int new_floor, int kø[], int *stop, int *c_f2, int *m_d);
int kø_manager(int kø[], int pri[]);
void kø_add_if_pressed(int kø[]);
int open_door(int kø[]);
int between_floors();
void startup();
int is_kø_empty(int kø[]); 
void kø_del_when_completed(int floor ,int kø[]);
int emergency_stop(int kø[]);
//void start_from_undefined(int floor, int direction);

//NEEDS CHANGES
/*
go_to_floor()       Stopper når queue_down stemmer med retning
kø_manager()        Små endringer
kø_add_if_pressed() Velger hvilken queu som oppdateres
open_door()         Sjekker hele tiden om det legges til kø
is_kø_empty()       Sjekker om køen er tom
kø_del_when_completed() Sletter alle ordre når man stopper på floor
*/