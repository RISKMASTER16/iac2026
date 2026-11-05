# Task 4 – Full F1 starting lights with reaction timer

```
clktick ── tick ─────► MUX(1) ─┐
lfsr_7 ── K ─► delay ─► MUX(0) ─┴─ en ─► f1_fsm ──► data_out ──► neopixels
                 ▲                        │
                 └─────── cmd_delay ───────┤
clktick.en, MUX select ◄─── cmd_seq ──────┘
switch (vbdFlag) ─────────────────────► f1_fsm.trigger
```

## How to play

```sh
cd task4
./f1_top.sh
```

1. Press the switch. The lights come on one per second.
2. When all 8 are on, they go out after a **random** delay.
3. Press the switch as fast as you can.
4. Your reaction time appears in the TFT header and on the 7-segment display (ms).

Pressing while the lights are still on shows **Jump start!**
Set `N_1SEC` in `f1_top_tb.cpp` to your Task 3 value (or turn the knob to it).

## Files

| File | What it is |
|---|---|
| `f1_top.sv` | Top level wiring everything together, including the MUX |
| `f1_fsm.sv` | Task 2 FSM extended with `trigger`, `cmd_seq`, `cmd_delay` |
| `clktick.sv` | From Task 3: 1-second ticks while the lights come on |
| `lfsr_7.sv` | From Task 1: free-running, gives the random delay length `K` |
| `delay.sv` | Provided: after a trigger, counts `K` cycles then pulses `time_out` |
| `f1_top_tb.cpp`, `f1_top.sh` | Testbench with the reaction timer, and build script |
| `delay_tb.cpp`, `delay.sh` | Provided test for `delay.sv` on its own |

## The FSM

| State | Lights | cmd_seq | cmd_delay | Leaves when |
|---|---|---|---|---|
| S0 | off | 0 | 0 | `trigger` (switch press) |
| S1–S7 | 1 to 7 on | **1** | 0 | `en` (clktick tick) |
| S8 | all on | 0 | **1** | next cycle, always |
| S9 | all on | 0 | 0 | `en` (delay `time_out`) |
| back to S0 | off | | | |

- **`cmd_seq`** is high while the lights are coming on. It enables `clktick` and
  makes the MUX pass `tick` to `en`. When it drops, the MUX passes `time_out`.
- **`cmd_delay`** starts the random delay. It has to be a **one-cycle pulse**:
  `delay.sv` reloads its counter every cycle that `trigger` is high, so holding
  it high would stop it ever counting down. That's why there are two "all on"
  states: S8 sends the pulse, S9 waits.

## Why the delay is random

The 7-bit LFSR runs every clock cycle, so the value `K` it holds when the delay
starts depends on exactly how many cycles passed since reset, which depends on
when you pressed the switch. That's effectively unpredictable to a human.
`K` is 1–127 cycles, i.e. up to about 5 seconds at N = 24.

## Reaction timer (testbench)

- When `data_out` goes from `FF` to `00`, the testbench calls `vbdInitWatch()`.
- The next switch press is treated as your reaction, not a new start: the
  testbench reads `vbdElapsed()` and displays it, and keeps `trigger` at 0.
- Any other press while idle is passed to the FSM as `trigger`.

## The SYNCASYNCNET line

`lfsr_7` uses an asynchronous reset (Task 1 requires it), but `clktick`,
`delay` and `f1_fsm` use synchronous resets. Verilator's `-Wall` flags one net
used both ways. It's harmless here, since `rst` is a clean pulse for the first
two cycles, so `f1_top.sv` switches that one check off with a `lint_off` comment.

## Checked in simulation (N = 4)

Three runs: lights came on every 5 cycles; the all-on time was 43, 112 and
13 cycles (random each time); `cmd_delay` pulsed exactly once per run; and a
press in the middle of the sequence was ignored.
