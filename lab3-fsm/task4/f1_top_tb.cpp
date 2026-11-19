// Testbench for the full F1 starting lights + reaction timer (Task 4).
//
// 1. Press the switch: the lights come on one per second.
// 2. After all 8 are on, they go out after a random delay.
// 3. Press the switch again as fast as you can.
// 4. Your reaction time (ms) appears in the header and on the 7-segment display.
// Pressing while the lights are still on counts as a jump start.
#include "Vf1_top.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include "../vbuddy.cpp"

#include <cstdio>

#define MAX_SIM_CYC 10000000
#define N_1SEC 24           // change to your calibrated value from Task 3

int main(int argc, char **argv, char **env)
{
    int simcyc;
    int tick;
    int prev_lights = 0;
    bool timing = false;    // true from lights-out until the switch is pressed
    char msg[32];

    Verilated::commandArgs(argc, argv);
    Vf1_top *top = new Vf1_top;
    Verilated::traceEverOn(true);
    VerilatedVcdC *tfp = new VerilatedVcdC;
    top->trace(tfp, 99);
    tfp->open("f1_top.vcd");

    if (vbdOpen() != 1) return (-1);
    vbdHeader("L3T4: Press!");
    vbdSetMode(1);          // one-shot: flag is 1 once per press

    top->clk     = 1;
    top->rst     = 1;
    top->trigger = 0;
    top->N       = N_1SEC;

    for (simcyc = 0; simcyc < MAX_SIM_CYC; simcyc++)
    {
        for (tick = 0; tick < 2; tick++)
        {
            if (simcyc < 10000) tfp->dump(2 * simcyc + tick);
            top->clk = !top->clk;
            top->eval();
        }

        int lights = top->data_out & 0xFF;
        vbdBar(lights);
        vbdCycle(simcyc);

        // lights just went out: start the stopwatch
        if (prev_lights == 0xFF && lights == 0) {
            vbdInitWatch();
            timing = true;
        }
        prev_lights = lights;

        top->rst = (simcyc < 2);
        bool pressed = vbdFlag();

        if (pressed && timing) {
            // reaction press: read the stopwatch, don't restart the lights
            int ms = vbdElapsed();
            timing = false;
            std::sprintf(msg, "React: %d ms", ms);
            vbdHeader(msg);
            std::printf("%s\n", msg);
            // show it in decimal on the 7-segment display (up to 9999 ms)
            vbdHex(4, (ms / 1000) % 10);
            vbdHex(3, (ms / 100) % 10);
            vbdHex(2, (ms / 10) % 10);
            vbdHex(1, ms % 10);
            top->trigger = 0;
        }
        else if (pressed && lights != 0) {
            vbdHeader("Jump start!");
            std::printf("Jump start!\n");
            top->trigger = 0;
        }
        else {
            if (pressed) vbdHeader("L3T4: Go...");
            top->trigger = pressed;     // press while idle starts the sequence
        }

        // clktick divisor: knob value once you turn it, otherwise N_1SEC
        int v = vbdValue();
        top->N = (v > 0) ? v : N_1SEC;

        if (Verilated::gotFinish() || vbdGetkey() == 'q')
            break;
    }

    vbdClose();
    tfp->close();
    exit(0);
}
