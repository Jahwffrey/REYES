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
	}
	return;
}

RtVoid RiIdentity(){
	for(int i = 0;i < 4;i++){
		for(int j = 0;j < 4;j++){
			if(i == j){
				RiCurrentContext -> CurrentTransform[i][j] = 1;
			} else {
				RiCurrentContext -> CurrentTransform[i][j] = 0;
			}
		}
	}
	return;
}

void JohnPrint(){
	std::cout << "Print!\n";
	return;
}
