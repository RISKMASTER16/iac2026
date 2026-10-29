# Task 4 – Displaying count as BCD (multi-module design)

Files: `top.sv`, `counter.sv` (Task 1 enable version), `bin2bcd.sv` (course-provided), `top_tb.cpp`, `doit.sh`

## Structure

```
top
├── myCounter : counter   (clk, rst, en → count[7:0])
└── myDecoder : bin2bcd   (x = count → BCD[11:0])
```

`count` is an internal wire inside `top`, connecting the two modules. The only output is `bcd`: three 4-bit decimal digits (hundreds, tens, units).

## bin2bcd – "shift and add 3" (double dabble)

The binary input `x` is shifted left one bit at a time into a 20-bit register (8 bits of `x` + 3 × 4 BCD bits). Before each shift, any BCD digit that is **≥ 5 gets 3 added**, so that after the shift (×2) it carries correctly into the next decimal digit. After 8 shifts, the top 12 bits are the BCD result. It's all inside `always_comb`, so it is combinational logic, not clocked. See lecture 4 of IAC.

## Build changes (`doit.sh`)

```bash
verilator -Wall --cc --trace top.sv counter.sv bin2bcd.sv --top-module top --exe top_tb.cpp
make -j -C obj_dir/ -f Vtop.mk Vtop
obj_dir/Vtop
```

- All `.sv` files are listed, and `--top-module top` names the outermost module.
- Every generated name now comes from `top`: `Vtop.h`, `Vtop.mk`, `obj_dir/Vtop`.
- The lab's `top.sv` declares an input `v` that is never used. With `-Wall`, Verilator treats that warning as an error, so `v` is removed here.

## Testbench (`top_tb.cpp`)

- `#include "Vtop.h"` and `Vtop* top = new Vtop;`
- Display the three BCD digits instead of hex:

```cpp
vbdHex(3, (int(top->bcd) >> 8) & 0xF);   // hundreds
vbdHex(2, (int(top->bcd) >> 4) & 0xF);   // tens
vbdHex(1,  int(top->bcd)       & 0xF);   // units
```

- `top->en = vbdFlag();` in the default toggle mode.

The display now counts **000 → 255** in decimal, then wraps back to 000.

## Problems hit

- `%Error: Specified --top-module 'top' was not found`: the folder was missing `counter.sv`, and the testbench was misspelled `tob_tb.cpp`.
