#include "machine.h"
#include "timer.h"
#include <stdio.h>

void machine_init(WashingMachine *machine)
{
    machine->mode = MODE_NONE;
    machine->state = IDLE;
    machine->door_status = DOOR_OPEN;

    machine->remaining_time = 0;
    machine->detergent_present = 0;
    machine->start_requested = 0;
    machine->timer_running = 0;
}


/*
 * Select the wash mode.
 * Mode can only be changed while the machine is in IDLE.
 */
void machine_select_mode(WashingMachine *machine, WashMode mode)
{
    /* TODO: Implement mode selection logic */
    if(machine->state == IDLE && mode != MODE_NONE)
    {
        machine->mode = mode ; 
        printf("Wash mode selected. \n");
        return;
    }if(machine->state == POWER_FAILURE){   // This is the extra statement i have added.
        printf("There is power failure. Mode cannot be changed. \n");
        return;
    }
    else{
        printf("Mode cannot be changed while the machine is running.\n");  
    }
    
}


/*
 * Start the washing machine.
 *
 * Conditions:
 * - Machine must be IDLE
 * - A valid mode must be selected
 * - Door must be closed
 * - If detergent is absent, start request must remain pending
 */
void machine_start(WashingMachine *machine)
{
    /* TODO: Implement start logic */    
    
    // if we use return it will again ask the user for a input
    if(machine->state != IDLE){
        printf("The Machine is not ready to start. \n");
        return;
    }
    if(machine->mode == MODE_NONE){
        printf("Please select a valid wash mode.\n");
        return;
    }if(machine->door_status != DOOR_CLOSED){
        printf("Please close the door. \n");
        return;
    }if(!machine->detergent_present){
        machine->state = WAITING_FOR_DETERGENT;
        machine->start_requested = 1;
        machine->remaining_time = get_mode_duration(machine->mode);
        printf("Detergent is empty. Waiting for Detergent. \n");
        return;
    }

    machine->state = RUNNING;
    machine->door_status = DOOR_LOCKED;
    machine->remaining_time = get_mode_duration(machine->mode);
    machine->start_requested = 0;
    machine->timer_running = 1;

    printf("Washing Machine Started. \n");
}


/*
 * Abort the current washing cycle.
 * Aborting should stop the cycle and unlock the door.
 */
void machine_abort(WashingMachine *machine)
{
    /* TODO: Implement abort logic */
    if(machine->state != RUNNING){
        printf("No washing cycle is currently running. \n");
        return;
    }

    machine->state = IDLE; 
    machine->detergent_present = 0; // It should drain the detergent also.
    machine->door_status = DOOR_CLOSED;
    machine->timer_running = 0;
    machine->remaining_time = 0;

    printf("Washing cycle aborted. \n");
}


/*
 * Open the door.
 * The door cannot be opened while it is locked.
 */
void machine_open_door(WashingMachine *machine)
{

    /* TODO: Implement door opening logic */
    if(machine->door_status == DOOR_LOCKED){
        printf("The door is locked , It can't be opened. \n");  
        return;
    }if(machine->door_status == DOOR_OPEN){   // this is the extra condition I have added.
        printf("Door is already opened. \n");
        return;
    }
    machine->door_status = DOOR_OPEN;
    printf("Door Opened. \n");
}


/*
 * Close the door.
 */
void machine_close_door(WashingMachine *machine)
{
    /* TODO: Implement door closing logic */
    if(machine->door_status == DOOR_CLOSED){   // This is the extra condition I have added
        printf("The door is already closed. \n");
        return;
    }
    if(machine->door_status == DOOR_LOCKED){    
        printf("The door is already locked. \n");
        return;
    }
    machine->door_status = DOOR_CLOSED;
    printf("Door Closed. \n");

}


/*
 * Fill detergent.
 * If a start request is already pending,
 * the machine should automatically start.
 */
void machine_fill_detergent(WashingMachine *machine)
{
    /* TODO: Implement detergent logic */

    if (machine->state != IDLE &&
        machine->state != WAITING_FOR_DETERGENT)
    {
        printf("Cannot add detergent. Machine is not idle or waiting for detergent.\n");
        return;
    }

    machine->detergent_present = 1;
    printf("Detergent filled.\n");

    /* If machine was waiting for detergent after START was requested */
    if (machine->state == WAITING_FOR_DETERGENT &&
        machine->start_requested == 1)
    {
        machine->state = RUNNING;
        machine->door_status = DOOR_LOCKED;
        machine->start_requested = 0;
        machine->timer_running = 1;

        printf("Detergent detected. Washing started automatically.\n");
    }
}


// The is a question that while there is power failure , can the machine cycle be aborted.
// detergent cannot be filled while the machine is running or in some other state.