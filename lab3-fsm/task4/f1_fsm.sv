// Full F1 starting-light FSM (Task 4).
//
//  S0  all off, waiting            -- trigger -->  S1
//  S1..S7  lights come on one by one (cmd_seq = 1, en = 1 s tick)
//  S7  -- en -->  S8
//  S8  all on, start random delay  (cmd_delay = 1 for exactly one cycle)
//  S9  all on, waiting for delay   (en = delay time_out)  -- en -->  S0
//
// cmd_seq selects the MUX in the top level:
//   cmd_seq = 1 -> en comes from clktick (1 s steps while lights come on)
//   cmd_seq = 0 -> en comes from delay.time_out (random wait before lights out)
//
// cmd_delay must be a single-cycle pulse: delay.sv reloads its counter every
// cycle that trigger is high, so holding it high would stop it counting.
module f1_fsm (
    input   logic       rst,
    input   logic       en,
    input   logic       trigger,     // start the sequence (switch press)
    input   logic       clk,
    output  logic [7:0] data_out,
    output  logic       cmd_seq,     // high while the lights are coming on
    output  logic       cmd_delay    // one-cycle pulse to start the random delay
);

typedef enum {S0, S1, S2, S3, S4, S5, S6, S7, S8, S9} my_state;
my_state current_state, next_state;

// state register
always_ff @(posedge clk)
    if (rst) current_state <= S0;
    else     current_state <= next_state;

// next-state logic
always_comb
    case (current_state)
        S0:      next_state = trigger ? S1 : S0;
        S1:      next_state = en ? S2 : S1;
        S2:      next_state = en ? S3 : S2;
        S3:      next_state = en ? S4 : S3;
        S4:      next_state = en ? S5 : S4;
        S5:      next_state = en ? S6 : S5;
        S6:      next_state = en ? S7 : S6;
        S7:      next_state = en ? S8 : S7;
        S8:      next_state = S9;                // one cycle only
        S9:      next_state = en ? S0 : S9;      // en = time_out here
        default: next_state = S0;
    endcase

// output logic (Moore)
always_comb begin
    cmd_seq   = 1'b0;
    cmd_delay = 1'b0;
    case (current_state)
        S0:      data_out = 8'b0000_0000;
        S1:      begin data_out = 8'b0000_0001; cmd_seq = 1'b1; end
        S2:      begin data_out = 8'b0000_0011; cmd_seq = 1'b1; end
        S3:      begin data_out = 8'b0000_0111; cmd_seq = 1'b1; end
        S4:      begin data_out = 8'b0000_1111; cmd_seq = 1'b1; end
        S5:      begin data_out = 8'b0001_1111; cmd_seq = 1'b1; end
        S6:      begin data_out = 8'b0011_1111; cmd_seq = 1'b1; end
        S7:      begin data_out = 8'b0111_1111; cmd_seq = 1'b1; end
        S8:      begin data_out = 8'b1111_1111; cmd_delay = 1'b1; end
        S9:      data_out = 8'b1111_1111;
        default: data_out = 8'b0000_0000;
    endcase
end

endmodule
