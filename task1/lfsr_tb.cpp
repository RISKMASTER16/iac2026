// Vbuddy testbench for the 4-bit LFSR.
// Each press of the rotary-encoder switch steps the LFSR once.
// The value is shown on the neopixel bar and the 7-segment display.
#include "Vlfsr.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include "../vbuddy.cpp"

#define MAX_SIM_CYC 1000000

int main(int argc, char **argv, char **env)
{
    int simcyc;
    int tick;

    Verilated::commandArgs(argc, argv);
    Vlfsr *top = new Vlfsr;
    Verilated::traceEverOn(true);
    VerilatedVcdC *tfp = new VerilatedVcdC;
    top->trace(tfp, 99);
    tfp->open("lfsr.vcd");

    if (vbdOpen() != 1) return (-1);
    vbdHeader("L3T1: LFSR");
    vbdSetMode(1);              // one-shot: flag is 1 once per press

    top->clk = 1;
    top->rst = 1;
    top->en  = 0;

    for (simcyc = 0; simcyc < MAX_SIM_CYC; simcyc++)
    {
        for (tick = 0; tick < 2; tick++)
        {
            if (simcyc < 10000) tfp->dump(2 * simcyc + tick);
            top->clk = !top->clk;
            top->eval();
        }

        vbdBar(top->data_out & 0xFF);
        vbdHex(1, top->data_out & 0xF);
        vbdCycle(simcyc);

        top->rst = (simcyc < 2);
        top->en  = vbdFlag();

        if (Verilated::gotFinish() || vbdGetkey() == 'q')
            break;
    }

    vbdClose();
    tfp->close();
    exit(0);
}
