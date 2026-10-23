#include <stdio.h>

typedef struct {
    float kp;
    float ki;
    float kd;
    
    float error;
    float last_error;
    float integral;
} PID_Controller;

PID_Controller motor_pid = {
    .kp = 15.5f, 
    .ki = 0.0f,
    .kd = 1.2f,
    .error = 0,
    .last_error = 0,
    .integral = 0
};

float tinh_toan_pid(float setpoint, float goc_thuc_te, float dt) {
    motor_pid.error = setpoint - goc_thuc_te;
    float p_out = motor_pid.kp * motor_pid.error;
    motor_pid.integral += motor_pid.error * dt;
    float i_out = motor_pid.ki * motor_pid.integral;
    float derivative = (motor_pid.error - motor_pid.last_error) / dt;
    float d_out = motor_pid.kd * derivative;
    motor_pid.last_error = motor_pid.error;
    float output = p_out + i_out + d_out;
    return output;
}