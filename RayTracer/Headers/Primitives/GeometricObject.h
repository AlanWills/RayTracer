#pragma once

#include "Ray.h"
#include "ShadeRec.h"


class GeometricObject
{
  public:
    virtual bool hit(const Ray& rau, double& tmin, ShadeRec& shadeRec) const = 0;
};