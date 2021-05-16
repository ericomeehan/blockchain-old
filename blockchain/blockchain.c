//
// ==================================
// blockchain
//
// an open blockchain network for anything.
// ==================================
//
// blockchain.c
//
// Eric Meehan
// 5/12/21
//
//

#include "blockchain.h"

#include "interfaces/cli.h"


int main(int argc, const char **argv)
{
    if (isatty(fileno(stdin)))
    {
        return BLOCKCHAIN_cli(argc, argv);
    }
}
