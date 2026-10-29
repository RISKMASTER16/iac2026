#include "Vtop.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include "vbuddy.cpp"     // include vbuddy functions

int main(int argc, char **argv, char **env){
    int i;
    int clk;

    Verilated::commandArgs(argc, argv);
    Vtop* top = new Vtop;                   // top points to the top-level model (counter + bin2bcd)

    Verilated::traceEverOn(true);
    VerilatedVcdC* tfp = new VerilatedVcdC;
    top->trace (tfp, 99);                   // trace up to 99 levels of hierarchy
    tfp->open ("top.vcd");

    // init Vbuddy
    if (vbdOpen()!=1) return(-1);
    vbdHeader("Lab 1: BCD");
    // flag left in default TOGGLE mode: press button to switch en on/off

    // initialise simulation inputs
    top->clk = 1;
    top->rst = 1;
    top->en  = 0;

    for(i = 0; i < 1000; i++){

        // dump variables into VCD file and toggle clock
        for (clk = 0; clk < 2; clk++){
            tfp->dump (2*i + clk);
            top->clk = !top->clk;
            top->eval ();
        }

        // ++++ Send BCD value to Vbuddy (3 decimal digits)
        vbdHex(3, (int(top->bcd) >> 8) & 0xF);   // hundreds
        vbdHex(2, (int(top->bcd) >> 4) & 0xF);   // tens
        vbdHex(1,  int(top->bcd)       & 0xF);   // units
        vbdCycle(i+1);
        // ---end of Vbuddy output section

        top->rst = (i<2);        // reset for the first two cycles only
        top->en  = vbdFlag();    // button controls en

        if(Verilated::gotFinish())  break;
    }

    vbdClose();
    tfp->close();
    exit(0);
}
