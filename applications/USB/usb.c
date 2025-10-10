#include "usb.h"

#define RT_DEVICE_FLAG_NONBLOCKING 0
/* USB线程入口函数 */
static void Usb_Thread_Entry(void *arg)
{
    char rx_buf[Usb_Msg_Maxlen];

    /* 查找USB CDC设备 */
    usb_dev = rt_device_find("vcom");
    if (usb_dev == RT_NULL)
    {
        rt_kprintf("usb vcom device not found!\r\n");
        return;
    }

    /* 打开设备（中断接收+非阻塞） */
    if (rt_device_open(usb_dev, RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX | RT_DEVICE_FLAG_NONBLOCKING) != RT_EOK)
    {
        rt_kprintf("open usb vcom failed!\n");
        return;
    }

    rt_kprintf("usb vcom task started\n");

    while (1)
    {
        rt_size_t n = rt_device_read(usb_dev, 0, rx_buf, sizeof(rx_buf));
        if (n > 0)
        {
            if (n >= Usb_Msg_Maxlen)
                n = Usb_Msg_Maxlen - 1;
            rx_buf[n] = '\0';

            /* 发送到队列，如果队列满就丢弃 */
            if (rt_mq_send(&usb_mq, rx_buf, n + 1) != RT_EOK)
            {
                rt_kprintf("usb mq full, drop data\r\n");
            }
        }
        rt_thread_mdelay(5);
    }
}

/* 获取USB数据（非阻塞） */
rt_size_t Usb_Recv(char *buf, rt_size_t len)
{
    if (rt_mq_recv(&usb_mq, buf, len, RT_WAITING_NO) == RT_EOK)
        return rt_strlen(buf);
    return 0;
}

/* 系统初始化函数 */
int usb_sample(void)
{
    /* 初始化队列 */
    if (rt_mq_init(&usb_mq, "usb_mq", usb_mq_pool, Usb_Msg_Maxlen,
                   sizeof(usb_mq_pool), RT_IPC_FLAG_FIFO) != RT_EOK)
    {
        rt_kprintf("usb message queue init failed!\r\n");
        return -1;
    }

    /* 启动USB线程 */
    rt_thread_t tid_usb = rt_thread_create("usb_task",
                                           Usb_Thread_Entry, RT_NULL,
                                           Usb_Thread_Stack_Size,
                                           Usb_Thread_Priority,
                                           Usb_Thread_Timeslice);
    if (tid_usb)
        rt_thread_startup(tid_usb);
    else
        rt_kprintf("usb thread create failed!\r\n");

    rt_kprintf("usb_sample initialized\n");
    return RT_EOK;
}
MSH_CMD_EXPORT(usb_sample, usb device sample);
