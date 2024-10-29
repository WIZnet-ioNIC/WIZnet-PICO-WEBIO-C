/**
 * @file	userHandler.c
 * @brief	User Control Example
 * @version 1.0
 * @date	2014/07/15
 * @par Revision
 *			2014/07/15 - 1.0 Release
 * @author
 * \n\n @par Copyright (C) 1998 - 2014 WIZnet. All rights reserved.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "userHandler.h"
#include "wizchip_conf.h"
#include "pico/stdlib.h"
#include "httpParser.h"

// Data IO Status
typedef enum
{
	On = 1,
	Off = 0
} IO_Status_Type;

extern uint16_t LED_pin[10];

// Pre-defined Get CGI functions
void make_json_netinfo(uint8_t *buf, uint16_t *len);

// Pre-defined Set CGI functions
int8_t set_diostate(uint8_t *uri);

uint8_t predefined_get_cgi_processor(uint8_t *uri_name, uint8_t *buf, uint16_t *len)
{
	uint8_t ret = 1; // ret = 1 means 'uri_name' matched
	// uint8_t cgibuf[14] = {0, };
	int8_t cgi_dio = -1;
	int8_t cgi_ain = -1;

	if (strcmp((const char *)uri_name, "todo.cgi") == 0)
	{
		// to do
		; // make_json_todo(buf, len);
	}
	else if (strcmp((const char *)uri_name, "get_netinfo.cgi") == 0)
	{
		make_json_netinfo(buf, len);
	}
	else
	{
		if ((cgi_dio < 0) && (cgi_ain < 0))
			ret = 0;
	}

	return ret;
}

uint8_t predefined_set_cgi_processor(uint8_t *uri_name, uint8_t *uri, uint8_t *buf, uint16_t *len)
{
	uint8_t ret = 1; // ret = '1' means 'uri_name' matched
	uint8_t val = 0;

	if (strcmp((const char *)uri_name, "todo.cgi") == 0)
	{
		// to do
		; // val = todo(uri);
		  //*len = sprintf((char *)buf, "%d", val);
	}
	else if (strcmp((const char *)uri_name, "set_diostate.cgi") == 0)
	{
		val = set_diostate(uri);
		*len = sprintf((char *)buf, "%d", val);
	}
	else
	{
		ret = 0;
	}

	return ret;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Pre-defined Get CGI functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void make_json_netinfo(uint8_t *buf, uint16_t *len)
{
	wiz_NetInfo netinfo;
	// ctlnetwork(CN_GET_NETINFO, (void*) &netinfo);
	wizchip_getnetinfo((void *)&netinfo);
	// DHCP: 1 - Static, 2 - DHCP

	*len = sprintf((char *)buf, "NetinfoCallback({\"mac\":\"%02X:%02X:%02X:%02X:%02X:%02X\","
								"\"ip\":\"%d.%d.%d.%d\","
								"\"gw\":\"%d.%d.%d.%d\","
								"\"sn\":\"%d.%d.%d.%d\","
								"\"dns\":\"%d.%d.%d.%d\","
								"\"dhcp\":\"%d\"});",
								netinfo.mac[0], netinfo.mac[1], netinfo.mac[2], netinfo.mac[3], netinfo.mac[4], netinfo.mac[5],
								netinfo.ip[0], netinfo.ip[1], netinfo.ip[2], netinfo.ip[3],
								netinfo.gw[0], netinfo.gw[1], netinfo.gw[2], netinfo.gw[3],
								netinfo.sn[0], netinfo.sn[1], netinfo.sn[2], netinfo.sn[3],
								netinfo.dns[0], netinfo.dns[1], netinfo.dns[2], netinfo.dns[3],
								netinfo.dhcp);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Pre-defined Set CGI functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int8_t set_diostate(uint8_t *uri)
{
	uint8_t *param;
	uint8_t pin = 0, val = 0;
	if ((param = get_http_param_value((char *)uri, "pin"))) // GPIO;
	{
		pin = (uint8_t)ATOI(param, 10);

		if ((param = get_http_param_value((char *)uri, "val"))) // State; high(on)/low(off)
		{
			val = (uint8_t)ATOI(param, 10);
		}
		printf("set_diostate.cgi LED out pin[%d] \r\n", pin);
		printf("%d\r\n", val);
		if (val == On)
		{
			gpio_put(LED_pin[pin], On);
		}
		else
		{
			gpio_put(LED_pin[pin], Off);
		}
	}

	return pin;
}