//class to measure the robot position
#include"encoder.h"

#ifndef ACA_DEFINITIONS
#define ACA_DEFINITIONS
#define ACA_WHEEL_RADIUS   0.050 //m
#define ACA_WHEEL_DISTANCE 0.100 //m 
#endif

struct Ivector3 {
    float x;
    float y;
    float phi;
};

class odometry
{
private:
    Ivector3 _position{.x=0, .y=0, .phi=0};
    float wheel_radius = ACA_WHEEL_RADIUS;
    float wheel_length = ACA_WHEEL_DISTANCE;
    encoder left_enc, right_enc;
    
public:
    odometry();
    void init(encoder _left_enc, encoder _right_enc, float _wheel_radius, float _wheels_distance);
    Ivector3 getPosition();
    ~odometry();    
};


