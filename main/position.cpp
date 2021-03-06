#include "position.h"
#include "encoder.h"
#include "math.h"

odometry::odometry()
{
}

void odometry::init(encoder _left_enc, encoder _right_enc, float _wheel_radius, float _wheels_distance)
{
    left_enc  = _left_enc;
    right_enc = _right_enc;
    wheel_radius = _wheel_radius;
    wheel_length = _wheels_distance;
}

Ivector3 odometry::getPosition()
{
    float left_dist_ang;
    left_enc.getDistAngularRad(&left_dist_ang);
    float right_dist_ang;
    right_enc.getDistAngularRad(&right_dist_ang);

    float linear_dist = wheel_radius * (left_dist_ang + right_dist_ang) / 2;
    float delta_phi = wheel_radius * (right_dist_ang - left_dist_ang) / (2*wheel_length);

    position.x = position.x + linear_dist * cos(position.phi + delta_phi/2);
    position.y = position.y + linear_dist * sin(position.phi + delta_phi/2);
    position.phi = position.phi + delta_phi;

    return position;
}

odometry::~odometry()
{
    //TODO
}