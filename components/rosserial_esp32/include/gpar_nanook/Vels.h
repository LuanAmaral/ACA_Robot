#ifndef _ROS_gpar_nanook_Vels_h
#define _ROS_gpar_nanook_Vels_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace gpar_nanook
{

  class Vels : public ros::Msg
  {
    public:
      uint32_t vels_length;
      typedef int8_t _vels_type;
      _vels_type st_vels;
      _vels_type * vels;

    Vels():
      vels_length(0), st_vels(), vels(nullptr)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      *(outbuffer + offset + 0) = (this->vels_length >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->vels_length >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->vels_length >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->vels_length >> (8 * 3)) & 0xFF;
      offset += sizeof(this->vels_length);
      for( uint32_t i = 0; i < vels_length; i++){
      union {
        int8_t real;
        uint8_t base;
      } u_velsi;
      u_velsi.real = this->vels[i];
      *(outbuffer + offset + 0) = (u_velsi.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->vels[i]);
      }
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      uint32_t vels_lengthT = ((uint32_t) (*(inbuffer + offset))); 
      vels_lengthT |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1); 
      vels_lengthT |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2); 
      vels_lengthT |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3); 
      offset += sizeof(this->vels_length);
      if(vels_lengthT > vels_length)
        this->vels = (int8_t*)realloc(this->vels, vels_lengthT * sizeof(int8_t));
      vels_length = vels_lengthT;
      for( uint32_t i = 0; i < vels_length; i++){
      union {
        int8_t real;
        uint8_t base;
      } u_st_vels;
      u_st_vels.base = 0;
      u_st_vels.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->st_vels = u_st_vels.real;
      offset += sizeof(this->st_vels);
        memcpy( &(this->vels[i]), &(this->st_vels), sizeof(int8_t));
      }
     return offset;
    }

    virtual const char * getType() override { return "gpar_nanook/Vels"; };
    virtual const char * getMD5() override { return "8021afc0c403e86df61ee0d7e4e54714"; };

  };

}
#endif
