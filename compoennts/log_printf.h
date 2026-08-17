#ifndef _LOG_PRINTF_H
#define _LOG_PRINTF_H

#ifdef __cplusplus /* If C++ - compiler: Use C linkage */
extern "C"
{
#endif

#define LOG_USART_SOURCE 0
#define LOG_RTT_SOURCE 1

#define LOG_PRINTF_SOURCE LOG_RTT_SOURCE

#include "stdio.h"

#if(LOG_PRINTF_SOURCE == LOG_USART_SOURCE)
extern int usart1_printf(char *fmt, ...);
#else
extern int SEGGER_RTT_printf(unsigned BufferIndex, const char * sFormat, ...);
#endif
    
#define RT_DEBUG // 定义该宏以启用 LOG_x
    // #define RT_DEBUG_COLOR

#if(LOG_PRINTF_SOURCE == LOG_USART_SOURCE)
// 串口
#define log_kprintf usart1_printf
#if (DBG_LEVEL >= DBG_LOG)
#define LOG_COM(fmt, ...) usart1_printf(fmt, ##__VA_ARGS__)
#else
#define LOG_COM(...)
#endif

#else
// SEGGER_RTT
#define log_kprintf(fmt, ...)     SEGGER_RTT_printf(0, fmt, ##__VA_ARGS__)
#if (DBG_LEVEL >= DBG_LOG)
#define LOG_COM(fmt, ...)      SEGGER_RTT_printf(0, fmt, ##__VA_ARGS__)
#else
#define LOG_COM(...)
#endif

#endif


/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2016-11-12     Bernard      The first version
 * 2018-05-25     armink       Add simple API, such as LOG_D, LOG_E
 */

/*
 * The macro definitions for debug
 *
 * These macros are defined in static. If you want to use debug macro, you can
 * use as following code:
 *
 * In your C/C++ file, enable/disable DEBUG_ENABLE macro, and then include this
 * header file.
 *
#define DBG_TAG           "MOD_TAG"
#define DBG_LVL           DBG_INFO
#include "log_printf.h"          // must after of DBG_LVL, DBG_TAG or other options

 *
 * Then in your C/C++ file, you can use LOG_X macro to print out logs:
 * LOG_D("this is a debug log!");
 * LOG_E("this is a error log!");
 */

/* the debug log will force enable when RT_DEBUG macro is defined */
#if defined(RT_DEBUG) && !defined(DBG_ENABLE)
#define DBG_ENABLE
#endif

/* it will force output color log when RT_DEBUG_COLOR macro is defined */
#if defined(RT_DEBUG_COLOR) && !defined(DBG_COLOR)
#define DBG_COLOR
#endif

#if defined(RT_USING_ULOG)
/* using ulog compatible with rtdbg  */
#include <ulog.h>
#else

/* DEBUG level */
#define DBG_ERROR 0
#define DBG_WARNING 1
#define DBG_INFO 2
#define DBG_LOG 3

#ifdef DBG_TAG
#ifndef DBG_SECTION_NAME
#define DBG_SECTION_NAME DBG_TAG
#endif
#else
/* compatible with old version */
#ifndef DBG_SECTION_NAME
#define DBG_SECTION_NAME "DBG"
#endif
#endif /* DBG_TAG */

#ifdef DBG_ENABLE

#ifdef DBG_LVL
#ifndef DBG_LEVEL
#define DBG_LEVEL DBG_LVL
#endif
#else
/* compatible with old version */
#ifndef DBG_LEVEL
#define DBG_LEVEL DBG_WARNING
#endif
#endif /* DBG_LVL */

/*
 * The color for terminal (foreground)
 * BLACK    30
 * RED      31
 * GREEN    32
 * YELLOW   33
 * BLUE     34
 * PURPLE   35
 * CYAN     36
 * WHITE    37
 */
#ifdef DBG_COLOR
#define _DBG_COLOR(n) log_kprintf("\033[" #n "m")
#define _DBG_LOG_HDR(lvl_name, color_n) \
    log_kprintf("\033[" #color_n "m[" lvl_name "/" DBG_SECTION_NAME "] ")
#define _DBG_LOG_X_END \
    log_kprintf("\033[0m\n")
#else
#define _DBG_COLOR(n)
#define _DBG_LOG_HDR(lvl_name, color_n) \
    log_kprintf("[" lvl_name "/" DBG_SECTION_NAME "] ")
#define _DBG_LOG_X_END \
    log_kprintf("\r\n")
#endif /* DBG_COLOR */

/*
 * static debug routine
 * NOTE: This is a NOT RECOMMENDED API. Please using LOG_X API.
 *       It will be DISCARDED later. Because it will take up more resources.
 */
#define dbg_log(level, fmt, ...)         \
    if ((level) <= DBG_LEVEL)            \
    {                                    \
        switch (level)                   \
        {                                \
        case DBG_ERROR:                  \
            _DBG_LOG_HDR("E", 31);       \
            break;                       \
        case DBG_WARNING:                \
            _DBG_LOG_HDR("W", 33);       \
            break;                       \
        case DBG_INFO:                   \
            _DBG_LOG_HDR("I", 32);       \
            break;                       \
        case DBG_LOG:                    \
            _DBG_LOG_HDR("D", 0);        \
            break;                       \
        default:                         \
            break;                       \
        }                                \
        log_kprintf(fmt, ##__VA_ARGS__); \
        _DBG_COLOR(0);                   \
    }

#define dbg_here                                      \
    if ((DBG_LEVEL) <= DBG_LOG)                       \
    {                                                 \
        log_kprintf(DBG_SECTION_NAME " Here %s:%d\n", \
                    __FUNCTION__, __LINE__);          \
    }

#define dbg_log_line(lvl, color_n, fmt, ...) \
    do                                       \
    {                                        \
        _DBG_LOG_HDR(lvl, color_n);          \
        log_kprintf(fmt, ##__VA_ARGS__);     \
        _DBG_LOG_X_END;                      \
    } while (0)

// dbg_log_e_line 固定串口输出
#define dbg_log_e_line(lvl, color_n, fmt, ...)      \
    do                                              \
    {                                               \
        LOG_COM("[" lvl "/" DBG_SECTION_NAME "] "); \
        LOG_COM(fmt, ##__VA_ARGS__);                \
        LOG_COM("\r\n");                            \
    } while (0)

#define dbg_raw(...) log_kprintf(__VA_ARGS__);

#else
#define dbg_log(level, fmt, ...)
#define dbg_here
#define dbg_enter
#define dbg_exit
#define dbg_log_line(lvl, color_n, fmt, ...)
#define dbg_raw(...)
#endif /* DBG_ENABLE */

#if (DBG_LEVEL >= DBG_LOG)
#define LOG_D(fmt, ...) dbg_log_line("D", 0, fmt, ##__VA_ARGS__)
#else
#define LOG_D(...)
#endif

#if (DBG_LEVEL >= DBG_INFO)
#define LOG_I(fmt, ...) dbg_log_line("I", 32, fmt, ##__VA_ARGS__)
#else
#define LOG_I(...)
#endif

#if (DBG_LEVEL >= DBG_WARNING)
#define LOG_W(fmt, ...) dbg_log_line("W", 33, fmt, ##__VA_ARGS__)
#else
#define LOG_W(...)
#endif

#if (DBG_LEVEL >= DBG_ERROR)
#define LOG_E(fmt, ...) dbg_log_line("E", 31, fmt, ##__VA_ARGS__);
#else
#define LOG_E(...)
#endif

#define LOG_RAW(...) dbg_raw(__VA_ARGS__)

#endif /* defined(RT_USING_ULOG) && define(DBG_ENABLE) */

#ifdef __cplusplus /* If C++ - compiler: End of C linkage */
}
#endif

#endif
