//John Thomason
//ID 3058344

#include "Ri.h"
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <string.h>

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

RtVoid RiFormat(RtInt xresolution,RtInt yresolution,RtFloat pixelaspectratio){
	RiCurrentContext -> XResolution = xresolution;
	RiCurrentContext -> YResolution = yresolution;
	RiCurrentContext -> PixelAspectRatio = pixelaspectratio;
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
	//With no stack, so nothing
	//RiIdentity();
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

RtVoid RiProjection(RtToken name,RtToken paramname,RtFloat *fov){
	if(strcmp(name,RI_PERSPECTIVE) == 0){
		if(strcmp(paramname,RI_FOV) == 0){
			RiPerspective(*fov);
		} else {
			RiPerspective(M_PI/2);
		}
	} else if(strcmp(name,RI_ORTHOGRAPHIC) == 0){
		//THIS IS WRONG!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		std::cout << "Here am BAD 2!!!";
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
}

RtVoid RiWorldBegin(){
	for(int j = 0;j < 4;j++){
		for(int i = 0;i < 4;i++){
			RiCurrentContext -> ViewTransform[i][j] = RiCurrentContext -> CurrentTransform[i][j];
		}
	}
	return;
}

RtVoid RiWorldEnd(){
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


RtVoid RiTransform(RtMatrix mat){
	for(int j = 0;j < 4;j++){
		for(int i = 0;i < 4;i++){
			RiCurrentContext -> CurrentTransform[i][j] = mat[i][j];
		}
	}
	return;
}

RtVoid RiConcatTransform(RtMatrix trans){
	RtMatrix tmp/*,rgt*/ = {0,0,0,0,
			0,0,0,0,
			0,0,0,0,
			0,0,0,0};

	for(int j = 0;j < 4;j++){
		for(int i = 0;i < 4;i++){
			for(int l = 0;l < 4;l++){
				//tmp[i][j] += (RiCurrentContext -> CurrentTransform[l][j]) * trans[i][l];
				tmp[i][j] += (RiCurrentContext -> CurrentTransform[l][j]) * trans[l][i];
				//tmp[i][j] += (RiCurrentContext -> CurrentTransform[l][i]) * trans[l][j];
			}
		}
	}
	
	//RtMatrix rgt = RiCurrentContext -> CurrentTransform;

	/*for(int j = 0;j < 4;j++){
		for(int i = 0;i < 4;i++){
			rgt[i][j] = RiCurrentContext -> CurrentTransform[i][j];// = tmp[i][j];
		}
	}

	tmp[0][0] = lft[0][0]*rgt[0][0] + lft[1][0]*rgt[0][1] + lft[2][0]*rgt[0][2] + lft[3][0]*rgt[0][3];
	tmp[0][1] = lft[0][0]*rgt[1][0] + lft[1][0]*rgt[1][1] + lft[2][0]*rgt[1][2] + lft[3][0]*rgt[1][3];
	tmp[0][2] = lft[0][0]*rgt[2][0] + lft[1][0]*rgt[2][1] + lft[2][0]*rgt[2][2] + lft[3][0]*rgt[2][3];
	tmp[0][3] = lft[0][0]*rgt[3][0] + lft[1][0]*rgt[3][1] + lft[2][0]*rgt[3][2] + lft[3][0]*rgt[3][3];
	
	tmp[1][0] = lft[0][1]*rgt[0][0] + lft[1][1]*rgt[0][1] + lft[2][1]*rgt[0][2] + lft[3][1]*rgt[0][3];
	tmp[1][1] = lft[0][1]*rgt[1][0] + lft[1][1]*rgt[1][1] + lft[2][1]*rgt[1][2] + lft[3][1]*rgt[1][3];
	tmp[1][2] = lft[0][1]*rgt[2][0] + lft[1][1]*rgt[2][1] + lft[2][1]*rgt[2][2] + lft[3][1]*rgt[2][3];
	tmp[1][3] = lft[0][1]*rgt[3][0] + lft[1][1]*rgt[3][1] + lft[2][1]*rgt[3][2] + lft[3][1]*rgt[3][3];

	tmp[2][0] = lft[0][2]*rgt[0][0] + lft[1][2]*rgt[0][1] + lft[2][2]*rgt[0][2] + lft[3][2]*rgt[0][3];
	tmp[2][1] = lft[0][2]*rgt[1][0] + lft[1][2]*rgt[1][1] + lft[2][2]*rgt[1][2] + lft[3][2]*rgt[1][3];
	tmp[2][2] = lft[0][2]*rgt[2][0] + lft[1][2]*rgt[2][1] + lft[2][2]*rgt[2][2] + lft[3][2]*rgt[2][3];
	tmp[2][3] = lft[0][2]*rgt[3][0] + lft[1][2]*rgt[3][1] + lft[2][2]*rgt[3][2] + lft[3][2]*rgt[3][3];
	
	tmp[3][0] = lft[0][3]*rgt[0][0] + lft[1][3]*rgt[0][1] + lft[2][3]*rgt[0][2] + lft[3][3]*rgt[0][3];
	tmp[3][1] = lft[0][3]*rgt[1][0] + lft[1][3]*rgt[1][1] + lft[2][3]*rgt[1][2] + lft[3][3]*rgt[1][3];
	tmp[3][2] = lft[0][3]*rgt[2][0] + lft[1][3]*rgt[2][1] + lft[2][3]*rgt[2][2] + lft[3][3]*rgt[2][3];
	tmp[3][3] = lft[0][3]*rgt[3][0] + lft[1][3]*rgt[3][1] + lft[2][3]*rgt[3][2] + lft[3][3]*rgt[3][3];
	*/

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

	RtMatrix tmp = {1,0,0,0,
			0,cos(angle),-1*sin(angle),0,
			0,sin(angle),cos(angle),0,
			0,0,0,1};
	
	//http://www.euclideanspace.com/maths/geometry/rotations/conversions/angleToMatrix/
	//RtMatrix tmp = {t*dx*dx + co   ,t*dx*dy - dz*si,t*dx*dz + dy*si,0,
	//		t*dx*dy + dz*si,t*dy*dy + co   ,t*dy*dz - dx*si,0,
	//		t*dx*dz - dy*si,t*dy*dz + dx*si,t*dz*dz + co   ,0,
	//		0	       ,0	       ,0	       ,1};
	RiConcatTransform(tmp);
	return;
}

RtVoid RiPerspective(RtFloat fov){
	RtFloat ScaleFactor = 1.0 /(tan(fov/2));
	RtFloat ScaleFactorAspect = 1.0 /(tan(fov/(2*RiCurrentContext->PixelAspectRatio)));
	RtHpoint tmp1 = {0,0,RiCurrentContext->Near,1};
	RtHpoint tmp2 = {0,0,RiCurrentContext->Far,1};
	RiMultHpoint(tmp1);
	RiMultHpoint(tmp2);
	//RtFloat nr = tmp1[2];
	//RtFloat fr = tmp2[2];
	RtFloat nr = RiCurrentContext -> Near;
	RtFloat fr = RiCurrentContext -> Far;

	std::cout << "fov: " << fov << "\n" << "near: " << nr << "\nfar: " << fr << "\n";
	
	/*RtMatrix tmp = {ScaleFactor * (1.0/RiCurrentContext -> PixelAspectRatio),0,0,0,
			0,ScaleFactor,0,0,
			0,0,fr/(fr - nr),(-1*nr*fr)/(fr - nr),
			0,0,1,0};*/
	
	RtMatrix tmp = {ScaleFactor,0,0,0,
			0,ScaleFactorAspect,0,0,
			0,0,fr/(fr - nr),(-1*nr*fr)/(fr - nr),
			0,0,-1,0};
	RiConcatTransform(tmp);
}



//Internal Stuff
RtVoid RiMultHpoint(RtHpoint pt){
	RtHpoint npt = {0,0,0,0};
	for(int j = 0;j < 4;j++){
		for(int i = 0;i < 4;i++){
			npt[j] += RiCurrentContext -> CurrentTransform[i][j] * pt[i];
		}
	}

	for(int i = 0;i < 4;i++){
		pt[i] = npt[i];
	}
	return;	
}

RtVoid RiProjHpoint(RtHpoint pt){
	RtHpoint npt = {0,0,0,0};
	for(int j = 0;j < 4;j++){
		for(int i = 0;i < 4;i++){
			npt[j] += RiCurrentContext -> ViewTransform[i][j] * pt[i];
		}
	}
	
	for(int i = 0;i < 4;i++){
		pt[i] = npt[i];
	}

	
	//JohnPrintHpoint(pt);

	
	RtHpoint npt2 = {0,0,0,0};
	for(int j = 0;j < 4;j++){
		for(int i = 0;i < 4;i++){
			npt2[j] += RiCurrentContext -> ScreenTransform[i][j] * pt[i];
		}
	}

	pt[3] = npt2[3];	
	for(int i = 0;i < 3;i++){
		pt[i] = npt2[i]/(npt2[3] + 0.000001);
	}	

	return;	
}

RtVoid RiClearBuffer(){
	for(int i = 0;i < RiCurrentContext -> XResolution;i++){
		for(int j = 0;j < RiCurrentContext -> YResolution;j++){
			RiCurrentContext -> FrameBuffer[i][j] = 0;
		}
	}
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
	RiMultHpoint(pt);
	RiProjHpoint(pt);
	JohnPrintHpoint(pt);
	if(pt[3] < -(RiCurrentContext -> Near) && pt[0] >= 0 && pt[1] >= 0 && (RtInt)pt[0] < RiCurrentContext -> XResolution && (RtInt)pt[1] < RiCurrentContext -> YResolution){
		RiCurrentContext -> FrameBuffer[(RtInt)pt[0]][(RtInt)pt[1]] = 1;
	}
	return;
}
