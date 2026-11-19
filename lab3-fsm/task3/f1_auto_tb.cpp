// Testbench for the Task 3 challenge: F1 lights step automatically every ~1 s.
// N comes from the rotary encoder (the value you found when calibrating
// clktick against a 60 bpm metronome). Until you turn the knob, N_1SEC is used.
#include "Vf1_auto.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include "../vbuddy.cpp"

#define MAX_SIM_CYC 1000000
#define N_1SEC 24           // change to your calibrated value

int main(int argc, char **argv, char **env)
{
    int simcyc;
    int tick;

    Verilated::commandArgs(argc, argv);
    Vf1_auto *top = new Vf1_auto;
    Verilated::traceEverOn(true);
    VerilatedVcdC *tfp = new VerilatedVcdC;
    top->trace(tfp, 99);
    tfp->open("f1_auto.vcd");

    if (vbdOpen() != 1) return (-1);
    vbdHeader("L3T3: F1 auto");

    top->clk = 1;
    top->rst = 1;
    top->N   = N_1SEC;

    for (simcyc = 0; simcyc < MAX_SIM_CYC; simcyc++)
    {
        for (tick = 0; tick < 2; tick++)
        {
            if (simcyc < 10000) tfp->dump(2 * simcyc + tick);
            top->clk = !top->clk;
            top->eval();
        }

        vbdBar(top->data_out & 0xFF);
        vbdCycle(simcyc);

        top->rst = (simcyc < 2);
        int v = vbdValue();
        top->N = (v > 0) ? v : N_1SEC;

        if (Verilated::gotFinish() || vbdGetkey() == 'q')
            break;
    }

    vbdClose();
    tfp->close();
    exit(0);
}
