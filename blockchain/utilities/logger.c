//
// ==================================
// blockchain
//
// an open blockchain network for anything.
// ==================================
//
// UTIL_logger.c
//
// Eric Meehan
// 5/15/21
//
//

#include "logger.h"

void BLOCKCHAIN_UTIL_logger(FILE *output, char *message)
{
    struct tm *raw_time;
    time_t time_object = time(NULL);
    raw_time = gmtime(&time_object);
    char timestamp[20] = {0};
    sprintf(timestamp, "%d-%02d-%02d %02d:%02d:%02d", raw_time->tm_year + 1900, raw_time->tm_mon + 1, raw_time->tm_mday, raw_time->tm_hour, raw_time->tm_min, raw_time->tm_sec);
    fprintf(output, "%s\t%s", timestamp, message);
}
