/* ========================================
 *
 * isyq
 * 8/23/2016
 * V1.3.0.0
 *
 * A UART logger component.
 *
 * ========================================
*/

#include <stdarg.h>
#include <stdio.h>
#include "cyfitter.h"
#include "logger.h"

/* Max buffer length */
#define MAX_LOGGER_BUFFER_SIZE              (128u)

static _Bool            hasLoggerInitialized;   /* Ensure logger is initialized only once */
static logger_config_t  loggerConfig;           /* Global config variable */

/* Function declaration */
void logger_start(logger_level_t level, uart_baud_rate_t baudRate);
void logger_fatal(const char* format, ...);
void logger_error(const char* format, ...);
void logger_trace(const char* format, ...);
void logger_debug(const char* format, ...);
void logger_array(const uint8* data, uint16 len);

logger_t    logger = {
    .config   = &loggerConfig,
    .t        = logger_trace,
    .e        = logger_error,
    .f        = logger_fatal,
    .d        = logger_debug,
	.start    = logger_start,
	.list     = logger_array,
};                             /* Global logger */

/* Nibble of a char */
const char HEX_CHAR_TABLE[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F',};

/*******************************************************************************
* Function Name: logger_array
********************************************************************************
*
* Summary:
*  Print array data. It's ended with \r\n.
*
* Parameters:
*  data: pointer of data
*  len: length of data
*
* Returns:
*  None
*
*
*******************************************************************************/
void logger_array(const uint8* data, uint16 len)
{
    if (!hasLoggerInitialized) {
        return;   
    }
    
    if (LOGGER_LEVEL_TRACE > loggerConfig.level) {
        return;   
    }    
    
    if (data == NULL || len <= 0) {
        return;   
    }
    
    char seperator = ' ';
    
    uint16 i;
    for (i = 0; i < len; i++) {
        UART_PUTC(HEX_CHAR_TABLE[(data[i] >> 4) & 0x0F]);
        UART_PUTC(HEX_CHAR_TABLE[(data[i]) & 0x0F]);
        if (i != len - 1) {
            UART_PUTC(seperator);
        }
        UART_BUSY;                  /* Prevent being stuck in some cases */
    }

    UART_PUTC('\r');
    UART_PUTC('\n');

    /* Make sure all data in the buffer is sent */
    UART_BUSY;
}

/*******************************************************************************
* Function Name: logger_raw
********************************************************************************
*
* Summary:
*  Print information with special logger level.
*
* Parameters:
*  level: logger level.
*  format: format string like 'printf'.
*  ...: variable parameters.
*
* Returns:
*  None
*
*
*******************************************************************************/
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
    vprintf(format, args);          /* printf perform at '\n' */
    va_end(args);
    
    /* Print all data in buffer, even not ended with '\n'.
     * Note, this funciton is only valid in GCC compiler.
     */    
    fflush(stdout);
}

/*******************************************************************************
* Function Name: logger_printUint32
********************************************************************************
*
* Summary:
*  Print a uint32 data as decimal.
*
* Parameters:
*  data: a decimal data.
*
* Returns:
*  None
*
*
*******************************************************************************/
void logger_printUint32(uint32 data)
{
	char char_table[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
	uint32 int_len[] = {
		1000000000,
		100000000,
		10000000,
		1000000,
		100000,
		10000,
		1000,
		100,
		10,
		1,
	};
	uint32 d = data;		/* Temp data */
	uint8  i, j;			/* Loop variable */
	uint8  data_len = 0;	/* Data bit length */

	if (data < 10) {
		UART_PUTC(char_table[data]);	
		return;
	}	
	
	/* Find input data length */
	for (i = 0; i < 10; i++) {
		if (d >= int_len[i]) {
			data_len = 10 - i;	
			break;
		}
	}
	
	/* Print each bit */
	for (j = 0; j < data_len; j++) {
		UART_PUTC(char_table[d / int_len[i + j]]);
		d = d % int_len[i + j];	
	}
}

/*******************************************************************************
* Function Name: logger_format
********************************************************************************
*
* Summary:
*  Print information with file and function location.
*
* Parameters:
*  level: logger level.
*  file: file where calling this function.
*  function: function where calling this API.
*  line: code line.
*  format: format string like 'printf'.
*  ...: variable parameters.
*
* Returns:
*  None
*
*
*******************************************************************************/
void logger_format(logger_level_t level, const char* file, const char* function, uint32 line, const char* format, ...)
{    
    if (!hasLoggerInitialized) {
        return;   
    }
    
    if (level > loggerConfig.level) {
        return;   
    }
    
    /* Print format text. */
	UART_PUTS("[");
    UART_PUTS(file);
    UART_PUTS("|");
    UART_PUTS(function);
    UART_PUTS("()");
    UART_PUTS("|L");
	logger_printUint32(line);
	UART_PUTS("] ");
    UART_BUSY;
    
    va_list args;
    va_start(args, format);
    vprintf(format, args);      /* printf perform at '\n' */
    va_end(args);
    
    /* Print all data in buffer, even not ended with '\n'.
     * Note, this funciton is only valid in GCC compiler.
     */
    fflush(stdout);
}

/*******************************************************************************
* Function Name: logger_debug
********************************************************************************
*
* Summary:
*  Print debug information.
*
* Parameters:
*  format: format string like 'printf'.
*  ...: variable parameters.
*
* Returns:
*  None
*
*
*******************************************************************************/
void logger_debug(const char* format, ...)
{
    if (!hasLoggerInitialized) {
        return;   
    }
    
    if (loggerConfig.level < LOGGER_LEVEL_DEBUG) {
        return;   
    }
    
    va_list args;
    va_start(args, format);
    vprintf(format, args);      /* printf perform at '\n' */
    va_end(args);
    
    /* Print all data in buffer, even not ended with '\n'.
     * Note, this funciton is only valid in GCC compiler.
     */
    fflush(stdout); 
}

/*******************************************************************************
* Function Name: logger_trace
********************************************************************************
*
* Summary:
*  Print trace information.
*
* Parameters:
*  format: format string like 'printf'.
*  ...: variable parameters.
*
* Returns:
*  None
*
*
*******************************************************************************/
void logger_trace(const char* format, ...)
{
    if (!hasLoggerInitialized) {
        return;   
    }
    
    if (loggerConfig.level < LOGGER_LEVEL_TRACE) {
        return;   
    }
    
    va_list args;
    va_start(args, format);
    vprintf(format, args);      /* printf perform at '\n' */
    va_end(args);
    
    /* Print all data in buffer, even not ended with '\n'.
     * Note, this funciton is only valid in GCC compiler.
     */
    fflush(stdout); 
}

/*******************************************************************************
* Function Name: logger_error
********************************************************************************
*
* Summary:
*  Print error information.
*
* Parameters:
*  format: format string like 'printf'.
*  ...: variable parameters.
*
* Returns:
*  None
*
*
*******************************************************************************/
void logger_error(const char* format, ...)
{
    if (!hasLoggerInitialized) {
        return;   
    }
    
    if (loggerConfig.level < LOGGER_LEVEL_ERROR) {
        return;   
    }
    
    va_list args;
    va_start(args, format);
    vprintf(format, args);      /* printf perform at '\n' */
    va_end(args);
    
    /* Print all data in buffer, even not ended with '\n'.
     * Note, this funciton is only valid in GCC compiler.
     */    
    fflush(stdout);
}

/*******************************************************************************
* Function Name: logger_fatal
********************************************************************************
*
* Summary:
*  Print fatal information.
*
* Parameters:
*  format: format string like 'printf'.
*  ...: variable parameters.
*
* Returns:
*  None
*
*
*******************************************************************************/
void logger_fatal(const char* format, ...)
{
    if (!hasLoggerInitialized) {
        return;   
    }
    
    if (loggerConfig.level < LOGGER_LEVEL_FATAL) {
        return;   
    }
    
    va_list args;
    va_start(args, format);
    vprintf(format, args);      /* printf perform at '\n' */
    va_end(args);
    
    /* Print all data in buffer, even not ended with '\n'.
     * Note, this funciton is only valid in GCC compiler.
     */    
    fflush(stdout);
}

/*******************************************************************************
* Function Name: uart_setBaudRate
********************************************************************************
*
* Summary:
*  Set UART baud rate.
*
* Parameters:
*  baudRate: UART baud rate.
*
* Returns:
*  None
*
* Note:
*  I only test 115200 and below, I hear from somewhere that BLE 042 Kit doesn't 
*  support bigger baud rate as the limit of kitprog.
*
*******************************************************************************/
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

/*******************************************************************************
* Function Name: logger_init
********************************************************************************
*
* Summary:
*  Initialize logger system.
*
* Parameters:
*  None
*
* Returns:
*  None
*
*******************************************************************************/
void logger_init(void)
{
    if (!hasLoggerInitialized) {  
        /* Set baud rate */
        uart_setBaudRate(loggerConfig.baudRate);
        
        /* Start timer */
        // TODO: it seems doesn't matter if not calling it!!??
        UARTCLK_START();
        /* Start SCB component */
        UART_START();
        
        hasLoggerInitialized = 1;
    }
}

/*******************************************************************************
* Function Name: logger_start
********************************************************************************
*
* Summary:
*  Start logger system with logger level and baud rate.
*
* Parameters:
*  level:  logger level.
*  baudRate:  baud rate.
*
* Returns:
*  None
*
*******************************************************************************/
void logger_start(logger_level_t level, uart_baud_rate_t baudRate)
{         
    hasLoggerInitialized = 0;

    loggerConfig.baudRate = baudRate;
    loggerConfig.level    = level;
    loggerConfig.heapSize = CYDEV_HEAP_SIZE;        /* System heap size */
    
 

    /* Disable the logger. As SCB is not started, so it dosn't consume power */
    if (level == LOGGER_LEVEL_DISABLE) {
        return;
    }
    
    _Bool heapSizeInsufficient = 0;
    
    /* printf function requires heap size being largger than 0x200 */
    if (loggerConfig.heapSize < 0x200) {
        heapSizeInsufficient = 1;
        loggerConfig.baudRate = UART_BAUD_RATE_115200; /* Used for print error */
    }

    logger_init();
    
    if (heapSizeInsufficient) {
        /* Print error with original function */
        UART_PUTS("ERROR: Heap size is insufficient, set it to 0x200 or more.\r\n");
        UART_BUSY;              /* Make sure all data in buffer is sent */
        
        UART_STOP();            /* Stop SCB */
        UARTCLK_STOP();         /* Stop SCB timer */

        hasLoggerInitialized = 0;
    }
}

/*******************************************************************************
* Function Name: logger_stop
********************************************************************************
*
* Summary:
*  Stop logger system.
*
* Parameters:
*  None
*
* Returns:
*  None
*
* Note:
*  I don't test this function as it's never used.
*
*******************************************************************************/
void logger_stop(void)
{
    if (hasLoggerInitialized) {
        UART_STOP();            /* Stop SCB */
        UARTCLK_STOP();         /* Stop SCB timer */
        
        hasLoggerInitialized = 0;
    }
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
    
    UART_BUSY;          /* Make sure all data in buffer is sent */
    return len;
}



/* [] END OF FILE */
