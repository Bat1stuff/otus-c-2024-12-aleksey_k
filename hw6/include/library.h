#ifndef LOGGER_LIBRARY_H
#define LOGGER_LIBRARY_H

typedef enum {
  LEVEL_DEBUG,
  LEVEL_INFO,
  LEVEL_WARNING,
  LEVEL_ERROR,
  LEVEL_CRITICAL
} LogLevel;

#define LOG_DEBUG(X) write2log(LEVEL_DEBUG, X, __FILE__, __LINE__)
#define LOG_INFO(X) write2log(LEVEL_INFO, X, __FILE__, __LINE__)
#define LOG_WARNING(X) write2log(LEVEL_WARNING, X, __FILE__, __LINE__)
#define LOG_ERROR(X) write2log(LEVEL_ERROR, X, __FILE__, __LINE__)
#define LOG_CRITICAL(X) write2log(LEVEL_CRITICAL, X, __FILE__, __LINE__)

void loggerInit(const char * filePath);
void loggerClear(void);
void write2log(LogLevel level, char *message, char *filePath, int line);

#endif // LOGGER_LIBRARY_H