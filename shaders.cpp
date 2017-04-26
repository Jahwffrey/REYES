//John Thomason
//ID 3058344
//
#include "shaders.h"
#include "Ri.h"
#include <math.h>
#include <iostream>

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

RtFloat l_pos[3] = {0,-2,5};

void checkerboard(void){
	RtInt val = (((RtInt)(_U * 100) % 10) > 4) xor (((RtInt)(_V * 100) % 10) > 4);
	_Cs[0] = val;
	_Cs[1] = val;
	_Cs[2] = val;
}

void phong_lighting(void){
	RtFloat l_dir[3] = {l_pos[0] - _P[0],l_pos[1] - _P[1],l_pos[2] - _P[2]};
	RtFloat l_dir_mag = sqrt(l_dir[0]*l_dir[0] + l_dir[1]*l_dir[1] + l_dir[2]*l_dir[2]);
	l_dir[0] = l_dir[0]/l_dir_mag;
	l_dir[1] = l_dir[1]/l_dir_mag;
	l_dir[2] = l_dir[1]/l_dir_mag;
	RtFloat cross = _N[0]*l_dir[0] + _N[1]*l_dir[1] * _N[2]*l_dir[2];
	_Cs[0] = _Cs[0] * cross;
	_Cs[1] = _Cs[1] * cross;
	_Cs[2] = _Cs[2] * cross;
}
