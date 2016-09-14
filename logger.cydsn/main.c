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

int main()
{
    CyGlobalIntEnable;
    
    logger.start(LOGGER_LEVEL_DEBUG, UART_BAUD_RATE_115200);
	
    logger.t("Trace: This is most frequently used.\r\n");
    logger.e("Error: Capture an error.\r\n");
    logger.f("Fatal: Trigger a fatal error!\r\n");
	
    logger.t("\r\nArray Data(len = %d): ", 10);
    uint8 testData[10] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J'};
	logger.list(testData, 10);
	
	p("\r\nIt's a clean debug log.\r\n");
    d("It's a debug log with extra information.\r\n");  
	
    
    for(;;)
    {
    }
}

/* [] END OF FILE */
