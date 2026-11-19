// 4-bit LFSR, primitive polynomial 1 + X^3 + X^4.
// Shifts left each enabled clock; the new bit 0 is sreg[4] XOR sreg[3]
// (bits 3 and 2 here, since we number from 0).
// Cycles through all 15 non-zero values before repeating.
module lfsr (
    input   logic       clk,
    input   logic       rst,
    input   logic       en,
    output  logic [3:0] data_out
);

logic [3:0] sreg;

// asynchronous reset: rst is in the sensitivity list, so it takes effect
// immediately rather than waiting for the next clock edge
always_ff @(posedge clk, posedge rst)
    if (rst)     sreg <= 4'b0001;               // reset to 1, never 0 (0 is a dead state)
    else if (en) sreg <= {sreg[2:0], sreg[3] ^ sreg[2]};

assign data_out = sreg;

endmodule
