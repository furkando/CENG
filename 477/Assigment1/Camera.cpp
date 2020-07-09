#include "Camera.h"
#include <cstring>

using namespace std;
Camera::Camera(int id,                     // Id of the camera
               const char *imageName,      // Name of the output PPM file
               const Vector3f &pos,        // Camera position
               const Vector3f &gaze,       // Camera gaze direction
               const Vector3f &up,         // Camera up direction
               const ImagePlane &imgPlane) // Image plane parameters
{
     this->id = id;
     this->imgPlane = imgPlane;
     strcpy(this->imageName, imageName);
     this->pos = pos;
     this->gaze = gaze;
     this->up = up;
     m = pos + (gaze * imgPlane.distance);
     u = gaze.Cross(up);
     lu = u * imgPlane.left;
     tv = (up)*imgPlane.top;
     q = m + lu + tv;
}

/* Takes coordinate of an image pixel as row and col, and
 * returns the ray going through that pixel. 
 */
Ray Camera::getPrimaryRay(int col, int row) const
{

     float su = (imgPlane.right - imgPlane.left) * (col + 0.5) / imgPlane.nx;
     float sv = (imgPlane.top - imgPlane.bottom) * (row + 0.5) / imgPlane.ny;
     Vector3f d = (q + (u * su) - (up * sv)) - pos;

     return Ray(pos, d);
}
