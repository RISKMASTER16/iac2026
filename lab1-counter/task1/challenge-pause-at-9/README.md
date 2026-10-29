# Task 1 challenge – pause for 3 cycles at 0x9

> Modify the testbench so that you stop counting for 3 cycles once the counter reaches 0x9, and then resume counting. You may also need to change the stimulus for `rst`.

`counter.sv` is unchanged from Task 1. Only the testbench changes. This version runs on Vbuddy, with the button (`vbdFlag()`) as the normal enable.

## How it works

The testbench keeps two extra variables:

- `pause`: how many cycles are left to hold `en` low.
- `held`: whether this visit to 9 has already triggered a pause. Without it, `count == 9` stays true while paused and the pause would restart forever. It re-arms once `count` moves off 9, so the pause happens again after the 8-bit counter wraps past `FF`.

```cpp
if (top->count == 9 && !held) { pause = 3; held = true; }
if (top->count != 9) held = false;

if (pause > 0) { top->en = 0; pause--; }
else           { top->en = vbdFlag(); }
```

`rst` was changed to `(i<2)` only. With the old `(i==15)`, the reset landed in the middle of the pause and wiped the count.

## Result

`count` sits at `09` for 4 displayed cycles (the cycle it arrives plus 3 paused), then continues to `0A`, even with the flag on.
