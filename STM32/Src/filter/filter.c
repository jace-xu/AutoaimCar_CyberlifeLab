#include "filter.h"

#define 	MOTOR_ALPHA    0.04

/*
	@brief 一阶低通滤波器
*/
float LowPassfilter_First(LowPassfilter *LowPassfilter,float data){
	float output = LowPassfilter->alpha * data + (1 - LowPassfilter->alpha)*LowPassfilter->lastData;
	LowPassfilter->lastData = output;
	return output;
}

LowPassfilter motor0_omiga_filter = {0,MOTOR_ALPHA};
LowPassfilter motor1_omiga_filter = {0,MOTOR_ALPHA};


// Q=0.1 R=1.0 为初始推荐值
// Q 增大 → 更信任测量值，跟踪更快但更抖
// R 增大 → 更信任预测值，更平滑但响应变慢
KalmanFilter motor0_kalman = {.x=0, .p=1.0f, .q=0.1f, .r=1.0f};
KalmanFilter motor1_kalman = {.x=0, .p=1.0f, .q=0.1f, .r=1.0f};

/*
	@brief 一维卡尔曼滤波器
*/
float KalmanFilter_Update(KalmanFilter *kf, float measurement){
	// 预测步
	float p_pred = kf->p + kf->q;

	// 卡尔曼增益
	float k = p_pred / (p_pred + kf->r);

	// 更新步
	kf->x = kf->x + k * (measurement - kf->x);
	kf->p = (1.0f - k) * p_pred;

	return kf->x;
}
