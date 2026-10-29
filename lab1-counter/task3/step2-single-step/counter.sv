module counter #(
  parameter WIDTH = 8
)(
  // interface signals
  input  logic             clk,      // clock
  input  logic             rst,      // reset
  input  logic             ld,       // step: count up by one when high
  output logic [WIDTH-1:0] count     // count output
);

always_ff @ (posedge clk)
  if (rst) count <= {WIDTH{1'b0}};
  else     count <= ld ? count + {{WIDTH-1{1'b0}},1'b1} : count;
  // ld is select line for a MUX that selects count + 1 or hold
endmodule
