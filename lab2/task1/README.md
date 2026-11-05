# Task 1 – Simple sinewave generator

A counter steps through addresses, and a ROM holding 256 sine samples turns
each address into a sample. Plotting the ROM output gives a sinewave.

```
incr ──► counter ──address[7:0]──► rom (256 x 8) ──► dout[7:0]
                       ▲                 ▲
clk ───────────────────┴─────────────────┘
```

## Files

| File | What it is |
|---|---|
| `sinegen.py` | Python script that generates `sinerom.mem` (256 cosine samples, 0–254, in hex) |
| `sinerom.mem` | ROM contents, loaded by `$readmemh` at the start of simulation |
| `counter.sv` | Counter that adds `incr` every clock cycle (Lab 1 added 1) |
| `rom.sv` | 256 x 8 synchronous ROM, parameterised by `ADDRESS_WIDTH` and `DATA_WIDTH` |
| `sinegen.sv` | Top level: wires the counter output into the ROM address |
| `sinegen_tb.cpp` | Verilator testbench: plots `dout` on Vbuddy; knob sets the frequency |
| `doit.sh` | Builds and runs everything |
| `vbuddy.cpp` | Vbuddy library (provided) |

**You also need your own `vbuddy.cfg`** (the USB port from Lab 1) in this folder.

## How it works

### counter.sv
```systemverilog
if (rst)     count <= {WIDTH{1'b0}};
else if (en) count <= count + incr;
```
`incr` is the step size. Bigger steps skip samples, so you get through one full
sine cycle faster: **one cycle takes 256 / incr clock cycles**.
`incr` must be declared `[WIDTH-1:0]`. Left as plain `logic` it is 1 bit and
can only be 0 or 1.

### rom.sv
- `logic [DATA_WIDTH-1:0] rom_array [2**ADDRESS_WIDTH-1:0];` is 256 locations of 8 bits.
- The `initial` block runs once at the start and fills the array from `sinerom.mem`.
- `dout <= rom_array[addr];` inside `always_ff @(posedge clk)` makes the output
  synchronous, so `dout` appears one clock after the address.

### sinegen.sv
Two instantiations joined by the internal wire `address`:
`.count (address)` on the counter and `.addr (address)` on the ROM.
The parameters are passed down (`#(.WIDTH(A_WIDTH))`, `#(.ADDRESS_WIDTH(A_WIDTH), ...)`)
so changing `A_WIDTH`/`D_WIDTH` at the top resizes everything.

### sinegen_tb.cpp
- Runs for 1,000,000 cycles; press **q** to quit early.
- Reset is held for the first 2 cycles, `en = 1`.
- **Test-yourself challenge (done):** `top->incr = vbdValue()`, so turning the
  rotary encoder changes the frequency. The encoder starts at 0, and `incr = 0`
  would freeze the counter (flat line), so the testbench uses 1 until you turn it.
- Only the first 10,000 cycles are written to `sinegen.vcd`, so the file stays small.

## Run it

```sh
cd task1
./doit.sh
```

You should see `Loading rom.` in the terminal, the header **L2T1: SigGen** on
Vbuddy, and a sinewave scrolling across the screen. Turn the knob to speed it up.

### What to expect as you turn the knob
| incr | samples per cycle | looks like |
|---|---|---|
| 1 | 256 | smooth, slow sine |
| 16 | 16 | sine made of visible steps |
| 64 | 4 | four points per cycle |
| 128 | 2 | jumps between top and bottom |
| > 128 | – | appears to get **slower** again: aliasing (`256 − k` looks like `k`) |
