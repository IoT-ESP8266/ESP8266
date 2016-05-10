#ifndef __USER_CONFIG_H__
#define __USER_CONFIG_H__

#define SSID "WPbence"
#define SSID_PASSWORD "almafa7493"

#define GPIO_IN 2

#define user_procTaskPrio        0
#define user_procTaskQueueLen    1

#define DATASERVER "sayumi.gigafreak.net"
#define DATASERVERPORT 12346

#define I2C_SLEEP_TIME 100

// SDA on GPIO2
#define I2C_SDA_MUX PERIPHS_IO_MUX_MTMS_U
#define I2C_SDA_FUNC FUNC_GPIO14
#define I2C_SDA_PIN 5

// SCK on GPIO14
#define I2C_SCK_MUX PERIPHS_IO_MUX_MTDI_U
#define I2C_SCK_FUNC FUNC_GPIO12
#define I2C_SCK_PIN 6
#endif
