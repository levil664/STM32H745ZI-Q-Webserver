/**
 ******************************************************************************
  * File Name          : LWIP.c
  * Description        : This file provides initialization code for LWIP
  *                      middleWare.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
  
/* Includes ------------------------------------------------------------------*/
#include "lwip.h"
#include "lwip/init.h"
#include "lwip/netif.h"
#if defined ( __CC_ARM )  /* MDK ARM Compiler */
#include "lwip/sio.h"
#endif /* MDK ARM Compiler */
#include "ethernetif.h"

/* USER CODE BEGIN 0 */

#include <stdio.h>
#include "main.h"
#include "usart.h"
#include "lwip/netifapi.h"

/* USER CODE END 0 */
/* Private function prototypes -----------------------------------------------*/
static void ethernet_link_status_updated(struct netif *netif);
/* ETH Variables initialization ----------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/* Variables Initialization */
struct netif gnetif;
ip4_addr_t ipaddr;
ip4_addr_t netmask;
ip4_addr_t gw;

/* USER CODE BEGIN 2 */

void ethernet_status_callback(struct netif *netif)
{
	uint8_t msg[48];
	size_t msg_len;

	if (netif_is_up(netif))
	{
		uint8_t msg_ip[20];
		ipaddr_ntoa_r(&netif->ip_addr, (char* )msg_ip, 20);
		msg_len = sprintf((char*) msg, "STATUS connected @ %s\r\n", (char*) msg_ip);
	}
	else
	{
		 msg_len = sprintf((char *)msg, "STATUS down @ %lu\r\n", HAL_GetTick()/1000);
	}

	HAL_UART_Transmit(&huart3, msg, msg_len, HAL_MAX_DELAY);
}

uint8_t ethernet_ip_check(struct netif *netif)
{
	  osDelay(2000);
	  uint8_t msg[127];

	  if(netif->ip_addr.addr != 0)
	  {
		  char msg_ip[18], msg_mask[18], msg_gate[18];
		  ipaddr_ntoa_r(&netif->ip_addr, msg_ip, 20);
		  ipaddr_ntoa_r(&netif->netmask, msg_mask, 20);
		  ipaddr_ntoa_r(&netif->gw, msg_gate, 20);
		  size_t msg_len = sprintf((char *)msg, "LINK after 2s @ %s; %s; %s\r\n",  msg_ip, msg_mask, msg_gate);
		  HAL_UART_Transmit(&huart3, msg, msg_len, HAL_MAX_DELAY);
		  return 1;
	  }
	  else
	  {
		  size_t msg_len = sprintf((char *)msg, "No IP after 2 seconds\r\n");
		  HAL_UART_Transmit(&huart3, msg, msg_len, HAL_MAX_DELAY);
		  return 0;
	  }
}

/* USER CODE END 2 */

/**
  * LwIP initialization function
  */
void MX_LWIP_Init(void)
{
  /* Initilialize the LwIP stack with RTOS */
  tcpip_init( NULL, NULL );

  /* Set static IP addresses */
  IP4_ADDR(&ipaddr, 192, 168, 1, 3);
  IP4_ADDR(&netmask, 255, 255, 255, 0);
  IP4_ADDR(&gw, 192, 168, 1, 1);

  /* add the network interface (IPv4/IPv6) with RTOS */
  netif_add(&gnetif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &tcpip_input);

  /* Registers the default network interface */
  netif_set_default(&gnetif);

  /* Set the network interface up */
  netif_set_up(&gnetif);

  /* Set the link callback function, this function is called on change of link status */
  netif_set_link_callback(&gnetif, ethernet_link_status_updated);

  /* Create the Ethernet link handler thread */
  osThreadDef(EthLink, ethernet_link_thread, osPriorityBelowNormal, 0, configMINIMAL_STACK_SIZE * 2);
  osThreadCreate(osThread(EthLink), &gnetif);

  /* Add status callback for future status updates */
  netif_set_status_callback(&gnetif, ethernet_status_callback);

  // Check the link status initially
  ethernet_link_status_updated(&gnetif);
}

static void ethernet_link_status_updated(struct netif *netif) 
{
  if (netif_is_up(netif))
  {
    /* USER CODE BEGIN 5 */

    // Do we have an IP?
    if (netif->ip_addr.addr != 0)
    {
      return;
    }

    /* USER CODE END 5 */
  }
  else /* netif is down */
  {
    /* USER CODE BEGIN 6 */

    // Ethernet cable disconnected
    // Release IP
    ipaddr.addr = 0;
    netmask.addr = 0;
    gw.addr = 0;
    netifapi_netif_set_addr(&gnetif, &ipaddr, &netmask, &gw);

    /* USER CODE END 6 */
  }
}

#if defined ( __CC_ARM )  /* MDK ARM Compiler */
/* Serial communication functions remain unchanged */
sio_fd_t sio_open(u8_t devnum)
{
  sio_fd_t sd;
  sd = 0; // dummy code
  return sd;
}

void sio_send(u8_t c, sio_fd_t fd)
{
  /* USER CODE BEGIN 8 */
  /* USER CODE END 8 */
}

u32_t sio_read(sio_fd_t fd, u8_t *data, u32_t len)
{
  u32_t recved_bytes;
  recved_bytes = 0; // dummy code
  return recved_bytes;
}

u32_t sio_tryread(sio_fd_t fd, u8_t *data, u32_t len)
{
  u32_t recved_bytes;
  recved_bytes = 0; // dummy code
  return recved_bytes;
}
#endif /* MDK ARM Compiler */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
