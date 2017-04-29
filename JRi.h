//John Thomason
//ID 3058344

#ifndef _JRIPOINT_H_
#define _JRIPOINT_H_

#include "Ri.h"
#include <vector>

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
		RtVoid Set(RtFloat a,RtFloat b,RtFloat c,RtFloat d);
		RtVoid Normalize();
		RtVoid Transform();
};

class JRiVertex {
	private:
		JRiPoint* pos;//Position
		JRiPoint* norm;//Normal
		JRiPoint* col;//Color
		JRiPoint* opa;//Opacity
		JRiPoint* texpos;//texture coord
	public:
		JRiVertex(RtFloat x,RtFloat y,RtFloat z,RtFloat nx,RtFloat ny,RtFloat nz,RtFloat r,RtFloat g,RtFloat b,RtFloat ar,RtFloat ag,RtFloat ab,RtFloat tx,RtFloat ty,RtFloat du,RtFloat dv);
		~JRiVertex();
		RtVoid Set(RtFloat x,RtFloat y,RtFloat z,RtFloat nx,RtFloat ny,RtFloat nz,RtFloat r,RtFloat g,RtFloat b,RtFloat ar,RtFloat ag,RtFloat ab,RtFloat tx,RtFloat ty,RtFloat du,RtFloat dv);
		RtVoid MoveToScreen();
		RtVoid Draw();
		JRiPoint* GetPos();
		JRiPoint* GetNorm();
		JRiPoint* GetCol();
		JRiPoint* GetOpa();
		JRiPoint* GetTexPos();
		RtVoid CopyNorm(JRiPoint* onorm);
		RtVoid Transform();
};

class JRiMesh {
	private:
		JRiVertex*** mesh;
		RtInt width;
		RtInt height;
	public:
		JRiMesh(RtInt w,RtInt h);
		~JRiMesh();
		RtVoid Set(RtInt mx,RtInt my,RtFloat x,RtFloat y,RtFloat z,RtFloat nx,RtFloat ny,RtFloat nz,RtFloat r,RtFloat g,RtFloat b,RtFloat ar,RtFloat ag, RtFloat ab,RtFloat tx,RtFloat ty,RtFloat du,RtFloat dv);
		RtVoid Draw();
		RtInt GetWidth();
		RtInt GetHeight();
		RtVoid DrawMicropolygon(JRiVertex* ul,JRiVertex* ur,JRiVertex* ll,JRiVertex* lr);
		RtVoid GetBarycentricCoords(JRiPixel* px,JRiPoint* a,JRiPoint* b,JRiPoint* c,RtFloat* uv);
		RtVoid SetShaderVals(RtInt x,RtInt y);
		RtVoid SetVertexFromShaderVals(RtInt x,RtInt y);
		RtVoid CalcVertexValsForShader(RtInt x,RtInt y);
};

class JRiImage {
	private: 
		RtFloat*** img;
		RtInt width;
		RtInt height;
	public:
		RtVoid GetUVPoint(RtFloat u,RtFloat v,RtFloat* vals);
		RtInt GetWidth();
		RtInt GetHeight();
		RtVoid SetImageVal(RtInt x,RtInt y,RtFloat* vals);
		JRiImage(RtInt w,RtInt h);
		~JRiImage();
};

//A global array for holding images
//I just set there to be 16,383 of them cuz i felt like it
//I dont really have any specific protections to make this work, i'll just trust that example images work correctly
extern std::vector<JRiImage*> images;

#endif
