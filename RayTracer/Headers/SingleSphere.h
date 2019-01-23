#ifndef __SINGLE_SPHERE__
#define __SINGLE_SPHERE__

#include "Tracer.h"

class SingleSphere: public Tracer {
	public:
		SingleSphere(World* _worldPtr);					
		
		virtual											
		~SingleSphere(void);
		
		virtual RGBColor	
		trace_ray(const Ray& ray) const;
};

#endif
