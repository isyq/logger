/* ========================================
 *
 * isyq
 * 8/30/2016
 * V1.3.0.1
 *
 * A UART logger component.
 *
 * ========================================
*/

#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <stdarg.h>    
#include "project.h"
#include "cytypes.h"

/* This macro is set corresponding to UART component name. */
#define UART_START         CyUart_Start
#define UART_STOP          CyUart_Stop
#define UART_PUTC(c)       CyUart_UartPutChar(c)
#define UART_PUTS(s)       CyUart_UartPutString(s)   
#define UARTCLK_START      CyUartClk_Start
#define UARTCLK_STOP       CyUartClk_Stop
    
/* Wait TX transaction complete, TX_FIFO_SR_VALID means no byte is sending at the moment. */
#define UART_BUSY          while(0 != (CyUart_SpiUartGetTxBufferSize() + CyUart_GET_TX_FIFO_SR_VALID))

typedef enum {
    UART_BAUD_RATE_110              = 110u,
    UART_BAUD_RATE_300              = 300u,
    UART_BAUD_RATE_1200             = 1200u,
    UART_BAUD_RATE_2400             = 2400u,
    UART_BAUD_RATE_4800             = 4800u,
    UART_BAUD_RATE_9600             = 9600u,
    UART_BAUD_RATE_19200            = 19200u,
    UART_BAUD_RATE_38400            = 38400u,
    UART_BAUD_RATE_57600            = 57600u,
    UART_BAUD_RATE_115200           = 115200u,
    UART_BAUD_RATE_230400           = 230400u,
    UART_BAUD_RATE_460800           = 460800u,
    UART_BAUD_RATE_921600           = 921600u,
} uart_baud_rate_t;

typedef enum {
    LOGGER_LEVEL_DISABLE,
    LOGGER_LEVEL_FATAL,
    LOGGER_LEVEL_ERROR,
    LOGGER_LEVEL_TRACE,
    LOGGER_LEVEL_DEBUG,
} logger_level_t;

typedef struct {
    uart_baud_rate_t   baudRate;
    logger_level_t     level;
    uint16             heapSize;
} logger_config_t;

typedef void (*logger_callback)(const char* format, ...);

typedef struct {
    logger_config_t config;
    logger_callback t;
    logger_callback e;
    logger_callback f;
} logger_t;

/* This is main interface */
extern logger_t     logger;

void logger_start(logger_level_t level, uart_baud_rate_t baudRate);
void logger_array(const uint8* data, uint16 len);

void logger_format(logger_level_t level, const char* file, const char* function, const char* format, ...);
/* This is for debug use */
#define p(...)      logger_format(LOGGER_LEVEL_DEBUG, __FILE__, __FUNCTION__, __VA_ARGS__)


#endif /* __LOG_H__ */
