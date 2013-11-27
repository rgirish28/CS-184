#include "raytracer.h"
#include "algebra3.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <vector>
#include <stack>

void Scene::render(){
	
	vec2 sample = vec2(0,0);
	vec3 color;
	Ray ray;
	
	while(sampler->generateSample(&sample)){
		color = vec3(0,0,0);
		camera->generateRay(sample, &ray);
		raytracer->trace(ray, &color,vec3(1,1,1),1.0);
		film->commit(sample,color);
	}
	film->write();
	
}

double Scene::getX(){
	
	return pixels[0];
	
}

double Scene::getY(){
	
	return pixels[1];
	
}

vector<Sphere> Scene::getSpheres(){
	return spheres;
	
}
vector<Triangle> Scene::getTriangles(){
	return triangles;
	
}

vector<Light> Scene::getLights(){
	return lights;
	
}

bool Scene::readvals(stringstream &s, const int numvals, double* values) 
{
  for (int i = 0; i < numvals; i++) {
    s >> values[i]; 
    if (s.fail()) {
      cout << "Failed reading value " << i << " will skip\n"; 
      return false;
    }
  }
  return true; 
}

void Scene::readfile(char* file){
  string str, cmd; 
	vec3 ambient;
	vec3 diffuse;
  vec3 specular;
	double shininess;
	vec3 emission;
	vec3 attenuation = vec3(1,0,0);
	
	vector<vec3> vertices;
	
  ifstream in;
  in.open(file); 
  if (in.is_open()) {
		
   stack <mat4> transfstack; 
   transfstack.push(mat4(identity3D()));
	
	 getline (in, str); 
	 
   while (in) {
		 
		 
     if ((str.find_first_not_of(" \t\r\n") != string::npos) && (str[0] != '#')) {
       // Ruled out comment and blank lines 

       stringstream s(str);
       s >> cmd; 
       double values[10]; // Position and color for light, colors for others
       // Up to 10 params for cameras.  
       bool validinput; // Validity of input 

       if (cmd == "directional") {
         validinput = readvals(s, 6, values); // Position/color for lts.
         if (validinput) {
					 
					 vec3 posn = vec3(values[0],values[1],values[2]);
					 vec3 col = vec3(values[3],values[4],values[5]);
					 vec3 dir = vec3(transfstack.top() * vec4(posn, 0), 3);
					 Light light = Light(dir,col,0); 
					 
					 lights.push_back(light);

         
				 }
         
       }
			 
       else if (cmd == "point") {
         validinput = readvals(s, 6, values); // Position/color for lts.
         if (validinput) {
					 
					 vec3 posn = vec3(values[0],values[1],values[2]);
					 vec3 col = vec3(values[3],values[4],values[5]);
					 vec3 dir = vec3(transfstack.top() * vec4(posn, 1));
					 Light light = Light(dir,col,1,attenuation); 
					 
					 lights.push_back(light);

         
				 }
         
       }
       else if (cmd == "attenuation") {
         validinput = readvals(s, 3, values); // Position/color for lts.
         if (validinput) {
					 
					 attenuation = vec3(values[0],values[1],values[2]);

         
				 }
         
       }
			 
			 else if (cmd == "maxdepth") {
			 	validinput = readvals(s,1,values); 
			  if (validinput) { 
					maxdepth = values[0];
				} 
			}
			 
			 else if (cmd == "size") {
			 	validinput = readvals(s,2,values); 
			  if (validinput) { 
					pixels = vec2(values[0],values[1]);
					film = new Film(pixels); 
				} 
			}
			  else if (cmd == "output") {
			 		s>>name; 
				} 
			 	else if (cmd == "camera") {
          validinput = readvals(s,10,values); 
          if (validinput) { 
            	camera = new Camera( vec3(values[0],values[1],values[2]), vec3(values[3],values[4],values[5]), vec3(values[6],values[7],values[8]), values[9] );
					} 
				}
				
			 	else if (cmd == "sphere") {
          validinput = readvals(s,4,values); 
          if (validinput) { 
						
            Sphere sphere = Sphere( vec3(values[0],values[1],values[2]), values[3]);
						sphere.setBSDF(ambient, diffuse, specular, emission, shininess);
						sphere.setTransform(transfstack.top());
						spheres.push_back (sphere);
						
          } 
				}
			 	else if (cmd == "tri") {
          validinput = readvals(s,3,values); 
          if (validinput) { 
						
						vec4 temp0 = vec4(vertices[(int)values[0]]);
						vec4 temp1 = vec4(vertices[(int)values[1]]);
						vec4 temp2 = vec4(vertices[(int)values[2]]);
						
				 	  vec3 vert0 = vec3(transfstack.top() * temp0);
				 	  vec3 vert1 = vec3(transfstack.top() * temp1);
				 	  vec3 vert2 = vec3(transfstack.top() * temp2);
						
            Triangle triangle = Triangle( vert0, vert1,vert2);
						triangle.setBSDF(ambient, diffuse, specular, emission, shininess);
						triangles.push_back (triangle);
						
          } 
				}
				else if(cmd == "maxverts"){
					validinput = readvals(s,1,values);
					if (validinput){
					}
				}
			 	else if(cmd == "vertex"){
					validinput = readvals(s,3,values);
					if (validinput){
						vec3 v = vec3(values[0],values[1],values[2]);
						vertices.push_back(v);
					}
				}
        else if (cmd == "ambient") {
          validinput = readvals(s, 3, values); // colors 
          if (validinput) {
            ambient = vec3(values[0],values[1],values[2]);
          }
        } else if (cmd == "diffuse") {
          validinput = readvals(s, 3, values); 
          if (validinput) {
            diffuse = vec3(values[0],values[1],values[2]);
          }
        } else if (cmd == "specular") {
          validinput = readvals(s, 3, values); 
          if (validinput) {
            specular = vec3(values[0],values[1],values[2]);
          }
        } else if (cmd == "emission") {
          validinput = readvals(s, 3, values); 
          if (validinput) {
            emission = vec3(values[0],values[1],values[2]);
          }
        } else if (cmd == "shininess") {
          validinput = readvals(s, 1, values); 
          if (validinput) {
            shininess = values[0]; 
          }
				}
        else if (cmd == "translate") {
          validinput = readvals(s,3,values); 
          if (validinput) {

            // YOUR CODE FOR HW 2 HERE.  
            // Think about how the transformation stack is affected
            // You might want to use helper functions on top of file. 
            // Also keep in mind what order your matrix is!
			  
						vec3 t = vec3(values[0],values[1],values[2]);
				

					  mat4 m = transfstack.top() * translation3D(t);
					  transfstack.pop();
					  transfstack.push(m);

          }
        }
        else if (cmd == "scale") {
          validinput = readvals(s,3,values); 
          if (validinput) {

            // YOUR CODE FOR HW 2 HERE.  
            // Think about how the transformation stack is affected
            // You might want to use helper functions on top of file.  
            // Also keep in mind what order your matrix is!
			  
						vec3 t = vec3(values[0],values[1],values[2]);
				

					  mat4 m = transfstack.top() * scaling3D(t);
					  transfstack.pop();
					  transfstack.push(m);
          }
        }
        else if (cmd == "rotate") {
          validinput = readvals(s,4,values); 
          if (validinput) {

            // YOUR CODE FOR HW 2 HERE. 
            // values[0..2] are the axis, values[3] is the angle.  
            // You may want to normalize the axis (or in Transform::rotate)
            // See how the stack is affected, as above.  
            // Note that rotate returns a mat3. 
            // Also keep in mind what order your matrix is!

			  vec3 axis = vec3(values[0],values[1],values[2]);
				double ang = values[3];

			  mat4 m = transfstack.top() *  rotation3D(axis, ang);
			  transfstack.pop();
			  transfstack.push(m);
          }
        }

        // I include the basic push/pop code for matrix stacks
        else if (cmd == "pushTransform") {
          transfstack.push(transfstack.top()); 
        } else if (cmd == "popTransform") {
          if (transfstack.size() <= 1) {
            cerr << "Stack has no elements.  Cannot Pop\n"; 
          } else {
            transfstack.pop(); 
          }
        }  
				
			 	
       else {
         cerr << "Unknown Command: " << cmd << " Skipping \n"; 
       }  
    }
		
		
		 getline (in, str); 
   }
 }
 
 else {
	 cerr << "Unable to Open Input Data File " << file<< "\n"; 
	 throw 2; 
 }


	
}

Scene::Scene(){
	
	sampler = new Sampler();
	raytracer = new Raytracer();
	maxdepth = 5;
}

/*

Scene::Scene(vec3 eye, vec2 output, vec3 up, double fovy,vec3 point1, vec3 point2, vec3 point3)
{
	this->pixels = output;
	sampler = new Sampler();
	film = new Film(this->pixels);
	raytracer = new Raytracer;
	camera = new Camera(eye, vec3(1,0,0), up, fovy);
	triangle= new Triangle(point1, point2, point3);
	triangle1= new Triangle(point1, point3, vec3(-1,+1,0));
	sphere = new Sphere(vec3(0,0,0),1.5);
	
	
}*/