#include <stdio.h>

#include "machine.h"
#include "input.h"
#include "display.h"
#include "timer.h"
#include "power.h"


int main(void)
{
    WashingMachine machine;
    UserInput input;

    machine_init(&machine);

    printf("Washing Machine Simulator Started.\n");


    while (1)
    {
        input = get_user_input();


        switch (input)
        {
            case INPUT_START:

                machine_start(&machine);

                break;


            case INPUT_ABORT:

                machine_abort(&machine);

                break;


            case INPUT_SELECT_HEAVY:

                machine_select_mode(&machine,
                                    MODE_HEAVY);

                break;


            case INPUT_SELECT_NORMAL:

                machine_select_mode(&machine,
                                    MODE_NORMAL);

                break;


            case INPUT_SELECT_LIGHT:

                machine_select_mode(&machine,
                                    MODE_LIGHT);

                break;


            case INPUT_DOOR_OPEN:

                machine_open_door(&machine);

                break;


            case INPUT_DOOR_CLOSE:

                machine_close_door(&machine);

                break;


            case INPUT_DETERGENT_FILLED:

                machine_fill_detergent(&machine);

                break;


            case INPUT_POWER_OFF:

                power_failure(&machine);

                break;


            case INPUT_POWER_ON:

                power_restore(&machine);

                break;


            case INPUT_SHOW_STATUS:

                display_status(&machine);

                break;


            case INPUT_INVALID:

                display_message("Invalid input.");

                break;


            case INPUT_EXIT:

                printf("Exiting simulator.\n");

                return 0;
        }


        /*
         * One iteration represents one simulated
         * unit of washing time.
         */
        if (machine.state == RUNNING)
        {
            timer_tick(&machine);
        }
    }


    return 0;
}