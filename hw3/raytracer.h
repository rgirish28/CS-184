#include <iostream>
#include "algebra3.h"
#include "FreeImage.h"
#include <vector>
#include <string>


class Scene;
class Sampler;
class Film;
class Sphere;
class Raytracer;
class Ray;
class Camera;
class Triangle;
class Light;


class Sampler
{
	vec2 count;
	
public:
	Sampler();
	bool generateSample(vec2* sample);
	
};


class Film{
private:
	vec3 colors[480][640];
public:
	Film(vec2 pixels);
	void write();
	void commit(vec2 sample, vec3 color);
};

class Scene  
{

	private:
		vec2 pixels;
		string name;
		vector<Sphere> spheres;
		vector<Triangle> triangles;
		vector<Light> lights;
		Sampler* sampler;
		Film* film;
		Raytracer *raytracer;
		Camera *camera;
		double maxdepth;
	
	
	public:
		Scene();
		void render();
	  double getX();
		double getY();
		vector<Sphere> getSpheres();
		vector<Triangle> getTriangles();
		void readfile(char *file);
		bool readvals(stringstream &s, const int numvals, double* values);
		vector<Light> getLights();
		double getMaxdepth(){return maxdepth;}
		string getOutput(){return name;}
	 
};

class Sphere
{
private:
	vec3 center;
	double radius;
	vec3 ambient, diffuse, specular, emission;
	double shininess; 
	mat4 transform;
public:
	Sphere(vec3 center, double radius);
	bool intersect(Ray r, double* t);
	void setBSDF(vec3 ambient, vec3 diffuse, vec3 specular, vec3 emission, double shininess);
	vec3 getSpecular(){return specular;}
	void setTransform(mat4 transform);
  vec3 getCenter(){return center;}
	mat4 getTransform(){return transform;}
	vec3 getColor(vec3 point, vec3 normal, vec3 origin);
	bool trace(Ray ray, double distance);
};
	
class Raytracer{
	
public:
	Raytracer();
	void trace(Ray ray, vec3* color,vec3 component, double depth);
};

class Ray{
private:
	vec3 pos;
	vec3 dir;
	float t_min, t_max;

public:
  Ray() { }
	Ray(vec3 pos, vec3 dir);
  vec3 getP() { return pos; }
  vec3 getD() { return dir; }

  void addP(vec3 P) { pos = P; }
  void addD(vec3 D) { dir = D; }
};

class Camera{
private:
	vec3 eye;
	vec3 center;
	double fovy; 
	vec3 up;
	
public:
	Camera(vec3 eye, vec3 center,vec3 up, double fovy);
	void generateRay(vec2& sample, Ray* ray);
	
};

class Triangle{
private:
	vec3 point1,point2,point3;
	vec3 ambient, diffuse, specular, emission;
	double shininess; 
public:
	Triangle(){}
	Triangle(vec3 point1,vec3 point2,vec3 point3);
	bool intersect(Ray r, double* t);
	void setBSDF(vec3 ambient, vec3 diffuse, vec3 specular, vec3 emission, double shininess);
	vec3 getSpecular(){return specular;}
	vec3 getColor(vec3 point, vec3 origin);
	bool trace(Ray ray,double distance);
	vector<vec3> getPoints(){ vector<vec3> points; points.push_back(point1);points.push_back(point2);points.push_back(point3); return points;}
};

class Light{
private:
	vec3 position;
	vec3 color;
	int type;
	vec3 attenuation;
public:
	Light(vec3 posn, vec3 col, int kind, vec3 att) {position = posn; color = col; type = kind; attenuation = att;}
	Light(vec3 posn, vec3 col, int kind) {position = posn; color = col; type = kind;}
	vec3 getPos(){return position;}
	vec3 getColor(){return color;}
	int getType(){return type;}
	vec3 getAtt(){return attenuation;}
	
};



extern Scene *scene;