//John Thomason
//ID 3058344

#include "Ri.h"
#include "JRiPoint.h"
#include <iostream>
#include <math.h>

int main(){
	RiBegin(RI_NULL);
		RiFormat(80,40,0.5);
		//RiFrameAspectRatio(2.0/1.0);
		//RiFrameBegin(0);
		
		RiClipping(0.1,10000);
		RiIdentity();
		//RtFloat fov = M_PI/2;//90 degrees
		//RtFloat fov = 1.0472;//60 degrees
		RtFloat fov = 0.174533/2;//10 degrees
		
		//Screen Transform	
		RiTranslate(20,20,0);
		RiProjection(RI_PERSPECTIVE,"fov",&fov);
		
		RiIdentity();

		JRiPoint* test = new JRiPoint(2,2,2,1);

		//Camera Transform
		RiTranslate(0,0,20);

		RiWorldBegin();
		RiIdentity();
		
		//Rendering
		for(int tm = 0;tm < 100000;tm++){
		RiTransformBegin();
		//RiTranslate(-1,0,0);
		for(int i = -1;i < 3;i+=2){
			for(int j = -1;j < 3;j+=2){
				for(int k = -1;k < 3;k+=2){
					JRiPoint tmp = JRiPoint(3*i,6*j,6*k,1);	
					tmp.MoveToScreen();
					tmp.DumpToScreen();
				}
			}
		}
		
		RiTransformEnd();
		JohnPrint();
		RiClearBuffer();
		std::cin.get();
		RiRotate(0.1,0,1,0);
		}
		RiWorldEnd();


	RiEnd();
}
