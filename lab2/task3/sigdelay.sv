// Audio delay line.
// Each clock cycle, the mic sample is written into the RAM at wr_addr
// (from a counter: 0, 1, 2 ... 511, 0 ...). At the same time we read back
// from rd_addr = wr_addr - offset, i.e. the sample written `offset` cycles
// ago. So delayed_signal is mic_signal delayed by `offset` samples.
module sigdelay #(
    parameter A_WIDTH = 9,
              D_WIDTH = 8
)(
    input  logic               clk,
    input  logic               rst,
    input  logic               wr,              // write enable
    input  logic               rd,              // read enable
    input  logic [A_WIDTH-1:0] offset,          // delay, in samples
    input  logic [D_WIDTH-1:0] mic_signal,      // sample from the microphone
    output logic [D_WIDTH-1:0] delayed_signal   // delayed copy of the input
);

logic [A_WIDTH-1:0] wr_addr;
logic [A_WIDTH-1:0] rd_addr;

assign rd_addr = wr_addr - offset;   // wraps round automatically (mod 512)

counter #(.WIDTH(A_WIDTH)) addrCounter (
    .clk   (clk),
    .rst   (rst),
    .en    (1'b1),
    .incr  ({{(A_WIDTH-1){1'b0}}, 1'b1}),   // always step by 1
    .count (wr_addr)
);

ram2ports #(.ADDRESS_WIDTH(A_WIDTH), .DATA_WIDTH(D_WIDTH)) delayRam (
    .clk     (clk),
    .wr_en   (wr),
    .rd_en   (rd),
    .wr_addr (wr_addr),
    .rd_addr (rd_addr),
    .din     (mic_signal),
    .dout    (delayed_signal)
);

endmodule
