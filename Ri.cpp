//John Thomason
//ID 3058344

#include "Ri.h"
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <string.h>

const RtFloat PI_DIV_180 = M_PI/180.0;

RtToken RI_PERSPECTIVE = "perspective";
RtToken RI_ORTHOGRAPHIC = "orthographic";
RtToken RI_FOV = "fov";
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

RtVoid RiTransformBegin(){
	//With no stack, just reset transform;
	RiIdentity();
	return;
}

RtVoid RiTransformEnd(){
	//With no stack, do nothing
	return;
}

RtVoid RiClipping(RtFloat near,RtFloat far){
	RiCurrentContext -> Near = near;
	RiCurrentContext -> Far = far;
	return;
}

RtVoid RiProjection(RtToken name,RtToken paramname,RtFloat fov){
	if(strcmp(name,RI_PERSPECTIVE) == 0){
		if(strcmp(paramname,RI_FOV) == 0){
			RiPerspective(fov);
		} else {
			RiPerspective(90.0);
		}
	} else if(strcmp(name,RI_ORTHOGRAPHIC) == 0){
		RtFloat ZScale = 1.0/(RiCurrentContext-> Far - RiCurrentContext -> Near);
		RtMatrix ortho = {1,0,0,0,
				  0,1,0,0,
				  0,0,ZScale,-(RiCurrentContext -> Near),
				  0,0,0,0};
		RiConcatTransform(ortho);
	} //else if (name == RI_NULL){
	
	for(int j = 0;j < 4;j++){
		for(int i = 0;i < 4;i++){
			RiCurrentContext -> ScreenTransform[i][j] = RiCurrentContext -> CurrentTransform[i][j];
		}
	}
	//}
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


RtVoid RiTransform(RtMatrix mat){
	for(int j = 0;j < 4;j++){
		for(int i = 0;i < 4;i++){
			RiCurrentContext -> CurrentTransform[i][j] = mat[i][j];
		}
	}
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

RtVoid RiScale(RtFloat dx,RtFloat dy,RtFloat dz){
	RtMatrix tmp = {dx,0 ,0 ,0,
			0 ,dy,0 ,0,
			0 ,0 ,dz,0,
			0 ,0 ,0 ,1};
	RiConcatTransform(tmp);
	return;
}

RtVoid RiRotate(RtFloat angle,RtFloat dx,RtFloat dy,RtFloat dz){
	RtFloat len = sqrt(pow(dx,2) + pow(dy,2) + pow(dz,2));
	dx = dx / len;
	dy = dy / len;
	dz = dz / len;
	RtFloat co = cos(angle);
	RtFloat si = sin(angle);
	RtFloat t = 1 - co;

	//http://www.euclideanspace.com/maths/geometry/rotations/conversions/angleToMatrix/
	RtMatrix tmp = {t*dx*dx + co   ,t*dx*dy - dz*si,t*dx*dz + dy*si,0,
			t*dx*dy + dz*si,t*dy*dy + co   ,t*dy*dz - dx*si,0,
			t*dx*dz - dy*si,t*dy*dz + dx*si,t*dz*dz + co   ,0,
			0	       ,0	       ,0	       ,1};
	RiConcatTransform(tmp);
	return;
}

RtVoid RiPerspective(RtFloat fov){
	//https://www.scratchapixel.com/lessons/3d-basic-rendering/perspective-and-orthographic-projection-matrix/building-basic-perspective-projection-matrix
	RtFloat ScaleFactor = 1.0 /(tan(fov/2) * PI_DIV_180);
	//RtFloat FocalLength = (RiCurrentContext -> XResolution/RiCurrentContext -> YResolution)/tan(fov/2);
	RtFloat nr = RiCurrentContext -> Near;
	RtFloat fr = RiCurrentContext -> Far;
	RtMatrix tmp = {ScaleFactor,0          ,0		    	  ,0 ,
			0	   ,ScaleFactor,0		    	  ,0 ,
			0	   ,0	       ,(-1 * fr     ) / (fr - nr),-1,
			0	   ,0	       ,(-1 * fr * nr) / (fr - nr),0 };
	RiConcatTransform(tmp);
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
	//JohnPrintHpoint(pt);
	RiMultHpoint(pt);
	//JohnPrintHpoint(pt);
	if(pt[0] > 0 && pt[1] > 0 && (RtInt)pt[0] < RiCurrentContext -> XResolution && (RtInt)pt[1] < RiCurrentContext -> YResolution){
		RiCurrentContext -> FrameBuffer[(RtInt)pt[0]][(RtInt)pt[1]] = 1;
	}
	return;
}
