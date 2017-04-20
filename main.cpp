//John Thomason
//ID 3058344

#include "Ri.h"
#include "JRi.h"
#include <iostream>
#include <math.h>

int main(){
	RiBegin(RI_NULL);
		RiFormat(640,480,1);
		//RiFrameAspectRatio(2.0/1.0);
		//RiFrameBegin(0);
		
		RiClipping(0.1,10000);
		RiIdentity();
		//RtFloat fov = M_PI/2;//90 degrees
		//RtFloat fov = 1.0472;//60 degrees
		RtFloat fov = 0.174533/2;//10 degrees
		
		//Screen Transform	
		RiTranslate(320,240,0);
		RiProjection(RI_PERSPECTIVE,"fov",&fov);
		
		RiIdentity();

		JRiPoint* test = new JRiPoint(2,2,2,1);

		//Camera Transform
		RiTranslate(0,0,5);

		RiWorldBegin();
		RiIdentity();
		
		//Rendering
		RiTransformBegin();
		JRiMesh* testmesh = new JRiMesh(16,16);
		//RiTranslate(-1,0,0);
		//RiTranslate(20,20,0);
		RiRotate(0.25,1,1,0);
		for(int j = 0;j < 16;j++){
			for(int i = 0;i < 16;i++){
				testmesh->set(i,j,
					      i,j,0,
					      1,1,1,	
					      i/16.0,j/16.0,1,0,
					      0,0);
			}
		}
		testmesh->Draw();
		RiTransformEnd();
		delete(testmesh);
		//JohnPrint();
		RiDisplay("tmp.ppm","file","rgb");	
		RiWorldEnd();


	RiEnd();
}
