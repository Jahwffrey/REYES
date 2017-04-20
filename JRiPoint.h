//John Thomason
//ID 3058344

#ifndef _JRIPOINT_H_
#define _JRIPOINT_H_

#include "Ri.h"

class JRiPoint{
	private:
		RtHpoint pt;
	public:
		JRiPoint(RtFloat x,RtFloat y,RtFloat z,RtFloat w);
		RtFloat x();
		RtFloat y();
		RtFloat z();
		RtFloat w();
		RtFloat r();
		RtFloat g();
		RtFloat b();
		RtFloat a();
		RtVoid Mult(RtMatrix* mat);
		RtVoid MoveToScreen();
		RtVoid DumpToScreen();
		RtVoid set(RtFloat a,RtFloat b,RtFloat c,RtFloat d);
		RtVoid normalize();
};

class JRiVertex {
	private:
		JRiPoint* pos;//Position
		JRiPoint* norm;//Normal
		JRiPoint* col;//Color
		JRiPoint* texpos;//texture coord
	public:
		JRiVertex(RtFloat x,RtFloat y,RtFloat z,RtFloat nx,RtFloat ny,RtFloat nz,RtFloat r,RtFloat g,RtFloat b,RtFloat a,RtFloat tx,RtFloat ty);
		~JRiVertex();
};

#endif
