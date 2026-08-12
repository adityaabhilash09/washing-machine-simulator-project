#include <stdio.h>
#include "input.h"


UserInput get_user_input(void)
{
    int choice;

    printf("\n");
    printf("====================================\n");
    printf("       WASHING MACHINE SIMULATOR    \n");
    printf("====================================\n");

    printf("1.  Start\n");
    printf("2.  Abort\n");
    printf("3.  Select Heavy Mode\n");
    printf("4.  Select Normal Mode\n");
    printf("5.  Select Light Mode\n");
    printf("6.  Open Door\n");
    printf("7.  Close Door\n");
    printf("8.  Fill Detergent\n");
    printf("9.  Power Off\n");
    printf("10. Power On\n");
    printf("11. Show Status\n");
    printf("12. Exit\n");

    printf("------------------------------------\n");
    printf("Enter choice: ");

    if (scanf("%d", &choice) != 1)
    {
        /*
         * Clear invalid input from stdin.
         */
        int ch;

        while ((ch = getchar()) != '\n' && ch != EOF)
        {
            /* discard input */
        }

        return INPUT_INVALID;
    }

    switch (choice)
    {
        case 1:
            return INPUT_START;

        case 2:
            return INPUT_ABORT;

        case 3:
            return INPUT_SELECT_HEAVY;

        case 4:
            return INPUT_SELECT_NORMAL;

        case 5:
            return INPUT_SELECT_LIGHT;

        case 6:
            return INPUT_DOOR_OPEN;

        case 7:
            return INPUT_DOOR_CLOSE;

        case 8:
            return INPUT_DETERGENT_FILLED;

        case 9:
            return INPUT_POWER_OFF;

        case 10:
            return INPUT_POWER_ON;

        case 11:
            return INPUT_SHOW_STATUS;

        case 12:
            return INPUT_EXIT;

        default:
            return INPUT_INVALID;
    }
}