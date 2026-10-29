# Task 3 step 2 – single-stepping counter

The `v` input is removed, and `ld` now chooses between "add one" and "hold":

```systemverilog
else count <= ld ? count + {{WIDTH-1{1'b0}},1'b1} : count;
```

Testbench: one-shot mode as in step 1, with `top->ld = vbdFlag();` (no `top->v`).

Each button press gives exactly one cycle with `ld = 1`, so the counter advances by exactly one per press, i.e. it single-steps. The cycle counter on the TFT keeps running while the count stays put between presses.
