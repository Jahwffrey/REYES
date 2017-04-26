//John Thomason
//ID 3058344
//
#include "shaders.h"
#include "Ri.h"

//extern RtColor _Cs;//color of a point
//extern RtColor _Os;//opacity of a point
//extern RtPoint _P;//3d position of a point
//extern RtVector _dPdu;//Derivative of position of a point along u
//extern RtVector _dPdv;//Derivative of position of a point along v
//extern RtNormal _N;//The normal of point
//extern RtFloat _U;//texture coord x
//extern RtFloat _V;//texture coord v
//extern RtFloat _dU;//derivative of surface params
//extern RtFloat _dV;//derivative of surface params

void checkerboard(void){
	RtInt val = (((RtInt)(_U * 100) % 10) > 4) && (((RtInt)(_V * 100) % 10) > 4);
	_Cs[0] = val;
	_Cs[1] = val;
	_Cs[2] = val;
}
