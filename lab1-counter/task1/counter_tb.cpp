#include "Vcounter.h"
#include "verilated.h"
#include "verilated_vcd_c.h"

/* these are libraries that contain the programs
to generate waveforms & have structs for top, tfp */

int main(int argc, char **argv, char **env){
    int i;
    int clk;

    Verilated::commandArgs(argc, argv);
    // init top verilog instance
    Vcounter* top = new Vcounter;
    // init trace dump
    // top is a pointer that points to a Vcounter

    Verilated::traceEverOn(true);
    // always stays true to generate waveforms
    VerilatedVcdC* tfp = new VerilatedVcdC;
    top->trace (tfp, 99);
    // 99 = how many levels of module hierarchy to trace
    tfp->open ("counter.vcd");

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

        top->rst = (i<2) | (i==15);
        // reset before clock cycle 2 gives 2 resets
        // to clear hardware of previous nos
        // reset at 15 has only been introduced for this exercise
        top->en = (i<4);
        // en is high for the first 4 cycles only (count reaches 2, see README)

        if(Verilated::gotFinish())  exit(0);
        // sometimes a program can finish early
        // so we initiate premature exit
    }

    tfp->close();
    exit(0);
}
