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

int CHECK_SIZE_X = 10;
int CHECK_SIZE_Y = 10;

//RtFloat l_pos[3] = {0,-2,100};
//RtFloat l_dir[3] = {0,-2,3};
RtFloat l_dir[3] = {3,0,-2};
RtFloat l_amb = 0;//.2;
//RtFloat l_pos[3] = {0,0,3};
RtFloat v_dir[3] = {0,0,1};

void checkerboard(void){
	RtInt val = (((RtInt)(_U * 100) % CHECK_SIZE_X) > 4) xor (((RtInt)(_V * 100) % CHECK_SIZE_Y) > 4);
	_Cs[0] = val;
	_Cs[1] = val;
	_Cs[2] = val;
}

void phong_lighting(void){
	//RtFloat l_dir[3] = {l_pos[0] - _P[0],l_pos[1] - _P[1],l_pos[2] - _P[2]};
	//RtFloat l_dir[3] = {0,0,1};
	RtFloat l_dir_n[3] = {0,0,0};
	RtFloat l_dir_mag = (float)sqrt(pow(l_dir[0],2) + pow(l_dir[1],2) + pow(l_dir[2],2));
	l_dir_n[0] = l_dir[0]/l_dir_mag;
	l_dir_n[1] = l_dir[1]/l_dir_mag;
	l_dir_n[2] = l_dir[2]/l_dir_mag;
	
	RtFloat hv_dir[3] = {l_dir_n[0] + v_dir[0],l_dir_n[1] + v_dir[1],l_dir_n[2] + v_dir[2]};
	RtFloat hv_dir_mag = sqrt(hv_dir[0]*hv_dir[0] + hv_dir[1]*hv_dir[1] + hv_dir[2]*hv_dir[2]);
	hv_dir[0] = hv_dir[0]/hv_dir_mag;
	hv_dir[1] = hv_dir[1]/hv_dir_mag;
	hv_dir[2] = hv_dir[2]/hv_dir_mag;
	
	RtFloat dot = std::max((float)0,_N[0]*l_dir_n[0] + _N[1]*l_dir_n[1] + _N[2]*l_dir_n[2]);
	RtFloat dot_2 = std::max((float)0,_N[0]*hv_dir[0] + _N[1]*hv_dir[1] + _N[2]*hv_dir[2]);//_N[0]*l_dir_n[0] + _N[1]*l_dir_n[1] + _N[2]*l_dir_n[2];
	_Cs[0] = std::min((float)1,_Cs[0] * dot + l_amb * _Cs[0]);// + dot_2*dot_2*dot_2*dot_2);
	//_Cs[1] = std::min((float)1,_Cs[1] * dot + l_amb * _Cs[1]);// + dot_2*dot_2*dot_2*dot_2);
	//_Cs[2] = std::min((float)1,_Cs[2] * dot + l_amb * _Cs[2]);// + dot_2*dot_2*dot_2*dot_2);
}

void random_shader(void){
	//_Cs[1] = 1;
	//_Cs[2] = 1;
	_Cs[0] = (rand() % 10000)/10000.0;
	_Cs[1] = (rand() % 10000)/10000.0;
	_Cs[2] = (rand() % 10000)/10000.0;
	return;
	//_P[0] += (-2 + rand()%5)/20.0;
	//_P[1] += (-2 + rand()%5)/20.0;
	//_P[2] += (-2 + rand()%5)/20.0;
}

void texture_zero(void){
	texture(0);
}
