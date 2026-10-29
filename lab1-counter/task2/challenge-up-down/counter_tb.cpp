#include "Vcounter.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include "vbuddy.cpp"     // include vbuddy functions

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

    for(i = 0; i < 1000; i++){

        // dump variables into VCD file and toggle clock
        for (clk = 0; clk < 2; clk++){
            tfp->dump (2*i + clk);
            top->clk = !top->clk;
            top->eval ();
        }

        // ++++ Send count value to Vbuddy
        vbdPlot(int(top->count), 0, 900);
        vbdCycle(i+1);
        // ---end of Vbuddy output section

        top->rst = (i<2);
        top->en = vbdFlag();             // button sets direction: 1 = up, 0 = down

        if(Verilated::gotFinish())  exit(0);
    }

    vbdClose();
    tfp->close();
    exit(0);
}
