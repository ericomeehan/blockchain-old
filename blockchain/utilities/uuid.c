//
// ==================================
// blockchain
//
// an open blockchain network for anything.
// ==================================
//
// UTIL_uuid.c
//
// Eric Meehan
// 5/16/21
//
//

#include "uuid.h"

void BLOCKCHAIN_UTIL_gen_uuid(char *uuid)
{
        uuid_t binuuid;
        uuid_generate_random(binuuid);
        uuid_unparse_upper(binuuid, uuid);
}
