#include "ets_sys.h"
#include "driver/uart.h"
#include "driver/i2c.h"
#include "driver/i2c_hd44780.h"
#include "osapi.h"
#include "os_type.h"
#include "user_interface.h"
#include "user_config.h"

#include "user_network.h"
#include <driver/httpclient.h>

volatile uint32_t PIN_IN;
#define DELAY 60000 /* milliseconds */
os_event_t    user_procTaskQueue[user_procTaskQueueLen];
static void user_procTask(os_event_t *events);

static void ICACHE_FLASH_ATTR
user_procTask(os_event_t *events)
{
}

void user_rf_pre_init(void)
{
}


LOCAL os_timer_t hello_timer;
LOCAL void ICACHE_FLASH_ATTR hello_cb(void *arg)
{
	static int counter = 0;

	uint16 adc=system_adc_read();
	ets_uart_printf("Analog #%d!\r\n", adc);

	char url[90] = "http://86.105.52.156/add.php?temp1=";
	char url_data[100];
	os_sprintf(url_data,"http://86.105.52.156/add.php?temp1=%d",adc);
	char numberAsString[5];
		ets_uart_printf("-----------------------------\r\n");
	ets_uart_printf("%s",url_data);
	ets_uart_printf("-----------------------------\r\n");


	//char* light=(char *)system_adc_read();
	//ets_uart_printf(light);
	//char* url_data;

	//strcpy(url_data, light);
	//ets_uart_printf((url_data, light));


	http_get(url_data, "", http_callback_example);
}

void user_init(void)
{
    char ssid[32] = SSID;
    char password[64] = SSID_PASSWORD;
    struct station_config stationConf;

    PIN_IN = GPIO_IN;

    system_timer_reinit();
    // 115200
    UARTInit();

    os_memcpy(&stationConf.ssid, ssid, 32);
    os_memcpy(&stationConf.password, password, 32);

    //Setup wifi
    wifi_set_opmode(0x1);
    wifi_set_phy_mode(PHY_MODE_11N);
    wifi_station_set_config(&stationConf);

    network_init();

    
    os_printf("\n\rStartup done\n\r");

	// Set up a timer to send the message
	// os_timer_disarm(ETSTimer *ptimer)
	os_timer_disarm(&hello_timer);
	// os_timer_setfn(ETSTimer *ptimer, ETSTimerFunc *pfunction, void *parg)
	os_timer_setfn(&hello_timer, (os_timer_func_t *)hello_cb, (void *)0);
	// void os_timer_arm(ETSTimer *ptimer,uint32_t milliseconds, bool repeat_flag)
	os_timer_arm(&hello_timer, DELAY, 1);


}

