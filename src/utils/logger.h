/******************************************************************************
 * Project: Klic Smart Irrigation System
 *
 * Description:
 * Logger implementation to print to a `Print` object using several logger
 * levels
 *
 * License:
 *    This project is open for use, modification, and distribution for personal
 *    or educational purposes only. Commercial use is strictly prohibited
 *    without explicit permission from the author
 ******************************************************************************/
#pragma once

#include <Print.h>

typedef enum
{
    // Log at debug level
    LOG_DEBUG,
    // Log at info level
    LOG_INFO,
    // Log at warning level
    LOG_WARN,
    // Log at error level
    LOG_ERROR,
    // Log at any level
    LOG_MASTER
} eLogLevel_t;

typedef enum
{
    EndLine
} LoggerSpecialChar_t;

typedef enum
{
    LOGGER_TEXT_BOLD,
    LOGGER_TEXT_YELLOW,
    LOGGER_TEXT_RED,
    LOGGER_TEXT_GREEN,
    LOGGER_TEXT_RESET
} LoggerFormatText_e;

typedef enum
{
    INT_BIN = 2,
    INT_OCT = 8,
    INT_DEC = 10,
    INT_HEX = 16
} LoggerIntBase_e;

const char *GetStringLogLevel(eLogLevel_t);

class SysLogger : public Print
{
    eLogLevel_t log_lvl;
    Print *cout;
    bool canLog = false;
    LoggerIntBase_e intBase;

public:
    SysLogger(Print *printOut)
        : cout(printOut), intBase(INT_DEC)
    {
    }

    void setLogOutput (Print *printOut)
    {
        this->cout = printOut;
    }

    size_t write(const uint8_t *buffer, size_t size);
    size_t write(uint8_t _byte);

    eLogLevel_t logLevel() const { return this->log_lvl; }
    void setLogLevel(const eLogLevel_t &_lvl) { this->log_lvl = _lvl; }

    SysLogger &operator<<(LoggerIntBase_e);
    SysLogger &operator<<(LoggerFormatText_e);
    SysLogger &operator<<(LoggerSpecialChar_t);
    SysLogger &operator<<(eLogLevel_t);
    SysLogger &operator<<(const __FlashStringHelper *);
    SysLogger &operator<<(const String &);
    SysLogger &operator<<(const char[]);
    SysLogger &operator<<(char);
    SysLogger &operator<<(unsigned char);
    SysLogger &operator<<(int);
    SysLogger &operator<<(unsigned int);
    SysLogger &operator<<(long);
    SysLogger &operator<<(unsigned long);
    SysLogger &operator<<(double);
    SysLogger &operator<<(const Printable &);

    void log(const __FlashStringHelper *, eLogLevel_t);
    void log(const String &, eLogLevel_t);
    void log(const char[], eLogLevel_t);
    void log(char, eLogLevel_t);
    void log(unsigned char, eLogLevel_t, int = DEC);
    void log(int, eLogLevel_t, int = DEC);
    void log(unsigned int, eLogLevel_t, int = DEC);
    void log(long, eLogLevel_t, int = DEC);
    void log(unsigned long, eLogLevel_t, int = DEC);
    void log(double, eLogLevel_t, int = 2);
    void log(const Printable &, eLogLevel_t);
};

extern SysLogger logger;
