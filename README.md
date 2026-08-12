# Washing Machine Simulator — Lab 1

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

## Compilation

Using GCC:

```bash
gcc main.c machine.c input.c display.c timer.c power.c -o washing_machine
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