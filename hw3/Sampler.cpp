#include "algebra3.h"
#include <iostream>
#include "raytracer.h"

bool Sampler::generateSample(vec2* sample){

	if(count[0]<scene->getY()){
		*sample = count;
		count[1] = count[1]+1.0;
		if(count[1]>scene->getX()){
			count[1] = 0.5;
			count[0] = count[0]+1.0;
		}
		return true;
	}
	else 
		return false;	
}


Sampler::Sampler()
{
	count[0] = 0.5;
	count[1] = 0.5;
}
