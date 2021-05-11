//
// ==================================
// blockchain
//
// an open blockchain network for anything.
// ==================================
//
// Utilities.h
//
// Eric Meehan
// 5/10/21
//
//

/*
 Some generic utilities that are useful throughout the project, but don't belong anywhere on their own.
 */

#ifndef Utilities_h
#define Utilities_h

#include "../main.h"

#include "../accounts/Account.h"

// MARK: FUNCTION PROTOTYPES

bool initialize(void);
bool connect_to_database(void);
bool create_database_tables(void);
bool user_is_active(Account *user);

#endif /* Utilities_h */
