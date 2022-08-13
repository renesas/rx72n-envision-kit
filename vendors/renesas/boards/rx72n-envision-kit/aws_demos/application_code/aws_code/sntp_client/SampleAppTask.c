/*
 * FreeRTOS V202112.00
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */

/*
 * This file belongs to the demo for showing use of the coreSNTP library for synchronizing
 * system time with internet time and maintaining correct wall-clock time.
 *
 * This file shows how an application task can query Coordinated Universal Time (UTC) from system,
 * that is maintained and synchronized periodically in the SNTP client (daemon) task of the demo
 * project. Refer to the the SNTPClientTask.c file in this project for the SNTP client task.
 */

/* Standard includes. */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"

/* Demo includes. */
#include "common_demo_include.h"
#include "aws_demo_config.h"

/* for RX Family FIT */
#include "r_sys_time_rx_if.h"
#include "r_simple_filesystem_on_dataflash_if.h"
#include "rx72n_envision_kit_system.h"

/*-----------------------------------------------------------*/

/**
 * @brief The periodicity of the application task in query and logging system time. This is the time
 * interval between consecutive system clock time queries in the sample application task.
 */
#define CLOCK_QUERY_TASK_DELAY_MS    ( 1000 )

/*-----------------------------------------------------------*/

void printTime( const UTCTime_t * pUnixTime )
{
#if 0 // CC-RX does not support time.h
    struct tm * currTime;
    time_t time;

    /* Obtain the broken-down UTC representation of the current system time. */
    time = pUnixTime->secs;
    currTime = gmtime( &time );

    /* Log the time as both UNIX timestamp and Human Readable time. */
    LogInfo( ( "Time:\nUNIX=%lusecs %lums\nHuman Readable=%lu-%02lu-%02lu %02luh:%02lum:%02lus",
               pUnixTime->secs, pUnixTime->msecs,
               currTime->tm_year + 1900, currTime->tm_mon + 1, currTime->tm_mday,
               currTime->tm_hour, currTime->tm_min, currTime->tm_sec ) );
#endif
}

/*************************************************************************************/

/* Sample application task that will query and log system time every second. */
void sntp_task( void * pvParameters )
{
    UTCTime_t systemTime;
	uint32_t unix_time;
    SFD_HANDLE sfd_handle_timezone;
	uint8_t *timezone;
	uint32_t timezone_length;
	TASK_INFO *task_info = pvParameters;


    initializeSystemClock();

    /* Create the SNTP client task that is responsible for synchronizing system time with the time servers
     * periodically. This is created as a high priority task to keep the SNTP client operation unhindered. */
    xTaskCreate( sntpTask,                 /* Function that implements the task. */
                 "SntpClientTask",         /* Text name for the task - only used for debugging. */
                 democonfigDEMO_STACKSIZE, /* Size of stack (in words, not bytes) to allocate for the task. */
                 NULL,                     /* Task parameter - not used in this case. */
                 configMAX_PRIORITIES - 1, /* Task priority, must be between 0 and configMAX_PRIORITIES - 1. */
                 NULL );

	sfd_handle_timezone = R_SFD_FindObject((uint8_t *)timezone_label, strlen((char *)timezone_label));
	if(sfd_handle_timezone == SFD_HANDLE_INVALID)
	{
		sfd_handle_timezone = R_SFD_SaveObject((uint8_t *)timezone_label, strlen((char *)timezone_label), SYS_TIME_UTC, strlen((char *)SYS_TIME_UTC) + 1); /* +1 means string terminator '\0' */
	}
	R_SFD_GetObjectValue(sfd_handle_timezone, (uint8_t **)&timezone, &timezone_length);

    while( 1 )
    {
    	/* We should wait for the network to be up before getting time. */
        while( FreeRTOS_IsNetworkUp() == pdFALSE )
        {
            vTaskDelay(300);
        }

        systemGetWallClockTime( &systemTime );

        unix_time = systemTime.secs;
        if(SYS_TIME_ERR_BAD_TIME_OFFSET == R_SYS_TIME_ConvertUnixTimeToSystemTime(unix_time, &task_info->sys_time, timezone))
        {
    		sfd_handle_timezone = R_SFD_SaveObject((uint8_t *)timezone_label, strlen((char *)timezone_label), SYS_TIME_UTC, strlen((char *)SYS_TIME_UTC));
        }
        R_SYS_TIME_SetCurrentTime(&task_info->sys_time);

        vTaskDelay( pdMS_TO_TICKS( CLOCK_QUERY_TASK_DELAY_MS ) );
    }
}

/*-----------------------------------------------------------*/
