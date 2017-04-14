//John Thomason
//ID 3058344

#include "Ri.h"
#include <iostream>

RiContext* RiCurrentContext;


//Graphics States
RtVoid RiBegin(RtToken name){
	RiCurrentContext = new RiContext();
	RiIdentity();
	return;
}
RtVoid RiEnd(){
	for(int i = 0;i < RiCurrentContext -> XResolution;i++){
		delete(RiCurrentContext -> FrameBuffer[i]);	
	}
	delete(RiCurrentContext -> FrameBuffer);
	delete(RiCurrentContext);
	return;
}

RtVoid RiFormat(RtInt xresolution,RtInt yresolution,RtFloat pixelaspectration){
	RiCurrentContext -> XResolution = xresolution;
	RiCurrentContext -> YResolution = yresolution;
	RiCurrentContext -> PixelAspectRation = pixelaspectration;
	RiCurrentContext -> FrameBuffer = new RtInt*[xresolution];
	for(int i = 0;i < xresolution;i++){
		RiCurrentContext -> FrameBuffer[i] = new RtInt[yresolution];	
		for(int j = 0;j < yresolution;j++){
			RiCurrentContext -> FrameBuffer[i][j] = 0;
		}
	}
	return;
}

//Transformation Stuff
RtVoid RiIdentity(){
	for(int j = 0;j < 4;j++){
		for(int i = 0;i < 4;i++){
			if(i == j){
				RiCurrentContext -> CurrentTransform[i][j] = 1;
			} else {
				RiCurrentContext -> CurrentTransform[i][j] = 0;
			}
		}
	}
	return;
}

RtVoid RiConcatTransform(RtMatrix trans){
	RtMatrix tmp;
	for(int j = 0;j < 4;j++){
		for(int i = 0;i < 4;i++){
			tmp[i][j] = 0.0;
			for(int l = 0;l < 4;l++){
				tmp[i][j] += RiCurrentContext -> CurrentTransform[l][j] * trans[i][l];
			}
		}
	}

	for(int j = 0;j < 4;j++){
		for(int i = 0;i < 4;i++){
			RiCurrentContext -> CurrentTransform[i][j] = tmp[i][j];
		}
	}

	return;
}




void JohnPrint(){
	for(int j = 0;j < RiCurrentContext -> YResolution;j++){
		for(int i = 0;i < RiCurrentContext -> XResolution;i++){
			std::cout << RiCurrentContext -> FrameBuffer[i][j];
		}
		std::cout << "\n";
	}
	return;
}

void JohnPrintMat(){
	for(int j = 0;j < 4;j++){
		for(int i = 0;i < 4;i++){
			std::cout << RiCurrentContext -> CurrentTransform[i][j] << "|";
		}
		std::cout << "\n";
	}
}
