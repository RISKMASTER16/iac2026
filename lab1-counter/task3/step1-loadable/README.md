# Task 3 step 1 – loadable counter

`en` is replaced by a load signal `ld` and a preset value `v`:

```systemverilog
else count <= ld ? v : count + {{WIDTH-1{1'b0}},1'b1};
// ld is select line for a MUX that selects count or override
```

Testbench:

```cpp
vbdSetMode(1);              // one-shot flag (once, before the loop)
...
top->rst = (i<2);
top->ld  = vbdFlag();       // 1 for one cycle each time you press the button
top->v   = vbdValue();      // value set by turning the encoder
```

The counter counts up freely. Turn the encoder to choose a value, then press the button: the counter jumps to that value on the next clock edge and carries on counting from there. One-shot mode means `ld` is high for exactly one cycle, so the load happens once rather than every cycle.
