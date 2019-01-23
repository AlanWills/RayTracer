#pragma once

#include "GeometricObject.h"


class Sphere : public GeometricObject
{
  public:
    Sphere();
    Sphere(const Point3D& centre, double radius);

    const Point3D& getCentre() const { return m_centre; }
    void setCentre(const Point3D& centre) { m_centre = centre; }

    double getRadius() const { return m_radius; }
    void setRadius(double radius) { m_radius = radius; }

    bool hit(const Ray& ray, double& tMin, ShadeRec& shadeRect) const override;

  private:
    static const double s_epsilon;

    Point3D m_centre;
    double m_radius;
};