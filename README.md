#  Blockchain

## List of commands:

The blockchain command line application provides access to three blockchain features: the author, publisher, and librarian.  

```
blockchain                          // The binary executable
        [username]                  // The user account that will be utilized in this session (may require a password)        
            author                  // Used for creating blocks
                -f                  // Data will be taken from a file
                    /path/to/file
                -b                  // Data will be taken from the subsequent bytes
                    [raw bytes]
                    
                        OUTPUT: [hex digest of the newly created block]
                    
                    
            publisher               // Used for sharing blocks
                [hex digest of block to be shared]
                    255.255.255.255 // IP to share this block with (optional; can enumerate addresses)
                    
                        OUTPUT: [hex digest of the shared block]
                    
                    
            librarian               // Used for retrieving blocks
                -b                  // Specifies that a block will be identified by name
                    [hex digest of block to be retrieved]
                    
                        OUTPUT: [raw block data]
                    
                -q                  // Specifies that an SQL query will be used
                    [SQL query]
                    
                        OUTPUT: [SQL results]
```

These commands are designed to be issued in tandem through pipes.  For example:

```
blockchain eric librarian -q "SELECT * FROM blocks WHERE previous = $(blockchain eric publisher $(blockchain eric author -b "hello world");"
```

Creates a "hello world" block, shares it with the network, then queries the local database for any blocks that reference it.
