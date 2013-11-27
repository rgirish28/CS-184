#include <iostream>
#include "algebra3.h"
#include "raytracer.h"
#include "FreeImage.h"
#include <math.h>
#include <algorithm> 

bool Sphere::intersect(Ray r, double* t){
	
	double A = r.getD()*r.getD();
	double B = 2*(r.getD()*(r.getP() - center));
	double C = (r.getP() - center)*(r.getP() - center) - radius*radius;
	
	double disc = B*B - 4*A*C;
	
	if(disc < 0)
		return false;
	
	double t0 = (-B - sqrt(disc))/2*A;
	double t1 = (-B + sqrt(disc))/2*A;
	
	if (t0 > t1)
	    {
	        // if t0 is bigger than t1 swap them around
	        double temp = t0;
	        t0 = t1;
	        t1 = temp;
	    }
			
	if (t1<0)
		return false;
	
	if(t0<0){
		*t = t1;
		return true;
	}
	
	else
	{
		*t = t0;
		return true; 
	}

	

  /* compute q as described above
	 float distSqrt = sqrtf(disc);
  float q;
  if (B < 0)
      q = (-B - distSqrt)/2.0;
  else
      q = (-B + distSqrt)/2.0;

  // compute t0 and t1
  float t0 = q / A;
  float t1 = C / q;
	
	if (t0 > t1)
	    {
	        // if t0 is bigger than t1 swap them around
	        float temp = t0;
	        t0 = t1;
	        t1 = temp;
	    }

	    // if t1 is less than zero, the object is in the ray's negative direction
	    // and consequently the ray misses the sphere
	    if (t1 < 0)
	        return false;

	    // if t0 is less than zero, the intersection point is at t1
	    if (t0 < 0)
	    {
	        *t = t1;
	        return true;
	    }
	    // else the intersection point is at t0
	    else
	    {
	        *t = t0;
	        return true;
	    }
	
			*/
	
}

bool Sphere::trace(Ray ray, double distance){
	
	vector<Sphere> spheres = scene->getSpheres();
	vector<Triangle> triangles = scene->getTriangles();
	
	double t1;
	
	for(int i=0;i<spheres.size();i++){
		
		Sphere sphere = spheres[i];
		
		mat4 transform = sphere.getTransform();
		
		mat4 inverse = transform.inverse();
		
		
		vec4 P = vec4(ray.getP(),1);
		vec4 D = vec4(ray.getD(),0);
		
		vec3 tP = vec3(inverse*P);
		vec3 tD = vec3(inverse*D,3).normalize();
		
		Ray new1 = Ray(tP,tD);
		
		
		
		if(sphere.intersect(new1,&t1)){
			vec3 P1 = tP + t1*tD;
			vec3 point = vec3(transform * vec4(P1),3);
			
			double dist = (ray.getP()-point).length();
			if(dist<distance&&t1>0){
				return false;
			}
				
		}		
	}
	
	for(int i=0;i<triangles.size();i++){
		
		Triangle triangle = triangles[i];
		
		if(triangle.intersect(ray,&t1)){
			vec3 point = ray.getP() + t1*ray.getD();
			double dist = (ray.getP()-point).length();
			if(dist<distance&&t1>0){
				return false;
			}
		}
	}	
	
	return true;
	
}

vec3 Sphere::getColor(vec3 point, vec3 normal, vec3 origin){
	
	vector<Light> lights = scene->getLights();
	

	vec3 cols = emission + ambient;
	
	
	for(int i=0;i<lights.size();i++){
		
		Light l = lights[i];
		vec3 direction;
		vec3 eyedirn = (origin - point).normalize() ; 

		double atten = 1.0;
		
		if(l.getType()==0){
			direction = l.getPos().normalize();
		}
		else{
			direction = (l.getPos()-point).normalize();
			double dist = (l.getPos() - point).length();
			atten = l.getAtt()[0] + l.getAtt()[1] * dist +
			  l.getAtt()[2] * pow(dist, 2);
		
		}
		
		
		vec3 lightPoint = point+0.001*direction;
		
		Ray r = Ray(lightPoint,direction);
		
		double dist = (l.getPos() - point).length();
		
		double v1 = 0.0;
		
				
		if(trace(r,dist))
			v1 = 1.0;
		

		
		vec3 halfvec = (direction + eyedirn).normalize() ; 
		double nDotL = normal*direction  ;        
		
		vec3 diffuseComp = vec3(diffuse[0]*l.getColor()[0],diffuse[1]*l.getColor()[1],diffuse[2]*l.getColor()[2]);
		
			 
    vec3 lambert = v1 *(diffuseComp/atten) * std::max (nDotL, 0.0) ;  

    double nDotH = normal*halfvec ; 
		
		vec3 specularComp = vec3(specular[0]*l.getColor()[0],specular[1]*l.getColor()[1],specular[2]*l.getColor()[2]);
	
    vec3 phong = v1*(specularComp/atten) * pow (std::max(nDotH, 0.0), shininess) ; 

    vec3 retval = lambert + phong ; 
		
		cols = cols+retval;
		
	}
	
	
	return cols;
	
	
}



void Sphere::setBSDF(vec3 ambient, vec3 diffuse, vec3 specular, vec3 emission, double shininess){
	
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
	this->emission = emission;
	this->shininess = shininess;
}


void Sphere::setTransform(mat4 transform){
	this->transform = transform;
}


Sphere::Sphere(vec3 center, double radius)
{
	this->center = center;
	this->radius = radius;
}


