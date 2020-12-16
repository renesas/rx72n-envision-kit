/**********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO
 * THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2020 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : audio_task.c
 * Description  : 
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         : 20.11.2020 1.00     First Release
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
/* for using C standard library */
#include <stdio.h>
#include <string.h>

/* for using FIT Module */
#include "platform.h"
#include "r_cmtw_rx_if.h"
#include "r_ssi_api_rx_config.h"
#include "r_ssi_api_rx_if.h"
#include "r_ssi_api_rx_pinset.h"
#include "r_byteq_if.h"

/* for using Amazon FreeRTOS */
#include "FreeRTOS.h"
#include "semphr.h"

/* for RX72N Envision Kit system common header */
#include "rx72n_envision_kit_system.h"

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/
#define MEMS_MIC            (1)                 /* Output signal from microphone */
#define SINE_WAVE           (0)                 /* Output signal sine wave data */
#define OUTPUT_SIGNAL_FROM  SINE_WAVE
//#define OUTPUT_SIGNAL_FROM  MEMS_MIC

/* PCM Buffer definition */
#define BUFSIZE             (1024)              /* PCM Buffer Size (1sample = stareo) */
#define BUFNUM              (3)                 /* number of PCM Buffer */
#define CHANNELS            (2)                 /* number of PCM data channel */
#define READ_SIZE           (512)
#define READ_SECTOR_NUM     (6)
#define QUIE_NUM            (24)
#define READ_NUM            (24)
#define PCM_WIDTH           (3)
#define I2S_PCM_WIDTH       (4)

/* index for PCM buffer information array */
#define CURRENT             (0)                 /* index 0 : current information */
#define NEXT                (1)                 /* index 1 : current information */
#define BUFINFONUM          (2)                 /* array size, must be 2 for reload mechanizm */

/* CMT Cycle(Hz) */
#define CMT_CYCLE_HZ        (12000)     /* Sampring Frequency(48kHz) / Number of PCM data samples to write/read(4) */
/* CMTW Cycle(us) */
#define CMTW_CYCLE_US       (83)        /* Value = ((1 / CMT_CYCLE_HZ) * 1000000) or lower. */

/* LED Blink Count */
#define LED_CYCLE_COUNT     (1200)      /* Cycle is 0.1s. Count is 0.1 / (1 / CMT_CYCLE_HZ). */

/**********************************************************************************************************************
 Local Typedef definitions
 *********************************************************************************************************************/
/* PCM buffer information to interfece between main routine & isr */
typedef struct ssi_buf_info
{
    int32_t * p_buf_start; /* PCM buffer addres to transfer PCM data */
    uint16_t pcm_samples;  /* Number of PCM data samples in buffer   */
} buf_info_t;

/* definitions to request PCM buffer information */
typedef enum {
    NO_REQUEST   = 0x00,   /* no request                             */
    DATA_REQUEST = 0x01,   /* next PCM buffer information request    */
}  buf_info_request_t;

/* definitions to stop PCM transfer operation */
typedef enum {
    FLAG_RUN  = 0x00,      /* no stop request                         */
    FLAG_STOP = 0x01,      /* PCM data transfer operation stop request */
}  stop_flag_t;

/* error_information */
typedef enum {
    NO_ERROR = 0x00,
    MALLOC_FAILED,
	BYTEQ_OPEN_ERROR,
	BYTEQ_GET_ERROR,
    CMT_CH_GET_ERROR,
    CMT_START_ERROR,
    SSI_OPEN_ERROR_CH0,
    SSI_OPEN_ERROR_CH1,
    SSI_START_ERROR_CH0,
    SSI_START_ERROR_CH1,
    SSI_WRITE_FAILED,
    SSI_READ_FAILED,
    PCM_BUFFER_ERROR,
    SSI_STOP_ERROR_CH0,
    SSI_STOP_ERROR_CH1,
    SSI_CLOSE_ERROR_CH0,
    SSI_CLOSE_ERROR_CH1,
    CMT_STOP_ERROR,
    CMT_CLOSE_ERROR
} audio_error_t;

/* PCM buffer */
struct pcm_buf
{
    int32_t *pcm_buffer;
};

/* definitions to PCM buffer malloc status */
typedef enum {
    NOT_EXECUTE = 0x00,        /* Does not executed the malloc */
    EXECUTED_MALLOC = 0x01,    /* Executed the malloc */
}  malloc_flag_t;

/* SSI Register */
typedef R_BSP_VOLATILE_EVENACCESS struct st_ssie  ssi_regs_t;

/**********************************************************************************************************************
 Exported global functions
 *********************************************************************************************************************/
void audio_task( void * pvParameters );
void cmt_callback( void *arg );
void d2audio_play_start( void );
void d2audio_play_stop( void );
void d2audio_record_start( void );
void d2audio_record_stop( void );
uint32_t d2audio_Initial_data_set(uint8_t *read_buff, uint16_t read_size);
int32_t d2audio_data_set(uint8_t *read_buff, uint16_t read_size);
int32_t d2audio_data_read(void);

/**********************************************************************************************************************
 Private (static) variables and functions
 *********************************************************************************************************************/
/* PCM buffer information variables to interface. */
static buf_info_t buf_info_tx[BUFINFONUM];      /* information for transmit operation */
#if OUTPUT_SIGNAL_FROM == MEMS_MIC
static buf_info_t buf_info_rx[BUFINFONUM];      /* information for receive operation  */
#endif

/* variables to request PCM buffer information */
#if OUTPUT_SIGNAL_FROM == MEMS_MIC
static volatile buf_info_request_t buf_info_request_rx;  /* request for receive operation */
#endif

/* PCM buffer */
struct pcm_buf g_pcmbuf_tx[BUFNUM];
#if OUTPUT_SIGNAL_FROM == MEMS_MIC
struct pcm_buf g_pcmbuf_rx[BUFNUM];
#endif

int32_t g_idx_buf_tx = 0;               /* index of pcm_buffer_tx; */
#if OUTPUT_SIGNAL_FROM == MEMS_MIC
int32_t g_idx_buf_rx = 0;               /* index of pcm_buffer_rx; */
#endif

malloc_flag_t g_pcmbuf_tx_malloc = NOT_EXECUTE;
#if OUTPUT_SIGNAL_FROM == MEMS_MIC
malloc_flag_t g_pcmbuf_rx_malloc = NOT_EXECUTE;
#endif

/* Return code */
static volatile ssi_ret_t g_ssi_ret = SSI_ERR_PARAM;
static volatile cmtw_err_t cmtw_ret = CMTW_ERR_BAD_CHAN;

uint32_t g_pcm_buf_size;
uint8_t *g_pcm_buf_ptr;
uint32_t g_que_index;
byteq_hdl_t g_byte_que;
uint8_t g_read_buff[READ_SIZE];
static volatile audio_error_t g_error_code = NO_ERROR;

/* callback counter */
static volatile int32_t g_cmt_cb_cnt = 0;

#if OUTPUT_SIGNAL_FROM == SINE_WAVE
/* Test wave */
static const int32_t test_wave[BUFSIZE*CHANNELS] =
{
#include "r_test_wave.h"
};
#endif

/* stop flag */
static volatile stop_flag_t stop_flag_play;
static volatile stop_flag_t stop_flag_record;

/* request flag for GUI */
uint8_t g_d2audio_play_request = 0;
uint8_t g_d2audio_play_stop_request = 0;
uint8_t g_d2audio_record_request = 0;
int32_t g_previous_sd_detect_status = -1;
SemaphoreHandle_t g_xSemaphore;

/**********************************************************************************************************************
 Exported global variables
 *********************************************************************************************************************/
static void ssi_start_for_speakers( void );
static void ssi_write( void );
#if OUTPUT_SIGNAL_FROM == MEMS_MIC
static void ssi_start_for_mems_mic( void );
static void ssi_read( void );
#endif
static void ssi_stop ( void );
static void set_buf_info( const buf_info_t * p_buf_info, const int32_t * p_buf, const uint16_t pcm_samples );
static void audio_trap( audio_error_t error_code );
static void led_blink( void );

extern void d2audio_list_add(char *pstring);
extern void d2audio_file_search(void);
extern int32_t d2audio_read_data(uint8_t *read_buff, uint16_t request_read_size, uint16_t *read_size);
extern int32_t get_sdc_sd_detection(void);

/**********************************************************************************************************************
 * Function Name: audio_task
 * Description  : 
 * Arguments    : void * pvParameters : 
 * Return Value : none
 *********************************************************************************************************************/
void audio_task( void * pvParameters )
{
    cmtw_channel_settings_t cmtw_config;
    uint32_t volatile buf_size;
    uint32_t file_read_size;
    ssi_regs_t *p_ssi_reg;
    int32_t sd_detect_status;
    int32_t ret_code;
	uint16_t que_no_used_size;
    uint16_t que_space;

    /* PCM buffer size initialization. */
    buf_size = (BUFSIZE*CHANNELS)*4;
    g_pcmbuf_tx_malloc = NOT_EXECUTE;
#if OUTPUT_SIGNAL_FROM == MEMS_MIC
    g_pcmbuf_rx_malloc = NOT_EXECUTE;
#endif

    /* Initialize port for LED */
    PORT4.PDR.BYTE = 1;

    stop_flag_play = FLAG_STOP;
    stop_flag_record = FLAG_STOP;
    memset(&g_read_buff, sizeof(g_read_buff), 0x00);
    g_que_index = 0;
    g_pcm_buf_size = READ_SIZE * READ_SECTOR_NUM * QUIE_NUM;
    g_pcm_buf_ptr = (uint8_t *)pvPortMalloc(g_pcm_buf_size);
    que_space = READ_SIZE * READ_SECTOR_NUM;
    g_xSemaphore = xSemaphoreCreateMutex();

    /*** PCM data transfer operation ***/
    while(1)
    {
    	sd_detect_status = get_sdc_sd_detection();
    	if ((sd_detect_status == 0 ) &&
    		(g_previous_sd_detect_status != sd_detect_status))
    	{
    		g_previous_sd_detect_status = sd_detect_status;
    	    d2audio_file_search();
    	}
        if (FLAG_STOP == stop_flag_play )
        {
            if (1 == g_d2audio_play_request)
            {
                 /* Initialize for SPEAKER */
                 ssi_start_for_speakers();

                 /* CMTW Start */
                 memset(&cmtw_config, 0, sizeof(cmtw_config));
                 cmtw_config.time_unit = CMTW_TIME_USEC;
                 cmtw_config.clock_divisor = CMTW_CLK_DIV_8;
                 cmtw_config.clear_source = CMTW_CLR_CMT;
                 cmtw_config.cm_timer.time = CMTW_CYCLE_US;
                 cmtw_config.cm_timer.actions = CMTW_ACTION_CALLBACK;
                 cmtw_ret = R_CMTW_Open(CMTW_CHANNEL_0, &cmtw_config, cmt_callback);
                 if (CMTW_SUCCESS != cmtw_ret)
                 {
                     audio_trap(CMT_CH_GET_ERROR);
                 }

                 cmtw_ret = R_CMTW_Control(CMTW_CHANNEL_0, CMTW_CMD_START);
                 if (CMTW_SUCCESS != cmtw_ret)
                 {
                     audio_trap(CMT_START_ERROR);
                 }

                 stop_flag_play = FLAG_RUN;
            }
        }

        if (FLAG_STOP == stop_flag_record )
        {
            if (1 == g_d2audio_record_request)
            {
#if OUTPUT_SIGNAL_FROM == MEMS_MIC
                 /* Initialize for MEMS MIC */
                 ssi_start_for_mems_mic();
#endif
                 stop_flag_record = FLAG_RUN;
            }
        }

        if (FLAG_RUN == stop_flag_play )
        {
        	if (g_d2audio_play_stop_request == 0)
        	{
        		R_BYTEQ_Unused(g_byte_que, &que_no_used_size);
                if (que_no_used_size > que_space)
                {
                	ret_code = d2audio_data_read();
                	if (ret_code != 0)
                	{
                		g_d2audio_play_stop_request = 1;
                	}
                }
        	}

            if (0 == g_d2audio_play_request)
            {
                do
                {
                    cmtw_ret = R_CMTW_Control(CMTW_CHANNEL_0, CMTW_CMD_STOP);
                } while (CMTW_SUCCESS != cmtw_ret);

                cmtw_ret = R_CMTW_Close(CMTW_CHANNEL_0);
                if (CMTW_SUCCESS != cmtw_ret)
                {
                    audio_trap(CMT_CLOSE_ERROR);
                }

                ssi_stop();
                R_BYTEQ_Close(g_byte_que);
                stop_flag_play = FLAG_STOP;
                stop_flag_record = FLAG_STOP;
                g_d2audio_play_stop_request = 0;
                g_previous_sd_detect_status = -1;
                vPortFree(g_pcm_buf_ptr);
            }
        }

        if (FLAG_RUN == stop_flag_record )
        {
#if OUTPUT_SIGNAL_FROM == MEMS_MIC
            /* Check Receive Data Full Flag. */
            p_ssi_reg = (ssi_regs_t *)&SSIE0;

            if(1 == p_ssi_reg->SSIFSR.BIT.RDF)
            {
                ssi_read();
            }

            /* SSI data receive information setting */
            if ( DATA_REQUEST == buf_info_request_rx )  /* checks PCM buffer information request */
            {
                /* the next receive PCM buffer information setting */
                set_buf_info( &buf_info_rx[NEXT], g_pcmbuf_rx[g_idx_buf_rx++].pcm_buffer, BUFSIZE );
                buf_info_request_rx = NO_REQUEST;
                g_idx_buf_rx %= BUFNUM;

                /* execute signal processing (e.g. attenuation) here, if needed */

                /* copy operation for PCM data loopback */
                memcpy( g_pcmbuf_tx[g_idx_buf_tx].pcm_buffer, g_pcmbuf_rx[g_idx_buf_rx].pcm_buffer, buf_size );
            }

            if (0 == g_d2audio_record_request)
            {
                do
                {
                    cmtw_ret = R_CMTW_Control(CMTW_CHANNEL_0, CMTW_CMD_STOP);
                } while (CMTW_SUCCESS != cmtw_ret);

                cmtw_ret = R_CMTW_Close(CMTW_CHANNEL_0);
                if (CMTW_SUCCESS != cmtw_ret)
                {
                    audio_trap(CMT_CLOSE_ERROR);
                }

                ssi_stop();

                stop_flag_play = FLAG_STOP;
                stop_flag_record = FLAG_STOP;
            }
#endif /* OUTPUT_SIGNAL_FROM == MEMS_MIC */
        }
    }

} /* End of function audio_task */

#if OUTPUT_SIGNAL_FROM == MEMS_MIC
/**********************************************************************************************************************
 * Function Name: ssi_start_for_mems_mic
 * Description  : Initialize SSI for the MEMS microphone.
 * Arguments    : none
 * Return Value : none
 *********************************************************************************************************************/
static void ssi_start_for_mems_mic( void )
{
    uint32_t malloc_size;
    uint8_t i;

    if (NOT_EXECUTE == g_pcmbuf_rx_malloc)
    {
        /* PCM buffer initialization. */
        malloc_size = BUFSIZE * CHANNELS * 4;

        for(i=0; i<BUFNUM; i++)
        {
            g_pcmbuf_rx[i].pcm_buffer = pvPortMalloc(malloc_size);
            if (g_pcmbuf_rx[i].pcm_buffer == NULL)
            {
                audio_trap(MALLOC_FAILED);
            }
            memset(g_pcmbuf_rx[i].pcm_buffer, 0, malloc_size);
        }
        g_pcmbuf_rx_malloc = EXECUTED_MALLOC;
    }

    /* MEMS microphone */
    g_ssi_ret = R_SSI_Open(SSI_CH0);
    if ( SSI_SUCCESS == g_ssi_ret )
    {
        /* first receive PCM buffer information setting */
        g_idx_buf_rx = 0;
        set_buf_info( &buf_info_rx[CURRENT], g_pcmbuf_rx[g_idx_buf_rx++].pcm_buffer, BUFSIZE );
        g_idx_buf_rx %= BUFNUM;

        /* request for the next PCM buffer information, just for the start operation */
        buf_info_request_tx = NO_REQUEST; /* the next PCM buffer information request */

        /* configures SSI0 pins SSI functions */
        R_SSI_PinSet_SSIE0();

        /* MEMS microphone input enable. */
        g_ssi_ret = R_SSI_Start(SSI_CH0);
        if ( SSI_SUCCESS != g_ssi_ret )
        {
            audio_trap(SSI_START_ERROR_CH0);
        }
    }
    else
    {
        audio_trap(SSI_OPEN_ERROR_CH0);
    }
} /* End of function ssi_start_for_mems_mic */
#endif /* OUTPUT_SIGNAL_FROM == MEMS_MIC */

/**********************************************************************************************************************
 * Function Name: ssi_start_for_speakers
 * Description  : Initialize SSI for the Powered speakers.
 * Arguments    : none
 * Return Value : none
 *********************************************************************************************************************/
static void ssi_start_for_speakers( void )
{
    uint32_t malloc_size;
    uint8_t i;

    if (NOT_EXECUTE == g_pcmbuf_tx_malloc)
    {
        /* PCM buffer initialization. */
        malloc_size = BUFSIZE * CHANNELS * 4;

        for(i=0; i<BUFNUM; i++)
        {
            g_pcmbuf_tx[i].pcm_buffer = pvPortMalloc(malloc_size);
            if (g_pcmbuf_tx[i].pcm_buffer == NULL)
            {
                audio_trap(MALLOC_FAILED);
            }
            memset(g_pcmbuf_tx[i].pcm_buffer, 0, malloc_size);
        }
        g_pcmbuf_tx_malloc = EXECUTED_MALLOC;

#if OUTPUT_SIGNAL_FROM == SINE_WAVE
        memcpy(g_pcmbuf_tx[g_idx_buf_tx].pcm_buffer, test_wave, malloc_size);
#endif
    }

    /* Powered speakers */
    g_ssi_ret = R_SSI_Open(SSI_CH1);
    if ( SSI_SUCCESS == g_ssi_ret )
    {
        /* PCM buffer information initial setting before start operation */
        /* first transmit PCM buffer information setting */
        g_idx_buf_tx = 0;
        set_buf_info( &buf_info_tx[CURRENT], g_pcmbuf_tx[g_idx_buf_tx++].pcm_buffer, BUFSIZE );
        g_idx_buf_tx %= BUFNUM;

        /* configures SSI1 pins SSI functions */
        R_SSI_PinSet_SSIE1();

        /* Powered speakers output enable */
        g_ssi_ret = R_SSI_Start(SSI_CH1);
        if ( SSI_SUCCESS != g_ssi_ret )
        {
            audio_trap(SSI_START_ERROR_CH1);
        }
    }
    else
    {
        audio_trap(SSI_OPEN_ERROR_CH1);
    }
} /* End of function ssi_start_for_speakers */

/**********************************************************************************************************************
 * Function Name: ssi_write
 * Description  : Write data to the TX FIFO when the TX FIFO is empty.
 * Arguments    : none
 * Return Value : none
 *********************************************************************************************************************/
static void ssi_write( void )
{
    int8_t ssi_ret;
    uint16_t samples;   /* 1sample = stareo */
    uint8_t write_buff_index;
    byteq_err_t err_code;
    uint8_t write_buf[I2S_PCM_WIDTH];
    const void *write_buff;

    write_buf[0] = 0x00;
    for(write_buff_index = 0; write_buff_index < (I2S_PCM_WIDTH -1); write_buff_index++)
    {
        err_code = R_BYTEQ_Get(g_byte_que, &write_buf[write_buff_index]);
        if (err_code == BYTEQ_ERR_QUEUE_EMPTY)
        {
        	g_d2audio_play_request = 0;
        	break;
        }
    }
    if (err_code == BYTEQ_SUCCESS)
    {
        ssi_ret = R_SSI_Write(SSI_CH1, (const void *)&write_buf[0], (uint8_t)I2S_PCM_WIDTH );
        if ( ssi_ret <= 0 )
        {
        	g_d2audio_play_request = 0;
        }
    }
} /* End of function ssi_write */

#if OUTPUT_SIGNAL_FROM == MEMS_MIC
/**********************************************************************************************************************
 * Function Name: ssi_read
 * Description  : Read data from the RX FIFO when the RX FIFO is full.
 * Arguments    : none
 * Return Value : none
 *********************************************************************************************************************/
static void ssi_read( void )
{
    int8_t ssi_ret;
    uint16_t samples;   /* 1sample = stareo */

    /* gets the number to write from buf_info_rx[CURRENT]. */
    samples = buf_info_rx[CURRENT].pcm_samples;     /* This value remains when limiting is not needed */

    /* checks "samples" if larger than limit, limits the number. */
    if ( samples >= 4u )    /* case 24 bit PCM data */
    {
        samples = 4u;
    }

    /* write PCM data to RX FIFO */
    if ( samples > 0 )
    {
        ssi_ret = R_SSI_Read(SSI_CH0, (void *)buf_info_rx[CURRENT].p_buf_start, (uint8_t)samples );

        /* update PCM buffer information */
        if ( ssi_ret >= 0 )
        {
            buf_info_rx[CURRENT].p_buf_start += (ssi_ret * CHANNELS);
            buf_info_rx[CURRENT].pcm_samples -= (uint16_t)ssi_ret;
        }
        else 
        {
            audio_trap(SSI_READ_FAILED);
        }
    }

    /* check remaind current samples
        if no samples remaind, sets next buffer, and requests next PCM buffer information */
    if ( 0u == buf_info_rx[CURRENT].pcm_samples )    /* are all samples transferred ? */
    {
        if ( NO_REQUEST == buf_info_request_rx )     /* is PCM buffer information updated ? */
        {
            buf_info_rx[CURRENT] = buf_info_rx[NEXT];/* loads next buffer information */
            buf_info_request_rx = DATA_REQUEST;      /* requests next PCM buffer information */
        }
        else
        {
            audio_trap(PCM_BUFFER_ERROR);
        }
    }
} /* End of function ssi_read */
#endif /* OUTPUT_SIGNAL_FROM == MEMS_MIC */

/**********************************************************************************************************************
 * Function Name: ssi_stop
 * Description  : 
 * Arguments    : none
 * Return Value : none
 *********************************************************************************************************************/
static void ssi_stop(void)
{
#if OUTPUT_SIGNAL_FROM == MEMS_MIC
    /*** PCM data transfer stop operation ***/
    if ( SSI_SUCCESS != R_SSI_Stop( SSI_CH0 ) )
    {
        audio_trap(SSI_STOP_ERROR_CH0);
    }
#endif
    if ( SSI_SUCCESS != R_SSI_Stop( SSI_CH1 ) )
    {
        audio_trap(SSI_STOP_ERROR_CH1);
    }

    /* set ports output "0", before SSI0 closed. */
//    init_port_ssi0();

    /* SSI close operation */
#if OUTPUT_SIGNAL_FROM == MEMS_MIC
    if ( SSI_SUCCESS != R_SSI_Close( SSI_CH0 ) )
    {
        audio_trap(SSI_CLOSE_ERROR_CH0);
    }
#endif

    if ( SSI_SUCCESS != R_SSI_Close( SSI_CH1 ) )
    {
        audio_trap(SSI_CLOSE_ERROR_CH1);
    }
} /* End of function ssi_stop */

/**********************************************************************************************************************
 * Function Name: cmt_callback
 * Description  : Callback function for CMT.
 * Arguments    : void * arg : pointer to channel data.
 * Return Value : none
 *********************************************************************************************************************/
void cmt_callback( void *arg )
{
    ssi_regs_t *p_ssi_reg;

    led_blink();

    if (g_d2audio_play_request == 1)
    {
        /* Check Transmit Data Empty Flag. */
        p_ssi_reg = (ssi_regs_t *)&SSIE1;
        if(1 == p_ssi_reg->SSIFSR.BIT.TDE)
        {
            ssi_write();
        }
    }
} /* End of function cmt_callback */

/**********************************************************************************************************************
 * Function Name: set_buf_info
 * Description  : buffer information setting.
 * Arguments    : buf_info_t * p_buf_info : pointer to PCM buffer information
 *              : int16_t * p_buf         : pointer to PCM buffer
 *              : uint16_t pcm_samples    : number of PCM samples
 * Return Value : none
 *********************************************************************************************************************/
static void set_buf_info( const buf_info_t * p_buf_info, const int32_t * p_buf, const uint16_t pcm_samples )
{
    buf_info_t * p_info;

    p_info = (buf_info_t *)p_buf_info;
    p_info->p_buf_start = (int32_t *)p_buf;
    p_info->pcm_samples = pcm_samples;
} /* End of function set_buf_info() */

/**********************************************************************************************************************
 * Function Name: audio_trap
 * Description  : Trap function.
 * Arguments    : audio_error_t error_code : error code
 * Return Value : none
 *********************************************************************************************************************/
static void audio_trap( audio_error_t error_code )
{
    g_error_code = error_code;
    while(1)
    {
        R_BSP_NOP();
    }
} /* End of function cmt_callback */

/**********************************************************************************************************************
 * Function Name: led_blink
 * Description  : Blink the LED.
 * Arguments    : none
 * Return Value : none
 *********************************************************************************************************************/
static void led_blink( void )
{
    g_cmt_cb_cnt++;

    if(g_cmt_cb_cnt > LED_CYCLE_COUNT)
    {
        if(PORT4.PIDR.BIT.B0 == 1)
        {
            PORT4.PODR.BIT.B0 = 0;
        }
        else
        {
            PORT4.PODR.BIT.B0 = 1;
        }
        g_cmt_cb_cnt = 0;
    }
} /* End of function led_blink */

/**********************************************************************************************************************
 * Function Name: d2audio_play_start
 * Description  : 
 * Arguments    : none
 * Return Value : none
 *********************************************************************************************************************/
void d2audio_play_start(void)
{
	uint16_t read_size = 0;
	uint32_t ret_code;
	uint8_t buff_count;
    byteq_err_t err_code;

    if (g_d2audio_play_request == 0)
    {
        err_code = R_BYTEQ_Open(g_pcm_buf_ptr, g_pcm_buf_size, &g_byte_que);
    	if (err_code != BYTEQ_SUCCESS)
    	{
    		audio_trap(BYTEQ_OPEN_ERROR);
    	}
    	for (buff_count = 0; buff_count<READ_NUM; buff_count++)
    	{
        	ret_code = d2audio_read_data(&g_read_buff[0], (READ_SIZE), &read_size);
        	if (ret_code == 0)
        	{
            	ret_code = d2audio_Initial_data_set(&g_read_buff[0], read_size);
            	if (ret_code != 0)
            	{
            		break;
            	}
        	}
    	}
    	if ((read_size > 0) && (ret_code == 0))
    	{
            g_d2audio_play_request = 1;
    	}
    }
} /* End of function d2audio_play_start */

uint32_t d2audio_Initial_data_set(uint8_t *read_buff, uint16_t read_size)
{
	uint32_t ret_code = 0;
	uint16_t read_count;
	byteq_err_t err_code;
	uint8_t set_data;
	uint8_t surplus;

	err_code = BYTEQ_SUCCESS;
	for (read_count = 0; read_count < read_size; read_count++ )
	{
		set_data = *(read_buff + read_count);
		err_code = R_BYTEQ_Put(g_byte_que, set_data);
		if (err_code == BYTEQ_ERR_QUEUE_FULL)
		{
			ret_code = 1;
			break;
		}
	}
	return ret_code;
}

int32_t d2audio_data_read(void)
{
	uint16_t read_size = 0;
	uint16_t que_no_used_size;
	int32_t ret_code = 0;
	uint8_t buff_count;
    byteq_err_t err_code;
    uint16_t que_space;

	ret_code = d2audio_read_data(&g_read_buff[0], (READ_SIZE), &read_size);
	if ((ret_code == 0) || (read_size > 0))
	{
    	ret_code = d2audio_data_set(&g_read_buff[0], read_size);
	}
	return ret_code;

} /* End of function d2audio_play_start */

int32_t d2audio_data_set(uint8_t *read_buff, uint16_t read_size)
{
	int32_t ret_code = 0;
	uint16_t read_count;
	byteq_err_t err_code;
	uint8_t set_data;
	uint8_t surplus;

	for (read_count = 0; read_count < read_size; read_count++ )
	{
		set_data = *(read_buff + read_count);
		err_code = R_BYTEQ_Put(g_byte_que, set_data);
		if (err_code == BYTEQ_ERR_QUEUE_FULL)
		{
			ret_code = -1;
			break;
		}
	}
	return ret_code;
}



/**********************************************************************************************************************
 * Function Name: d2audio_play_stop
 * Description  : 
 * Arguments    : none
 * Return Value : none
 *********************************************************************************************************************/
void d2audio_play_stop(void)
{
    if (g_d2audio_play_request == 1)
    {
        g_d2audio_play_request = 0;
    }
} /* End of function d2audio_play_stop */

/**********************************************************************************************************************
 * Function Name: d2audio_record_start
 * Description  : 
 * Arguments    : none
 * Return Value : none
 *********************************************************************************************************************/
void d2audio_record_start(void)
{
    if (g_d2audio_record_request == 0)
    {
        g_d2audio_record_request = 1;
    }
} /* End of function d2audio_record_start */

/**********************************************************************************************************************
 * Function Name: d2audio_record_stop
 * Description  : 
 * Arguments    : none
 * Return Value : none
 *********************************************************************************************************************/
void d2audio_record_stop(void)
{
    if (g_d2audio_record_request == 1)
    {
        g_d2audio_record_request = 0;
    }
} /* End of function d2audio_record_stop */

