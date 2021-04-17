//
// ==================================
// blockchain
//
// an open blockchain network for anything.
// ==================================
//
// main.c
//
// Eric Meehan
// 4/10/21
//
//

#include <libeom/libeom.h>

#include "../../libeom/Systems/Terminal.h"

#include <stdio.h>
#include <string.h>

#include "Block.h"

void interpreter(const char *command);

int main(int argc, const char *argv[])
{
    terminal(argc, argv, interpreter);
}

void interpreter(const char *command)
{
    printf("%s\n", command);
}
