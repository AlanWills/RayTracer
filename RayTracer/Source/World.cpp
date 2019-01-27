#include "pch.h"

#include "World.h"
#include "Constants.h"
#include "SingleSphere.h"

#include <fstream>

struct lwrite
{
  unsigned long value;
  unsigned      size;
  lwrite(unsigned long value, unsigned size) :
    value(value), size(size)
  { }
};

//--------------------------------------------------------------------------
inline std::ostream& operator << (std::ostream& outs, const lwrite& v)
{
  unsigned long value = v.value;
  for (unsigned cntr = 0; cntr < v.size; cntr++, value >>= 8)
    outs.put(static_cast <char> (value & 0xFF));
  return outs;
}

// -------------------------------------------------------------------- default constructor

// tracer_ptr is set to NULL because the build functions will always construct the appropriate tracer
// ambient_ptr is set to a default ambient light because this will do for most scenes
// camera_ptr is set to NULL because the build functions will always have to construct a camera
// and set its parameters

World::World(void) :
  background_color(black),
	tracer_ptr(nullptr)
{
}

//------------------------------------------------------------------ render_scene
void World::build()
{
  vp.set_hres(200);
  vp.set_vres(200);
  vp.set_pixel_size(1.0f);
  vp.set_gamma(1.0f);

  background_color = black;
  tracer_ptr.reset(new SingleSphere(this));

  sphere.setCentre(0);
  sphere.setRadius(85);
}

//------------------------------------------------------------------ render_scene

// This uses orthographic viewing along the zw axis

void 												
World::render_scene(void) const {

	RGBColor	pixel_color;	 	
	Ray			ray;					
	int 		hres 	= vp.hres;
	int 		vres 	= vp.vres;
	float		s		= vp.s;
	float		zw		= 100.0;				// hardwired in

	ray.setDirection(Vector3D(0, 0, -1));
	
  std::vector<float> imgdata(vres * hres * 3);

  for (int r = 0; r < vres; r++)			// up
  {
    for (int c = 0; c < hres; c++) 
    {	// across 					
      ray.setOrigin(Point3D(s * (c - hres / 2.0 + 0.5), s * (r - vres / 2.0 + 0.5), zw));
      pixel_color = tracer_ptr->trace_ray(ray);
      imgdata[r * vres * 3 + c * 3 + 0] = pixel_color.r;
      imgdata[r * vres * 3 + c * 3 + 1] = pixel_color.g;
      imgdata[r * vres * 3 + c * 3 + 2] = pixel_color.b;
      /*image.set_pixel(r, c, png::rgb_pixel(
        static_cast<png::byte>(pixel_color.r * 255), 
        static_cast<png::byte>(pixel_color.g * 255), 
        static_cast<png::byte>(pixel_color.b * 255)));*/
    }
  }

  //image.write("output.png");

  //std::ofstream f("MyImage.bin", std::ios::out | std::ios::trunc | std::ios::binary);
  //if (!f) return;

  //// Some basic
  //unsigned long headers_size = 14  // sizeof( BITMAPFILEHEADER )
  //  + 40; // sizeof( BITMAPINFOHEADER )
  //unsigned long padding_size = 0;// (4 - ((hres * 3) % 4)) % 4;
  //unsigned long pixel_data_size = vres * ((hres * 3) + padding_size);

  //// Write the BITMAPFILEHEADER
  //f.put('B').put('M');        
  //f << lwrite(headers_size + pixel_data_size, 4);  // bfSize
  //f << lwrite(0, 2);  // bfReserved1
  //f << lwrite(0, 2);  // bfReserved2
  //f << lwrite(headers_size, 4);  // bfOffBits

  //// Write the BITMAPINFOHEADER
  //f << lwrite(40, 4);  // biSize
  //f << lwrite(hres, 4);  // biWidth
  //f << lwrite(vres, 4);  // biHeight
  //f << lwrite(1, 2);  // biPlanes
  //f << lwrite(24, 2);  // biBitCount
  //f << lwrite(0, 4);  // biCompression=BI_RGB
  //f << lwrite(pixel_data_size, 4);  // biSizeImage
  //f << lwrite(0, 4);  // biXPelsPerMeter
  //f << lwrite(0, 4);  // biYPelsPerMeter
  //f << lwrite(0, 4);  // biClrUsed
  //f << lwrite(0, 4);  // biClrImportant

  //// Write the pixel data
  //for (float fl : imgdata)
  //{
  //  f.write(reinterpret_cast<const char*>(&fl), sizeof(float));
  //}

  //for (int row = 0; row < vres; row++)           // bottom-to-top
  //{
  //  for (int col = 0; col < hres; col++)  // left-to-right
  //  {
  //    //unsigned char value;
  //    //double        d = 0.0;

  //    //// If we haven't overrun the end of our input, convert it to a grayscale value.
  //    //// Input is clamped to the range [0, 1], where 0 --> black and 1 --> white.
  //    //if (begin != end)
  //    //{
  //    //  d = *begin++;

  //    //  if (d < 0.0) d = 0.0;
  //    //  else if (d > 1.0) d = 1.0;

  //    //  value = 255 * d;
  //    //}

  //    // Otherwise we just use the default grayscale value
  //    //else value = 0;
  //    f.write(reinterpret_cast<const char*>(&(imgdata[row * vres + 3 * col + 0])), sizeof(float));
  //    f.write(reinterpret_cast<const char*>(&(imgdata[row * vres + 3 * col + 1])), sizeof(float));
  //    f.write(reinterpret_cast<const char*>(&(imgdata[row * vres + 3 * col + 2])), sizeof(float));
  //  }

  //  if (padding_size) f << lwrite(0, padding_size);
  //}

  std::fstream imgout("MyImage.bin", std::ios::out | std::ios::binary);
  for (int i = 0; i < vres * hres * 3; ++i)
  {
    imgout.write(reinterpret_cast<const char*>(imgdata.data() + i), sizeof(float));
  }

  imgout.close();

 /* FILE * fp = fopen("file.bmp", "wb");
  fwrite(bmpfile_header, sizeof(bmpfile_header), 1, fp);
  fwrite(bmp_dib_v3_header, sizeof(bmp_dib_v3_header_t), 1, fp);

  for (int i = 0; i < 200; i++) {
    for (int j = 0; j < 200; j++) {
      fwrite(&image[j][i][2], 1, 1, fp);
      fwrite(&image[j][i][1], 1, 1, fp);
      fwrite(&image[j][i][0], 1, 1, fp);
    }
  }

  fclose(fp);*/
}