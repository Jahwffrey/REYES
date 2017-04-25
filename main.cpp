//John Thomason
//ID 3058344

#include "Ri.h"
#include "JRi.h"
#include <iostream>
#include <math.h>

//THING TO REMEMBER TO DO:!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//My Coordinate system reversed! Why and how do i fix it?
//RiColor!!
//Worry about transforming normals
//make sure RiBegin correclty sets all defaults
//Make sure not leaking any memory
//Make everything work by degrees

int main(){
	RiBegin(RI_NULL);
		//RiFormat(320/2,240/2,1);
		RiFormat(640,480,1);
		//RiFrameAspectRatio(2.0/1.0);
		//RiFrameBegin(0);
		
		RiClipping(0.1,10000);
		RiIdentity();
		//RtFloat fov = M_PI/2;//90 degrees
		//RtFloat fov = 1.0472;//60 degrees
		RtFloat fov = 0.174533/2/2/2/2;//10 degrees
		
		//Screen Transform	
		//RiTranslate(320/4,240/4,0);
		RiTranslate(320,240,0);
		RiProjection(RI_PERSPECTIVE,"fov",&fov);
		
		RiIdentity();

		JRiPoint* test = new JRiPoint(2,2,2,1);

		//Camera Transform
		RiTranslate(0,0,5);

		RiWorldBegin();
			RiIdentity();
	
			//RiRotate(0.2,1,0,0);	
			//Rendering
			RiTransformBegin();
				RiSphere(1,-1,1,360.0,RI_NULL);
			RiTransformEnd();
			RiDisplay("tmp.ppm","file","rgb");	
		RiWorldEnd();
	RiEnd();
}
