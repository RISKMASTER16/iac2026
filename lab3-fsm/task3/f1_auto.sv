// Task 3 challenge: step through the F1 lights automatically, once per tick.
// clktick produces a one-cycle pulse every N+1 clock cycles. Feeding that
// pulse into the FSM's en input means the FSM moves on one state per tick.
// Calibrate N (Task 3) so that a tick is ~1 second.
module f1_auto #(
    parameter WIDTH = 16
)(
    input  logic             clk,
    input  logic             rst,
    input  logic [WIDTH-1:0] N,          // tick every N+1 clock cycles
    output logic [7:0]       data_out
);

logic tick;                              // interconnect: clktick -> f1_fsm

clktick #(.WIDTH(WIDTH)) oneSecond (
    .clk  (clk),
    .rst  (rst),
    .en   (1'b1),
    .N    (N),
    .tick (tick)
);

f1_fsm lights (
    .clk      (clk),
    .rst      (rst),
    .en       (tick),
    .data_out (data_out)
);

endmodule
