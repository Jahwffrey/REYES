//John Thomason
//ID 3058344

#include "Ri.h"
#include "JRi.h"
#include "shaders.h"
#include <iostream>
#include <math.h>

//THING TO REMEMBER TO DO:!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//make sure RiBegin correclty sets all defaults including clipping planes
//Make everything work by degrees as opposed to radians
//implement near culling
//bumpy shader via perlin noise
//what was that strange matrix math for normals??
//phong lighting specular component!!
//make sure not leaking any memory
//frame begin and frame end correctly!
//Look through for various alarmed messages
//PORT TO CIMAGE and windows!!
//write out actual image types, not ppm
//try all the render scenes!
//Make sure RiWorldBegin freezes changing all rendering options!
//If JRI.h not included here, get a double free error??
//FrameAspectRatio is never use and the way i am gettign action FOV is all kinds of screwed up!

int main(){
	RiBegin(RI_NULL);
		//RiFormat(320/2,240/2,1);
		RiClipping(0.1,10000);
		RiFormat(320,240,1);
		//RiFrameAspectRatio(2.0/1.0);
		//RiFrameBegin(0);
		
		RiIdentity();
		//RtFloat fov = M_PI/2;//90 degrees
		//RtFloat fov = 1.0472;//60 degrees
		RtFloat fov = 2;//45.0;//1.0;//45.0;//0.785398;//45 degrees
//0.174533/2/2/2/2;//10 degrees
		
		//Screen Transform	
		//RiTranslate(320/4,240/4,0);
		RiTranslate(320/2,240/2,0);
		RiProjection(RI_PERSPECTIVE,"fov",&fov);
		
		//Camera Transform
		RiTranslate(0,0,2.5);

		RiWorldBegin();
			RiTransformBegin();
			RiTransformEnd();
			RtColor col = {1,0,0};
			//RtColor opa = {1,1,1};
			RtColor opa = {1,1,1};
			RiColor(col);
			RiOpacity(opa);
			//RiMakeTexture("",0);
			RiSurface(phong_lighting);
			RiTransformBegin();	
				RiRotate(45,1,0,0);	
				RiSphere(2,-2,2,360,RI_NULL);
			RiTransformEnd();
			RiDisplay("tmp.ppm","file","rgb");	
		RiWorldEnd();
	RiEnd();
}
