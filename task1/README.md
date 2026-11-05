# Task 1 – LFSR and pseudo-random binary sequence

A **linear feedback shift register** shifts its bits along each clock and
feeds an XOR of two bits back into the empty end. With the right taps (a
*primitive polynomial*) it visits every non-zero value once before repeating,
which looks random but is completely predictable.

## Files

| File | What it is |
|---|---|
| `lfsr.sv` | 4-bit LFSR, polynomial 1 + X³ + X⁴ (15-value cycle) |
| `lfsr_7.sv` | Challenge: 7-bit LFSR, polynomial 1 + X³ + X⁷ (127-value cycle) |
| `verify.sh`, `verify.cpp` | GTest checks for `lfsr.sv` (provided) |
| `verify_7.sh`, `verify_7.cpp` | GTest checks for `lfsr_7.sv` (provided) |
| `lfsr_tb.cpp`, `doit.sh` | Vbuddy demo: press the switch to step the LFSR |

## How it works

```systemverilog
always_ff @(posedge clk, posedge rst)
    if (rst)     sreg <= 4'b0001;
    else if (en) sreg <= {sreg[2:0], sreg[3] ^ sreg[2]};
```

- `{sreg[2:0], new_bit}` shifts everything left one place; the new bit goes in at bit 0.
- The new bit is `sreg[3] ^ sreg[2]`. The slides number the bits 1–4, so this is
  "sreg4 XOR sreg3" from the diagram.
- **Asynchronous reset:** `posedge rst` is in the sensitivity list, so reset
  happens the moment `rst` goes high, not at the next clock edge.
- It resets to **1, not 0**. All-zeros is a dead state: 0 XOR 0 = 0, so it would
  stay at 0 forever.

Sequence: `1 → 2 → 4 → 9 → 3 → 6 → D → A → 5 → B → 7 → F → E → C → 8 → 1 …`

**7-bit version:** the polynomial 1 + X³ + X⁷ means taps at bits 7 and 3
(bits 6 and 2 numbering from 0):
```systemverilog
sreg <= {sreg[5:0], sreg[6] ^ sreg[2]};
```

## Run it

```sh
cd task1
./verify.sh        # expect: [  PASSED  ] 3 tests.
./verify_7.sh      # expect: [  PASSED  ] 2 tests.
./doit.sh          # Vbuddy demo (needs your vbuddy.cfg in this folder)
```

The verify scripts are the provided ones, with one change: they now pick the
right GTest flags on Linux/WSL as well as macOS (the originals only used Homebrew).
