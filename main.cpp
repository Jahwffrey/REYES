//John Thomason
//ID 3058344

#include "Ri.h"
#include "JRi.h"
#include "shaders.h"
#include <iostream>
#include <math.h>

//THING TO REMEMBER TO DO:!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//TEXTURES!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//TEXTURES!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//TEXTURES!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//TEXTURES!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//TEXTURES!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//TEXTURES!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//TEXTURES!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//TEXTURES!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//TEXTURES!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//TEXTURES!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//TEXTURES!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//TEXTURES!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//TEXTURES!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//TEXTURES!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//I think my coords are still weird -> z is still backwards!!
//make sure RiBegin correclty sets all defaults including clipping planes
//Make everything work by degrees as opposed to radians
//implement near culling
//make things with normals pointing away not try to be rendered
//bounding box finding must depend on displacement shaders!
//make sure am using all necessary shader global vars
//i already know i didnt do derivatives of position
//each channel can have its own alpha??
//bumpy shader via perlin noise
//what was that strange matrix math for normals??
//phong lighting specular component!!
//make sure not leaking any memory
//TEXTURES!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//Look through for various alarmed messages
//full non-opacity has annoying noisy errors
//write out actual image types, not ppm

int main(){
	RiBegin(RI_NULL);
		//RiFormat(320/2,240/2,1);
		RiClipping(0.1,10000);
		RiFormat(640,480,1);
		//RiFrameAspectRatio(2.0/1.0);
		//RiFrameBegin(0);
		
		RiIdentity();
		//RtFloat fov = M_PI/2;//90 degrees
		//RtFloat fov = 1.0472;//60 degrees
		RtFloat fov = 0.174533/2/2/2/2;//10 degrees
		
		//Screen Transform	
		//RiTranslate(320/4,240/4,0);
		RiTranslate(320,240,0);
		RiProjection(RI_PERSPECTIVE,"fov",&fov);
		
		RiIdentity();

		//Camera Transform
		RiTranslate(0,0,5);

		RiWorldBegin();
			RtColor col = {1,1,1};
			//RtColor opa = {1,1,1};
			RtColor opa = {0.4,0.4,0.4};
			RiColor(col);
			RiOpacity(opa);
			RiSurface(checkerboard);
			//RiSurface(random_shader);
			//RiSurface(phong_lighting);
			RiIdentity();
			//RiRotate(1.57/2,1,0,0);	
			//RiRotate(1.57,0,0,1);	
			//Rendering
			RiTransformBegin();
				RiRotate(1.57,1,0,0);
				RiTranslate(1,0,0);	
				RiSphere(2,-2,2,360,RI_NULL);
				//RiTorus(2,1,0,360,360,RI_NULL);
			RiTransformEnd();
			RiTransformBegin();
				RiRotate(1.57,1,0,0);
				RiTranslate(-1,0,0);	
				RiSphere(2,-2,2,360,RI_NULL);
				//RiTorus(2,1,0,360,360,RI_NULL);
			RiTransformEnd();
			RiDisplay("tmp.ppm","file","rgb");	
		RiWorldEnd();
	RiEnd();
}
