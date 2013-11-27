#include <iostream>
#include "algebra3.h"
#include "raytracer.h"
#include "FreeImage.h"
#include <math.h>



Camera::Camera(vec3 eye, vec3 center, vec3 up, double fovy)
{
	this->eye = eye;
	this->center = center;
	this->fovy = fovy*M_PI/180;
	this->up = up; 
}


void Camera::generateRay(vec2& sample, Ray* ray){
  
	
	double z = tan(fovy / 2);
  if (z == 0) {
	exit(1);
  }
  z = (1 / z) * scene->getY();

  double fovx = 2 * atan(scene->getX()/ z);
	
	
  double width = scene->getX();
  double height = scene->getY();
		
		
		
	vec3 w = (eye-center).normalize();
	vec3 u = (w^up).normalize();
	vec3 v = (w^u);
  
	double alpha = -(tan(fovx/2) * (sample[1] - (width / 2))) / (width / 2);
  double beta = (tan(fovy / 2) * ((height / 2) - sample[0])) / (height / 2);
	
	vec3 dir = (alpha*u+beta*v-w);
	
	ray->addP(eye);
	ray->addD(dir.normalize());
}


