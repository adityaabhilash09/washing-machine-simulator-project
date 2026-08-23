#include <stdio.h>
#include "power.h"


/*
 * Handle power failure while the machine is RUNNING.
 *
 * Requirements:
 * - Preserve the current remaining time.
 * - Preserve the unfinished cycle.
 * - Keep the door locked.
 * - Move the machine to POWER_FAILURE state.
 */
void power_failure(WashingMachine *machine)
{
    /* TODO: Implement power failure logic */
    if(machine->state != RUNNING){
        printf("No active washing cycle. \n");
        return;
    }
    if(machine->state == RUNNING){
        machine->state = POWER_FAILURE;
        machine->door_status = DOOR_LOCKED;
        machine->timer_running = 0;

        printf("Power failure detected. \n");
        printf("Remaining time preserved : %d minutes. \n", machine->remaining_time);
    }

}


/*
 * Handle restoration of power.
 *
 * Requirements:
 * - Power restoration is valid only from POWER_FAILURE.
 * - Resume the unfinished washing cycle.
 * - Preserve the remaining time.
 * - Keep the door locked.
 * - Return to RUNNING state.
 */
void power_restore(WashingMachine *machine)
{
    /* TODO: Implement power restoration logic */
    if(machine->state != POWER_FAILURE){
        printf("Machine is not in power failure state. \n");
        return;
    }
    if(machine->state == POWER_FAILURE){
        machine->door_status = DOOR_LOCKED;
        machine->state = RUNNING;
        machine->timer_running = 1;

        printf("Power restored. \n");
        printf("Resuming wash cycle. \n");
        printf("Remaining time : %d minutes. \n",machine->remaining_time);

    }
}