#include "kalman.h"

// Q=0.1 R=1.0 为初始推荐值，Q/R 比值决定响应速度与平滑度的权衡
// Q 增大 → 更信任测量值，跟踪更快但更抖
// R 增大 → 更信任预测值，更平滑但响应变慢
KalmanFilter motor0_kalman = {.x=0, .p=1.0f, .q=0.1f, .r=1.0f};
KalmanFilter motor1_kalman = {.x=0, .p=1.0f, .q=0.1f, .r=1.0f};

float KalmanFilter_Update(KalmanFilter *kf, float measurement){
	// 预测步：状态不变，协方差增加过程噪声
	float p_pred = kf->p + kf->q;

	// 卡尔曼增益：p_pred 大（不确定）则更信任测量值
	float k = p_pred / (p_pred + kf->r);

	// 更新步：融合预测与测量
	kf->x = kf->x + k * (measurement - kf->x);
	kf->p = (1.0f - k) * p_pred;

	return kf->x;
}
