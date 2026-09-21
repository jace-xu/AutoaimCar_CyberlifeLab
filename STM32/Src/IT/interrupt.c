#include "interrupt.h"


// ���can���շ���
void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
    if (RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE)
    {
        FDCAN_RxHeaderTypeDef RxHeader;
        uint8_t RxData[8];

        if (HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &RxHeader, RxData) == HAL_OK)
        {
            // ���� FDCAN1 ������ (���1)
            if (hfdcan->Instance == FDCAN1)
            {
                if (RxHeader.Identifier == 0x205)   // ���1���� ID
                {
                    uint16_t raw_angle = (uint16_t)(RxData[0] << 8) | RxData[1];
                    measure_motor0.motor_angle = (raw_angle / 8192.0f) * 360.0f;

                    int16_t raw_speed = (int16_t)(RxData[2] << 8) | RxData[3];
                    // measure_motor0.motor_omiga = LowPassfilter_First(&motor0_omiga_filter,(float)raw_speed / 60);
                    measure_motor0.motor_omiga = KalmanFilter_Update(&motor0_kalman,(float)raw_speed / 60);

                    int16_t raw_current = (int16_t)(RxData[4] << 8) | RxData[5];
                    measure_motor0.motor_current = (raw_current / 16384.0f) * 3000.0f;

                    measure_motor0.motor_temp = (float)((int8_t)RxData[6]);
                }
            }
            // ���� FDCAN2 ������ (���2)
            else if (hfdcan->Instance == FDCAN2)
            {
                if (RxHeader.Identifier == 0x205)   // ���2���� ID
                {
                    uint16_t raw_angle = (uint16_t)(RxData[0] << 8) | RxData[1];
                    measure_motor1.motor_angle = (raw_angle / 8192.0f) * 360.0f;

                    int16_t raw_speed = (int16_t)(RxData[2] << 8) | RxData[3];
                    // measure_motor1.motor_omiga = LowPassfilter_First(&motor1_omiga_filter,(float)raw_speed / 60);
                    measure_motor1.motor_omiga = KalmanFilter_Update(&motor1_kalman,(float)raw_speed / 60);

                    int16_t raw_current = (int16_t)(RxData[4] << 8) | RxData[5];
                    measure_motor1.motor_current = (raw_current / 16384.0f) * 3000.0f;

                    measure_motor1.motor_temp = (float)((int8_t)RxData[6]);
                }
            }
        }
    }
}


// ��ʱ����ѯ
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim == &htim1){
		
		omiga_loopControl();
	}
}
