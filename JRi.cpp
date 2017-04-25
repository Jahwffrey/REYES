//John Thomason
//ID 3058344

#include "JRi.h"
#include "Ri.h"
#include <math.h>
#include <algorithm>
#include <iostream>

JRiPoint::JRiPoint(RtFloat x,RtFloat y,RtFloat z,RtFloat w){
	pt[0] = x;
	pt[1] = y;
	pt[2] = z;
	pt[3] = w;
}

RtFloat JRiPoint::x(){
	return pt[0];
}

RtFloat JRiPoint::y(){
	return pt[1];
}

RtFloat JRiPoint::z(){
	return pt[2];
}

RtFloat JRiPoint::w(){
	return pt[3];
}

RtFloat JRiPoint::r(){
	return pt[0];
}

RtFloat JRiPoint::g(){
	return pt[1];
}

RtFloat JRiPoint::b(){
	return pt[2];
}

RtFloat JRiPoint::a(){
	return pt[3];
}

RtVoid JRiPoint::MoveToScreen(){
	Mult(&(RiCurrentContext -> CurrentTransform));
	Mult(&(RiCurrentContext -> ViewTransform));
	Mult(&(RiCurrentContext -> ScreenTransform));

	for(int i = 0;i < 3;i++){
		pt[i] = pt[i]/(pt[3] + 0.000001);
	}
	return;
}

RtVoid JRiPoint::Mult(RtMatrix* mat){
	RtHpoint npt = {0,0,0,0};
	for(int j = 0;j < 4;j++){
		for(int i = 0;i < 4;i++){
			npt[j] += (*mat)[i][j] * pt[i];
		}
	}
	
	for(int i = 0;i < 4;i++){
		pt[i] = npt[i];
	}
}

RtVoid JRiPoint::DumpToScreen(RtFloat r,RtFloat g,RtFloat b){
	if(pt[3] < -(RiCurrentContext -> Near) && pt[0] >= 0 && pt[1] >= 0 && (RtInt)pt[0] < RiCurrentContext -> XResolution && (RtInt)pt[1] < RiCurrentContext -> YResolution){
		//RiCurrentContext -> FrameBuffer[(RtInt)pt[0]][(RtInt)pt[1]].r = 255;
		WriteFrameBuffer(pt[0],pt[1],r*255,g*255,b*255,0,pt[3]);
	}
}

RtVoid JRiPoint::Set(RtFloat a,RtFloat b,RtFloat c,RtFloat d){
	pt[0] = a;
	pt[1] = b;
	pt[2] = c;
	pt[3] = d;
	return;
}
RtVoid JRiPoint::Normalize(){
	RtFloat len = sqrt(pow(pt[0],2) + pow(pt[1],2) + pow(pt[2],2));
	pt[0] = pt[0]/len;
	pt[1] = pt[1]/len;
	pt[2] = pt[2]/len;
	return;
}

//Vertex
JRiVertex::JRiVertex(RtFloat x,RtFloat y,RtFloat z,RtFloat nx,RtFloat ny,RtFloat nz,RtFloat r,RtFloat g,RtFloat b,RtFloat a,RtFloat tx,RtFloat ty){
	pos = new JRiPoint(x,y,z,1);	
	norm = new JRiPoint(nx,ny,nz,1);
	norm->Normalize();	
	col = new JRiPoint(r,g,b,a);	
	texpos = new JRiPoint(tx,ty,0,0);	
}

JRiVertex::~JRiVertex(){
	delete(pos);
	delete(norm);
	delete(col);
	delete(texpos);
}

RtVoid JRiVertex::Set(RtFloat x,RtFloat y,RtFloat z,RtFloat nx,RtFloat ny,RtFloat nz,RtFloat r,RtFloat g,RtFloat b,RtFloat a,RtFloat tx,RtFloat ty){
	pos->Set(x,y,z,1);	
	norm->Set(nx,ny,nz,1);
	norm->Normalize();	
	col->Set(r,g,b,a);	
	texpos->Set(tx,ty,0,0);	
	return;
}

RtVoid JRiVertex::MoveToScreen(){
	pos->MoveToScreen();
	return;
}

RtVoid JRiVertex::Draw(){
	//pos->DumpToScreen(col->r(),col->g(),col->b());
	
	return;
}
JRiPoint* JRiVertex::GetPos(){
	return pos;
}
JRiPoint* JRiVertex::GetNorm(){
	return norm;
}
JRiPoint* JRiVertex::GetCol(){
	return col;
}
JRiPoint* JRiVertex::GetTexPos(){
	return texpos;
}
//Mesh
JRiMesh::JRiMesh(RtInt w,RtInt h){
	width = w;//w;
	height = h;//h;
	mesh = new JRiVertex**[width];
	for(int i = 0;i < width;i++){
		mesh[i] = new JRiVertex*[height];
		for(int j = 0;j < height;j++){
			mesh[i][j] = new JRiVertex(0,0,0,0,0,0,0,0,0,0,0,0);
		}
	}
}

JRiMesh::~JRiMesh(){
	for(int i = 0;i < width;i++){
		for(int j = 0;j < height;j++){
			delete(mesh[i][j]);
		}
		delete(mesh[i]);
	}
	delete(mesh);
}

RtVoid JRiMesh::Set(RtInt mx,RtInt my,RtFloat x,RtFloat y,RtFloat z,RtFloat nx,RtFloat ny,RtFloat nz,RtFloat r,RtFloat g,RtFloat b,RtFloat a,RtFloat tx,RtFloat ty){
	mesh[mx][my] -> Set(x,y,z,nx,ny,nz,r,g,b,a,tx,ty);
	return;
}

bool JRiMesh::SampleInsideMicrotriangle(JRiPixel* px,JRiPoint* a,JRiPoint* b,JRiPoint* c){
	//Determine if a point is in the triangle via barycentric coords
	//2d Vectors
	RtFloat vx_x = b->x() - a->x();
	RtFloat vx_y = b->y() - a->y();
	RtFloat vy_x = c->x() - a->x();
	RtFloat vy_y = c->y() - a->y();
	RtFloat vp_x = px->u - a->x();
	RtFloat vp_y = px->v - a->y();

	//Dot Products
	RtFloat dotxx = vx_x * vx_x + vx_y * vx_y;	
	RtFloat dotxy = vx_x * vy_x + vx_y * vy_y;	
	RtFloat dotxp = vx_x * vp_x + vx_y * vp_y;	
	RtFloat dotyy = vy_x * vy_x + vy_y * vy_y;	
	RtFloat dotyp = vy_x * vp_x + vy_y * vp_y;

	//Compute new coords
	RtFloat divisor = dotxx * dotyy - dotxy*dotxy;
	RtFloat nu = (dotyy*dotxp - dotxy*dotyp)/divisor;
	RtFloat nv = (dotxx*dotyp - dotxy*dotxp)/divisor;
		
	//Do the check
	return ((nu >= 0) && (nv >= 0) && (nu + nv < 1));
}

RtVoid JRiMesh::DrawMicropolygon(JRiVertex* ul,JRiVertex* ur,JRiVertex* ll,JRiVertex* lr){
	RtFloat uarr[4] = {ul->GetPos()->x(),ur->GetPos()->x(),ll->GetPos()->x(),lr->GetPos()->x()};
	RtFloat varr[4] = {ul->GetPos()->y(),ur->GetPos()->y(),ll->GetPos()->y(),lr->GetPos()->y()};

	//Bound	
	RtFloat minu = uarr[0];
	RtFloat maxu = uarr[0];	
	RtFloat minv = varr[0];
	RtFloat maxv = varr[0];
	for(int i = 1;i < 4;i++){
		if(uarr[i] < minu) minu = uarr[i];
		if(uarr[i] > maxu) maxu = uarr[i];
		if(varr[i] < minv) minv = varr[i];
		if(varr[i] > maxv) maxv = varr[i];
	}

	//Loop over relevant screen pixels
	for(int j = std::max(0,(int)minv);j <= std::min(RiCurrentContext -> YResolution - 1,(int)maxv);j++){
		for(RtInt i = std::max(0,(RtInt)minu);i <= std::min(RiCurrentContext -> XResolution - 1,(RtInt)maxu);i++){
			//Loop over samples
			for(RtInt l = 0;l < RiCurrentContext -> YSamples;l++){
				for(RtInt k = 0;k < RiCurrentContext -> XSamples;k++){
					//Check if sample is in quad
					JRiPixel* px = RiCurrentContext -> FrameBuffer[i][j][k][l];
					if(SampleInsideMicrotriangle(px,ul->GetPos(),ur->GetPos(),ll->GetPos()) || SampleInsideMicrotriangle(px,ur->GetPos(),ll->GetPos(),lr->GetPos())){
						//Color the sample
						if(px->z > (ul->GetPos()->w())){
							px->r = ul->GetCol()->r();
							px->g = ul->GetCol()->g();
							px->b = ul->GetCol()->b();
							px->a = ul->GetCol()->a();
							px->z = ul->GetPos()->w();
						}
					}

				}
			}
		}
	}

	return;
}

RtVoid JRiMesh::Draw(){
	//Project all vertices to screen space
	for(int j = 0;j < height;j++){
		for(int i = 0;i < width;i++){
			mesh[i][j] -> MoveToScreen();
		}
	}
	//Draw each micropolygon
	for(int j = 0;j < height - 1;j++){
		for(int i = 0;i < width - 1;i++){
			//DrawMicropolygon(mesh[i][j],mesh[(i + 1)%width][j],mesh[i][(j + 1)%height],mesh[(i + 1)%width][(j + 1)%height]);
			DrawMicropolygon(mesh[i][j],mesh[(i + 1)][j],mesh[i][(j + 1)],mesh[(i + 1)][(j + 1)]);
		}
	}
	return;
}


RtInt JRiMesh::GetWidth(){
	return width;
}

RtInt JRiMesh::GetHeight(){
	return height;
}
