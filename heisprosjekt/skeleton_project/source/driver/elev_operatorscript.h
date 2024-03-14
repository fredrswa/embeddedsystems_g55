/**
 * @file
 * @brief This file contains the main operator functions for the elevator.
*/

#pragma once

/**
 * @brief This function is used to start the elevator. It will move the elevator down until it reaches a floor, and then stop the elevator and set the floor indicator to the floor it is at.
 * @param[in] new_floor Floor the elevator is to go to based on last call
 * @param[in, out] queue[] All floors that are called
 * @param[in, out] stop Has the elevator received a stop call
 * @param[in, out] c_f2 Floor memory for stopping function
 * @param[in, out] m_d The last motor direction
 * @param[in, out] down_queue The elevator will stop at these floors if it is travelling downwards
 * @param[in, out] up_queue[] The elevator will stop at these floors if it is travelling upwards
 * @param[in, out] cabin_queue[] The elevator will stop at all these floors
 * 
*/

int go_to_floor(int new_floor, int queue[], int *stop, int *c_f2, int *m_d, int down_queue[], int up_queue[], int cabin_queue[]);

/**
 * @brief This function is used to open the door of the elevator. 
 * It will also care for the stop button and obstruction button.
 * Stopping keeps the door open for 3 seconds.
 * Obstruction keeps the door open until the obstruction is removed and then waits three seconds.
 * Obstruction can be enabled at all times when the door is open.
 * @param[in] queue The elevator needs to be able to update calls
 * @param[in] down_queue The elevator needs to be able to update calls
 * @param[in] up_queue The elevator needs to be able to update calls
 * @param[in] cabin_queue The elevator needs to be able to update calls
*/

int open_door(int queue[], int down_queue[], int up_queue[], int cabin_queue[]);

/**
 * @brief Creates a new_floor to call based on all calls in the system.
 * @param[in] queue[] All floors that are called
 * @param[in] pri[] Used to prioritize floors based on previous calls
 * @param[in] down_queue[] All floors called down (floor panels)
 * @param[in] up_queue[] All floors called up (floor panels)
 * @param[in] cabin_queue[] All floors called by cabing (lift panel)
*/


int queue_manager(int queue[], int pri[], int down_queue[], int up_queue[], int cabin_queue[]);


/**
 * @brief This function is used to add a floor to the queue if a floor or lift panel button is pressed.
 * @param[in, out] queue[] All floors that are called
 * @param[in, out] down_queue[] All floors called down (floor panels)
 * @param[in, out] up_queue[] All floors called up (floor panels)
 * @param[in, out] cabin_queue[] All floors called by cabing (lift panel)
*/
void queue_add_if_pressed(int queue[], int down_queue[], int up_queue[], int cabin_queue[]);


/**
 * @brief This function checks if the lift is between floors.
 * @return 1 if the lift is between floors, 0 if the lift is at a floor
*/

int between_floors();


/**
 * @brief This function is used to start the elevator. It will move the elevator down until it reaches a floor, and then stop the elevator and set the floor indicator to the floor it is at.
 * This function does not allow calls.
 * It checks if the elevator is in a undefined state. 
 * It disregards the possibility of being below the first floor or above the last floor.
 * It will start goind downwards until it reaches a defined state.
 * Then stop at that floor and wait
*/

void startup();


/**
 * @brief Checks if there are any calls
 * @return 1 if queue is empty, 0 if not
*/

int is_queue_empty(int queue[]); 


/**
 * @brief Updates queue value when lift arrives at floor.
 * Deletes the calls.
 * @param[in] floor Which floor the lift has arrived at
 * @param[in, out] queue Is updated
 * @param[in, out] down_queue Is updated
 * @param[in, out] up_queue Is updated
 * @param[in, out] cabin_queue Is updated
*/

void queue_del_when_completed(int floor ,int queue[], int down_queue[], int up_queue[], int cabin_queue[]);


/**
 * @brief Adds a stop functionality to the lift
 * Takes into account if the lift is at a floor or between floors
 * If it is at a floor the door opens with the same functionality as open_door.
 * @param[in, out] queue Deletes all calls
 * @param[in, out] down_queue Deletes all calls
 * @param[in, out] up_queue Deletes all calls
 * @param[in, out] cabin_queue Deletes all calls
 * 
*/


int emergency_stop(int queue[], int down_queue[], int up_queue[], int cabin_queue[]);