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
    
    logger_start(UART_BAUD_RATE_115200, LOGGER_LEVEL_TRACE);
    
    uint8 testData[10] = {100, 101, 102, 103, 104, 105, 106, 107, 108, 109};
    logger_byte(LOGGER_DATA_TYPE_BIN, 0xF1);
    logger_array('x', testData, 10, '_');
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
