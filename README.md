# Lab 3 – Finite State Machines

EIE2 Instruction Architectures & Compilers (IAC). Lab sheet: [ELEC50010-IAC/Lab3-FSM](https://github.com/ELEC50010-IAC/Lab3-FSM).

| Task | What it builds | Key new idea |
|---|---|---|
| [Task 0](task0/) | GTest example | Automated testing (verification) |
| [Task 1](task1/) | 4-bit and 7-bit LFSRs | Pseudo-random sequences; asynchronous reset |
| [Task 2](task2/) | F1 light sequence FSM | `typedef enum` states; state register / next-state / output blocks |
| [Task 3](task3/) | clktick calibration; lights stepping every second | Clock dividers; simulation isn't real time |
| [Task 4](task4/) | Full F1 lights + reaction timer | Combining modules with control signals and a MUX |

## Results

- Task 0: 2/2 tests pass.
- Task 1: `verify.sh` 3/3 and `verify_7.sh` 2/2 pass.
- Task 2: `verify.sh` 2/2 pass.
- Every design compiles with `verilator -Wall` with no warnings.
- Tasks 3 and 4 were checked in simulation (see their READMEs).

## Running

`vbuddy.cpp` lives in this folder; the testbenches include it as `../vbuddy.cpp`.
**Copy your `vbuddy.cfg` into each task folder** you run on Vbuddy.

| Task | GTest check | Vbuddy |
|---|---|---|
| 0 | `./doit.sh` | – |
| 1 | `./verify.sh`, `./verify_7.sh` | `./doit.sh` |
| 2 | `./verify.sh` | `./doit.sh` |
| 3 | – | `./clktick.sh`, then `./f1_auto.sh` |
| 4 | – | `./delay.sh`, then `./f1_top.sh` |

GTest on Ubuntu/WSL: `sudo apt install libgtest-dev`. The verify scripts were
changed to use the system GTest on Linux and Homebrew's on macOS.
