# Task 3 – Vbuddy parameter and flag in one-shot mode

| Folder | Design |
|--------|--------|
| [`step1-loadable/`](step1-loadable/) | Pressing the button loads the rotary-encoder value `v` into the counter |
| [`step2-single-step/`](step2-single-step/) | Counter only increments once per button press |

## One-shot mode

`vbdSetMode(1);` goes **once, after `vbdOpen()` and before the loop**. It is a setting stored on Vbuddy, and it is sent over the serial port, so the port must be open first.

| Mode | Button press makes `vbdFlag()`... | Like |
|------|-----------------------------------|------|
| Toggle (0, default) | flip between 0 and 1 and **stay** there | a light switch |
| One-shot (1) | return 1 on the **next read only**, then reset to 0 | a doorbell |

One press between cycles 3 and 4:

| Cycle | 1 | 2 | 3 | 4 | 5 | 6 |
|---|---|---|---|---|---|---|
| Toggle | 0 | 0 | 0 | 1 | 1 | 1 |
| One-shot | 0 | 0 | 0 | 1 | 0 | 0 |

With `top->ld = vbdFlag();`, one-shot mode gives a single-cycle pulse on `ld`. In toggle mode `ld` would stay high and the counter would keep reloading or stepping on every cycle. In hardware, this is what an edge detector does.

`vbdValue()` returns the parameter set by turning the rotary encoder. It is shown at the bottom left of the TFT in decimal and hex.
