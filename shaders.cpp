//John Thomason
//ID 3058344
//
#include "shaders.h"
#include "Ri.h"
#include <math.h>
#include <iostream>
#include <vector>

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
RtFloat l_dir[3] = {3,0,-4};
RtFloat l_amb[3] = {0.1,0.1,0.1};
//RtFloat l_pos[3] = {0,0,3};
RtFloat v_dir[3] = {0,0,-1};

std::vector<RtFloat **> perlin_vect;

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
	
	RtFloat hv_dir[3] = {(l_dir_n[0] + v_dir[0])/2,(l_dir_n[1] + v_dir[1])/2,(l_dir_n[2] + v_dir[2])/2};
	//RtFloat hv_dir_mag = sqrt(hv_dir[0]*hv_dir[0] + hv_dir[1]*hv_dir[1] + hv_dir[2]*hv_dir[2]);
	//hv_dir[0] = hv_dir[0]/hv_dir_mag;
	//hv_dir[1] = hv_dir[1]/hv_dir_mag;
	//hv_dir[2] = hv_dir[2]/hv_dir_mag;
	
	RtFloat dot = std::max((float)0,_N[0]*l_dir_n[0] + _N[1]*l_dir_n[1] + _N[2]*l_dir_n[2]);
	RtFloat dot_2 = std::max((float)0,_N[0]*hv_dir[0] + _N[1]*hv_dir[1] + _N[2]*hv_dir[2]);//_N[0]*l_dir_n[0] + _N[1]*l_dir_n[1] + _N[2]*l_dir_n[2];
	_Cs[0] = std::min((float)1,_Cs[0] * dot + l_amb[0] * _Cs[0] + (float)pow(dot_2,20)*3);
	_Cs[1] = std::min((float)1,_Cs[1] * dot + l_amb[1] * _Cs[1] + (float)pow(dot_2,20)*3);
	_Cs[2] = std::min((float)1,_Cs[2] * dot + l_amb[2] * _Cs[2] + (float)pow(dot_2,20)*3);
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

void shrink(void){
	_P[0] -= _N[0]*0.5;
	_P[1] -= _N[1]*0.5;
	_P[2] -= _N[2]*0.5;
}

//RtFloat BUMP_AMPLITUDE = 1;
//RtFloat BUMP_MIN_FREQ_EXP = 2;
//RtFloat BUMP_MAX_FREQ_EXP = 6;

RtVoid generate_perlin_grid(RtFloat** grid,int freq){
	int nm = (int)pow(2,freq);
	for(int i = 0; i < nm;i++){
		grid[i] = new RtFloat[nm];
		for(int j = 0;j < nm;j++){
			grid[i][j] = rand()%2;
		}
	}
}

RtFloat get_perlin_val(RtFloat u,RtFloat v,int freq){
	while(perlin_vect.size() < freq){
		perlin_vect.push_back(RI_NULL);
	}
	if(perlin_vect[freq - 1] == RI_NULL){
		perlin_vect[freq - 1] = new RtFloat*[(int)pow(2,freq)];
		generate_perlin_grid(perlin_vect[freq - 1],freq);
	}
	RtFloat nm = (float)pow(2,freq);
	RtInt inm = (int)nm;
	RtFloat x = u * nm;
	RtFloat y = v * nm;

	RtInt ix = (RtInt)x;
	RtInt iy = (RtInt)y;

	
	RtFloat ul_grad_x = perlin_vect[freq-1][ix % inm][iy % inm] * -1 + perlin_vect[freq-1][(ix + 1) % inm][iy % inm];
	RtFloat ul_grad_y = perlin_vect[freq-1][ix % inm][iy % inm] * -1 + perlin_vect[freq-1][ix % inm][(iy + 1) % inm];
	
	RtFloat ur_grad_x = perlin_vect[freq-1][(ix + 1) % inm][iy % inm] * -1 + perlin_vect[freq-1][(ix + 2) % inm][iy % inm];
	RtFloat ur_grad_y = perlin_vect[freq-1][(ix + 1) % inm][iy % inm] * -1 + perlin_vect[freq-1][(ix + 1) % inm][(iy + 1) % inm];

	RtFloat lr_grad_x = perlin_vect[freq-1][(ix + 1) % inm][(iy + 1) % inm] * -1 + perlin_vect[freq-1][(ix + 2) % inm][(iy + 1) % inm];
	RtFloat lr_grad_y = perlin_vect[freq-1][(ix + 1) % inm][(iy + 1) % inm] * -1 + perlin_vect[freq-1][(ix + 1) % inm][(iy + 2) % inm];

	RtFloat ll_grad_x = perlin_vect[freq-1][ix % inm][(iy + 1) % inm] * -1 + perlin_vect[freq-1][(ix + 1) % inm][(iy + 1) % inm];
	RtFloat ll_grad_y = perlin_vect[freq-1][ix % inm][(iy + 1) % inm] * -1 + perlin_vect[freq-1][ix % inm][(iy + 2) % inm];

	RtFloat s = (x - (float)(ix + 1))*ur_grad_x + (y - (float)iy)*ur_grad_y;
	RtFloat t = (x - (float)ix)*ul_grad_x + (y - (float)iy)*ul_grad_y; //ul vector dot gradient of upper left
	RtFloat u2 = (x - (float)(ix + 1))*lr_grad_x + (y - (float)(iy+1))*lr_grad_y;	
	RtFloat v2 = (x - (float)(ix))*ll_grad_x + (y - (float)(iy+1))*ll_grad_y;	

	RtFloat Sx = 3*pow((x - (float)(ix)),2) - 2*pow(x - (float)ix,3);
	RtFloat a = (1 - Sx)*t + Sx*s;
	RtFloat b = (1 - Sx)*v2 + Sx*u2;
	RtFloat Sy = 3*pow((y - (float)(iy)),2) - 2*pow(y - (float)iy,3);
	RtFloat out = (1-Sy)*b + Sy*a;

	return out;
}

void BUMPY(void){
	RtFloat scal = 0;
	for(int i = BUMP_MIN_FREQ_EXP;i <= BUMP_MAX_FREQ_EXP;i++){
		scal += get_perlin_val(_U,_V,i)/pow(2,i);
	}
	_P[0] += _N[0] * BUMP_AMPLITUDE * scal;
	_P[1] += _N[1] * BUMP_AMPLITUDE * scal;
	_P[2] += _N[2] * BUMP_AMPLITUDE * scal;
	return;
}
