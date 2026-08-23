#include "timer.h"
#include <stdio.h>
#include <unistd.h>

/*
 * Return the wash duration for the selected mode:
 * Heavy  -> 45 minutes
 * Normal -> 30 minutes
 * Light  -> 20 minutes
 */
int get_mode_duration(WashMode mode)
{
    /* TODO: Implement mode duration logic */
    switch (mode){
        case MODE_HEAVY : return 45;
        case MODE_LIGHT : return 20;
        case MODE_NORMAL : return 30;
        default : return 0;
    }
}


/*
 * Timer operates only while the machine is RUNNING.
 *
 * Decrease the remaining time and handle completion
 * when the timer reaches zero.
 */
void timer_tick(WashingMachine *machine)
{
    /* TODO: Implement timer logic */
    if(machine->state != RUNNING){
        return;
    }
    if(machine->remaining_time > 0){
        machine->remaining_time -- ;
    }
    if(machine->remaining_time == 0){

        machine->timer_running = 0;
        machine->state = IDLE;
        machine->door_status = DOOR_CLOSED;
        machine->detergent_present = 0;

        printf("*** Washing cycle completed. ***");

    }
}


/*
 * Background timer thread.
 *
 * One real second represents one simulated minute.
 * The timer should continue running independently
 * of user input.
 */
void *timer_thread(void *arg)
{
    /* TODO: Implement background timer logic */
    WashingMachine *machine = (WashingMachine *)arg;
    while (1)
    {
        if (machine->timer_running)
        {
            sleep(1);
            timer_tick(machine);
        }
        else
        {
            sleep(1);
        }
    }
    return NULL;
}