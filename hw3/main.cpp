#include <iostream>
#include "raytracer.h"
#include "algebra3.h"
#include "FreeImage.h"

using namespace std;

Scene *scene;

int main(int argc, char** argv){
	
	FreeImage_Initialise();
	
	if(argc<2){
		cout<<"No scene file!"<<endl;
		exit(1);
	}
	
	scene =  new Scene();
	scene->readfile(argv[1]);
	
	scene->render();
	
	
	
}