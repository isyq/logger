/* ========================================
 *
 * isyq
 * 8/23/2016
 * V1.0.0.0
 *
 * Used to printf LOG information over UART
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
    
/* Wait TX transaction complete, TX_FIFO_SR_VALID 
 * means no byte is sending at the moment.
 */
#define UART_BUSY          while(0 != (CyUart_SpiUartGetTxBufferSize() + CyUart_GET_TX_FIFO_SR_VALID))

    
#define UART_VALID_BAUD_RATE_SIZE       (10u)
    
typedef enum {
    UART_BAUD_RATE_110              = 110u,
    UART_BAUD_RATE_300              = 300u,
    UART_BAUD_RATE_1200             = 1200u,
    UART_BAUD_RATE_2400             = 2400u,
    UART_BAUD_RATE_4800             = 4800u,
    UART_BAUD_RATE_9600             = 9600u,
    UART_BAUD_RATE_115200           = 115200u,
    UART_BAUD_RATE_230400           = 230400u,
    UART_BAUD_RATE_460800           = 460800u,
    UART_BAUD_RATE_921600           = 921600u,
} uart_baud_rate_t;

typedef enum {
    LOGGER_DATA_TYPE_BIN,
    LOGGER_DATA_TYPE_DEC,
    LOGGER_DATA_TYPE_HEX,
} logger_data_type_t;


typedef enum {
    LOGGER_LEVEL_FATAL,
    LOGGER_LEVEL_ERROR,
    LOGGER_LEVEL_TRACE,
} logger_level_t;

typedef struct {
    uart_baud_rate_t   baudRate;
    logger_level_t     level;
    uint16             heapSize;
} logger_config_t;

/* Set global log level */
#define DEFAULT_LOGGER_LEVEL        LOGGER_LEVEL_TRACE

void logger_start(logger_config_t* config);
void logger_byte(logger_data_type_t type, uint8 data);
void logger_array(logger_data_type_t type, const uint8* data, uint8 len, char seperator);
void logger_raw(logger_level_t level, const char* format, ...);
void logger_format(logger_level_t level, const char* file, const char* function, const char* format, ...);


/* Interfaces to print user message. */
#define logger_p(...)       logger_raw(LOGGER_LEVEL_TRACE, __VA_ARGS__)

#define logger_trace(...)    logger_format(LOGGER_LEVEL_TRACE, __FILE__, __FUNCTION__, __VA_ARGS__)
#define logger_error(...)    logger_format(LOGGER_LEVEL_ERROR, __FILE__, __FUNCTION__, __VA_ARGS__)
#define logger_fatal(...)    logger_format(LOGGER_LEVEL_FATAL, __FILE__, __FUNCTION__, __VA_ARGS__)


#endif /* __LOG_H__ */
