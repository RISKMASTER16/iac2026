// Vbuddy testbench for the F1 light FSM (Task 2, Step 3).
// vbdFlag() in one-shot mode drives en, so each press of the switch
// moves the FSM on one state. data_out is shown on the neopixel bar.
#include "Vf1_fsm.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include "../vbuddy.cpp"

#define MAX_SIM_CYC 1000000

int main(int argc, char **argv, char **env)
{
    int simcyc;
    int tick;

    Verilated::commandArgs(argc, argv);
    Vf1_fsm *top = new Vf1_fsm;
    Verilated::traceEverOn(true);
    VerilatedVcdC *tfp = new VerilatedVcdC;
    top->trace(tfp, 99);
    tfp->open("f1_fsm.vcd");

    if (vbdOpen() != 1) return (-1);
    vbdHeader("L3T2: F1 FSM");
    vbdSetMode(1);              // one-shot: flag is 1 for one read per press

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
