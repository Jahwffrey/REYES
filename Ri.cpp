//John Thomason
//ID 3058344

#include "Ri.h"
#include <iostream>

//Global Variables
//RtMatrix* RiCurrentTransform;
//RtInt* RiFrameBuffer;
RtMatrix RiCurrentTransform;//current transformation
RtInt* RiFrameBuffer;//The screen


//Graphics States
void RiBegin(RtToken name){
	RiIdentity();
	return;
}
void RiEnd(){
	return;
}

void RiIdentity(){
	for(int i = 0;i < 4;i++){
		for(int j = 0;j < 4;j++){
			if(i == j){
				RiCurrentTransform[i][j] = 1;
			} else {
				RiCurrentTransform[i][j] = 0;
			}
		}
	}
	return;
}

void JohnPrint(){
	std::cout << "Print!\n";
	return;
}
