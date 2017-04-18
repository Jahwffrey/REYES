//John Thomason
//ID 3058344

#include "Ri.h"
#include <iostream>
#include <math.h>

int main(){
	RiBegin(RI_NULL);
		RiFormat(80,40,1);
		//RiFrameAspectRatio(2.0/1.0);
		//RiFrameBegin(0);
		
		RiClipping(0.1,10000);
		RiIdentity();
		RtFloat fov = M_PI/2;//90 degrees
		
		//Screen Transform	
		RiTranslate(20,20,0);
		RiProjection(RI_PERSPECTIVE,"fov",&fov);
		
		RiIdentity();

		//Camera Transform
		RiTranslate(0,0,5);

		RiWorldBegin();
		RiIdentity();
		
		//Rendering
		for(int tm = 0;tm < 100000;tm++){
		RiTransformBegin();
		//RiRotate(0.1,0,1,0);
		RiTranslate(-1,0,0);
		for(int i = -3;i < 4;i++){
			for(int j = -3;j < 4;j++){
				for(int k = -2;k > -4;k--){
					RtHpoint tmp = {4*(i + 0.5),4*(j + 0.5),k,1};
					JohnPoint(tmp);
				}
			}
		}
		
		RiTransformEnd();
		//std::cout<<"\n\n\n\n\n\n\n\n\n\n\n\n";
		//std::cout<<"\n\n\n\n\n\n\n\n\n\n\n\n";
		//std::cout<<"\n\n\n\n\n\n\n\n\n\n\n\n";
		//std::cout<<"\n\n\n\n\n\n\n\n\n\n\n\n";
		//std::cout<<"\n\n\n\n\n\n\n\n\n\n\n\n";
		JohnPrint();
		RiClearBuffer();
		std::cin.get();
		}
		RiWorldEnd();


	RiEnd();
}
