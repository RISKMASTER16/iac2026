# Lab 2 – Signal Generation and Capture

EIE2 Instruction Architectures & Compilers (IAC). Lab sheet: [ELEC50010-IAC/Lab2-SigGen](https://github.com/ELEC50010-IAC/Lab2-SigGen).

| Task | What it builds | Key new idea |
|---|---|---|
| [Task 1](task1/) | Sinewave generator: counter → ROM | Memory initialised from a file (`$readmemh`); `incr` controls frequency |
| [Task 2](task2/) | Two sinewaves with a phase offset | Dual-port ROM; offset 64 = 90° (sine + cosine) |
| [Task 3](task3/) | Audio delay line | Dual-port RAM read and written in the same cycle |

All three designs compile with `verilator -Wall` with no warnings, and were checked
in simulation: Task 1's output matches the ROM sample-for-sample for several
`incr` values, Task 2's second output is exactly `offset` samples ahead, and
Task 3's output is the input delayed by exactly `offset` samples (up to 511).

## Running a task

Each folder is self-contained. Copy your **`vbuddy.cfg`** (the USB port that
worked in Lab 1) into the folder, then:

```sh
cd task1        # or task2 / task3
./doit.sh
```

Press **q** to quit. If you get "permission denied", run `chmod +x doit.sh` once.

## Parameterisation recap

All the modules are written in terms of `ADDRESS_WIDTH` / `DATA_WIDTH`, so the same
code works at any size:

```systemverilog
rom #(10, 9) sineRom_1024x9 (...);                 // positional: 1024 x 9
rom #(.DATA_WIDTH(9)) sineRom (...);               // named: only one parameter changed
```

- n address bits → 2ⁿ locations
- data width → bits per location
- Parameters you don't override keep their defaults. To set only the second one,
  use the named form.

## Troubleshooting

| Symptom | Likely cause |
|---|---|
| `make: *** obj_dir/: No such file or directory` | The Verilator step failed first. Scroll up to the first `%Error`. |
| `syntax error, unexpected IDENTIFIER` on a parameter | Missing comma between parameters |
| `unexpected ..., expecting ';'` near the top | Port list must end with `);` |
| `unexpected endmodule` | Missing `;` or `end` just above it |
| `expecting ':'` in a bus | Bus ranges need `[high:low]`, e.g. `[DATA_WIDTH-1:0]` |
| Verilator stops on a `%Warning` | `-Wall` makes lint warnings fatal; fix the line it names |
| Flat line on Vbuddy | `en` is 0, `incr` is 0, `sinerom.mem` not found (run from inside the task folder), or plotting the wrong signal (`top->dout`) |
| No header on Vbuddy | `vbuddy.cfg` missing or wrong port; replug the Vbuddy |
