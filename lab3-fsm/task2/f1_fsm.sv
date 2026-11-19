// Formula 1 starting lights: a Moore FSM with 9 states.
// Each clock cycle that en = 1, move to the next state.
// S0 = all off, S1 = 1 light ... S8 = all 8 lights, then back to S0.
module f1_fsm (
    input   logic       rst,
    input   logic       en,
    input   logic       clk,
    output  logic [7:0] data_out
);

typedef enum {S0, S1, S2, S3, S4, S5, S6, S7, S8} my_state;
my_state current_state, next_state;

// 1. state register
always_ff @(posedge clk)
    if (rst) current_state <= S0;
    else     current_state <= next_state;

// 2. next-state logic
always_comb
    if (!en) next_state = current_state;      // hold when not enabled
    else
        case (current_state)
            S0:      next_state = S1;
            S1:      next_state = S2;
            S2:      next_state = S3;
            S3:      next_state = S4;
            S4:      next_state = S5;
            S5:      next_state = S6;
            S6:      next_state = S7;
            S7:      next_state = S8;
            S8:      next_state = S0;
            default: next_state = S0;
        endcase

// 3. output logic (Moore: depends only on the state)
always_comb
    case (current_state)
        S0:      data_out = 8'b0000_0000;
        S1:      data_out = 8'b0000_0001;
        S2:      data_out = 8'b0000_0011;
        S3:      data_out = 8'b0000_0111;
        S4:      data_out = 8'b0000_1111;
        S5:      data_out = 8'b0001_1111;
        S6:      data_out = 8'b0011_1111;
        S7:      data_out = 8'b0111_1111;
        S8:      data_out = 8'b1111_1111;
        default: data_out = 8'b0000_0000;
    endcase

endmodule
