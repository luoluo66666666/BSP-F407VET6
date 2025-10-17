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
// #include "usb.h"
#include "usbd_core.h"
#include "usbd_cdc_acm.h"

/* defined the LED0 pin: PB1 */
#define LED0_PIN GET_PIN(C, 2)
#define LED1_PIN GET_PIN(C, 3)
#define POWER_CTRL_PIN GET_PIN(D, 14)

static void usb_thread_entry(void *arg)
{
}
extern void cdc_acm_init(uint8_t busid, uintptr_t reg_base);
extern void cdc_acm_data_send_with_dtr_test(uint8_t busid);
extern void cdc_acm_data_send(uint8_t busid, uint8_t *data, uint32_t len);
extern USB_NOCACHE_RAM_SECTION USB_MEM_ALIGNX uint8_t test_buffer[2048];
int main(void)
{
    int count = 1;
    /* set LED0 pin mode to output */
    rt_pin_mode(LED0_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(LED1_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(POWER_CTRL_PIN, PIN_MODE_OUTPUT);
    rt_pin_write(POWER_CTRL_PIN, PIN_HIGH);
    char buf[64];
    // usbd_initialize();
    const char msg[] = "hello cherryusb\r\n";
    memcpy(&test_buffer[0], msg, strlen(msg));

    cdc_acm_init(0, USB_OTG_FS_PERIPH_BASE);
    while (count++)
    {
        cdc_acm_data_send(0, test_buffer, sizeof(msg));
        rt_pin_write(LED0_PIN, PIN_HIGH);
        rt_pin_write(LED1_PIN, PIN_HIGH);
        rt_thread_mdelay(500);
        rt_pin_write(LED0_PIN, PIN_LOW);
        rt_pin_write(LED1_PIN, PIN_LOW);
        rt_thread_mdelay(500);
    }

    return RT_EOK;
}
