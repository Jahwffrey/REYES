//John Thomason
//ID 3058344
//Parts taken directly from RenderMan Interface for ANSI Standard C

#ifndef _RI_H_
#define _RI_H_
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

//Global Variables
extern RtMatrix RiCurrentTransform;//current transformation
extern RtInt* RiFrameBuffer;//The screen

//Graphics States
void RiBegin(RtToken name);
void RiEnd();

//Transforms
void RiIdentity();

void JohnPrint();
#endif
