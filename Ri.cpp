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

RtVoid RiTranslate(RtFloat dx,RtFloat dy,RtFloat dz){
	RtMatrix tmp = {1,0,0,dx,
			0,1,0,dy,
			0,0,1,dz,
			0,0,0,1};
	RiConcatTransform(tmp);
	return;
}

RtVoid RiConcatTransform(RtMatrix trans){
	RtMatrix tmp = {0,0,0,0,
			0,0,0,0,
			0,0,0,0,
			0,0,0,0};
	for(int j = 0;j < 4;j++){
		for(int i = 0;i < 4;i++){
			for(int l = 0;l < 4;l++){
				//tmp[i][j] += (RiCurrentContext -> CurrentTransform[l][j]) * trans[i][l];
				tmp[i][j] += (RiCurrentContext -> CurrentTransform[l][j]) * trans[l][i];
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

//Internal Stuff
RtVoid RiMultHpoint(RtHpoint pt){
	RtHpoint npt = {0,0,0,0};
	for(int j = 0;j < 4;j++){
		for(int i = 0;i < 4;i++){
			//std::cout << "Mult: " << RiCurrentContext -> CurrentTransform[i][j] << " * " << pt[i] << "\n"; 
			npt[j] += RiCurrentContext -> CurrentTransform[i][j] * pt[i];
		}
	}

	for(int i = 0;i < 4;i++){
		pt[i] = npt[i];
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
	return;
}

void JohnPrintHpoint(RtHpoint pt){
	for(int i = 0;i < 4;i++){
		std::cout << pt[i] << "|";
	}
	std::cout << "\n";
	return;
}

void JohnPoint(RtHpoint pt){
	JohnPrintHpoint(pt);
	RiMultHpoint(pt);
	JohnPrintHpoint(pt);
	if((RtInt)pt[0] < RiCurrentContext -> XResolution && (RtInt)pt[1] < RiCurrentContext -> YResolution){
		RiCurrentContext -> FrameBuffer[(RtInt)pt[0]][(RtInt)pt[1]] = 1;
	}
	return;
}
