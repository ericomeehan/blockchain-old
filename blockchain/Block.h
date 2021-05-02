//
// ==================================
// blockchain
//
// an open blockchain network for anything.
// ==================================
//
// Block.h
//
// Eric Meehan
// 4/10/21
//
//

#ifndef Block_h
#define Block_h

typedef unsigned char byte;


struct Block
{
    struct BlockHeaders
    {
        struct Incidentals
        {
            // A reference to another block in the chain.
            byte previous_hash[64];
            // A timestamp to designate when the block was created (seconds since epoch UTC)
            byte timestamp[32];
        }__attribute__((packed));
        
        struct Credentials
        {
            // The creator's public key.
            byte key[64];
            // A digital signature for the data.
            byte lock[64];
            // A nonce to satisfy the difficulty requirement.
            byte nonce[32];
        }__attribute__((packed));
    }__attribute__((packed));
}__attribute__((packed));

#endif /* Block_h */
