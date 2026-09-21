#ifndef MOTORCONTROL_H
#define MOTORCONTROL_H

#include "main.h"
#include "fdcan.h"
#include "pid.h"
#include "vofaDebug.h"
#include "filter.h"

typedef struct{
	float motor_angle;
	float motor_omiga;		 // 单位：rps
	float motor_current;   // 单位：mA
	float motor_temp;
}MOTOR;

extern MOTOR measure_motor0;
extern MOTOR measure_motor1;
extern MOTOR set_motor0;
extern MOTOR set_motor1;

extern PID pid0_omiga;        
extern PID pid1_omiga;

// 设置电流
void set_current(float current0, float current1);

// 速度环轮询控制
void omiga_loopControl(void);

// 设置速度
void set_omiga(float id,float target_omiga);

// 初始化
void motor_initial(void);
#endif
