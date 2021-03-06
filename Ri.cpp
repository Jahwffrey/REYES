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
RtColor _Cs;//color of a point
RtColor _Os;//opacity of a point
RtPoint _P;//3d position of a point
RtVector _dPdu;//Derivative of position of a point along u
RtVector _dPdv;//Derivative of position of a point along v
RtNormal _N;//The normal of point
RtFloat _U;//texture coord x
RtFloat _V;//texture coord v
RtFloat _dU;//derivative of surface params
RtFloat _dV;//derivative of surface params
RtFloat BUMP_AMPLITUDE = 1;
RtFloat BUMP_MIN_FREQ_EXP = 2;
RtFloat BUMP_MAX_FREQ_EXP = 6;

//RtVoid JRiImage::GetUVPoint(RtFloat u,RtFloat v,RtFloat* vals){
RtVoid texture(int ind){
	RtFloat vals[6];
	images[ind]->GetUVPoint(_U,_V,vals);
	_Cs[0] = vals[0];
	_Cs[1] = vals[1];
	_Cs[2] = vals[2];
	_Os[0] = vals[3];
	_Os[1] = vals[4];
	_Os[2] = vals[5];
	return;
}

void default_shader(void){
	return;
}

//RiContext Stuff
RtVoid WriteFrameBuffer(RtFloat x,RtFloat y,RtFloat r,RtFloat g,RtFloat b,RtFloat ar,RtFloat ag,RtFloat ab,RtFloat z){
			RtInt i = (RtInt)x;
			RtInt j = (RtInt)y;
			RiCurrentContext -> FrameBuffer[i][j][0][0]->r = r;
			RiCurrentContext -> FrameBuffer[i][j][0][0]->g = g;
			RiCurrentContext -> FrameBuffer[i][j][0][0]->b = b;
			RiCurrentContext -> FrameBuffer[i][j][0][0]->ar = ar;
			RiCurrentContext -> FrameBuffer[i][j][0][0]->ag = ag;
			RiCurrentContext -> FrameBuffer[i][j][0][0]->ab = ab;
			RiCurrentContext -> FrameBuffer[i][j][0][0]->z = z;
}

RiContext::RiContext(){};



RiContext::~RiContext(){
	//Im not deleting frame buffer cuz i want there to be only one preseved throughout runtime
	//DeleteFrameBuffer();
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
					FrameBuffer[i][j][k][l] = new JRiPixel(0,0,0,0,0,0,Far,(RtFloat)i + du,(RtFloat)j + dv,du,dv);
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

JRiPixel::JRiPixel(RtFloat rr,RtFloat gg,RtFloat bb,RtFloat aar,RtFloat aag,RtFloat aab, RtFloat zz,RtFloat uu,RtFloat vv,RtFloat duu,RtFloat dvv){
	r = rr;
	g = gg;
	b = bb;
	ar = aar;
	ag = aag;
	ab = aab;
	z = zz;
	//Set the u and v coordinate of this pixel on the screen
	//The one received by the constructor is that of it without jitter, so i should jitter it within
	RtFloat maxdu = 1.0/(RiCurrentContext -> XSamples);
	RtFloat maxdv = 1.0/(RiCurrentContext -> YSamples);
	u = uu + maxdu * ((RtFloat)(rand() % 1000))/1000.0;
	v = vv + maxdv * ((RtFloat)(rand() % 1000))/1000.0;
	du = duu;
	dv = dvv;
	next = RI_NULL;
}

JRiPixel::~JRiPixel(){
	if(next != RI_NULL) delete(next);
}
RtFloat JRiPixel::GetFinalR(){
	RtFloat col = 0;
	if(next != RI_NULL) col = next->GetFinalR();
	return std::min(col + r*ar,(float)1);
}

RtFloat JRiPixel::GetFinalG(){
	RtFloat col = 0;
	if(next != RI_NULL) col = next->GetFinalG();
	return std::min(col + g*ag,(float)1);
}

RtFloat JRiPixel::GetFinalB(){
	RtFloat col = 0;
	if(next != RI_NULL) col = next->GetFinalB();
	return std::min(col + b*ab,(float)1);
}

RtFloat JRiPixel::GetFinalAr(){
	RtFloat opa = 0;
	if(next != RI_NULL) opa = next->GetFinalAr();
	return std::min(ar + opa,(float)1);
}

RtFloat JRiPixel::GetFinalAg(){
	RtFloat opa = 0;
	if(next != RI_NULL) opa = next->GetFinalAg();
	return std::min(ag + opa,(float)1);
}

RtFloat JRiPixel::GetFinalAb(){
	RtFloat opa = 0;
	if(next != RI_NULL) opa = next->GetFinalAb();
	return std::min(ab + opa,(float)1);
}

RtVoid JRiPixel::AddSample(RtFloat sr,RtFloat sg,RtFloat sb,RtFloat sar,RtFloat sag,RtFloat sab,RtFloat sz){
	if(sz < RiCurrentContext -> Near) return;
	if(sar >= 0.99 && sag >= 0.99 && sab >= 0.99){
		//Full opaque
		if(z > sz){	
			if(next != RI_NULL) delete(next);
			r = sr;
			g = sg;
			b = sb;
			ar = sar;
			ag = sag;
			ab = sab;
			z = sz;
		} else {
			return;
			/*if(next == RI_NULL){
				next = new JRiPixel(sr,sg,sb,sar,sag,sab,sz,u,v,du,dv);
			} else {
				next->AddSample(sr,sg,sb,sar,sag,sab,sz);
			}*/
		}
	} else {
		//If this point is in front of me
		if(z > sz){
			//copy myself and put it behind myself before setting my new values, then put it into list
			JRiPixel* newpx = new JRiPixel(r,g,b,ar,ag,ab,z,u,v,du,dv);
			r = sr;
			g = sg;
			b = sb;
			ar = sar;
			ag = sag;
			ab = sab;
			z = sz;

			newpx->next = next;
			next = newpx;
		} else {
			//if this point is behind me
			if(ar >= 0.99 && ag >= 0.99 && ab >= 0.99){
				//if i am opaque
				return;
			} else {
				if(next == RI_NULL){
					next = new JRiPixel(sr,sg,sb,sar,sag,sab,sz,u,v,du,dv);
				} else {
					next->AddSample(sr,sg,sb,sar,sag,sab,sz);
				}
			}
		}
	}
	return;
}

//Graphics States
RtVoid RiBegin(RtToken name){
	RiCurrentContext = new RiContext();
	RiIdentity();
	RiCurrentContext -> XSamples = 4;
	RiCurrentContext -> YSamples = 4;	
	RiCurrentContext -> CurrentColor[0] = 0;
	RiCurrentContext -> CurrentColor[1] = 0;
	RiCurrentContext -> CurrentColor[2] = 0;
	RiCurrentContext -> CurrentOpacity[0] = 1;
	RiCurrentContext -> CurrentOpacity[1] = 1;
	RiCurrentContext -> CurrentOpacity[2] = 1;
	RiCurrentContext -> DisplacementShaderFunction = &default_shader;
	RiCurrentContext -> SurfaceShaderFunction = &default_shader;
	RiCurrentContext -> Near = 0.1;
	RiCurrentContext -> Far = 10000;	
	srand(time(NULL));
	return;
}
RtVoid RiEnd(){
	delete(RiCurrentContext);
	return;
}

RtVoid RiFormat(RtInt xresolution,RtInt yresolution,RtFloat pixelaspectratio){
	if(RiCurrentContext -> WorldBegun == 1) return;
	RiCurrentContext -> XResolution = xresolution;
	RiCurrentContext -> YResolution = yresolution;
	RiCurrentContext -> PixelAspectRatio = pixelaspectratio;
	RiCurrentContext -> FrameAspectRatio = (xresolution * pixelaspectratio)/yresolution;
	RiCurrentContext -> AllocateFrameBuffer();
	return;
}

RtVoid RiTransformBegin(){
	RiCurrentContext -> TransBegun = 1;
	return;
}

RtVoid RiTransformEnd(){
	//pretending to pop stack
	/*for(int j = 0;j < 4;j++){
		for(int i = 0;i < 4;i++){
			RiCurrentContext -> CurrentTransform[i][j] = RiCurrentContext -> ViewTransform[i][j];
		}
	}*/
	RiIdentity();
	RiCurrentContext -> TransBegun = 0;
	return;
}

RtVoid RiClipping(RtFloat near,RtFloat far){
	if(RiCurrentContext -> WorldBegun == 1) return;
	RiCurrentContext -> Near = near;
	RiCurrentContext -> Far = far;
	return;
}

RtVoid RiProjection(RtToken name,RtToken paramname,RtFloat *fov){
	if(strcmp(name,RI_PERSPECTIVE) == 0){
		if(strcmp(paramname,RI_FOV) == 0){
			RiPerspective(*fov);
		} else {
			RiPerspective(90.0);//M_PI/2);
		}
	} else if(strcmp(name,RI_ORTHOGRAPHIC) == 0){
		RtFloat ZScale = (RiCurrentContext->Far)/(RiCurrentContext-> Far - RiCurrentContext -> Near);
		RtMatrix ortho = {1,0,0,0,
				  0,1,0,0,
				  0,0,ZScale,0,
				  0,0,0,1};
		RiConcatTransform(ortho);
	} //else if (name == RI_NULL){
	
	for(int j = 0;j < 4;j++){
		for(int i = 0;i < 4;i++){
			RiCurrentContext -> ScreenTransform[i][j] = RiCurrentContext -> CurrentTransform[i][j];
		}
	}
	RiIdentity();
}

RtVoid RiWorldBegin(){
	RiCurrentContext -> WorldBegun = 1;
	for(int j = 0;j < 4;j++){
		for(int i = 0;i < 4;i++){
			RiCurrentContext -> ViewTransform[i][j] = RiCurrentContext -> CurrentTransform[i][j];
		}
	}
	RiIdentity();
	return;
}

RtVoid RiWorldEnd(){
	RiCurrentContext -> WorldBegun = 0;
	return;
}

RtVoid RiFrameAspectRatio(RtFloat rat){
	if(RiCurrentContext -> WorldBegun == 1) return;
	RiCurrentContext -> FrameAspectRatio = rat;
	return;
}

RtVoid RiPixelSamples(RtFloat xsamples,RtFloat ysamples){
	if(RiCurrentContext -> WorldBegun == 1) return;
	RiCurrentContext -> DeleteFrameBuffer();
	RiCurrentContext -> XSamples = xsamples;
	RiCurrentContext -> YSamples = ysamples;
	RiCurrentContext -> AllocateFrameBuffer();
	return;
}

RtVoid RiFrameBegin(RtInt frame){
	RiContext* nc = new RiContext();
	nc->Copy(RiCurrentContext);
	nc->prev = RiCurrentContext;
	RiCurrentContext = nc;
	RiCurrentContext -> FrameBegun = 1;
	RiCurrentContext -> CurrentFrame = frame;
	return;
}

RtVoid RiContext::Copy(RiContext* other){
	for(int j = 0;j < 4;j++){
		for(int i = 0;i < 4;i++){
			CurrentTransform[i][j] = other->CurrentTransform[i][j];
			ViewTransform[i][j] = other->ViewTransform[i][j];
			ScreenTransform[i][j] = other->ScreenTransform[i][j];
		}
	} 
	//They are all gana share the same frame buffer pointer
	FrameBuffer = other -> FrameBuffer;
	XResolution = other -> XResolution;
	YResolution = other -> YResolution;
	XSamples = other -> XSamples;
	YSamples = other -> YSamples;
	PixelAspectRatio = other -> PixelAspectRatio;
	FrameAspectRatio = other -> FrameAspectRatio;
	Near = other -> Near;
	Far = other -> Far;
	TransBegun = other -> TransBegun;
	WorldBegun = other -> WorldBegun;
	FrameBegun = other -> FrameBegun;
	CurrentFrame = other -> CurrentFrame;
	for(int i = 0;i < 3;i++){
		CurrentColor[i] = other -> CurrentColor[i];
		CurrentOpacity[i] = other -> CurrentOpacity[i];
	}
	DisplacementShaderFunction = other -> DisplacementShaderFunction;
	SurfaceShaderFunction = other -> SurfaceShaderFunction;
	//Im allocation a fresh frame buffer, the frame buffer is not a rendering option
	//AllocateFrameBuffer();
	return;
}

RtVoid RiFrameEnd(){
	RiContext* old = RiCurrentContext;
	RiCurrentContext = old->prev;
	delete(old);
	RiCurrentContext -> FrameBegun = 0;
	RiCurrentContext -> CurrentFrame = -1;
	for(int i = 0;i < images.size();i++){
		if(images[i] != RI_NULL) delete(images[i]);
	}
	return;
}

RtVoid RiDisplay(RtToken name,RtToken type,RtToken mode,RtToken paramlist,RtPointer orig){
	//I NEED TO IMPLEMENT SUPPORT FOR ORIGIN!!!!THIS ISNT DONE YET!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	std::stringstream fname;
	//If writing to file
	if(strcmp(type,"file") == 0){
		FILE *ofile = fopen(name,"wb");
		fprintf(ofile,"P6\n%d %d\n255\n",RiCurrentContext -> XResolution,RiCurrentContext -> YResolution);
		for(int j = 0;j < RiCurrentContext -> YResolution;j++){
			for(int i = 0;i < RiCurrentContext -> XResolution;i++){
				int nm = 0;
				unsigned char col[5];
				JRiPixel px(0,0,0,0,0,0,0,0,0,0,0);
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
					col[3] = px.ar;
				} else if(strcmp(mode,"rgbaz") == 0){
					nm = 5;
					col[0] = px.r;
					col[1] = px.g;
					col[2] = px.b;
					col[3] = px.ar;
					col[4] = px.z;
				} else if(strcmp(mode,"rgbz") == 0){
					nm = 4;
					col[0] = px.r;
					col[1] = px.g;
					col[2] = px.b;
					col[3] = px.z;
				} else if(strcmp(mode,"a") == 0){
					nm = 1;
					col[0] = px.ar;
				} else if(strcmp(mode,"az") == 0){
					nm = 2;
					col[0] = px.ar;
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
	RtFloat angl = (angle * M_PI)/180.0;
	RtFloat len = sqrt(pow(dx,2) + pow(dy,2) + pow(dz,2));
	dx = dx / len;
	dy = dy / len;
	dz = dz / len;
	RtFloat co = cos(angl);
	RtFloat si = sin(angl);
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
	RtFloat ff = (((fov * M_PI)/180.0)*M_PI)/180;
	//RtFloat ff = ((fov * M_PI)/180.0);
	
	RtFloat ScaleFactor = 1.0 /(tan(ff/2));
	RtFloat aspect = 1/(RiCurrentContext->PixelAspectRatio);
	RtFloat ScaleFactorAspect = 1.0 /(tan((ff/2)*aspect));
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
	//First find bounding box
	RtFloat bbox[4];
	FindBoundingBox(radius*2,bbox);
	RtFloat screenwidth = std::max(bbox[1] - bbox[0],bbox[3] - bbox[2]);

	//Then create the mesh
	JRiMesh* mesh = new JRiMesh((RtInt)(screenwidth*8.0),(RtInt)(screenwidth*4.0));
	//JRiMesh* mesh = new JRiMesh((RtInt)(screenwidth*4.0),(RtInt)(screenwidth*4.0));
	//JRiMesh* mesh = new JRiMesh((RtInt)(screenwidth/20.0),(RtInt)(screenwidth/40.0));
	RtFloat phimin = -M_PI/2.0;
	if(zmin > -radius) phimin = asin(zmin/radius);
	RtFloat phimax = M_PI/2.0;
	if(zmax < radius) phimax = asin(zmax/radius);
	RtFloat tm = (thetamax * M_PI)/180.0;
	for(RtInt j = 0;j < mesh->GetHeight();j++){
		RtFloat VInd = (RtFloat)j;//(j % mesh->GetHeight());
		for(RtInt i = 0;i < mesh->GetWidth();i++){
			RtFloat UInd = (RtFloat)i;//(i % mesh->GetWidth());
			RtFloat u = UInd/(RtFloat)(mesh->GetWidth() - 1);
			RtFloat v = VInd/(RtFloat)(mesh->GetHeight() - 1);
			RtFloat phi = phimin + v * (phimax - phimin);
			RtFloat theta = tm*u;
			RtFloat x = radius * cos(theta) * cos(phi);
			RtFloat y = radius * sin(theta) * cos(phi);
			RtFloat z = radius * sin(phi);
			mesh->Set(	i,j, //index
					x,y,z, //world pos
					x,y,z, //normal
					RiCurrentContext -> CurrentColor[0],RiCurrentContext -> CurrentColor[1],RiCurrentContext -> CurrentColor[2],//Color
					RiCurrentContext -> CurrentOpacity[0],RiCurrentContext -> CurrentOpacity[1],RiCurrentContext -> CurrentOpacity[2],//Opacity
					u,v,//Texture coords
					0,0);//derivatives
		}
	}
	//Draw the mesh
	mesh->Draw();
	//Delete the mesh
	delete(mesh);
	return;
}


RtVoid RiCone(RtFloat height,RtFloat radius,RtFloat thetamax,RtPointer param){
	//First find bounding box
	RtFloat bbox[4];
	FindBoundingBox(std::max(height,radius*2),bbox);
	RtFloat screenwidth = std::max(bbox[1] - bbox[0],bbox[3] - bbox[2]);

	//Then create the mesh
	RtFloat tm = (thetamax * M_PI)/180.0;
	JRiMesh* mesh = new JRiMesh((RtInt)(screenwidth*4),(RtInt)(screenwidth*2));
	for(RtInt j = 0;j < mesh->GetHeight();j++){
		RtFloat VInd = (RtFloat)j;
		for(RtInt i = 0;i < mesh->GetWidth();i++){
			RtFloat UInd = (RtFloat)i;
			RtFloat u = UInd/(RtFloat)(mesh->GetWidth() - 1);
			RtFloat v = VInd/(RtFloat)(mesh->GetHeight() - 1);
			
			RtFloat theta = u * tm;
			RtFloat x = radius * (1 - v) * cos(theta);
			RtFloat y = radius * (1 - v) * sin(theta);
			RtFloat z = v * height;

			mesh->Set(	i,j, //index
					x,y,z, //world pos
					x,y,z, //normal                               //THIS IS INCORRECT!!!!
					RiCurrentContext -> CurrentColor[0],RiCurrentContext -> CurrentColor[1],RiCurrentContext -> CurrentColor[2],//Color
					RiCurrentContext -> CurrentOpacity[0],RiCurrentContext -> CurrentOpacity[1],RiCurrentContext -> CurrentOpacity[2],//Opacity
					u,v,//Texture coords
					0,0);//derivatives
		}
	}
	mesh->Draw();
	delete(mesh);
	return;
}

RtVoid RiCylinder(RtFloat radius,RtFloat zmin,RtFloat zmax,RtFloat thetamax, RtPointer param){
	//First find bounding box
	RtFloat bbox[4];
	FindBoundingBox(std::max(radius * 2,zmax - zmin),bbox);
	RtFloat screenwidth = std::max(bbox[1] - bbox[0],bbox[3] - bbox[2]);

	//Then create the mesh
	RtFloat tm = (thetamax * M_PI)/180.0;
	JRiMesh* mesh = new JRiMesh((RtInt)(screenwidth*4),(RtInt)(screenwidth*2));
	for(RtInt j = 0;j < mesh->GetHeight();j++){
		RtFloat VInd = (RtFloat)j;
		for(RtInt i = 0;i < mesh->GetWidth();i++){
			RtFloat UInd = (RtFloat)i;
			RtFloat u = UInd/(RtFloat)(mesh->GetWidth() - 1);
			RtFloat v = VInd/(RtFloat)(mesh->GetHeight() - 1);
			
			RtFloat theta = u * tm;
			RtFloat x = radius * cos(theta);
			RtFloat y = radius * sin(theta);
			RtFloat z = v * (zmax - zmin);

			mesh->Set(	i,j, //index
					x,y,z, //world pos
					x,y,z, //normal                               //THIS IS INCORRECT!!!!
					RiCurrentContext -> CurrentColor[0],RiCurrentContext -> CurrentColor[1],RiCurrentContext -> CurrentColor[2],//Color
					RiCurrentContext -> CurrentOpacity[0],RiCurrentContext -> CurrentOpacity[1],RiCurrentContext -> CurrentOpacity[2],//Opacity
					u,v,//Texture coords
					0,0);//derivatives
		}
	}
	mesh->Draw();
	delete(mesh);
	return;
}

RtVoid RiTorus(RtFloat majorradius,RtFloat minorradius,RtFloat phimin,RtFloat phimax,RtFloat thetamax, RtPointer param){
	//First find bounding box
	RtFloat bbox[4];
	FindBoundingBox(majorradius*2,bbox);
	RtFloat screenwidth = std::max(bbox[1] - bbox[0],bbox[3] - bbox[2]);

	//Then create the mesh
	RtFloat tm = (thetamax * M_PI)/180.0;
	RtFloat rpmi = (phimin * M_PI)/180.0;
	RtFloat rpma = (phimax * M_PI)/180.0;
	JRiMesh* mesh = new JRiMesh((RtInt)(screenwidth*4),(RtInt)(screenwidth*2));
	for(RtInt j = 0;j < mesh->GetHeight();j++){
		RtFloat VInd = (RtFloat)j;
		for(RtInt i = 0;i < mesh->GetWidth();i++){
			RtFloat UInd = (RtFloat)i;
			RtFloat u = UInd/(RtFloat)(mesh->GetWidth() - 1);
			RtFloat v = VInd/(RtFloat)(mesh->GetHeight() - 1);
			
			RtFloat theta = u * tm;
			RtFloat phi = rpmi + v*(rpma - rpmi);
			RtFloat rr = minorradius * cos(phi);
			RtFloat x = (majorradius + rr) * cos(theta);
			RtFloat y = (majorradius + rr) * sin(theta);
			RtFloat z = minorradius * sin(phi);

			mesh->Set(	i,j, //index
					x,y,z, //world pos
					x,y,z, //normal                               //THIS IS INCORRECT!!!!
					RiCurrentContext -> CurrentColor[0],RiCurrentContext -> CurrentColor[1],RiCurrentContext -> CurrentColor[2],//Color
					RiCurrentContext -> CurrentOpacity[0],RiCurrentContext -> CurrentOpacity[1],RiCurrentContext -> CurrentOpacity[2],//Opacity
					u,v,//Texture coords
					0,0);//derivatives
		}
	}
	mesh->Draw();
	delete(mesh);
	return;
}

//Shading
RtVoid RiColor(RtFloat* col){
	RiCurrentContext -> CurrentColor[0] = col[0];	
	RiCurrentContext -> CurrentColor[1] = col[1];	
	RiCurrentContext -> CurrentColor[2] = col[2];
	return;	
}

RtVoid RiSurface(void(*f)(void)){
	RiCurrentContext -> SurfaceShaderFunction = f;
}

RtVoid RiDisplacement(void(*f)(void)){
	RiCurrentContext -> DisplacementShaderFunction = f;
}

RtVoid RiOpacity(RtFloat* col){
	RiCurrentContext -> CurrentOpacity[0] = col[0];	
	RiCurrentContext -> CurrentOpacity[1] = col[1];	
	RiCurrentContext -> CurrentOpacity[2] = col[2];
	return;	
}

RtVoid RiMakeTexture(RtToken file,RtInt slot){
	while(slot >= images.size()){
		images.push_back(RI_NULL);
	}
	if(images[slot] != RI_NULL){
		delete(images[slot]);
	}
	images[slot] = new JRiImage(1000,1000);
	for(int j = 0;j < 1000;j++){
		for(int i = 0;i < 1000;i++){
			RtFloat u = i / 1000.0;
			RtFloat v = j / 1000.0;
			//RtFloat va[6] = {sin(u*2*M_PI*4),sin(v*2*M_PI*4),1,1,1,1};
			RtFloat va[6] = {u,v,1,1,u,v};
			images[slot] -> SetImageVal(i,j,va);	
		}
	}	
}

//Internal Stuff
RtVoid RiClearBuffer(){
	for(RtFloat j = 0;j < RiCurrentContext -> YResolution;j++){
		for(RtFloat i = 0;i < RiCurrentContext -> XResolution;i++){
			WriteFrameBuffer(i,j,0,0,0,0,0,0,0);
		}
	}
}

RtVoid RiGetSampledPixel(RtInt u,RtInt v,JRiPixel* col){
	for(RtInt j = 0;j < RiCurrentContext -> YSamples;j++){
		for(RtInt i = 0;i < RiCurrentContext -> XSamples;i++){
			col->r += RiCurrentContext -> FrameBuffer[u][v][i][j]->GetFinalR();
			col->g += RiCurrentContext -> FrameBuffer[u][v][i][j]->GetFinalG();
			col->b += RiCurrentContext -> FrameBuffer[u][v][i][j]->GetFinalB();
			col->ar += RiCurrentContext -> FrameBuffer[u][v][i][j]->GetFinalAr();
			col->ag += RiCurrentContext -> FrameBuffer[u][v][i][j]->GetFinalAg();
			col->ab += RiCurrentContext -> FrameBuffer[u][v][i][j]->GetFinalAb();
			col->z += RiCurrentContext -> FrameBuffer[u][v][i][j]->z;
		}
	}
	RtFloat div =(RtFloat)RiCurrentContext -> XSamples * (RtFloat)RiCurrentContext -> YSamples;
	col->r = (col->r/div)*255;
	col->g = (col->g/div)*255;
	col->b = (col->b/div)*255;
	col->ar = (col->ar/div)*255;
	col->ag = (col->ag/div)*255;
	col->ab = (col->ab/div)*255;
	col->z = (col->z/div)*255;
	return;
}

RtVoid FindBoundingBox(RtFloat diameter,RtFloat* vals){
	//umin,umax,vmin,vmax
	RtFloat radius = diameter/2;
	JRiPoint p1 = JRiPoint(-radius,-radius,-radius,1);
	JRiPoint p2 = JRiPoint(0,0,0,1);
	p1.Transform();
	p2.Transform();
	p1.MoveToScreen();
	p2.MoveToScreen();
	vals[0] = abs(p1.x()-p2.x());
	vals[1] = abs(p1.x()-p2.x());
	vals[2] = abs(p1.y()-p2.y());
	vals[3] = abs(p1.y()-p2.y());
	for(RtInt i = -1;i < 2;i++){
		for(RtInt j = -1;j < 2;j++){
			for(RtInt k = -1;k < 2;k++){
				JRiPoint pt = JRiPoint(i*radius,j*radius,k*radius,1);
				pt.Transform();
				pt.MoveToScreen();
				if(pt.x()-p2.x() < vals[0]) vals[0] = abs(pt.x()-p2.x());
				if(pt.x()-p2.x() > vals[1]) vals[1] = abs(pt.x()-p2.x());
				if(pt.y()-p2.y() < vals[2]) vals[2] = abs(pt.y()-p2.y());
				if(pt.y()-p2.y() > vals[3]) vals[3] = abs(pt.y()-p2.y());
			}
		}
	}
	return;
}

RtVoid RunDisplacementShader(){
	RiCurrentContext -> DisplacementShaderFunction();
	return;
}

RtVoid RunSurfaceShader(){
	RiCurrentContext -> SurfaceShaderFunction();
	return;
}
