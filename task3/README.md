# Task 3 – clktick and 1-second calibration

## clktick.sv (provided)

A down-counter that starts at `N`, counts down to 0, then outputs `tick = 1`
for one cycle and reloads `N`. So it ticks once every **N + 1** clock cycles: a
clock divider.

## Calibrating N

The Verilator simulation doesn't run in real time; how many simulated clock
cycles pass per second depends on your computer (and on the Vbuddy calls in
the loop, which are slow). So you find N by experiment:

```sh
cd task3
./clktick.sh
```

Play a 60 bpm metronome (search "metronome" on Google) and turn the knob until
the neopixels flash in time with it. The value shown bottom-left on the TFT is
your **N for a 1-second tick**. It's the `vbdValue()` the testbench feeds into N,
and one flash per beat means one tick per second. (The lab sheet quotes about 24
on an M1 MacBook.)

## Challenge: F1 lights stepping automatically

`f1_auto.sv` connects the tick to the FSM's `en`:

```
clktick ── tick ──► f1_fsm.en ──► data_out ──► neopixels
```

| File | What it is |
|---|---|
| `f1_auto.sv` | Top level: `clktick` + `f1_fsm` |
| `f1_fsm.sv` | Same as Task 2 |
| `f1_auto_tb.cpp`, `f1_auto.sh` | Testbench and build script |

```sh
./f1_auto.sh
```

Set `N_1SEC` at the top of `f1_auto_tb.cpp` to your calibrated value, or just
turn the knob to it; the knob overrides `N_1SEC` once it's non-zero.

Checked in simulation: with N = 4 the lights change exactly every 5 cycles.
