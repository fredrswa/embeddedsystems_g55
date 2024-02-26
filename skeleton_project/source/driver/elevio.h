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

void go_to_floor(int new_floor, int kø[]);
void open_door(int kø[]);
int between_floors();
void startup();
int is_kø_empty(int kø[]);
void kø_add_if_pressed(int kø[]);
int kø_manager(int kø[]);
void kø_del_when_completed(int floor, int kø[]);

