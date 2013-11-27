#include <iostream>
#include "algebra3.h"
#include "raytracer.h"
#include "FreeImage.h"


void Film::commit(vec2 sample, vec3 color){
	colors[(int)sample[0]][(int)sample[1]] = color;
	
}

void Film::write(){
	
	FIBITMAP *bitmap =  FreeImage_Allocate (scene->getX(), scene->getY(), 24);
	
	
	
	if (!bitmap )
	exit (1);
	
	for ( int i =0; i<scene->getX(); i++) {
			for ( int j =0; j<scene->getY(); j++) {
				
				vec3 color = colors[j][i];
				
				RGBQUAD colour;
				colour.rgbRed = color[2]*255.0;
				colour.rgbGreen = color[1]*255.0;
				colour.rgbBlue = color[0]*255.0;
				
				FreeImage_SetPixelColor (bitmap , i , j ,& colour );
	
			}
				
		
	}
	
	
	if ( FreeImage_Save (FIF_PNG, bitmap , scene->getOutput().c_str() , 0))
		cout << "Image s u c c e ssf u l l y saved ! " << endl ;		
}


Film::Film(vec2 pixels)
{
}


