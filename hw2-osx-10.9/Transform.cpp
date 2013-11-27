// Transform.cpp: implementation of the Transform class.

// Note: when you construct a matrix using mat4() or mat3(), it will be COLUMN-MAJOR
// Keep this in mind in readfile.cpp and display.cpp
// See FAQ for more details or if you're having problems.

#include "Transform.h"

// Helper rotation function.  Please implement this.  
mat3 Transform::rotate(const float degrees, const vec3& axis) 
{
	vec3 normaxis = glm::normalize(axis);
	
	float x = normaxis.x;
	float y = normaxis.y;
	float z = normaxis.z;
	
	float theta = degrees*pi/180;
	
	
  return mat3( (cos(theta)*mat3(1.0))+ ((1-cos(theta))*mat3(x*x,x*y,x*z,x*y,y*y,y*z,x*z,y*z,z*z))+sin(theta)*(mat3(0,z,-y,-z,0,x,y,-x,0)));
}

void Transform::left(float degrees, vec3& eye, vec3& up) 
{
	eye = eye*rotate(-degrees,up);
}

void Transform::up(float degrees, vec3& eye, vec3& up) 
{
	
	vec3 w = glm::normalize(eye);
	vec3 axis = glm::normalize(glm::cross(up,w));
	
	eye = eye*rotate(degrees,axis);
	up = up*rotate(degrees,axis);
	
}

mat4 Transform::lookAt(const vec3 &eye, const vec3 &center, const vec3 &up) 
{
	vec3 w = glm::normalize(eye-center);
	vec3 u = glm::normalize(glm::cross(up,w));
	vec3 v = glm::cross(w,u);
	
	
  return glm::transpose(mat4(u.x,u.y,u.z,-u.x*eye.x-u.y*eye.y-u.z*eye.z
			 ,v.x,v.y,v.z,-v.x*eye.x-v.y*eye.y-v.z*eye.z
		     ,w.x,w.y,w.z,-w.x*eye.x-w.y*eye.y-w.z*eye.z
		     ,0,0,0,1));

}

mat4 Transform::perspective(float fovy, float aspect, float zNear, float zFar)
{	
	float theta = fovy*pi/360;
	float d = cos(theta)/sin(theta);
  
  return mat4(d/aspect,0,0,0,0,d,0,0,0,0,((-zFar-zNear)/(zFar-zNear)),-1,0,0,((-2*zFar*zNear)/(zFar-zNear)),0);
}

mat4 Transform::scale(const float &sx, const float &sy, const float &sz) 
{ 
  return mat4(sx,0,0,0,0,sy,0,0,0,0,sz,0,0,0,0,1);
}

mat4 Transform::translate(const float &tx, const float &ty, const float &tz) 
{
  return mat4(1,0,0,0,0,1,0,0,0,0,1,0,tx,ty,tz,1);
}

// To normalize the up direction and construct a coordinate frame.  
// As discussed in the lecture.  May be relevant to create a properly 
// orthogonal and normalized up. 
// This function is provided as a helper, in case you want to use it. 
// Using this function (in readfile.cpp or display.cpp) is optional.  

vec3 Transform::upvector(const vec3 &up, const vec3 & zvec) 
{
  vec3 x = glm::cross(up,zvec); 
  vec3 y = glm::cross(zvec,x); 
  vec3 ret = glm::normalize(y); 
  return ret; 
}


Transform::Transform()
{

}

Transform::~Transform()
{

}
