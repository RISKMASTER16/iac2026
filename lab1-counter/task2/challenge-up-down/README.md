# Task 2 challenge – `en` controls direction

> Modify your counter and testbench files so that the `en` signal controls the direction of counting: '1' for up and '0' for down, via the `vbdFlag()` function.

## counter.sv

```systemverilog
if (rst) count <= {WIDTH{1'b0}};
else    if(en == 1)     count <= count + {{WIDTH-1{1'b0}}, 1'b1};
        else            count <= count - {{WIDTH-1{1'b0}}, 1'b1};
```

The counter now always counts, and `en` is the select line of a MUX choosing between the +1 and −1 results.

The logbook version used an unsized `1` inside the concatenation. Verilator rejects that with `-Wall` (`WIDTHCONCAT`), so it is written as `1'b1` here.

## Testbench

`top->en = vbdFlag();` sets the direction from the button. The count is plotted with `vbdPlot(int(top->count), 0, 900)` over 1000 cycles. A range of `0, 255` would use the full screen height for an 8-bit counter.

## What happens

- The flag starts at 0, so straight after reset the counter counts **down**: 0 − 1 wraps to 255 (8-bit unsigned), and the plot jumps to the top and slopes down.
- Each button press reverses the direction, giving a zig-zag. Passing 255 or 0 wraps round.
