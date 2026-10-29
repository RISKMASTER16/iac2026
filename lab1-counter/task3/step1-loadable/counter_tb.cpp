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
    vbdSetMode(1); // flag in one-shot mode

    // initialise simulation inputs
    top->clk = 1;
    top->rst = 1;
    top->ld = 0;
    top->v = 0;

    for(i = 0; i < 1000; i++){

        // dump variables into VCD file and toggle clock
        for (clk = 0; clk < 2; clk++){
            tfp->dump (2*i + clk);
            top->clk = !top->clk;
            top->eval ();
        }

        // ++++ Send count value to Vbuddy
        vbdHex(4, (int(top->count) >> 12) & 0xF);
        vbdHex(3, (int(top->count) >> 8) & 0xF);
        vbdHex(2, (int(top->count) >> 4) & 0xF);
        vbdHex(1, int(top->count) & 0xF);
        vbdCycle(i+1);
        // ---end of Vbuddy output section

        top->rst = (i<2);
        top->ld = vbdFlag();    // 1 for one cycle each time you press the button
        top->v  = vbdValue();   // value set by turning the encoder

        if(Verilated::gotFinish())  exit(0);
    }

    vbdClose();
    tfp->close();
    exit(0);
}
