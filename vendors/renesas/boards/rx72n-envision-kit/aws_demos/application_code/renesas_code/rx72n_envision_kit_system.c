
/**********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
 * SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2014(2016) Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : rx72n_envision_kit_system.c
 * Description  : rx72n_envision_kit_system global data
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *         : 18.07.2020 1.00 First Release
 *********************************************************************************************************************/

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/

#include "rx72n_envision_kit_system.h"

const char timezone_label[] = DATAFLASH_LABEL_NAME_TIMEZONE;
const char client_private_key_label[] = DATAFLASH_LABEL_NAME_CLIENT_PRIVATE_KEY;
const char client_certificate_label[] = DATAFLASH_LABEL_NAME_CLIENT_CERTIFICATE;
const char iot_thing_name_label[] = DATAFLASH_LABEL_NAME_IOT_THING_NAME;
const char mqtt_broker_endpoint_label[] = DATAFLASH_LABEL_NAME_MQTT_BROKER_ENDPOINT;
const char code_signer_certificate_label[] = DATAFLASH_LABEL_NAME_CODE_SIGNER_CERTIFICATE;
const char tcp_send_performance_server_ip_address_label[] = DATAFLASH_LABEL_NAME_TCP_SEND_PERFORMANCE_SERVER_IP_ADDRESS;
const char tcp_send_performance_server_port_number_label[] = DATAFLASH_LABEL_NAME_TCP_SEND_PERFORMANCE_SERVER_PORT_NUMBER;
const char tracealyzer_server_ip_address_label[] = DATAFLASH_LABEL_NAME_TRACEALYZER_SERVER_IP_ADDRESS;
const char tracealyzer_server_port_number_label[] = DATAFLASH_LABEL_NAME_TRACEALYZER_SERVER_PORT_NUMBER;

char *iot_thing_name;
char *mqtt_broker_endpoint;
char *signing_certificate_pem;

xSemaphoreHandle xSemaphoreCodeFlashAccess;
