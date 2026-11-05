#include "Vsinegen.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include "vbuddy.cpp"     // include vbuddy functions

#define MAX_SIM_CYC 1000000

int main(int argc, char **argv, char **env) {
    int simcyc;     // simulation clock count
    int tick;       // each clk cycle has two ticks for two edges

    Verilated::commandArgs(argc, argv);
    // init top verilog instance
    Vsinegen* top = new Vsinegen;
    // init trace dump
    Verilated::traceEverOn(true);
    VerilatedVcdC* tfp = new VerilatedVcdC;
    top->trace(tfp, 99);
    tfp->open("sinegen.vcd");

    // init Vbuddy
    if (vbdOpen() != 1) return(-1);
    vbdHeader("L2T1: SigGen");

    // initialize simulation inputs
    top->clk  = 1;
    top->rst  = 1;
    top->en   = 1;
    top->incr = 1;

    // run simulation for MAX_SIM_CYC clock cycles
    for (simcyc = 0; simcyc < MAX_SIM_CYC; simcyc++) {
        // dump variables into VCD file and toggle clock
        for (tick = 0; tick < 2; tick++) {
            // only trace the first 10,000 cycles so the .vcd file stays small
            if (simcyc < 10000) tfp->dump(2*simcyc + tick);
            top->clk = !top->clk;
            top->eval();
        }

        top->rst = (simcyc < 2);    // reset for the first two cycles only

        // Test-yourself challenge: rotary encoder sets the frequency.
        // The encoder starts at 0, and incr = 0 would freeze the counter
        // (flat line), so use 1 until the knob is turned.
        int v = vbdValue();
        top->incr = (v > 0) ? v : 1;

        // plot ROM output and print cycle count
        vbdPlot(int(top->dout), 0, 255);
        vbdCycle(simcyc);

        // either simulation finished, or 'q' is pressed
        if ((Verilated::gotFinish()) || (vbdGetkey() == 'q'))
            break;
    }

    vbdClose();
    tfp->close();
    exit(0);
}
