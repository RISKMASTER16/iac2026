# Task 3 – Capture and delay an audio signal (dual-port RAM)

The Vbuddy microphone sample is written into a RAM every clock cycle, and an
older sample is read back at the same time. The read-back signal is a
delayed copy of the input, and the knob sets how much delay.

```
mic_signal ─────────────────────────► din
                                        ram2ports (512 x 8) ──► dout = delayed_signal
counter ──wr_addr──────────────────► wr_addr
          └─► (− offset) ─ rd_addr ─► rd_addr
```

## Files

| File | What it is |
|---|---|
| `ram2ports.sv` | **New.** 512 x 8 dual-port RAM: one write port, one read port |
| `sigdelay.sv` | **New.** Top level: counter → write address; read address = write address − offset |
| `counter.sv` | Same as Task 1 (here 9 bits wide, `incr` tied to 1) |
| `sigdelay_tb.cpp` | Provided by the lab (unchanged) |
| `doit.sh` | Builds `sigdelay` |
| `vbuddy.cpp` | Provided |

Copy your `vbuddy.cfg` into this folder too.

## How it works

### RAM vs ROM
A ROM is only read. A RAM can also be written:
```systemverilog
always_ff @(posedge clk) begin
    if (wr_en) ram_array[wr_addr] <= din;    // write port
    if (rd_en) dout <= ram_array[rd_addr];   // read port
end
```
Because the two ports have separate addresses, you can write one location
and read a different one in the same clock cycle.

### Why 9 bits
512 locations need 9 address bits (2⁹ = 512). The testbench uses
`ADDRESS_WIDTH 9`, so `sigdelay` uses `A_WIDTH = 9` for the counter, the
addresses and `offset`.

### The delay
```systemverilog
assign rd_addr = wr_addr - offset;
```
The counter writes to 0, 1, 2 … 511, 0, 1 … (a circular buffer).
Reading `offset` locations behind the write pointer gives the sample written
`offset` cycles ago, so `delayed_signal` is `mic_signal` delayed by `offset`
samples. Subtraction wraps round mod 512 automatically, so the maximum delay is
511 samples.

### Port names
The testbench (provided) uses `wr`, `rd`, `offset`, `mic_signal` and
`delayed_signal`, so `sigdelay.sv` uses exactly those port names. Verilator
creates C++ variables with the same names as the top-level ports.

## Run it

```sh
cd task3
./doit.sh
```

Play a tone (e.g. https://onlinetonegenerator.com) near the Vbuddy. You'll see
two traces: the live mic signal and a shifted copy. Turning the knob changes the
shift.
