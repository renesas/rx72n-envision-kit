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

void vTaskClearUsage() {
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

#endif /* FREERTOS_FREERTOS_PP_FREERTOS_TASK_S_CADDITIONS_H */
