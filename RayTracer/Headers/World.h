#pragma once


// This file contains the declaration of the class World
// The World class does not have a copy constructor or an assignment operator, for the followign reasons:

// 1 	There's no need to copy construct or assign the World
// 2 	We wouldn't want to do this anyway, because the world can contain an arbitray amount of data
// 3 	These operations wouldn't work because the world is self-referencing:
//	 	the Tracer base class contains a pointer to the world. If we wrote a correct copy constructor for the 
// 	  	Tracer class, the World copy construtor would call itself recursively until we ran out of memory. 


#include <vector>

#include "ViewPlane.h"
#include "RGBColor.h"
#include "Tracer.h"
#include "Primitives/Sphere.h"

#include <memory>


using namespace std;

class RenderThread; 	//part of skeleton - wxRaytracer.h


class World {	
	public:
	
		ViewPlane					vp;
		RGBColor					background_color;
		std::unique_ptr<Tracer>	tracer_ptr;
		Sphere 						sphere;		// for Chapter 3 only

	public:
	
		World();												
		
		void 					
		build();

		void 												
		render_scene() const;
			
		void
		display_pixel(const int row, const int column, const RGBColor& pixel_color) const;
};