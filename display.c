#include <stdio.h>
#include "display.h"


static const char *mode_string(WashMode mode)
{
    switch (mode)
    {
        case MODE_HEAVY:
            return "Heavy";

        case MODE_NORMAL:
            return "Normal";

        case MODE_LIGHT:
            return "Light";

        case MODE_NONE:
            return "None";

        default:
            return "Unknown";
    }
}


static const char *state_string(MachineState state)
{
    switch (state)
    {
        case IDLE:
            return "IDLE";

        case WAITING_FOR_DETERGENT:
            return "WAITING FOR DETERGENT";

        case RUNNING:
            return "RUNNING";

        case POWER_FAILURE:
            return "POWER FAILURE";

        case COMPLETED:
            return "COMPLETED";

        case ABORTED:
            return "ABORTED";

        default:
            return "UNKNOWN";
    }
}


static const char *door_string(DoorStatus door)
{
    switch (door)
    {
        case DOOR_OPEN:
            return "OPEN";

        case DOOR_CLOSED:
            return "CLOSED";

        case DOOR_LOCKED:
            return "LOCKED";

        default:
            return "UNKNOWN";
    }
}


void display_status(const WashingMachine *machine)
{
    printf("\n");
    printf("------------- STATUS --------------\n");

    printf("Mode            : %s\n",
           mode_string(machine->mode));

    printf("State           : %s\n",
           state_string(machine->state));

    printf("Door            : %s\n",
           door_string(machine->door_status));

    printf("Remaining Time  : %d minutes\n",
           machine->remaining_time);

    printf("Detergent       : %s\n",
           machine->detergent_present
           ? "PRESENT"
           : "EMPTY");

    printf("Start Request   : %s\n",
           machine->start_requested
           ? "PENDING"
           : "NONE");

    printf("-----------------------------------\n");
}


void display_message(const char *message)
{
    printf("\n%s\n", message);
}