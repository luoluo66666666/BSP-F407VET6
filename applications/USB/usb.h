#ifndef __USB_H__
#define __USB_H__

#include <rtthread.h>
#include <rtdevice.h>

#define Usb_Thread_Stack_Size 1024 /* usb线程大小 */
#define Usb_Thread_Priority 20     /* usb线程优先级 */
#define Usb_Thread_Timeslice 10    /*  */

#define Usb_Msg_Maxlen 64 /* 单条消息最大长度 */
#define Usb_Msg_Num 10    /* 队列里能缓存的消息数量 */

static rt_device_t usb_dev = RT_NULL;                  /* USB设备句柄 */
static struct rt_messagequeue usb_mq;                  /* USB消息队列控制块 */
static char usb_mq_pool[Usb_Msg_Maxlen * Usb_Msg_Num]; /* 队列缓冲区 */

rt_size_t Usb_Recv(char *buf, rt_size_t len);
int usb_sample(void);

#endif
