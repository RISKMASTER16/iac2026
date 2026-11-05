#include "Vsinegen.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include "vbuddy.cpp"     // include vbuddy functions

#define MAX_SIM_CYC 1000000

int main(int argc, char **argv, char **env) {
    int simcyc;     // simulation clock count
    int tick;       // each clk cycle has two ticks for two edges

    Verilated::commandArgs(argc, argv);
    Vsinegen* top = new Vsinegen;
    Verilated::traceEverOn(true);
    VerilatedVcdC* tfp = new VerilatedVcdC;
    top->trace(tfp, 99);
    tfp->open("sinegen.vcd");

    // init Vbuddy
    if (vbdOpen() != 1) return(-1);
    vbdHeader("L2T2: Sin+Cos");

    // initialize simulation inputs
    top->clk    = 1;
    top->rst    = 1;
    top->en     = 1;
    top->incr   = 1;
    top->offset = 0;

    for (simcyc = 0; simcyc < MAX_SIM_CYC; simcyc++) {
        for (tick = 0; tick < 2; tick++) {
            if (simcyc < 10000) tfp->dump(2*simcyc + tick);
            top->clk = !top->clk;
            top->eval();
        }

        top->rst = (simcyc < 2);

        // rotary encoder sets the phase offset (64 -> 90 degrees apart)
        top->offset = vbdValue();

        // plot both waves on the same screen
        vbdPlot(int(top->dout1), 0, 255);
        vbdPlot(int(top->dout2), 0, 255);
        vbdCycle(simcyc);

        if ((Verilated::gotFinish()) || (vbdGetkey() == 'q'))
            break;
    }

    vbdClose();
    tfp->close();
    exit(0);
}
