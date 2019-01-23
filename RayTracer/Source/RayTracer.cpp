// RayTracer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

#include "World.h"


int main()
{
  World w;
  w.build();
  w.render_scene();

  return 0;
}