//John Thomason
//ID 3058344

#include "Ri.h"
#include "JRi.h"
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <sstream>
#include <stdlib.h>

RtToken RI_PERSPECTIVE = "perspective";
RtToken RI_ORTHOGRAPHIC = "orthographic";
RtToken RI_FOV = "fov";
RiContext* RiCurrentContext;

//RiContext Stuff
RtVoid WriteFrameBuffer(RtFloat x,RtFloat y,RtFloat r,RtFloat g,RtFloat b,RtFloat a,RtFloat z){
			RtInt i = (RtInt)x;
			RtInt j = (RtInt)y;
			RiCurrentContext -> FrameBuffer[i][j].r = r;
			RiCurrentContext -> FrameBuffer[i][j].g = g;
			RiCurrentContext -> FrameBuffer[i][j].b = b;
			RiCurrentContext -> FrameBuffer[i][j].a = a;
			RiCurrentContext -> FrameBuffer[i][j].z = z;
}

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
	RiCurrentContext -> FrameAspectRatio = (xresolution * pixelaspectratio)/yresolution;
	RiCurrentContext -> FrameBuffer = new JRiPixel* [xresolution];
	for(int i = 0;i < xresolution;i++){
		RiCurrentContext -> FrameBuffer[i] = new JRiPixel[yresolution];	
		for(int j = 0;j < yresolution;j++){
			RiCurrentContext -> FrameBuffer[i][j].r = 0;
			RiCurrentContext -> FrameBuffer[i][j].g = 0;
			RiCurrentContext -> FrameBuffer[i][j].b = 0;
			RiCurrentContext -> FrameBuffer[i][j].a = 0;
			RiCurrentContext -> FrameBuffer[i][j].z = 0;
		}
	}
	return;
}

RtVoid RiTransformBegin(){
	//With no stack, so nothing
	//RiIdentity();
	RiCurrentContext -> TransBegun = 1;
	return;
}

RtVoid RiTransformEnd(){
	//With no stack, do nothing
	RiCurrentContext -> TransBegun = 0;
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
	RiCurrentContext -> WorldBegun = 1;
	for(int j = 0;j < 4;j++){
		for(int i = 0;i < 4;i++){
			RiCurrentContext -> ViewTransform[i][j] = RiCurrentContext -> CurrentTransform[i][j];
		}
	}
	return;
}

RtVoid RiWorldEnd(){
	RiCurrentContext -> WorldBegun = 0;
	return;
}

RtVoid RiFrameAspectRatio(RtFloat rat){
	RiCurrentContext -> FrameAspectRatio = rat;
	return;
}

RtVoid RiPixelSamples(RtFloat xsamples,RtFloat ysamples){
	RiCurrentContext -> XSamples = xsamples;
	RiCurrentContext -> YSamples = ysamples;
	return;
}

RtVoid RiFrameBegin(RtInt frame){
	//NOT DONE! NOT DONE AT ALL!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	RiCurrentContext -> FrameBegun = 1;
	RiCurrentContext -> CurrentFrame = frame;
	return;
}

RtVoid RiFrameEnd(){
	RiCurrentContext -> FrameBegun = 0;
	RiCurrentContext -> CurrentFrame = -1;
	return;
}

RtVoid RiDisplay(RtToken name,RtToken type,RtToken mode,RtToken paramlist,RtPointer orig){
	//I NEED TO IMPLEMENT SUPPORT FOR ORIGIN!!
	std::stringstream fname;
	//If writing to file
	if(strcmp(type,"file") == 0){
		FILE *ofile = fopen(name,"wb");
		fprintf(ofile,"P6\n%d %d\n255\n",RiCurrentContext -> XResolution,RiCurrentContext -> YResolution);
		for(int j = 0;j < RiCurrentContext -> YResolution;j++){
			for(int i = 0;i < RiCurrentContext -> XResolution;i++){
				int nm = 0;
				unsigned char col[5];
				JRiPixel px = RiCurrentContext -> FrameBuffer[i][j];
				if(strcmp(mode,"rgb") == 0){	
					nm = 3;
					col[0] = px.r;
					col[1] = px.g;
					col[2] = px.b;
				} else if(strcmp(mode,"rgba") == 0){
					nm = 4;
					col[0] = px.r;
					col[1] = px.g;
					col[2] = px.b;
					col[3] = px.a;
				} else if(strcmp(mode,"rgbaz") == 0){
					nm = 5;
					col[0] = px.r;
					col[1] = px.g;
					col[2] = px.b;
					col[3] = px.a;
					col[4] = px.z;
				} else if(strcmp(mode,"rgbz") == 0){
					nm = 4;
					col[0] = px.r;
					col[1] = px.g;
					col[2] = px.b;
					col[3] = px.z;
				} else if(strcmp(mode,"a") == 0){
					nm = 1;
					col[0] = px.a;
				} else if(strcmp(mode,"az") == 0){
					nm = 2;
					col[0] = px.a;
					col[1] = px.z;
				} else if(strcmp(mode,"z") == 0){
					nm = 1;
					col[0] = px.z;
				}
				fwrite(col,1,nm,ofile);
			}
		}
		fclose(ofile);
	} else if (strcmp(type,"framebuffer") == 0){
		//NOT DONE! NOT DONE AT ALL!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		//???????
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
	RtFloat ScaleFactor = 1.0 /(tan(fov/2));
	RtFloat aspect = 1/(RiCurrentContext->PixelAspectRatio);
	RtFloat ScaleFactorAspect = 1.0 /(tan((fov/2)*aspect));
	RtFloat nr = RiCurrentContext -> Near;
	RtFloat fr = RiCurrentContext -> Far;

	RtMatrix tmp = {ScaleFactor,0,0,0,
			0,ScaleFactorAspect,0,0,
			0,0,fr/(fr - nr),(-1*nr*fr)/(fr - nr),
			0,0,-1,0};
	RiConcatTransform(tmp);
}

//Primitives
RtVoid RiSphere(RtFloat radius,RtFloat zmin,RtFloat zmax,RtFloat thetamax,RtPointer param){
	//CURRENTLY I AM CONSTRUCTING THE MESH, DRAWING IT, AND DELETING IT!!! THIS MAY CHANGE LATER!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
	JRiMesh* mesh = new JRiMesh(64,64);
	RtFloat phimin = -M_PI/2.0;
	if(zmin > -radius) phimin = asin(zmin/radius);
	RtFloat phimax = M_PI/2.0;
	if(zmax < radius) phimax = asin(zmax/radius);
	mesh->Draw();
	delete(mesh);
	return;
}


//Internal Stuff
RtVoid RiClearBuffer(){
	for(RtFloat j = 0;j < RiCurrentContext -> YResolution;j++){
		for(RtFloat i = 0;i < RiCurrentContext -> XResolution;i++){
			WriteFrameBuffer(i,j,0,0,0,0,0);
		}
	}
}
