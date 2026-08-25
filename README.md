## Table of Contents

- [Overview](#overview)
- [Project Structure](#project-structure)
- [Machine States](#machine-states)
- [State Transitions](#state-transitions)
- [Functional Requirements Implemented](#functional-requirements-implemented)
  - [1. `machine.c` — Core Machine Logic](#1-machinec--core-machine-logic)
  - [2. `timer.c` — Timing & Background Thread](#2-timerc--timing--background-thread)
  - [3. `power.c` — Power Failure & Recovery](#3-powerc--power-failure--recovery)
- [Sample Session](#sample-session)
- [Design Notes / Known Limitations](#design-notes--known-limitations)
- [Credits](#credits)

---

## Overview

A command-line simulation of a washing machine, implemented in C. The
program models the machine as a **finite state machine** and exposes a
menu-driven interface for feeding simulated inputs (mode selection, door
status, detergent status, power status) and observing the machine's
behaviour in real time.

The implementation is split across multiple `.c`/`.h` modules, each
responsible for one functional area — core state machine, input handling,
display, timing, and power management — following standard C practice of
separating a module's public interface (`.h`) from its implementation
(`.c`).

The simulator runs a menu-driven loop (`main.c`) that reads a numeric choice
from the user (`input.c`) and dispatches it to the corresponding
`machine_*` / `power_*` function. A `WashingMachine` struct
(`machine.h`) holds all the state:

```c
typedef struct
{
    WashMode     mode;               // MODE_NONE / HEAVY / NORMAL / LIGHT
    MachineState state;              // IDLE / WAITING_FOR_DETERGENT / RUNNING / POWER_FAILURE / COMPLETED / ABORTED
    DoorStatus   door_status;        // OPEN / CLOSED / LOCKED
    int          remaining_time;     // minutes left in the current cycle
    int          detergent_present;  // 0 or 1
    int          start_requested;    // 1 if START was pressed while waiting for detergent
    int          timer_running;      // 1 while the countdown thread should tick
} WashingMachine;
```

A **second POSIX thread** (`timer_thread` in `timer.c`) continuously
decrements `remaining_time` once per simulated "minute" (1 real second =
1 simulated minute) whenever `timer_running` is set, completely independent
of whether the user is typing something into the menu at that moment.

---

## Project Structure

```
washing-machine-simulator-project/
├── main.c                          # Entry point: menu loop + thread creation
├── machine.c / machine.h           # Machine state, mode, door, detergent logic 
├── timer.c   / timer.h             # Duration lookup, tick logic, background timer thread
├── power.c   / power.h             # Power-failure / power-restore logic 
├── input.c   / input.h             # Reads and validates menu choices
├── display.c / display.h           # Prints machine status / messages
├── Makefile                        # Build rules
├── Washing Machine Simulator.pdf           # Problem statement
├── Washing_Machine_Simulator_PPT_Lab1.pdf  # Slides with edge cases / requirements
└── reference_washing_machine.exe   # Reference binary (expected behavior)
```

### Why split into these files?

| File | Role |
|---|---|
| **`machine.h` / `machine.c`** | Defines the core data model (`WashMode`, `DoorStatus`, `MachineState`, `WashingMachine`) and the state-changing operations: select mode, start, abort, open/close door, fill detergent. This is the **heart of the FSM** — every transition rule lives here. |
| **`timer.h` / `timer.c`** | Owns everything time-related: mapping a `WashMode` to its duration, ticking the countdown down by one minute, and detecting/handling cycle completion. Also hosts `timer_thread`, the function run on the background `pthread` so the countdown is decoupled from the blocking `scanf` call in the input loop. |
| **`power.h` / `power.c`** | Isolates the power-failure/power-restore behavior from the rest of the machine logic, since it has its own preservation rules (remaining time, door lock, mode) that only apply while `RUNNING`. |
| **`input.h` / `input.c`** | Pure I/O boundary — reads one integer from `stdin`, validates it, and maps it to an `UserInput` enum. Keeps `scanf`/input-sanitization out of the business logic. |
| **`display.h` / `display.c`** | Pure output boundary — formats and prints the machine's status/messages. Keeps `printf` formatting out of the business logic. |
| **`main.c`** | Wires everything together: initializes the machine, spawns the timer thread, and runs the menu → dispatch loop. |
| **`Makefile`** | Builds all `.c` files into a single `washing_machine` binary with `gcc -Wall -Wextra -std=c11 -pthread`, and provides a `clean` target. |

Splitting the machine's *decision logic* (`machine.c`/`timer.c`/`power.c`)
from its *I/O* (`input.c`/`display.c`) means the FSM itself has zero
`scanf`/menu-formatting concerns — it only knows how to react to well-defined
events, which is what actually needed to be implemented for this assignment.

---

## Machine States

- **`IDLE`** — No cycle running; mode can be changed, door can be opened.
- **`WAITING_FOR_DETERGENT`** — Start requested, but detergent is missing;
  auto-starts once filled.
- **`RUNNING`** — Wash in progress; door locked, mode locked, timer counting
  down.
- **`POWER_FAILURE`** — Cycle paused; state and remaining time preserved
  until power is restored.
- **`COMPLETED`** — Timer reached zero; door unlocked, machine returns to
  `IDLE`.
- **`ABORTED`** — User aborted mid-cycle; door unlocked, machine returns to
  `IDLE`.

## State Transitions

- **`IDLE` → `RUNNING`**
  Trigger: `START` (mode set, door closed, detergent present)
  Door locks, timer set from mode duration.

- **`IDLE` → `WAITING_FOR_DETERGENT`**
  Trigger: `START` (mode set, door closed, no detergent)
  `start_requested` flag set; `remaining_time` pre-computed from mode.

- **`WAITING_FOR_DETERGENT` → `RUNNING`**
  Trigger: `DETERGENT_FILLED` (with `start_requested == 1`)
  Auto-starts; door locks, timer set.

- **`RUNNING` → `COMPLETED` → `IDLE`**
  Trigger: `remaining_time` reaches `0`
  Door unlocks, mode and detergent cleared, machine resets for a new cycle.

- **`RUNNING` → `ABORTED` → `IDLE`**
  Trigger: `ABORT`
  Door unlocks, detergent drained, cycle stops immediately.

- **`RUNNING` → `POWER_FAILURE`**
  Trigger: `machine->state == RUNNING` and `POWER_OFF`
  State and remaining time preserved; door stays locked.

- **`POWER_FAILURE` → `RUNNING`**
  Trigger: `POWER_ON` (`machine->state == POWER_FAILURE`)
  Resumes from the preserved `remaining_time`; door stays locked.

- **any → unchanged**
  Trigger: invalid input for current state (e.g. `START` with door open,
  `SELECT_MODE` while `RUNNING`/`POWER_FAILURE`, `ABORT` while not
  `RUNNING`)
  A message is displayed; no fields are modified.

---

## Functional Requirements Implemented

Below are the specific `/* TODO */` functions from the skeleton that I
implemented, grouped by file, with the edge cases each one accounts for.

### 1. `machine.c` — Core Machine Logic

#### `machine_init()`
Sets the machine to its default boot state: `MODE_NONE`, `IDLE`,
`DOOR_OPEN`, zeroed timers/flags. This is the baseline every other function
assumes as a starting point.

#### `machine_select_mode(machine, mode)`
- **Allowed:** only while `state == IDLE` and `mode != MODE_NONE`.
- **Edge case — power failure:** if the machine is in `POWER_FAILURE`, mode
  selection is explicitly blocked with its own message
  (*"There is power failure. Mode cannot be changed."*).
- **Edge case — mid-cycle:** any other non-`IDLE` state (`RUNNING`,
  `WAITING_FOR_DETERGENT`) rejects the change with
  *"Mode cannot be changed while the machine is running."*

#### `machine_start(machine)`
Validates, in order:
1. `state == IDLE` → otherwise *"The Machine is not ready to start."*
2. `mode != MODE_NONE` → otherwise *"Please select a valid wash mode."*
3. `door_status == DOOR_CLOSED` → otherwise *"Please close the door."*
4. `detergent_present` → if **false**, the machine does **not** reject the
   start outright. Instead it:
   - moves to `WAITING_FOR_DETERGENT`,
   - sets `start_requested = 1`,
   - **pre-computes and stores `remaining_time`** from the selected mode
     so the duration is already "reserved" and correct once detergent is
     added,
   - prints *"Detergent is empty. Waiting for Detergent."* and returns
     (without looping back for more input — `return` is used deliberately
     instead of falling through, so the menu re-prompts normally).
5. If all checks pass: `state = RUNNING`, door is `LOCKED`, `remaining_time`
   set from mode duration, `start_requested` cleared, `timer_running = 1`.

#### `machine_abort(machine)`
- **Allowed only from `RUNNING`** — otherwise
  *"No washing cycle is currently running."* This intentionally prevents
  aborting a cycle that was never actually started (e.g. while still
  `WAITING_FOR_DETERGENT`).
- On success: resets straight to `IDLE`, **drains the detergent**
  (`detergent_present = 0` — an explicit design decision, since an aborted
  wash shouldn't silently carry leftover detergent into the next cycle),
  unlocks/closes the door, stops the timer, and zeroes `remaining_time`.

#### `machine_open_door(machine)`
- **Blocked while `DOOR_LOCKED`** (i.e., while `RUNNING` or
  `POWER_FAILURE`) → *"The door is locked, it can't be opened."*
- **Extra edge case I added:** if the door is **already open**, it now
  reports *"Door is already opened."* instead of silently re-printing
  "Door Opened." — avoids a misleading repeated success message for a
  no-op action.

#### `machine_close_door(machine)`
- **Extra edge case I added:** if the door is **already closed**, reports
  *"The door is already closed."* instead of re-printing "Door Closed."
- **Extra edge case I added:** if the door is **already locked** (mid-cycle),
  reports *"The door is already locked."* — closing an already-locked door
  is a no-op, not an error, but shouldn't claim to have just closed it.
- Otherwise transitions `DOOR_OPEN → DOOR_CLOSED`.

#### `machine_fill_detergent(machine)`
- **Allowed only from `IDLE` or `WAITING_FOR_DETERGENT`** — otherwise
  *"Cannot add detergent. Machine is not idle or waiting for detergent."*
  (detergent can't be topped up mid-`RUNNING` cycle or during
  `POWER_FAILURE`).
- Sets `detergent_present = 1`.
- **Key edge case (auto-start):** if the machine was in
  `WAITING_FOR_DETERGENT` **and** `start_requested == 1` (i.e., the user had
  already pressed START earlier and was only blocked by missing detergent),
  filling the detergent **automatically transitions the machine to
  `RUNNING`**, locks the door, clears `start_requested`, and starts the
  timer — the user does not need to press START again.

---

### 2. `timer.c` — Timing & Background Thread

#### `get_mode_duration(mode)`
Straightforward lookup table required by the spec:
`HEAVY → 45`, `NORMAL → 30`, `LIGHT → 20`, anything else (`MODE_NONE`) → `0`.

#### `timer_tick(machine)`
- **No-op unless `state == RUNNING`** — the timer must not decrement time
  while `IDLE`, `WAITING_FOR_DETERGENT`, or `POWER_FAILURE`.
- Decrements `remaining_time` by 1 (guarded so it never goes negative).
- **Completion edge case:** once `remaining_time` hits `0`, the cycle is
  considered complete — the function resets the machine straight to `IDLE`
  (mode/detergent cleared implicitly via the state reset pattern used
  elsewhere), unlocks the door back to `CLOSED`, clears
  `detergent_present`, stops `timer_running`, mode changed to `MODE_NONE` and prints a completion
  banner. This is what actually realizes the *"ready for a new cycle"*
  requirement mentioned above — the FSM never has to sit in a separate
  `COMPLETED` state waiting for acknowledgment.

#### `timer_thread(arg)`
- Runs forever on its own `pthread`, started once from `main()`.
- **Core edge case — independence from user input:** the loop `sleep`s for
  1 second every iteration regardless of `timer_running`, and only calls
  `timer_tick()` when `timer_running` is true. This means:
  - the countdown advances in **real time**, even while the main thread is
    blocked waiting on `scanf()` for the next menu choice;
  - when the machine isn't running (`IDLE`, `WAITING_FOR_DETERGENT`,
    `POWER_FAILURE`), the thread simply idles without touching machine
    state, so it can't accidentally decrement time it shouldn't.
- One real second = one simulated minute, as required.

---

### 3. `power.c` — Power Failure & Recovery

#### `power_failure(machine)`
- **Allowed only from `RUNNING`** — otherwise *"No active washing cycle."*
  (you can't have a power failure interrupt a cycle that isn't running).
- On success:
  - `state → POWER_FAILURE`,
  - **door stays `LOCKED`** (safety — you shouldn't be able to open a
    washer mid-cycle just because the power dropped),
  - `timer_running = 0` so the background thread stops ticking, which is
    exactly how `remaining_time` gets **preserved** without any extra
    bookkeeping — it simply stops changing,
  - prints the remaining time so the user can see it was preserved.

#### `power_restore(machine)`
- **Allowed only from `POWER_FAILURE`** — otherwise
  *"Machine is not in power failure state."*
- On success:
  - door stays `LOCKED`,
  - `state → RUNNING`,
  - `timer_running = 1`, so the background thread immediately resumes
    ticking down from the **preserved** `remaining_time` — no time is lost
    or fabricated across the outage,
  - prints confirmation and the resumed remaining time.

---

## Sample Session

```
====================================
      WASHING MACHINE SIMULATOR
====================================
1. Start
2. Abort
3. Select Heavy Mode
4. Select Normal Mode
5. Select Light Mode
6. Open Door
7. Close Door
8. Fill Detergent
9. Power Off
10. Power On
11. Show Status
12. Exit
------------------------------------
Enter choice: 4
Wash mode selected.

Enter choice: 7
Door Closed.

Enter choice: 1
Detergent is empty. Waiting for Detergent.

Enter choice: 8
Detergent filled.
Detergent detected. Washing started automatically.

Enter choice: 11

------------- STATUS --------------
Mode            : Normal
State           : RUNNING
Door            : LOCKED
Remaining Time  : 30 minutes
Detergent       : PRESENT
Start Request   : NONE
-----------------------------------
```

---

## Design Notes / Known Limitations

- `remaining_time` is measured in whole minutes and only ever decremented
  by the background timer thread — there's no fractional-minute precision.
- `power_failure()`/`power_restore()` only guard against being called from
  the wrong state; they don't simulate *how* a power failure occurs (it's
  purely a user-triggered menu event, as required by the assignment).
- `ABORT` and `POWER_OFF` are both only valid from `RUNNING`, which means a
  cycle that's `WAITING_FOR_DETERGENT` can only be resolved by filling
  detergent — there's currently no way to cancel a pending start once
  requested (a reasonable follow-up enhancement).

---

## Credits

- Skeleton code, problem statement (`Washing Machine Simulator.pdf`) and
  edge-case slides (`Washing_Machine_Simulator_PPT_Lab1.pdf`) were provided
  as part of the assignment.
- All `TODO` function bodies in `machine.c`, `timer.c`, and `power.c`, plus
  the additional edge-case handling described above, were implemented by
  me.
- Reference: [AllenPrabu/washing-machine-simulator](https://github.com/AllenPrabu/washing-machine-simulator)
