#include "position.h"
#include "encoder.h"
#include "math.h"

odometry::odometry()
{
}

void odometry::init(encoder _left_enc, encoder _right_enc)
{
    left_enc  = _left_enc;
    right_enc = _right_enc;
}

Ivector3 odometry::getPosition()
{
    float left_dist_ang;
    left_enc.getDistAngularRad(&left_dist_ang);
    float right_dist_ang;
    right_enc.getDistAngularRad(&right_dist_ang);

    float linear_dist = wheel_radius * (left_dist_ang + right_dist_ang) / 2;
    float delta_phi = wheel_radius * (right_dist_ang - left_dist_ang) / (2*wheel_length);

    _position.x = _position.x + linear_dist * cos(_position.phi + delta_phi/2);
    _position.y = _position.y + linear_dist * sin(_position.phi + delta_phi/2);
    _position.phi = _position.phi + delta_phi;

    return _position;
}

odometry::~odometry()
{
    //TODO
}