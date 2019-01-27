using System;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ImageGenerator
{
    class Program
    {
        static void Main(string[] args)
        {
            string fileName = @"C:\Users\alawi\Documents\RayTracer\RayTracer\MyImage.bin";
            byte[] _imageBuffer = File.ReadAllBytes(fileName);
            float[] floats = new float[_imageBuffer.Length / 4];

            Buffer.BlockCopy(_imageBuffer, 0, floats, 0, _imageBuffer.Length);
            
            unsafe
            {
                fixed (byte* ptr = _imageBuffer)
                {
                    using (Bitmap image = new Bitmap(200, 200))
                    {
                        for (int y = 0; y < 200; ++y)
                        {
                            for (int x = 0; x < 200; ++x)
                            {
                                image.SetPixel(x, y, Color.FromArgb(
                                    255, 
                                    (int)(floats[y * 200 * 3 + x * 3] * 255),
                                    (int)(floats[y * 200 * 3 + x * 3 + 1] * 255),
                                    (int)(floats[y * 200 * 3 + x * 3 + 2] * 255)));
                            }
                        }
                        image.Save(@"greyscale.png");
                    }
                }
            }
        }
    }
}
