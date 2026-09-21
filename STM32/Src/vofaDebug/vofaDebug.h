#ifndef VOFADEBUG_H_
#define VOFADEBUG_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
// 包含 main.h 以获取 STM32 HAL 库的核心定义，例如 UART_HandleTypeDef
#include "main.h"
#include "string.h"
#include "usb_device.h"
#include "usbd_cdc_if.h"
#include "motorControl.h"
#include "filter.h"
/* Exported functions prototypes ---------------------------------------------*/
extern uint8_t USB_Rx_Buf[];
/**
  * @brief  通过 USB 发送一个浮点数的二进制数据。
  * @note   这是发送单个数据通道的核心函数。
  *         数据以设备的原生字节序（在 STM32 上是小端 Little-Endian）发送。
  * @param  data: 要发送的 float 类型数据。
  * @retval None
  */
void SendFloat(float data);

/**
  * @brief  通过 USB 发送 VOFA+ JustFloat 协议的帧尾。
  * @note   帧尾是 float 类型的 +Infinity (二进制为 0x00 0x00 80 7F)，
  *         用于标记一帧数据的结束。
  * @retval None
  */
void VOFASend(void);

/**
  * @brief  处理接收到的 USB 数据，并根据协议解析出 kp、ki、kd 参数。
  * @note   该函数需要在主循环中定期调用，以检查是否有新的数据到达。
  *         接收的数据格式假设为：前三个字节为标志（例如 "Kp="），后面紧跟一个 float 类型的值。
  * @retval None
  */
void VOFAReceive(float *kp, float *ki, float *kd);

#ifdef __cplusplus
}
#endif

#endif /* VOFADEBUG_H_ */
