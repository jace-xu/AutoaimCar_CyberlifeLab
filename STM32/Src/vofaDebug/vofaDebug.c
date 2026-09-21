#include "vofaDebug.h"
#include <string.h> // 确保包含了此头文件以使用 memcpy


uint8_t USB_Rx_Buf[64] = {0};
void SendFloat(float data){
	static uint8_t buffer[4];
	memcpy(buffer, &data, sizeof(float));
	uint8_t res;
	uint32_t start = HAL_GetTick();
	/* Try sending; if USB stack is busy, retry briefly */
	do {
		res = CDC_Transmit_HS(buffer, sizeof(float));
		if (res == USBD_OK) break;
	} while ((res == USBD_BUSY) && ((HAL_GetTick() - start) < 10)); 
	(void)res;
	return;
}


void VOFASend(void){
	/* Send +Infinity as frame tail using SendFloat to ensure safe buffer lifetime */
	SendFloat(INFINITY);
}


void VOFAReceive(float *kp, float *ki, float *kd) {
    // 假设 0x4B 0x70 0x3D 对应 'K' 'p' '='
    if((USB_Rx_Buf[0] == 0x4B) && (USB_Rx_Buf[1] == 0x70) && (USB_Rx_Buf[2] == 0x3D)){  
        // 【修改1：使用 memcpy 替代指针直接强转】
        memcpy(kp, USB_Rx_Buf + 3, sizeof(float)); 
        
        // 改变pid系数重置一下
        ResetPID(&pid0_omiga);
        ResetPID(&pid1_omiga);
        
        // 【修改2：清除标志位，防止死循环重复执行】
        memset(USB_Rx_Buf, 0, sizeof(USB_Rx_Buf)); // 或者简单粗暴 USB_Rx_Buf[0] = 0;
        return;
    }
    else if ((USB_Rx_Buf[0] == 0x4B) && (USB_Rx_Buf[1] == 0x69) && (USB_Rx_Buf[2] == 0x3D)) {
        memcpy(ki, USB_Rx_Buf + 3, sizeof(float));
        
        // 改变pid系数重置一下
        ResetPID(&pid0_omiga);
        ResetPID(&pid1_omiga);
        
        memset(USB_Rx_Buf, 0, sizeof(USB_Rx_Buf));
        return;
    }
    else if ((USB_Rx_Buf[0] == 0x4B) && (USB_Rx_Buf[1] == 0x64) && (USB_Rx_Buf[2] == 0x3D)) {
        memcpy(kd, USB_Rx_Buf + 3, sizeof(float));
        
        // 改变pid系数重置一下
        ResetPID(&pid0_omiga);
        ResetPID(&pid1_omiga);
        
        memset(USB_Rx_Buf, 0, sizeof(USB_Rx_Buf));
        return;
    }
}