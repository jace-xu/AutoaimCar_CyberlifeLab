#ifndef KALMAN_H
#define KALMAN_H

typedef struct {
	float x;    // 状态估计值
	float p;    // 估计误差协方差
	float q;    // 过程噪声（越大跟踪越快，越小越平滑）
	float r;    // 观测噪声（越大越平滑，越小跟踪越快）
} KalmanFilter;

extern KalmanFilter motor0_kalman;
extern KalmanFilter motor1_kalman;

float KalmanFilter_Update(KalmanFilter *kf, float measurement);

#endif
