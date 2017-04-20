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

class JRiPixel {
	public:
	RtFloat r;
	RtFloat g;
	RtFloat b;
	RtFloat a;
	RtFloat z;
};

//I decided I would hold the various globally accessible things inside of of context object
class RiContext {
	public:
	RtMatrix CurrentTransform;
	RtMatrix ViewTransform;
	RtMatrix ScreenTransform;
	JRiPixel** FrameBuffer;
	RtInt XResolution;
	RtInt YResolution;
	RtFloat XSamples;
	RtFloat YSamples;
	RtFloat PixelAspectRatio;
	RtFloat FrameAspectRatio;
	RtFloat Near;
	RtFloat Far;
	RtInt TransBegun;
	RtInt WorldBegun;
	RtInt FrameBegun;
	RtInt CurrentFrame;
};

//RiContext functions
RtVoid WriteFrameBuffer(RtFloat x,RtFloat y,RtFloat r,RtFloat g,RtFloat b,RtFloat a,RtFloat z);	

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
RtVoid RiFrameAspectRatio(RtFloat rat);
RtVoid RiPixelSamples(RtFloat xsamples,RtFloat ysamples);
RtVoid RiFrameBegin(RtInt frame);
RtVoid RiFrameEnd();
RtVoid RiDisplay(RtToken name,RtToken type,RtToken mode,RtToken paramlist = "",RtPointer orig = RI_NULL);


//Transforms
RtVoid RiIdentity();
RtVoid RiTranslate(RtFloat dx,RtFloat dy,RtFloat dz);
RtVoid RiTransform(RtMatrix mat);
RtVoid RiConcatTransform(RtMatrix trans);
RtVoid RiScale(RtFloat dx,RtFloat dy,RtFloat dz);
RtVoid RiRotate(RtFloat angle,RtFloat dx,RtFloat dy,RtFloat dz);
RtVoid RiPerspective(RtFloat fov);

//Primitives
RtVoid RiSphere(RtFloat radius,RtFloat zmin,RtFloat zmax,RtFloat thetamax,RtPointer param = RI_NULL);

//Internal Stuff
RtVoid RiClearBuffer();
#endif
