# Task 2 – Sine and cosine (dual-port ROM)

Two waves at the same frequency, with a phase difference you set with the
rotary encoder.

```
                  addr1 ─────────────────► port 1 ──► dout1
incr ─► counter ──┤                       rom2ports
                  └─► (+ offset) ─ addr2 ► port 2 ──► dout2
```

## Files

| File | What changed from Task 1 |
|---|---|
| `counter.sv` | Same as Task 1 |
| `rom2ports.sv` | **New.** Same ROM contents, but two address inputs and two outputs |
| `sinegen.sv` | Adds `offset` input; `addr2 = addr1 + offset`; outputs `dout1`, `dout2` |
| `sinegen_tb.cpp` | `offset = vbdValue()`, plots `dout1` then `dout2` |
| `doit.sh` | Builds with `rom2ports.sv` instead of `rom.sv` |
| `sinerom.mem`, `sinegen.py`, `vbuddy.cpp` | Same as Task 1 |

Copy your `vbuddy.cfg` into this folder too.

## How it works

### rom2ports.sv
A dual-port ROM is just one array read in two places in the same clock edge:
```systemverilog
always_ff @(posedge clk) begin
    dout1 <= rom_array[addr1];
    dout2 <= rom_array[addr2];
end
```

### The phase offset
```systemverilog
assign addr2 = addr1 + offset;
```
Port 2 always reads `offset` samples **ahead** of port 1 on the same wave.
`addr2` is 8 bits, so it wraps round past 255 automatically.

Since one full cycle is 256 samples:

| offset | phase difference |
|---|---|
| 0 | 0° (the two waves overlap) |
| 64 | 90° (sine and cosine: "quadrature" signals) |
| 128 | 180° (one is the other upside down) |

The phase difference is `offset × 360° / 256` and doesn't depend on `incr`.

## Run it

```sh
cd task2
./doit.sh
```

Turn the knob to 64 to see the two waves exactly a quarter cycle apart.
