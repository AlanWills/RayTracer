#include "pch.h"

#include "Primitives/Plane.h"


//--------------------------------------------------------------------------------------------------
const double Plane::s_epsilon = 0.001;

//--------------------------------------------------------------------------------------------------
Plane::Plane() :
  m_point(),
  m_normal(0, 1, 0)
{
}

//--------------------------------------------------------------------------------------------------
Plane::Plane(const Point3D& point, const Normal& normal) :
  m_point(point),
  m_normal(normal)
{
}

//--------------------------------------------------------------------------------------------------
bool Plane::hit(const Ray& ray, double& tMin, ShadeRec& shadeRect) const
{
  double t = (m_point - ray.getOrigin()) * m_normal / (ray.getDirection() * m_normal);

  if (t > s_epsilon)
  {
    tMin = t;
    shadeRect.normal = m_normal;
    shadeRect.local_hit_point = ray.getOrigin() + t * ray.getDirection();

    return true;
  }
  else
  {
    return false;
  }
}