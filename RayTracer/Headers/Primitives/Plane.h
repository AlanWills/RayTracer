#pragma once

#include "GeometricObject.h"


class Plane : public GeometricObject
{
  public:
    Plane();
    Plane(const Point3D& point, const Normal& normal);

    bool hit(const Ray& ray, double& tMin, ShadeRec& shadeRect) const override;

  private:
    static const double s_epsilon;

    Point3D m_point;
    Normal m_normal;
};