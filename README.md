# Washing Machine Simulator — Lab 1

## System Setup

The following software is required:

- **Visual Studio Code**
- **C/C++ extension for VS Code**
- **MSYS2**
- **GCC compiler**
- **Make**

### 1. Install VS Code

Install Visual Studio Code and install the **C/C++** extension from the Extensions panel.

### 2. Install MSYS2

Install MSYS2 and open the **MSYS2 UCRT64** terminal.

Install the required compiler and build tools using:

```bash
pacman -Syu
```

If MSYS2 asks you to close and reopen the terminal, do so and continue the update.

Then install GCC and Make:
```bash
pacman -S --needed mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-make
```

### 3. Verify the Installation

Open the **MSYS2 UCRT64** terminal and run:
```bash
gcc --version
```

and:
```bash
make --version
```

Both commands should display the installed versions.

### 4. Open the Project

Clone or download this repository and open the project folder in VS Code.

All compilation commands should be run from the project directory using the MSYS2 UCRT64 terminal.

## Objective

Implement the missing functions in the washing machine simulator using C.

The project is divided into multiple `.c` and `.h` files to demonstrate modular C programming.

## Files to Implement

You only need to implement the functions marked `TODO` in:

- `machine.c`
- `timer.c`
- `power.c`

Do not modify the provided `.h` files or the other `.c` files.

## Reference Material

Please refer to the following PDFs provided in this repository:

- **Washing Machine Simulator** — Problem Statement and Functional Requirements
- **Washing Machine Simulator — Lab 1** — States, inputs, machine structure, state transitions, and project structure

Read the PDFs carefully before implementing the missing functions.

## Timer Behavior

The washing machine uses a simulated timer:

- **1 real second = 1 simulated minute**
- Heavy Mode = 45 simulated minutes
- Normal Mode = 30 simulated minutes
- Light Mode = 20 simulated minutes
- The timer runs automatically in the background while the machine is washing.
- The remaining time can be viewed using **Show Status**.
- The timer does not print a countdown every second.
- When the timer reaches zero, the washing cycle completes and a completion message is displayed.

## Compilation

Using GCC:

```bash
gcc -Wall -Wextra -std=c11 -pthread main.c machine.c input.c display.c timer.c power.c -o washing_machine.exe
```

## Run

```bash
./washing_machine.exe
```

## Reference Executable

**reference_washing_machine.exe** is provided as a working reference implementation.

You may run it to understand the expected behaviour of the simulator.

Use the problem statement, Lab 1 PDF, state-transition diagrams, and reference executable to implement the missing functions.

```bash
Project Structure
WashingMachineSimulator/
├── main.c
├── machine.h
├── machine.c        ← IMPLEMENT
├── input.h
├── input.c
├── display.h
├── display.c
├── timer.h
├── timer.c          ← IMPLEMENT
├── power.h
├── power.c          ← IMPLEMENT
├── Makefile
├── reference_washing_machine.exe
├── Washing Machine Simulator.pdf
└── Washing_Machine_Simulator_PPT_Lab1.pdf
```