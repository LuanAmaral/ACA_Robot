#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_wifi.h"

//ROS libraries
#include "ros.h"
#include "geometry_msgs/Twist.h"
#include "esp_ros_wifi.h"

// Robot definitions
#ifndef ACA_DEFINITIONS
#define ACA_DEFINITIONS
#define ACA_WHEEL_RADIUS   0.050 //m
#define ACA_WHEEL_DISTANCE 0.100 //m 
#endif
//  Motor definitions
#ifndef MOTOR_DEFINITIONS
#define MOTOR_DEFINITIONS
#define MOTOR_LEFT_PIN_FOWARD  1
#define MOTOR_LEFT_PIN_BACKWARD  2
#define MOTOR_RIGHT_PIN_FOWARD  3
#define MOTOR_RIGHT_PIN_BACKWARD 4
#define MOTOR_MAX_VELOCITY 120 //RPM
#endif
// I2C definitions
#ifndef I2C_DEFINITIONS
#define I2C_DEFINITIONS
#define I2C_DATA_PIN (gpio_num_t)21
#define I2C_CLOCK_PIN (gpio_num_t)22
#define I2C_FREQ 500000
#endif

// Encoder definitions
#ifndef ENCODER_DEFINITIONS
#define ENCODER_DEFINITIONS
#define ENC_LEFT_PIN_A (gpio_num_t)5
#define ENC_LEFT_PIN_B (gpio_num_t)6
#define ENC_LEFT_PULSE_REV   20
#define ENC_RIGHT_PIN_A (gpio_num_t)7
#define ENC_RIGHT_PIN_B (gpio_num_t)8
#define ENC_RIGHT_PULSE_REV  20
#endif

#define BLINK_GPIO (gpio_num_t)2

//include peripherals libraries
#include "motor_dc.h"
#include "i2c.h"
#include "GY80.h"
#include "MPU6050.h"
#include "encoder.h"
#include "position.h"


#ifdef __cplusplus
extern "C"
{
    void app_main();
}
#endif

ros::NodeHandle nh;
geometry_msgs::Twist global_position;
ros::Publisher send_aca_position("/position", &global_position);

motorDC motor_dc_left;
motorDC motor_dc_right;

encoder left_encoder;
encoder right_encoder;
odometry position;


void setWheelsVel(const geometry_msgs::Twist &vel)
{
    float v_right = (2*vel.linear.x + vel.angular.z*ACA_WHEEL_DISTANCE)/(2*ACA_WHEEL_RADIUS);
    float v_left  = (2*vel.linear.x - vel.angular.z*ACA_WHEEL_DISTANCE)/(2*ACA_WHEEL_RADIUS);
    motor_dc_right.setVelocity(v_right);
    motor_dc_left.setVelocity(v_left);
}
ros::Subscriber<geometry_msgs::Twist> aca_velocity("/velocity", &setWheelsVel);

void initMotor();
void initOdometry();
void getAndPublishPosition(void *arg);

extern "C" void app_main()
{ 
    nh.initNode();
    initMotor();

    nh.subscribe(aca_velocity);
    nh.advertise(send_aca_position);
    
    xTaskCreate(getAndPublishPosition,
                "getAndPublishPosition",
                2048,
                NULL,
                5,
                NULL);
}

void initMotor()
{
    motor_dc_left.init(MCPWM_UNIT_0, MCPWM0A, MCPWM_TIMER_0, MOTOR_LEFT_PIN_FOWARD, MOTOR_LEFT_PIN_BACKWARD); 
    motor_dc_right.init(MCPWM_UNIT_1, MCPWM1A, MCPWM_TIMER_1, MOTOR_RIGHT_PIN_FOWARD, MOTOR_RIGHT_PIN_BACKWARD); 
    motor_dc_left.setMaxVelocity(MOTOR_MAX_VELOCITY);
    motor_dc_right.setMaxVelocity(MOTOR_MAX_VELOCITY);
}

void initOdometry()
{
    left_encoder.init(ENC_LEFT_PIN_A, ENC_LEFT_PIN_B, ENC_LEFT_PULSE_REV, PCNT_CHANNEL_0, PCNT_UNIT_0);
    right_encoder.init(ENC_RIGHT_PIN_A, ENC_RIGHT_PIN_B, ENC_RIGHT_PULSE_REV, PCNT_CHANNEL_1, PCNT_UNIT_1);
    position.init(left_encoder, right_encoder);
}


void getAndPublishPosition(void *arg)
{
    Ivector3 aca_position; 
    aca_position = position.getPosition();
    global_position.linear.x = aca_position.x;
    global_position.linear.y = aca_position.y;
    global_position.angular.z = aca_position.phi;

    send_aca_position.publish(&global_position);

    nh.spinOnce();
    vTaskDelay(100/portTICK_PERIOD_MS); //10Hz
}