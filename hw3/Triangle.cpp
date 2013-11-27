#include <iostream>
#include "algebra3.h"
#include "raytracer.h"
#include "FreeImage.h"
#include <math.h>


bool Triangle::intersect(Ray r, double* t){
	
	vec3 normal = (point2-point1)^(point3 - point1);
	
	vec3 n = normal.normalize();
	
	double nDotRay = r.getD()*n;    
	
	
	
	if(nDotRay==0)
		return false;
	
	*t = (point1*n - r.getP()*n)/(nDotRay);

	vec3 P = r.getP() + ((*t)*r.getD());
	
	
	vec3 edge0 = point2 - point1;
	vec3 VP0 = P - point1;
	    vec3 C = edge0^VP0;
	    if (n*C < 0){
	         // P is on the right side
					return false;
				}
	    // edge 1
	    vec3 edge1 = point3 - point2;
	    vec3 VP1 = P - point2;
	    C = edge1^VP1;
	    if (n*C < 0){
					return false;
				}
	    // edge 2
	    vec3 edge2 = point1 - point3;
	    vec3 VP2 = P - point3;
	    C = edge2^VP2;
	    if (n*C < 0){
					return false;
				}
			return true;
}
bool Triangle::trace(Ray ray,double distance){
	
	
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

vec3 Triangle::getColor(vec3 point, vec3 origin){
	
	vector<Light> lights = scene->getLights();
	

	vec3 cols = emission + ambient;
	
	vec3 normal = ((point2-point)^(point3-point)).normalize();
	
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

void Triangle::setBSDF(vec3 ambient, vec3 diffuse, vec3 specular, vec3 emission, double shininess){
	
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
	this->emission = emission;
	this->shininess = shininess;
}


Triangle::Triangle(vec3 point1, vec3 point2, vec3 point3)
{
	this->point1 = point1;
	this->point2 = point2;
	this->point3 = point3;
}


