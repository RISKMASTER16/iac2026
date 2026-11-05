// Two sinewaves at the same frequency, with a phase offset between them.
// Port 1 reads the ROM at the counter address; port 2 reads it at
// (counter address + offset). offset = 64 out of 256 is a quarter cycle,
// i.e. 90 degrees, giving sine and cosine ("quadrature") signals.
module sinegen #(
    parameter A_WIDTH = 8,
              D_WIDTH = 8
)(
    input  logic               clk,      // clock
    input  logic               rst,      // reset
    input  logic               en,       // enable
    input  logic [D_WIDTH-1:0] incr,     // increment for addr counter
    input  logic [A_WIDTH-1:0] offset,   // phase offset between the two waves
    output logic [D_WIDTH-1:0] dout1,    // wave 1
    output logic [D_WIDTH-1:0] dout2     // wave 2 (phase shifted)
);

logic [A_WIDTH-1:0] addr1;     // from the counter
logic [A_WIDTH-1:0] addr2;     // addr1 + offset (wraps round automatically)

assign addr2 = addr1 + offset;

counter #(.WIDTH(A_WIDTH)) addrCounter (
    .clk   (clk),
    .rst   (rst),
    .en    (en),
    .incr  (incr),
    .count (addr1)
);

rom2ports #(.ADDRESS_WIDTH(A_WIDTH), .DATA_WIDTH(D_WIDTH)) sineRom (
    .clk   (clk),
    .addr1 (addr1),
    .addr2 (addr2),
    .dout1 (dout1),
    .dout2 (dout2)
);

endmodule
