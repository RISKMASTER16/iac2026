# Task 2 – Formula 1 light sequence (FSM)

A 9-state Moore machine. Each clock cycle with `en = 1` moves to the next state,
and each state lights one more LED.

```
S0 ─en→ S1 ─en→ S2 ─en→ … ─en→ S8 ─en→ S0
00      01      03            FF      00
```

## Files

| File | What it is |
|---|---|
| `f1_fsm.sv` | The FSM |
| `verify.sh`, `verify.cpp` | GTest checks (provided) |
| `f1_fsm_tb.cpp`, `doit.sh` | Vbuddy testbench: each switch press steps one state |

## How it works

An FSM in SystemVerilog has three parts:

1. **State register** (`always_ff`): `current_state <= next_state` on each clock,
   or `S0` on reset.
2. **Next-state logic** (`always_comb`): if `en` is 0, stay put; otherwise go to
   the next state in the `case`.
3. **Output logic** (`always_comb`): a `case` on `current_state` that sets
   `data_out`. It depends only on the state, so this is a **Moore** machine.

`typedef enum {S0, ..., S8} my_state;` gives the states names instead of numbers.
Every `case` has a `default` so the combinational blocks always assign a value
(otherwise you get a latch).

## Vbuddy testbench

```cpp
vbdSetMode(1);              // one-shot mode
top->en = vbdFlag();        // 1 for one cycle per press
vbdBar(top->data_out & 0xFF);
```

In **one-shot mode** `vbdFlag()` returns 1 only once per press, so the FSM moves
exactly one state per press. (In the default toggle mode it would stay 1 and the
FSM would race round every cycle.)

## Run it

```sh
cd task2
./verify.sh        # expect: [  PASSED  ] 2 tests.
./doit.sh          # press the switch to step through the lights
```
