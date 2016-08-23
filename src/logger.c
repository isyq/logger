/* ========================================
 *
 * isyq
 * 8/23/2016
 * V1.0.0.1
 *
 * A UART logger component.
 *
 * ========================================
*/

#include <stdarg.h>
#include <stdio.h>
#include "cyfitter.h"
#include "logger.h"

#define MAX_LOGGER_BUFFER_SIZE              (128u)

static _Bool            hasLoggerInitialized = 0;
static logger_config_t  loggerConfig;

const char HEX_CHAR_TABLE[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F',};
static char LOGGER_LEVEL_PREAMBLE[] = {'F','E','T'};

int8 uart_setBaudRate(uart_baud_rate_t baudRate)
{
    uint8  uartOverSampling = CyUart_UART_OVS_FACTOR;        /* CyUart_SPI_UART.h */
    uint32 hfClkFreq        = CYDEV_BCLK__SYSCLK__HZ;        /* cyfitter.h */
	
    /* HFCLCK should be multiples of overSampling value */
    if (hfClkFreq % uartOverSampling != 0) {
        CYASSERT(0);
        return -1;          //ERR_UART_OVER_SAMPLING;
    }
    
    /* Divider = IntegerDivider + FractionalDivider  */   
	uint32 integralDivider = (hfClkFreq / uartOverSampling) / (uint32)baudRate;

	/* Fractional divider: it's multiples of 32 */
	uint32 fractionalDivider;
	uint32 n1 = (hfClkFreq / uartOverSampling) % (uint32)baudRate;
	if (n1 == 0) {
		fractionalDivider = 0;
	} else {
        /* Get clock fractional divider. Refer to datasheet of clk component */
		uint32 n2 = (uint32)baudRate / n1;
		fractionalDivider = 32 / n2;
	}

	/* Set UART clock frequency. */
	CyUartClk_SetFractionalDividerRegister((uint16) (integralDivider - 1), fractionalDivider);
    
    return 0;
}
void logger_init(void)
{
    if (!hasLoggerInitialized) { 
        UARTCLK_START();
        
        uart_setBaudRate(loggerConfig.baudRate);
        UART_START();
        
        hasLoggerInitialized = 1;
    }
}

void logger_start(logger_config_t* config)
{
    memcpy(&loggerConfig, config, sizeof(logger_config_t));
    
    /* || loggerConfig.heapSize > 0x5BB */
    if (loggerConfig.heapSize < 0x200) {
        UART_PUTS("ERROR: Heap size is not suitable.\r\n");
        CYASSERT(0);
    }
    
    logger_init();
}

void logger_stop(void)
{
    if (hasLoggerInitialized) {
        UART_STOP();
        UARTCLK_STOP();
        
        hasLoggerInitialized = 0;
    }
}

void logger_byte(logger_data_type_t type, uint8 data)
{
    if (type == LOGGER_DATA_TYPE_DEC) {
        UART_PUTC(data);
    }
    else if (type == LOGGER_DATA_TYPE_HEX) {
        UART_PUTC(HEX_CHAR_TABLE[(data >> 4) & 0x0F]);
        UART_PUTC(HEX_CHAR_TABLE[(data) & 0x0F]);
    }
    else if (type == LOGGER_DATA_TYPE_BIN) {
        int8 i;
        for (i = 7; i >= 0; i--) {
            UART_PUTC((uint8)((data >> i) & 0x01) + '0');
        }
    }
    else {}
    
    UART_BUSY;
}

void logger_array(logger_data_type_t type, const uint8* data, uint8 len, char seperator)
{
    uint8 i;
    for (i = 0; i < len; i++) {
        logger_byte(type, data[i]);
        if (i != len - 1) {
            UART_PUTC(seperator);
        }
    }
}

void logger_raw(logger_level_t level, const char* format, ...)
{
    if (!hasLoggerInitialized) {
        return;   
    }
    
    if (level > loggerConfig.level) {
        return;   
    }
    
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    
    UART_BUSY;
}

void logger_format(logger_level_t level, const char* file, const char* function, const char* format, ...)
{    
    if (!hasLoggerInitialized) {
        return;   
    }
    
    if (level > loggerConfig.level) {
        return;   
    }
    
    /* Print format text. */
    UART_PUTC(LOGGER_LEVEL_PREAMBLE[level]);
    UART_PUTS("|");
    UART_PUTS(file);
    UART_PUTS(">");
    UART_PUTS(function);
    UART_PUTS(": ");
    UART_BUSY;
    
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    
    UART_BUSY;
}


/**
 * Required by printf function. 
 * See SCB datasheet for more information.
 */
int _write(int file, char *ptr, int len)
{
    (void) file;
    int i;
    for (i = 0; i < len; i++) {
        UART_PUTC(*ptr++);
    }
    return len;
}



/* [] END OF FILE */
