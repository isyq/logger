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
#include <stdio.h>
#define N 10

int main()
{
    CyGlobalIntEnable;
    
    logger_start(LOGGER_LEVEL_DEBUG, UART_BAUD_RATE_115200);
    
    logger.t("Array Data(len = %d): ", N);
    uint8 testData[N];
    uint16 i;
    for (i = 0; i < N; i++) {
        testData[i] = i+1;   
    }
    logger_array(testData, N);  /* No need to add \r\n anymore */
    
    logger.t("Trace: This is most frequently used.\r\n");
    logger.e("Error: Capture an error.\r\n");
    logger.e("Fatal: Trigger a fatal error!\r\n");
    p("It's a debug information\r\n");
    p("It's a debug information too\r\n");
    
    
    for(;;)
    {
    }
}

/* [] END OF FILE */
