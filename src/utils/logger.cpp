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
#include "logger.h"
#include "core.h"
#include <Arduino.h>

// Local macro for prefix debug msg
#define LOG_DEBUG_PREFIX_MSG "[\033[1m\033[35mDEBUG\033[0m]: "
// Local macro for prefix info msg
#define LOG_INFO_PREFIX_MSG "[\033[1m\033[32mINFO \033[0m]: "
// Local macro for prefix warn msg
#define LOG_WARN_PREFIX_MSG "[\033[1m\033[93mWARN \033[0m]: "
// Local macro for prefix error msg
#define LOG_ERROR_PREFIX_MSG "[\033[1m\033[91mERROR\033[0m]: "

const char *LogLevelStrMap[] = {
    LOG_DEBUG_PREFIX_MSG,
    LOG_INFO_PREFIX_MSG,
    LOG_WARN_PREFIX_MSG,
    LOG_ERROR_PREFIX_MSG,
    "[     ]: "
};

const char *ColorAnsiCodeStr[] = {
    "\033[1m",
    "\033[93m",
    "\033[91m",
    "\033[32m",
    "\033[0m"
};

const char *GetStringLogLevel(eLogLevel_t log_lv)
{
    if (log_lv >= ARRAY_SIZE(LogLevelStrMap)) {
        return "[UNKNO]: ";
    }
    return LogLevelStrMap[(uint8_t)log_lv];
}

size_t SysLogger::write(const uint8_t *buffer, size_t size)
{
    if (this->cout != nullptr)
        return this->cout->write(buffer, size);
    return 0;
}

size_t SysLogger::write(uint8_t _byte)
{
    if (this->cout != nullptr)
        return this->cout->write(_byte);
    return 0;
}

SysLogger &SysLogger::operator<<(LoggerIntBase_e base)
{
    this->intBase = base;
    return *this;
}

SysLogger &SysLogger::operator<<(LoggerFormatText_e ansiCode)
{
    if (ansiCode < ARRAY_SIZE(ColorAnsiCodeStr)) {
        (*this) << ColorAnsiCodeStr[ansiCode];
    }
    return *this;
}

SysLogger &SysLogger::operator<<(LoggerSpecialChar_t specialChar)
{
    switch (specialChar)
    {
    case EndLine:
        (*this) << LOGGER_TEXT_RESET << F("\n\r");
        break;
    }
    return *this;
}

SysLogger &SysLogger::operator<<(eLogLevel_t lvl)
{
    this->canLog = lvl >= this->log_lvl;
    (*this) << LOGGER_TEXT_RESET << GetStringLogLevel(lvl);
    return *this;
}

SysLogger &SysLogger::operator<<(const __FlashStringHelper *msg)
{
    if (canLog && this->cout != nullptr)
        this->cout->print(msg);
    return *this;
}

SysLogger &SysLogger::operator<<(const String &msg)
{
    if (canLog && this->cout != nullptr)
        this->cout->print(msg);
    return *this;
}

SysLogger &SysLogger::operator<<(const char msg[])
{
    if (canLog && this->cout != nullptr)
        this->cout->print(msg);
    return *this;
}

SysLogger &SysLogger::operator<<(char msg)
{
    if (canLog && this->cout != nullptr)
        this->cout->print(msg);
    return *this;
}

SysLogger &SysLogger::operator<<(unsigned char msg)
{
    if (canLog && this->cout != nullptr)
        this->cout->print(msg, (uint8_t)intBase);
    return *this;
}

SysLogger &SysLogger::operator<<(int msg)
{
    if (canLog && this->cout != nullptr)
        this->cout->print(msg);
    return *this;
}

SysLogger &SysLogger::operator<<(unsigned int msg)
{
    if (canLog && this->cout != nullptr)
        this->cout->print(msg);
    return *this;
}

SysLogger &SysLogger::operator<<(long msg)
{
    if (canLog && this->cout != nullptr)
        this->cout->print(msg);
    return *this;
}

SysLogger &SysLogger::operator<<(unsigned long msg)
{
    if (canLog && this->cout != nullptr)
        this->cout->print(msg);
    return *this;
}

SysLogger &SysLogger::operator<<(double msg)
{
    if (canLog && this->cout != nullptr)
        this->cout->print(msg);
    return *this;
}

SysLogger &SysLogger::operator<<(const Printable &msg)
{
    if (canLog && this->cout != nullptr)
        this->cout->print(msg);
    return *this;
}

void SysLogger::log(const __FlashStringHelper *msg, eLogLevel_t lvl)
{
    (*this) << lvl << msg << EndLine;
}

void SysLogger::log(const String &msg, eLogLevel_t lvl)
{
    (*this) << lvl << msg << EndLine;
}

void SysLogger::log(const char msg[], eLogLevel_t lvl)
{
    (*this) << lvl << msg << EndLine;
}

void SysLogger::log(char msg, eLogLevel_t lvl)
{
    (*this) << lvl << msg << EndLine;
}

void SysLogger::log(unsigned char msg, eLogLevel_t lvl, int base)
{
    (*this) << lvl << msg << EndLine;
}

void SysLogger::log(int msg, eLogLevel_t lvl, int base)
{
    (*this) << lvl << msg << EndLine;
}

void SysLogger::log(unsigned int msg, eLogLevel_t lvl, int base)
{
    (*this) << lvl << msg << EndLine;
}

void SysLogger::log(long msg, eLogLevel_t lvl, int base)
{
    (*this) << lvl << msg << EndLine;
}

void SysLogger::log(unsigned long msg, eLogLevel_t lvl, int base)
{
    (*this) << lvl << msg << EndLine;
}

void SysLogger::log(double msg, eLogLevel_t lvl, int precision)
{
    (*this) << lvl << msg << EndLine;
}

void SysLogger::log(const Printable &msg, eLogLevel_t lvl)
{
    (*this) << lvl << msg << EndLine;
}
