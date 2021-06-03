#ifndef _ROS_SERVICE_Command_h
#define _ROS_SERVICE_Command_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace gpar_nanook
{

static const char COMMAND[] = "gpar_nanook/Command";

  class CommandRequest : public ros::Msg
  {
    public:
      typedef int8_t _vd_type;
      _vd_type vd;
      typedef int8_t _ve_type;
      _ve_type ve;

    CommandRequest():
      vd(0),
      ve(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      union {
        int8_t real;
        uint8_t base;
      } u_vd;
      u_vd.real = this->vd;
      *(outbuffer + offset + 0) = (u_vd.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->vd);
      union {
        int8_t real;
        uint8_t base;
      } u_ve;
      u_ve.real = this->ve;
      *(outbuffer + offset + 0) = (u_ve.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->ve);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      union {
        int8_t real;
        uint8_t base;
      } u_vd;
      u_vd.base = 0;
      u_vd.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->vd = u_vd.real;
      offset += sizeof(this->vd);
      union {
        int8_t real;
        uint8_t base;
      } u_ve;
      u_ve.base = 0;
      u_ve.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->ve = u_ve.real;
      offset += sizeof(this->ve);
     return offset;
    }

    virtual const char * getType() override { return COMMAND; };
    virtual const char * getMD5() override { return "b2018cf626f653053d33c5fd28de23dd"; };

  };

  class CommandResponse : public ros::Msg
  {
    public:

    CommandResponse()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
     return offset;
    }

    virtual const char * getType() override { return COMMAND; };
    virtual const char * getMD5() override { return "d41d8cd98f00b204e9800998ecf8427e"; };

  };

  class Command {
    public:
    typedef CommandRequest Request;
    typedef CommandResponse Response;
  };

}
#endif
