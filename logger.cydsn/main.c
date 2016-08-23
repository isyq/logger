/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include <project.h>
#include "../src/logger.h"

int main()
{
    CyGlobalIntEnable;
    
    logger_config_t loggerConfig = {
        .baudRate = UART_BAUD_RATE_115200,
        .level    = LOGGER_LEVEL_TRACE,
        .heapSize = CYDEV_HEAP_SIZE,
    };
    logger_start(&loggerConfig);
    
    uint8 testData[10] = {100, 101, 102, 103, 104, 105, 106, 107, 108, 109};
    logger_byte(LOGGER_DATA_TYPE_BIN, 0xF1);
    logger_array(LOGGER_DATA_TYPE_HEX, testData, 10, '_');
    logger_p("\r\nHello World %x %c %s\r\n", 100, 'A', "GOOD");
    logger_trace("Hello World %x %c %s\r\n", 100, 'A', "GOOD");
    logger_trace("Hello World %x %c %s\r\n", 100, 'A', "GOOD");
    logger_trace("Hello World %x %c %s\r\n", 100, 'A', "GOOD");
    
    for(;;)
    {
        /* Place your application code here. */
    }
}

/* [] END OF FILE */