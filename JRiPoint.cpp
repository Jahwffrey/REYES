//John Thomason
//ID 3058344

#include "JRiPoint.h"
#include "Ri.h"
#include <math.h>

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

RtVoid JRiPoint::DumpToScreen(){
	if(pt[3] < -(RiCurrentContext -> Near) && pt[0] >= 0 && pt[1] >= 0 && (RtInt)pt[0] < RiCurrentContext -> XResolution && (RtInt)pt[1] < RiCurrentContext -> YResolution){
		//RiCurrentContext -> FrameBuffer[(RtInt)pt[0]][(RtInt)pt[1]].r = 255;
		WriteFrameBuffer(pt[0],pt[1],255,255,255,0,pt[3]);
	}
}

RtVoid JRiPoint::set(RtFloat a,RtFloat b,RtFloat c,RtFloat d){
	pt[0] = a;
	pt[1] = b;
	pt[2] = c;
	pt[3] = d;
	return;
}
RtVoid JRiPoint::normalize(){
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
	norm->normalize();	
	col = new JRiPoint(r,g,b,a);	
	texpos = new JRiPoint(tx,ty,0,0);	
}

JRiVertex::~JRiVertex(){
	delete(pos);
	delete(norm);
	delete(col);
	delete(texpos);
}
