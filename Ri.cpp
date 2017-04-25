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
#include <time.h>

RtToken RI_PERSPECTIVE = "perspective";
RtToken RI_ORTHOGRAPHIC = "orthographic";
RtToken RI_FOV = "fov";
RiContext* RiCurrentContext;

//RiContext Stuff
RtVoid WriteFrameBuffer(RtFloat x,RtFloat y,RtFloat r,RtFloat g,RtFloat b,RtFloat a,RtFloat z){
			RtInt i = (RtInt)x;
			RtInt j = (RtInt)y;
			RiCurrentContext -> FrameBuffer[i][j][0][0]->r = r;
			RiCurrentContext -> FrameBuffer[i][j][0][0]->g = g;
			RiCurrentContext -> FrameBuffer[i][j][0][0]->b = b;
			RiCurrentContext -> FrameBuffer[i][j][0][0]->a = a;
			RiCurrentContext -> FrameBuffer[i][j][0][0]->z = RiCurrentContext -> Far;
}

RiContext::~RiContext(){
	DeleteFrameBuffer();
}

RtVoid RiContext::AllocateFrameBuffer(){
	FrameBuffer = new JRiPixel**** [XResolution];
	for(int i = 0;i < XResolution;i++){
		FrameBuffer[i] = new JRiPixel*** [YResolution];	
		for(int j = 0;j < YResolution;j++){
			FrameBuffer[i][j] = new JRiPixel** [(RtInt)XSamples];
			for(int k = 0;k < XSamples;k++){
				FrameBuffer[i][j][k] = new JRiPixel* [(RtInt)YSamples];
				for(int l = 0;l < YSamples;l++){
					RtFloat du = (RtFloat)k/XSamples;
					RtFloat dv = (RtFloat)l/XSamples;
					FrameBuffer[i][j][k][l] = new JRiPixel(0,0,0,0,0,(RtFloat)i + du,(RtFloat)j + dv);
				}
			}
		}
	}
}

RtVoid RiContext::DeleteFrameBuffer(){
	for(int i = 0;i < XResolution;i++){
		for(int j = 0;j < YResolution;j++){
			for(int k = 0;k < XSamples;k++){
				for(int l = 0;l < YSamples;l++){
					delete(FrameBuffer[i][j][k][l]);
				}
				delete(FrameBuffer[i][j][k]);
			}
			delete(FrameBuffer[i][j]);
		}	
		delete(FrameBuffer[i]);
	}
	delete(RiCurrentContext -> FrameBuffer);
};

JRiPixel::JRiPixel(RtFloat rr,RtFloat gg,RtFloat bb,RtFloat aa,RtFloat zz,RtFloat uu,RtFloat vv){
	r = rr;
	g = gg;
	b = bb;
	a = aa;
	z = zz;
	//Set the u and v coordinate of this pixel on the screen
	//The one received by the constructor is that of it without jitter, so i should jitter it within
	RtFloat maxdu = 1.0/(RiCurrentContext -> XSamples);
	RtFloat maxdv = 1.0/(RiCurrentContext -> YSamples);
	u = uu + maxdu * ((RtFloat)(rand() % 1000))/1000.0;
	v = vv + maxdv * ((RtFloat)(rand() % 1000))/1000.0;
}

//Graphics States
RtVoid RiBegin(RtToken name){
	RiCurrentContext = new RiContext();
	RiIdentity();
	RiCurrentContext -> XSamples = 4;
	RiCurrentContext -> YSamples = 4;	
	srand(time(NULL));
	return;
}
RtVoid RiEnd(){
	delete(RiCurrentContext);
	return;
}

RtVoid RiFormat(RtInt xresolution,RtInt yresolution,RtFloat pixelaspectratio){
	RiCurrentContext -> XResolution = xresolution;
	RiCurrentContext -> YResolution = yresolution;
	RiCurrentContext -> PixelAspectRatio = pixelaspectratio;
	RiCurrentContext -> FrameAspectRatio = (xresolution * pixelaspectratio)/yresolution;
	RiCurrentContext -> AllocateFrameBuffer();
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
	RiCurrentContext -> DeleteFrameBuffer();
	RiCurrentContext -> XSamples = xsamples;
	RiCurrentContext -> YSamples = ysamples;
	RiCurrentContext -> AllocateFrameBuffer();
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
				JRiPixel px(0,0,0,0,0,0,0);
				RiGetSampledPixel(i,j,&px);
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
			0,0,1,0};
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
	RtFloat tm = (thetamax * M_PI)/180.0;
	for(RtInt j = 0;j < mesh->GetHeight();j++){
		for(RtInt i = 0;i < mesh->GetWidth();i++){
			RtFloat u = (RtFloat)i/(RtFloat)mesh->GetWidth();
			RtFloat v = (RtFloat)j/(RtFloat)mesh->GetHeight();
			RtFloat phi = phimin + v * (phimax - phimin);
			RtFloat theta = tm*u;
			RtFloat x = radius * cos(theta) * cos(phi);
			RtFloat y = radius * sin(theta) * cos(phi);
			RtFloat z = radius * sin(phi);
			mesh->Set(	i,j,
					x,y,z,
					x,y,z,
					u,1,v,1,
					u,v);
		}
	}
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

RtVoid RiGetSampledPixel(RtInt u,RtInt v,JRiPixel* col){
	for(RtInt j = 0;j < RiCurrentContext -> YSamples;j++){
		for(RtInt i = 0;i < RiCurrentContext -> XSamples;i++){
			col->r += RiCurrentContext -> FrameBuffer[u][v][i][j]->r;
			col->g += RiCurrentContext -> FrameBuffer[u][v][i][j]->g;
			col->b += RiCurrentContext -> FrameBuffer[u][v][i][j]->b;
			col->a += RiCurrentContext -> FrameBuffer[u][v][i][j]->a;
			col->z += RiCurrentContext -> FrameBuffer[u][v][i][j]->z;
		}
	}
	RtFloat div =(RtFloat)RiCurrentContext -> XSamples * (RtFloat)RiCurrentContext -> YSamples;
	col->r = (col->r/div)*255;
	col->g = (col->g/div)*255;
	col->b = (col->b/div)*255;
	col->a = (col->a/div)*255;
	col->z = (col->z/div)*255;
	return;
}
