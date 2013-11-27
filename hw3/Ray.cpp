#include <iostream>
#include "algebra3.h"
#include "raytracer.h"
#include "FreeImage.h"




Ray::Ray(vec3 pos, vec3 dir)
{
	this->pos = pos;
	this->dir = dir; 

	t_min = 0.0;
	t_max = 1.0;
}
