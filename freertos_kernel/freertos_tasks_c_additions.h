#ifndef FREERTOS_FREERTOS_PP_FREERTOS_TASK_S_CADDITIONS_H
#define FREERTOS_FREERTOS_PP_FREERTOS_TASK_S_CADDITIONS_H

/*
 * This code comes from:
 * https://www.freertos.org/FreeRTOS_Support_Forum_Archive/May_2019/freertos_How_to_reset_vTaskGetRunTimeStats_425f1134aaj.html
 */

void vTaskClearUsageSingleList(List_t *pxList)
{
    configLIST_VOLATILE TCB_t *pxNextTCB, *pxFirstTCB;

    if( listCURRENT_LIST_LENGTH( pxList ) > ( UBaseType_t ) 0 )
    {
		listGET_OWNER_OF_NEXT_ENTRY( pxFirstTCB, pxList );

		/* Clear Usage for each task that is referenced from
		pxList.  See the definition of TaskStatus_t in task.h for the
		meaning of each TaskStatus_t structure member. */
		do {
			listGET_OWNER_OF_NEXT_ENTRY( pxNextTCB, pxList );
			pxNextTCB->ulRunTimeCounter = 0;
		} while( pxNextTCB != pxFirstTCB );
    }
}

void vTaskClearUsage(void) {
    UBaseType_t uxQueue = configMAX_PRIORITIES;
    vTaskSuspendAll();
    /* Clear runtime counter for each task in the Ready State */
    do {
    	uxQueue--;
        vTaskClearUsageSingleList(&( pxReadyTasksLists[ uxQueue ] ));
    } while( uxQueue > ( UBaseType_t ) tskIDLE_PRIORITY ); /* lint !e961 MISRA exception as the casts are only redundant for some ports. */

	/* Clear runtime counter for each task in the Blocked state. */
	vTaskClearUsageSingleList(( List_t * ) pxDelayedTaskList);
	vTaskClearUsageSingleList(( List_t * ) pxOverflowDelayedTaskList);


#if( INCLUDE_vTaskDelete == 1 )
	/* Clear runtime counter for each task that has been deleted but not yet cleaned up. */
	vTaskClearUsageSingleList(&xTasksWaitingTermination);
#endif


#if ( INCLUDE_vTaskSuspend == 1 )
	/* Clear runtime counter for each task in the Suspended state. */
	vTaskClearUsageSingleList(&xSuspendedTaskList);
#endif
	ulTaskSwitchedInTime = 0;
	ulTotalRunTime = 0;
	vConfigureTimerForRunTimeStats();
	xTaskResumeAll();
}

void vTaskGetCombinedRunTimeStats( char* pcWriteBuffer,  UBaseType_t uxClear)
{
	TaskStatus_t *pxTaskStatusArray;
    UBaseType_t uxArraySize, x;
    uint32_t ulTotalTime, ulStatsAsPercentage;
    char cStatus;

    pcWriteBuffer += sprintf( pcWriteBuffer, "Tsk# TaskName\t  State\tPrio\t  HWM  CPUtime(us)  CPUload\r\n");

    // Snapshot number of tasks
    // Allocate memory for the binary task data structure
    uxArraySize = uxTaskGetNumberOfTasks();
    pxTaskStatusArray = pvPortMalloc(uxArraySize * sizeof(TaskStatus_t));

    if( pxTaskStatusArray != NULL )
    {
        /* Generate the (binary) data – returns the number of tasks */
        uxArraySize = uxTaskGetSystemState(pxTaskStatusArray, uxArraySize, &ulTotalTime);

        /* For percentage calculations. */
        ulTotalTime /= 100UL;

        /* Create a human readable table from the binary data. */
        for( x = 0; x < uxArraySize; x++ )
        {
            pcWriteBuffer += sprintf( pcWriteBuffer, "%3u  ", pxTaskStatusArray[ x ].xTaskNumber );
            sprintf(pcWriteBuffer, "%s            ", pxTaskStatusArray[ x ].pcTaskName);
            pcWriteBuffer += 14; // Assumes task name is less than 12 characters and more than 2
            switch(pxTaskStatusArray[ x ].eCurrentState )
            {
                case eRunning:
                    cStatus = 'X';
                    break;
                case eReady:
                    cStatus = 'R';
                    break;
                case eBlocked:
                    cStatus = 'B';
                    break;
                case eSuspended:
                    cStatus = 'S';
                    break;
                case eDeleted:
                    cStatus = 'D';
                    break;
                default:        /* Should not get here, but it is included to prevent static checking errors. */
                    cStatus = 'I';
                    break;
            }
            pcWriteBuffer += sprintf( pcWriteBuffer, " %c    %2i    %6i", cStatus, pxTaskStatusArray[ x ].uxCurrentPriority, pxTaskStatusArray[ x ].usStackHighWaterMark);

            if(ulTotalTime > 0UL)
            {
                ulStatsAsPercentage = pxTaskStatusArray[ x ].ulRunTimeCounter / ulTotalTime;

                if( ulStatsAsPercentage > 0UL )
                {
                    pcWriteBuffer += sprintf( pcWriteBuffer, "%13u    %3u%%", pxTaskStatusArray[ x ].ulRunTimeCounter, ulStatsAsPercentage );
                }
                else
                {
                    pcWriteBuffer += sprintf( pcWriteBuffer, "%13u     <1%%", pxTaskStatusArray[ x ].ulRunTimeCounter );
                }

                if(uxClear)
                {
                    pxTaskStatusArray[ x ].xHandle->ulRunTimeCounter = 0;
                }
            }
            pcWriteBuffer += sprintf(pcWriteBuffer, "\r\n");
        }

        if(uxClear)
        {
            ulTaskSwitchedInTime = 0;
            ulTotalRunTime = 0;
        }
        vPortFree( pxTaskStatusArray );
    }
    else
    {
        mtCOVERAGE_TEST_MARKER();
    }
}

#endif /* FREERTOS_FREERTOS_PP_FREERTOS_TASK_S_CADDITIONS_H */
