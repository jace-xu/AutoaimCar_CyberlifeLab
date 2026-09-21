#include "motorControl.h"


// ����Ƶ��
#define  T									0.001f
// �ٶȻ�pid����
// ���0
#define  Kp_omiga_0    			 300.0f
#define  Ki_omiga_0    			 3.0f
#define  Kd_omiga_0          0.3f
#define  integral_limit_0    200.0f
#define  output_limit_0      2000.0f
// ���1
#define  Kp_omiga_1    			 0.0f
#define  Ki_omiga_1    			 0.0f
#define  Kd_omiga_1    			 0.0f
#define  integral_limit_1    0.0f
#define  output_limit_1      5.0f


MOTOR measure_motor0 = {0};
MOTOR measure_motor1 = {0};
MOTOR set_motor0 = {0};
MOTOR set_motor1 = {0};

PID pid0_omiga = {0};
PID pid1_omiga = {0};


// Ϊ���1,2 (ʹ��ID 0x1FE) ���͵�������ָ��
/**
 * @brief  �� GM6020 ������͵�������ָ�ID 1-4 ��Ӧ 0x1FF��
 * @note   ������Χ��-16384 ~ +16384 ��Ӧ -3000mA ~ +3000mA
 *         ������Χ�ᱻ�Զ�ǯλ������߽�
 */
static void GM6020_SendCurrent()
{
    FDCAN_TxHeaderTypeDef TxHeader;
    uint8_t TxData[8] = {0};
    int16_t current_mA0 = 0;
    int16_t current_mA1 = 0;

    // 1. �������ֵ��mA��ת��ΪЭ��Ҫ���ԭʼ����ֵ��-16384 ~ 16384��
    //    ������ raw = current_mA * (16384.0f / 3000.0f)
    //    ��ǯλ�� [-16384, 16384]
    float scale = 16384.0f / 3000.0f;  // Լ 5.46133

    // ��� 1
    float raw1 = set_motor0.motor_current * scale;
    if (raw1 > 16384.0f) raw1 = 16384.0f;
    if (raw1 < -16384.0f) raw1 = -16384.0f;
    current_mA0 = (int16_t)raw1;

    // ��� 2
    float raw2 = set_motor1.motor_current * scale;
    if (raw2 > 16384.0f) raw2 = 16384.0f;
    if (raw2 < -16384.0f) raw2 = -16384.0f;
    current_mA1 = (int16_t)raw2;

    // 2. ׼�����ݣ���Э��ʹ�ô��ģʽ�����ֽ���ǰ��
    TxData[0] = (uint8_t)(current_mA0 >> 8);   // ���1 ��8λ
    TxData[1] = (uint8_t)(current_mA0 & 0xFF); // ���1 ��8λ
    TxData[2] = (uint8_t)(current_mA1 >> 8);   // ���2 ��8λ
    TxData[3] = (uint8_t)(current_mA1 & 0xFF); // ���2 ��8λ
    // TxData[4]~[7] Ĭ��Ϊ 0����ʾ�����Ƶ��3�����4

    // 3. ���÷���֡ͷ (TxHeader)
    TxHeader.Identifier = 0x1FE;               // ID 
    TxHeader.IdType = FDCAN_STANDARD_ID;       // ��׼֡ (11λID)
    TxHeader.TxFrameType = FDCAN_DATA_FRAME;   // ����֡
    TxHeader.DataLength = FDCAN_DLC_BYTES_8;   // ���ݳ��� 8 �ֽ�
    TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    TxHeader.BitRateSwitch = FDCAN_BRS_OFF;
    TxHeader.FDFormat = FDCAN_CLASSIC_CAN;     // ���� CAN ģʽ
    TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS; // ����¼�����¼�
    TxHeader.MessageMarker = 0;

    // 4. ������Ϣ�� Tx FIFO ����
    if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader, TxData) != HAL_OK)
    {
        Error_Handler();
    }
}



// ���õ������,�ɲ�����
void set_current(float current0, float current1){
	set_motor0.motor_current = current0;
	set_motor1.motor_current = current1;
}

// ���õ��ת��,�ɲ�����
void set_omiga(float id,float target_omiga){
	if(id == 0){
		PID_SetTarget(&pid0_omiga,target_omiga);
		set_motor0.motor_omiga = target_omiga;
	}
	else if(id == 1){
		PID_SetTarget(&pid1_omiga,target_omiga);
		set_motor1.motor_omiga = target_omiga;
	}
	else return;
}



// ��ѯ����(�ٶ�--��������)
void omiga_loopControl(void){
	// ���0
  set_motor0.motor_current =  PID_compute(&pid0_omiga, measure_motor0.motor_omiga, T);
	
	// ���1
	
	// ���͵���ֵ
	GM6020_SendCurrent();
}




// --------------------------------- �����ʼ�� ----------------------------------------------
void motor_initial(void){
	
	// �ٶȻ�pid��ʼ��
	PID_Init(&pid0_omiga, Kp_omiga_0, Ki_omiga_0, Kd_omiga_0, integral_limit_0, output_limit_0);
	PID_Init(&pid1_omiga, Kp_omiga_1, Ki_omiga_1, Kd_omiga_1, integral_limit_1, output_limit_1);
	
	// ---------------------------can��ʼ��---------------------------------------------
	FDCAN_FilterTypeDef sFilterConfig1;

  sFilterConfig1.IdType = FDCAN_STANDARD_ID;
  sFilterConfig1.FilterIndex = 0;
  sFilterConfig1.FilterType = FDCAN_FILTER_MASK;
  sFilterConfig1.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
  sFilterConfig1.FilterID1 = 0x205;      								// ��Ҫ���ƣ�
  sFilterConfig1.FilterID2 = 0x000;      								// ����ȫ0��ȫ������
  sFilterConfig1.RxBufferIndex = 0;

  if (HAL_FDCAN_ConfigFilter(&hfdcan1, &sFilterConfig1) != HAL_OK)
  {
      Error_Handler();
  }

  // ���� FIFO0 Ϊ����ģʽ����ֹ�����
  if (HAL_FDCAN_ConfigRxFifoOverwrite(&hfdcan1, FDCAN_RX_FIFO0, FDCAN_RX_FIFO_OVERWRITE) != HAL_OK)
  {
      Error_Handler();
  }

  // ʹ�� FIFO0 ����Ϣ�жϣ����䵽�ж���0
  if (HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK)
  {
      Error_Handler();
  }
	
		// ���� FDCAN ����
  if (HAL_FDCAN_Start(&hfdcan1) != HAL_OK)
  {
      Error_Handler();
  }

	FDCAN_FilterTypeDef sFilterConfig2;

  sFilterConfig2.IdType = FDCAN_STANDARD_ID;
  sFilterConfig2.FilterIndex = 0;
  sFilterConfig2.FilterType = FDCAN_FILTER_MASK;
  sFilterConfig2.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
  sFilterConfig2.FilterID1 = 0x205;      								// ��Ҫ���ƣ�
  sFilterConfig2.FilterID2 = 0x000;      								// ����ȫ0��ȫ������
  sFilterConfig2.RxBufferIndex = 0;

  if (HAL_FDCAN_ConfigFilter(&hfdcan2, &sFilterConfig2) != HAL_OK)
  {
      Error_Handler();
  }

  // ���� FIFO0 Ϊ����ģʽ����ֹ�����
  if (HAL_FDCAN_ConfigRxFifoOverwrite(&hfdcan2, FDCAN_RX_FIFO0, FDCAN_RX_FIFO_OVERWRITE) != HAL_OK)
  {
      Error_Handler();
  }

  // ʹ�� FIFO0 ����Ϣ�жϣ����䵽�ж���0
  if (HAL_FDCAN_ActivateNotification(&hfdcan2, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK)
  {
      Error_Handler();
  }
	
		// ���� FDCAN ����
  if (HAL_FDCAN_Start(&hfdcan2) != HAL_OK)
  {
      Error_Handler();
  }
	// ----------------------------------------------------------------------------------
	
}


