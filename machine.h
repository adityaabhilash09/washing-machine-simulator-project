#ifndef MACHINE_H
#define MACHINE_H

typedef enum
{
    MODE_NONE,
    MODE_HEAVY,
    MODE_NORMAL,
    MODE_LIGHT
} WashMode;

typedef enum
{
    DOOR_OPEN,
    DOOR_CLOSED,
    DOOR_LOCKED
} DoorStatus;

typedef enum
{
    IDLE,
    WAITING_FOR_DETERGENT,
    RUNNING,
    POWER_FAILURE,
    COMPLETED,
    ABORTED
} MachineState;

typedef struct
{
    WashMode mode;
    MachineState state;
    DoorStatus door_status;

    int remaining_time;
    int detergent_present;
    int start_requested;

} WashingMachine;


/* Initialization */
void machine_init(WashingMachine *machine);

/* Mode */
void machine_select_mode(WashingMachine *machine, WashMode mode);

/* Machine operations */
void machine_start(WashingMachine *machine);
void machine_abort(WashingMachine *machine);

/* Door */
void machine_open_door(WashingMachine *machine);
void machine_close_door(WashingMachine *machine);

/* Detergent */
void machine_fill_detergent(WashingMachine *machine);

#endif