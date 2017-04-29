//John Thomason
//ID 3058344

#include "Ri.h"
#include "JRi.h"
#include "shaders.h"
#include <iostream>
#include <math.h>

//THING TO REMEMBER TO DO:!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//PORT TO CIMAGE and windows!!
//write out actual image types, not ppp, make RiDisplay work correctly
//try all the render scenes!
//If JRI.h not included here, get a double free error??
//Weird problem with shrink shader
//Make there be more micropolygons as in finder dicing
//MY FOV IS ALL KINDS OF SCREWED UP!!

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
		RtFloat fov = 45.0;//1.0;//45.0;//0.785398;//45 degrees
//0.174533/2/2/2/2;//10 degrees
		
		//Screen Transform	
		//RiTranslate(320/4,240/4,0);
		RiTranslate(320/2,240/2,0);
		RiProjection(RI_PERSPECTIVE,"fov",&fov);
		
		//Camera Transform
		RiTranslate(0,0,5);

		RiFrameBegin(2);
		RiWorldBegin();
			RiTransformBegin();
			RiTransformEnd();
			RtColor col = {1,0,0};
			//RtColor opa = {1,1,1};
			RtColor opa = {1,1,1};
			RiColor(col);
			RiOpacity(opa);
			//RiMakeTexture("",0);
			RiDisplacement(BUMPY);
			RiSurface(phong_lighting);
			BUMP_MIN_FREQ_EXP = 2;
			BUMP_MAX_FREQ_EXP = 8;
			BUMP_AMPLITUDE = 0.5;
			RiTransformBegin();	
				RiRotate(90,1,0,0);	
				//RiSphere(2,-2,2,360,RI_NULL);
				RiCone(2.0,1.0,360,RI_NULL);
			RiTransformEnd();
			RiDisplay("tmp.ppm","file","rgb");	
		RiWorldEnd();
		RiFrameEnd();
	RiEnd();
}
