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
#define N 30

int main()
{
    CyGlobalIntEnable;
    
    logger_start(LOGGER_LEVEL_DEBUG, UART_BAUD_RATE_115200);
    
    logger.t("Test Data: ");
    uint8 testData[N];
    uint16 i;
    for (i = 0; i < N; i++) {
        testData[i] = i+1;   
    }
    logger_array(testData, N);  /* No need to add \r\n anymore */
    
    logger.t("This is most frequently used.\r\n");
    logger.e("A biiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiig error...\r\n");
    p("It's a debug information\r\n");
    
    
    for(;;)
    {
    }
}

/* [] END OF FILE */
