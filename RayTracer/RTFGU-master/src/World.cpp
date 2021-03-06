// this file contains the definition of the World class


#include "World.h"
#include "Constants.h"


// utilities

#include "Vector3D.h"
#include "Point3D.h"
#include "Point2D.h"
#include "Normal.h"
#include "ShadeRec.h"
#include "Maths.h"

// build functions

#include "IRenderer.h"

#include <stdexcept>

namespace {

    const float ZW = 100.0f;

}


// -------------------------------------------------------------------- default constructor

// tracer_ptr is set to NULL because the build functions will always construct the appropriate tracer
// ambient_ptr is set to a default ambient light because this will do for most scenes
// camera_ptr is set to NULL because the build functions will always have to construct a camera
// and set its parameters

World::World() : renderer(new NullRenderer),
        background_color(BLACK),
        ambient_ptr(new Ambient),
        camera_ptr(NULL) {}



//------------------------------------------------------------------ destructor

World::~World(void) {

    if (ambient_ptr) {
        delete ambient_ptr;
        ambient_ptr = NULL;
    }


    if (camera_ptr) {
        delete camera_ptr;
        camera_ptr = NULL;
    }

    delete_objects();
    delete_lights();
}


//------------------------------------------------------------------ render_scene

// This uses orthographic viewing along the zw axis

void World::render_scene() {
    if (camera_ptr) {
        camera_ptr->render_scene(*this);
        return;
    }

    if (!tracer) {
        throw std::runtime_error("I need a tracer");
    }

    SamplerPtr sampler = vp.get_sampler();
    if (!sampler) {
        throw std::runtime_error("I need a sampler");
    }

    RGBColor	pixel_color;
    Ray			ray;
    ray.d = Vector3D(0, 0, -1);
    const int   NUM_SAMPLES = sampler->get_bundle_size();
    float       pix_size = vp.get_pixel_size();

    for (int r = 0; r < vp.vres; r++) {
        for (int c = 0; c <= vp.hres; c++) {
            pixel_color = background_color;

            const SampleBundle2D& samples = sampler->get_next();
            for ( SampleBundle2D::const_iterator sp = samples.begin();
                    sp != samples.end(); ++sp ) {
                float nx = pix_size * (c - 0.5f * vp.hres + sp->x);
                float ny = pix_size * (r - 0.5f * vp.vres + sp->y);
                ray.o = Point3D(nx, ny, ZW);
                pixel_color += tracer->trace_ray(ray);
            }

            pixel_color /= NUM_SAMPLES;
            display_pixel(r, c, pixel_color);
        }
    }
}


// ------------------------------------------------------------------ clamp

RGBColor
World::max_to_one(const RGBColor& c) const {
    float max_value = max(c.r, max(c.g, c.b));

    if (max_value > 1.0)
        return (c / max_value);
    else
        return (c);
}


// ------------------------------------------------------------------ clamp_to_color
// Set color to red if any component is greater than one

RGBColor
World::clamp_to_color(const RGBColor& raw_color) const {
    RGBColor c(raw_color);

    if (raw_color.r > 1.0 || raw_color.g > 1.0 || raw_color.b > 1.0) {
        c.r = 1.0;
        c.g = 0.0;
        c.b = 0.0;
    }

    return (c);
}


// ---------------------------------------------------------------------------display_pixel
// raw_color is the pixel color computed by the ray tracer
// its RGB floating point components can be arbitrarily large
// mapped_color has all components in the range [0, 1], but still floating point
// display color has integer components for computer display
// the Mac's components are in the range [0, 65535]
// a PC's components will probably be in the range [0, 255]
// the system-dependent code is in the function convert_to_display_color
// the function SetCPixel is a Mac OS function


void World::display_pixel(const int row, const int column, const RGBColor& raw_color) const {
    RGBColor mapped_color;

    if (vp.show_out_of_gamut)
        mapped_color = clamp_to_color(raw_color);
    else
        mapped_color = max_to_one(raw_color);

    if (vp.gamma != 1.0)
        mapped_color = mapped_color.powc(vp.inv_gamma);

    //have to start from max y coordinate to convert to screen coordinates
    int x = column;
    int y = vp.vres - row - 1;

    renderer->render(x, y, (int)(mapped_color.r * 255),
                     (int)(mapped_color.g * 255),
                     (int)(mapped_color.b * 255));
}


ShadeRec World::hit_bare_bones_objects(const Ray& ray) const {
    ShadeRec sr(*this);
    double t, tmin = kHugeValue;
    for (Objects::const_iterator iter = objects.begin(); iter != objects.end(); ++iter ) {
        if ((*iter)->hit(ray, t, sr)) {
            if (t < tmin) {
                sr.hit_an_object = true;
                tmin = t;
                sr.color = (*iter)->get_color();
            }
        }
    }
    return sr;
}


ShadeRec World::hit_objects(const Ray& ray) {

    ShadeRec	sr(*this);
    double		t;
    Normal normal;
    Point3D local_hit_point;
    float		tmin 			= kHugeValue;
    int 		num_objects 	= objects.size();

    for (int j = 0; j < num_objects; j++)
        if (objects[j]->hit(ray, t, sr) && (t < tmin)) {
            sr.hit_an_object	= true;
            tmin 				= t;
            sr.material_ptr     = objects[j]->get_material();
            sr.hit_point 		= ray.o + t * ray.d;
            normal 				= sr.normal;
            local_hit_point	 	= sr.local_hit_point;
        }

    if (sr.hit_an_object) {
        sr.t = tmin;
        sr.normal = normal;
        sr.local_hit_point = local_hit_point;
    }

    return(sr);
}



//------------------------------------------------------------------ delete_objects

// Deletes the objects in the objects array, and erases the array.
// The objects array still exists, because it's an automatic variable, but it's empty

void
World::delete_objects(void) {
    int num_objects = objects.size();

    for (int j = 0; j < num_objects; j++) {
        delete objects[j];
        objects[j] = NULL;
    }

    objects.erase (objects.begin(), objects.end());
}


//------------------------------------------------------------------ delete_lights

void
World::delete_lights(void) {
    int num_lights = lights.size();

    for (int j = 0; j < num_lights; j++) {
        delete lights[j];
        lights[j] = NULL;
    }

    lights.erase (lights.begin(), lights.end());
}

