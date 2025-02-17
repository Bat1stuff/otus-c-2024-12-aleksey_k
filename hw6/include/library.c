#include "library.h"

#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_BACKTRACE_LINES 100
#define DATE_TIME_LENGTH 100

static char * logFilePath = NULL;

void loggerInit(const char * filePath)
{
    if (logFilePath != NULL) {
        free(logFilePath);
        logFilePath = NULL;
    }

    if (filePath != NULL) {
        logFilePath = strdup(filePath);
        if (logFilePath == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
    }
}

void loggerClear(void)
{
    if (logFilePath != NULL) {
        free(logFilePath);
        logFilePath = NULL;
    }
}

static void printStacktrace(FILE *logFile)
{
    void *buffer[MAX_BACKTRACE_LINES];
    const int nPtrs = backtrace(buffer, MAX_BACKTRACE_LINES);
    char **strings = backtrace_symbols(buffer, nPtrs);
    if(strings == NULL) {
        perror("backtrace_symbols returned NULL");
        exit(EXIT_FAILURE);
    }

    fprintf(logFile, "\nStack trace:\n");
    for (int i = 0; i < nPtrs; i++) {
        fprintf(logFile, "%s\n", strings[i]);  // Print each symbol
    }
    fprintf(logFile, "------------------------------------------------------------------------------------------------------------------------\n");
    free(strings);
}

char * levelAsStr(LogLevel level)
{
    switch (level) {
        case LEVEL_DEBUG:
            return "DEBUG";
        case LEVEL_INFO:
            return "INFO";
        case LEVEL_WARNING:
            return "WARNING";
        case LEVEL_ERROR:
            return "ERROR";
        case LEVEL_CRITICAL:
            return "CRITICAL";

        default: return "UNKNOWN";
    }
}

void currentDateTimeStr(char * buffer)
{
    time_t rawTime;
    time(&rawTime);
    const struct tm *timeInfo = localtime(&rawTime);
    strftime(buffer, DATE_TIME_LENGTH, "%Y-%m-%d %H:%M:%S", timeInfo);
}

void write2log(const LogLevel level, char *message, char *filePath, const int line)
{
    char dateTimeStr[DATE_TIME_LENGTH];
    FILE *logFile = fopen(logFilePath, "a");
    if (logFile == NULL) {
        perror("error opening log file");
        exit(EXIT_FAILURE);
    }
    currentDateTimeStr(dateTimeStr);
    fprintf(logFile, "%s %s %s:%d \"%s\"", dateTimeStr, levelAsStr(level), filePath, line, message);
    if (level == LEVEL_ERROR || level == LEVEL_CRITICAL) {
        printStacktrace(logFile);
    }
    fprintf(logFile, "\n");
    fclose(logFile);
}
