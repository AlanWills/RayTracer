#pragma once

#include "Point3D.h"
#include "Vector3D.h"


class Ray
{
  public:
    Ray();
    Ray(const Point3D& origin, const Vector3D& direction);

    const Point3D& getOrigin() const { return m_origin; }
    void setOrigin(const Point3D& origin) { m_origin = origin; }

    const Vector3D& getDirection() const { return m_direction; }
    void setDirection(const Vector3D& direction) { m_direction = direction; }

  private:
    Point3D m_origin;
    Vector3D m_direction;
};