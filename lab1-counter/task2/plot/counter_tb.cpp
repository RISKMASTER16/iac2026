#include "Vcounter.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include "vbuddy.cpp" // include vbuddy function

int main(int argc, char **argv, char **env){
    int i;
    int clk;

    Verilated::commandArgs(argc, argv);
    Vcounter* top = new Vcounter;           // top points to the counter model

    Verilated::traceEverOn(true);
    VerilatedVcdC* tfp = new VerilatedVcdC;
    top->trace (tfp, 99);                   // trace up to 99 levels of hierarchy
    tfp->open ("counter.vcd");

    // init Vbuddy
    if (vbdOpen()!=1) return(-1);
    vbdHeader("Lab 1: Counter");

    // initialise simulation inputs
    top->clk = 1;
    top->rst = 1;
    top->en = 0;

    // run simulation for many clock cycles
    for(i = 0; i < 300; i++){

        // dump variables into VCD file and toggle clock
        for (clk = 0; clk < 2; clk++){
            tfp->dump (2*i + clk);
            top->clk = !top->clk;
            top->eval ();
        }

        // ++++ Plot count on the TFT instead of the 7-seg displays
        // (one dot per cycle, y scaled between 0 and 255)
        vbdPlot(int(top->count), 0, 255);
        vbdCycle(i+1);
        // ---end of Vbuddy output section

        top->rst = (i<2) | (i==15);
        top->en = vbdFlag();                // button starts/stops counting

        if(Verilated::gotFinish())  exit(0);
    }

    vbdClose();
    tfp->close();
    exit(0);
}
