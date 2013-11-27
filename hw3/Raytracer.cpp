#include <iostream>
#include "algebra3.h"
#include "raytracer.h"
#include "FreeImage.h"
#include <vector>
#include <math.h>

void Raytracer::trace(Ray ray, vec3* color,vec3 component, double depth){
	
	if(depth>scene->getMaxdepth()){
		
		*color+=vec3(0,0,0);
		return;
		
	}
	
	double t=100000000,s=100000000;
	double t1 =-2;
	int si=-1, ti=-1;
	vec3 finalpointS, finalpointT;

	
	vector<Sphere> spheres = scene->getSpheres();
	vector<Triangle> triangles = scene->getTriangles();
	
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
				
				double dist = sqrt( pow(ray.getP()[0]-point[0],2) + pow(ray.getP()[1]-point[1],2) + pow(ray.getP()[2]-point[2],2) );
				if(dist<s&&t1>0){
					s=dist;
					si = i;
					finalpointS = point;
				}
		}	
	}
	
	for(int i=0;i<triangles.size();i++){
		
		Triangle triangle = triangles[i];
		
		if(triangle.intersect(ray,&t1)){
			
			vec3 point = ray.getP() + t1*ray.getD();
			
			vec3 origin = ray.getP();
			
			double dist = sqrt( pow(origin[0]-point[0],2) + pow(origin[1]-point[1],2) + pow(origin[2]-point[2],2) );
			
			if(dist<t&&t1>0){
				t=dist;
				ti = i;
				finalpointT = point;
			}
		}	
	}
	
	
	if(s<t&&spheres.size()>0){
		
		Sphere sphere = spheres[si];
		
		mat4 inverse = sphere.getTransform().inverse();
		vec3 N = (vec3(inverse*finalpointS)-sphere.getCenter());
		vec3 normal = vec3(inverse.transpose()*(vec4(N,0)),3);
		normal.normalize();

    vec3 specular = sphere.getSpecular();
		
		vec3 specularComp = vec3(component[0]*specular[0],component[1]*specular[1],component[2]*specular[2]);
		
		vec3 pointCol = sphere.getColor(finalpointS,normal,ray.getP());
		
		vec3 mult = vec3(component[0]*pointCol[0],component[1]*pointCol[1],component[2]*pointCol[2]);
    

		*color += mult;
		
		vec3 vpar = (normal * ray.getD().normalize()) * normal;
		vec3 ref = (ray.getD() - 2*vpar).normalize();	
		
		Ray reflected = Ray(finalpointS+0.001*ref,ref);
		
		trace(reflected, color,specularComp, depth+1);
		
		
	}
	else if(s>t&&triangles.size()>0){
		Triangle triangle = triangles[ti];
		
		vector<vec3> points = triangle.getPoints();
		
		vec3 point1 = points[0];
		vec3 point2 = points[1];
		vec3 point3 = points[2];
	
		vec3 normal = ((point2-finalpointT)^(point3-finalpointT)).normalize();
		
    vec3 specular = triangle.getSpecular();
		vec3 specularComp = vec3(component[0]*specular[0],component[1]*specular[1],component[2]*specular[2]);
	
		vec3 pointCol = triangle.getColor(finalpointT,ray.getP());
		
		vec3 mult = vec3(component[0]*pointCol[0],component[1]*pointCol[1],component[2]*pointCol[2]);
    

		*color += mult;
		
		
		vec3 vpar = (normal * ray.getD().normalize()) * normal;
		vec3 ref = (ray.getD() - 2*vpar).normalize();	
		
		Ray reflected = Ray(finalpointT+0.001*ref,ref);
		
		trace(reflected, color,specularComp, depth+1);
		
	}
	if(si==-1&&ti==-1)
		*color += vec3(0,0,0);
	
	
}




Raytracer::Raytracer()
{

}
