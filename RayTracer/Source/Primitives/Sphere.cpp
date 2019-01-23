#include "pch.h"

#include "Primitives/Sphere.h"


//--------------------------------------------------------------------------------------------------
const double Sphere::s_epsilon = 0.001;

//--------------------------------------------------------------------------------------------------
Sphere::Sphere() :
  m_centre(),
  m_radius(1)
{
}

//--------------------------------------------------------------------------------------------------
Sphere::Sphere(const Point3D& centre, double radius) :
  m_centre(centre),
  m_radius(radius)
{
}

//--------------------------------------------------------------------------------------------------
bool Sphere::hit(const Ray& ray, double& tMin, ShadeRec& shadeRect) const
{
  Vector3D temp = ray.getOrigin() - m_centre;

  double t = 0;
  double a = ray.getDirection() * ray.getDirection();
  double b = 2 * temp * ray.getDirection();
  double c = temp * temp - m_radius * m_radius;
  double discriminant = b * b - 4 * a * c;

  if (discriminant < 0)
  {
    return false;
  }
  else
  {
    double e = std::sqrt(discriminant);
    double denominator = 2 * a;

    // Smaller root
    t = -(b + e) / denominator;

    if (t > s_epsilon)
    {
      tMin = t;
      shadeRect.normal = (temp + t * ray.getDirection()) / m_radius;
      shadeRect.local_hit_point = ray.getOrigin() + t * ray.getDirection();

      return true;
    }

    // Larger root
    t = (-b + e) / denominator;

    if (t > s_epsilon)
    {
      tMin = t;
      shadeRect.normal = (temp + t * ray.getDirection()) / m_radius;
      shadeRect.local_hit_point = ray.getOrigin() + t * ray.getDirection();

      return true;
    }

    return false;
  }
}