#include <iostream>
#include "algebra3.h"
#include "raytracer.h"
#include "FreeImage.h"
#include <vector>
#include <math.h>

mat3 Transform::rotate( double degrees,  vec3& axis) 
{
	vec3 normaxis = axis.normalize();
	
	float x = normaxis[0];
	float y = normaxis[1];
	float z = normaxis[2];
	
	float theta = degrees*M_PI/180;
	
	
  return mat3( (cos(theta)*mat3(1.0))+ ((1-cos(theta))*mat3(vec3(x*x,x*y,x*z),vec3(x*y,y*y,y*z),vec3(x*z,y*z,z*z)))+sin(theta)*(mat3(vec3(0,z,-y),vec3(-z,0,x),vec3(y,-x,0))));
}


mat4 Transform::scale( double &sx,  double &sy, double &sz) 
{ 
  return mat4(vec4(sx,0,0,0),vec4(0,sy,0,0),vec4(0,0,sz,0),vec4(0,0,0,1));
}

mat4 Transform::translate( double &tx,  double &ty,  double &tz) 
{
  return mat4(vec4(1,0,0,0),vec4(0,1,0,0),vec4(0,0,1,0),vec4(tx,ty,tz,1));
}



Transform::Transform()
{

}

