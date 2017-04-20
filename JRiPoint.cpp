//John Thomason
//ID 3058344

#include "JRiPoint.h"
#include "Ri.h"

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
		RiCurrentContext -> FrameBuffer[(RtInt)pt[0]][(RtInt)pt[1]].r = 255;
	}
}
