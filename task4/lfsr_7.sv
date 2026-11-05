// 7-bit LFSR, primitive polynomial 1 + X^3 + X^7.
// Feedback is sreg[7] XOR sreg[3], i.e. bits 6 and 2 numbering from 0.
// Cycles through all 127 non-zero values before repeating.
module lfsr_7 (
    input   logic       clk,
    input   logic       rst,
    input   logic       en,
    output  logic [6:0] data_out
);

logic [6:0] sreg;

always_ff @(posedge clk, posedge rst)
    if (rst)     sreg <= 7'b000_0001;
    else if (en) sreg <= {sreg[5:0], sreg[6] ^ sreg[2]};

assign data_out = sreg;

endmodule
