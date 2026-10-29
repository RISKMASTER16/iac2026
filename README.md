# IAC 2026 – Lab Logbook

Lab work for **ELEC50010 Instruction Set Architectures & Compilers** (EIE Year 2, Imperial College London).
Each lab has its own folder with the SystemVerilog designs, C++ testbenches, build scripts and notes from my logbook.

| Lab | Topic | Folder |
|-----|-------|--------|
| 1 | Counter: SystemVerilog, Verilator, GTKWave and Vbuddy | [`lab1-counter/`](lab1-counter/) |

## Tools

- [Verilator](https://verilator.org) turns SystemVerilog into a C++ model, built together with a C++ testbench
- GTKWave shows the `.vcd` waveform each run produces
- Vbuddy, the course board, links the simulation to a TFT screen, 7-segment displays and a rotary encoder over USB

Course material: [ELEC50010-IAC on GitHub](https://github.com/ELEC50010-IAC)

## Running any design

Every runnable folder has a `doit.sh`. From WSL/Linux:

```bash
source ./doit.sh
```

It moves into its own folder, re-attaches Vbuddy to WSL if needed, rebuilds with Verilator and runs the simulation.
`obj_dir/` and `*.vcd` files are generated on each run and are not committed.
