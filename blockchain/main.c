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
#include "../../libeom/Interfaces/Terminal.h"

#include <stdio.h>
#include <string.h>

#include "Block.h"

static struct Account user = NULL;

int interpreter(int argc, const char **argv);

int main(int argc, const char **argv)
{
    if (!user)
    {
        user = 
    }
}

int interpreter(int argc, const char **argv)
{
    for (int i = 0; i < argc; i++)
    {
        printf("%s\n", argv[i]);
    }
    return 0;
}
