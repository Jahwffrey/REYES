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
		RtVoid DumpToScreen(RtFloat r,RtFloat g,RtFloat b);
		RtVoid Set(RtFloat a,RtFloat b,RtFloat c,RtFloat d);
		RtVoid Normalize();
};

class JRiVertex {
	private:
		JRiPoint* pos;//Position
		JRiPoint* norm;//Normal
		JRiPoint* col;//Color
		JRiPoint* texpos;//texture coord
	public:
		JRiVertex(RtFloat x,RtFloat y,RtFloat z,RtFloat nx,RtFloat ny,RtFloat nz,RtFloat r,RtFloat g,RtFloat b,RtFloat a,RtFloat tx,RtFloat ty,RtFloat du,RtFloat dv);
		~JRiVertex();
		RtVoid Set(RtFloat x,RtFloat y,RtFloat z,RtFloat nx,RtFloat ny,RtFloat nz,RtFloat r,RtFloat g,RtFloat b,RtFloat a,RtFloat tx,RtFloat ty,RtFloat du,RtFloat dv);
		RtVoid MoveToScreen();
		RtVoid Draw();
		JRiPoint* GetPos();
		JRiPoint* GetNorm();
		JRiPoint* GetCol();
		JRiPoint* GetTexPos();
};

class JRiMesh {
	private:
		JRiVertex*** mesh;
		RtInt width;
		RtInt height;
	public:
		JRiMesh(RtInt w,RtInt h);
		~JRiMesh();
		RtVoid Set(RtInt mx,RtInt my,RtFloat x,RtFloat y,RtFloat z,RtFloat nx,RtFloat ny,RtFloat nz,RtFloat r,RtFloat g,RtFloat b,RtFloat a,RtFloat tx,RtFloat ty,RtFloat du,RtFloat dv);
		RtVoid Draw();
		RtInt GetWidth();
		RtInt GetHeight();
		RtVoid DrawMicropolygon(JRiVertex* ul,JRiVertex* ur,JRiVertex* ll,JRiVertex* lr);
		bool SampleInsideMicrotriangle(JRiPixel* px,JRiPoint* a,JRiPoint* b,JRiPoint* c);
};

#endif
