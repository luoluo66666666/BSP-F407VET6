/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-11-06     SummerGift   first version
 */

#include <board.h>
#include <rtdevice.h>
#include <rtthread.h>
#include "usb.h"

/* defined the LED0 pin: PB1 */
#define LED0_PIN GET_PIN(C, 2)
#define LED1_PIN GET_PIN(C, 3)
#define POWER_CTRL_PIN GET_PIN(D, 14)

static void usb_thread_entry(void *arg)
{
}

int main(void)
{
    int count = 1;
    /* set LED0 pin mode to output */
    rt_pin_mode(LED0_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(LED1_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(POWER_CTRL_PIN, PIN_MODE_OUTPUT);
    rt_pin_write(POWER_CTRL_PIN, PIN_HIGH);
    rt_pin_write(POWER_CTRL_PIN, PIN_HIGH);
    char buf[64];
    usb_sample();

    while (count++)
    {
        rt_size_t n = Usb_Recv(buf, sizeof(buf));
        if (n > 0)
        {
            rt_kprintf("recv from usb: %s\n", buf);
        }
        rt_pin_write(LED0_PIN, PIN_HIGH);
        rt_pin_write(LED1_PIN, PIN_HIGH);
        rt_thread_mdelay(500);
        rt_pin_write(LED0_PIN, PIN_LOW);
        rt_pin_write(LED1_PIN, PIN_LOW);
        rt_thread_mdelay(500);

        // rt_device_t dev;
        // dev = rt_device_find("vcom"); // 查找设备
        // rt_device_open(dev, RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX);

        // // 写数据
        // char tx_buf[] = "Hello from RT-Thread USB CDC\n";
        // rt_device_write(dev, 0, tx_buf, sizeof(tx_buf));

        // // 读数据（阻塞/非阻塞）
        // char rx_buf[64];
        // rt_size_t n = rt_device_read(dev, 0, rx_buf, sizeof(rx_buf));

        // rt_pin_write(POWER_CTRL_PIN, PIN_LOW);
    }

    return RT_EOK;
}
