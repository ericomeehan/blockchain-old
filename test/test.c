//
// ==================================
// blockchain
//
// an open blockchain network for anything.
// ==================================
//
// test.c
//
// Eric Meehan
// 5/14/21
//
//


#include "../blockchain/blockchain.h"
#include <stdio.h>

static struct Queue trials;

static void init(void);
void init(void)
{
    int num_tests = 0;
    bool(*)(void)tests_to_run[0] = {};
    for (int i = 0; i < num_tests; i++)
    {
        trials.push(&trials, tests_to_run[i]);
    }
}

static int main(int argc, char **argv)
{
    init();
    double num_trials = 0;
    double num_successes = 0;
    
    bool (*trial)(void);
    while ((trial = trials.peek(&trials)))
    {
        num_trials += 1;
        num_successes += trial();
        trials.pop(&trials);
    }
    
    printf("Test completed with a %.2f%% success rate\n", num_successes / num_trials);
}
