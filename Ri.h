//John Thomason
//ID 3058344
//Parts taken directly from RenderMan Interface for ANSI Standard C

#ifndef _RI_H_
#define _RI_H_
#include <cstddef>
typedef short RtBoolean;
typedef int RtInt;
typedef float RtFloat;
typedef RtFloat RtPoint[3];
typedef RtFloat RtColor[3];
typedef RtFloat RtPoint[3];
typedef RtFloat RtVector[3];
typedef RtFloat RtNormal[3];
typedef RtFloat RtHpoint[4];
typedef RtFloat RtMatrix[4][4];
typedef RtFloat RtBasis[4][4];
typedef RtFloat RtBound[6];
typedef char *RtString;
typedef void *RtPointer;
typedef char *RtToken;
#define RtVoid void
#define RI_NULL NULL

extern RtToken RI_PERSPECTIVE;
extern RtToken RI_ORTHOGRAPHIC;
extern RtToken RI_FOV;

//I decided I would hold the various globally accessible things inside of of context object
class RiContext {
	public:
	RtMatrix CurrentTransform;
	RtMatrix ViewTransform;
	RtMatrix ScreenTransform;
	RtInt** FrameBuffer;
	RtInt XResolution;
	RtInt YResolution;
	RtFloat PixelAspectRatio;
	RtFloat Near;
	RtFloat Far;
};

//Global Variables
extern RiContext *RiCurrentContext;

//Graphics States
RtVoid RiBegin(RtToken name);
RtVoid RiEnd();
RtVoid RiFormat(RtInt xresolution,RtInt yresolution,RtFloat pixelaspectration);
RtVoid RiTransformBegin();
RtVoid RiTransformEnd();
RtVoid RiClipping(RtFloat near,RtFloat far);
RtVoid RiProjection(RtToken name,RtToken paramname = RI_FOV,RtFloat *fov = RI_NULL);
RtVoid RiWorldBegin();
RtVoid RiWorldEnd();

//Transforms
RtVoid RiIdentity();
RtVoid RiTranslate(RtFloat dx,RtFloat dy,RtFloat dz);
RtVoid RiTransform(RtMatrix mat);
RtVoid RiConcatTransform(RtMatrix trans);
RtVoid RiScale(RtFloat dx,RtFloat dy,RtFloat dz);
RtVoid RiRotate(RtFloat angle,RtFloat dx,RtFloat dy,RtFloat dz);
RtVoid RiPerspective(RtFloat fov);

//Internal Stuff
RtVoid RiClearBuffer();

//Temp stuff
RtVoid JohnPrint();
#endif
