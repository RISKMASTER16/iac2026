// Full F1 starting-light machine (Task 4), wired as in images/F1_full.jpg:
//
//   clktick ── tick ──────► MUX(1) ─┐
//   lfsr_7 ── K ─► delay ─► MUX(0) ─┴─ en ─► f1_fsm ──► data_out (neopixels)
//                    ▲                         │
//                    └──────── cmd_delay ───────┤
//   clktick.en ◄──────────────── cmd_seq ──────┘ (also MUX select)
//   trigger (switch) ─────────────────────────► f1_fsm.trigger
module f1_top #(
    parameter N_WIDTH = 16,
              K_WIDTH = 7
)(
    input  logic               clk,
    // rst feeds lfsr_7 (asynchronous reset, as Task 1 requires) and the other
    // modules (synchronous reset). Verilator's SYNCASYNCNET lint flags mixing
    // the two on one net; it is safe here because rst is a clean pulse held
    // for the first two cycles, so we switch that one check off for this line.
    /* verilator lint_off SYNCASYNCNET */
    input  logic               rst,
    /* verilator lint_on SYNCASYNCNET */
    input  logic               trigger,    // switch press starts the sequence
    input  logic [N_WIDTH-1:0] N,          // clktick divisor for ~1 s ticks
    output logic [7:0]         data_out,
    output logic               cmd_seq,
    output logic               cmd_delay
);

logic               tick;       // ~1 s pulse from clktick
logic               time_out;   // pulse at the end of the random delay
logic [K_WIDTH-1:0] K;          // random delay length from the LFSR
logic               fsm_en;     // MUX output

clktick #(.WIDTH(N_WIDTH)) oneSecond (
    .clk  (clk),
    .rst  (rst),
    .en   (cmd_seq),
    .N    (N),
    .tick (tick)
);

lfsr_7 random (
    .clk      (clk),
    .rst      (rst),
    .en       (1'b1),           // free-running, so K depends on when you press
    .data_out (K)
);

delay #(.WIDTH(K_WIDTH)) randomDelay (
    .clk      (clk),
    .rst      (rst),
    .trigger  (cmd_delay),
    .n        (K),
    .time_out (time_out)
);

// the MUX
assign fsm_en = cmd_seq ? tick : time_out;

f1_fsm lights (
    .clk       (clk),
    .rst       (rst),
    .en        (fsm_en),
    .trigger   (trigger),
    .data_out  (data_out),
    .cmd_seq   (cmd_seq),
    .cmd_delay (cmd_delay)
);

endmodule
