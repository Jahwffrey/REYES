//John Thomason
//ID 3058344

#include "JRi.h"
#include "Ri.h"
#include <math.h>
#include <algorithm>
#include <iostream>

JRiPoint::JRiPoint(RtFloat x,RtFloat y,RtFloat z,RtFloat w){
	pt[0] = x;
	pt[1] = y;
	pt[2] = z;
	pt[3] = w;
}

RtFloat JRiPoint::x(){
	return pt[0];
}

RtFloat JRiPoint::y(){
	return pt[1];
}

RtFloat JRiPoint::z(){
	return pt[2];
}

RtFloat JRiPoint::w(){
	return pt[3];
}

RtFloat JRiPoint::r(){
	return pt[0];
}

RtFloat JRiPoint::g(){
	return pt[1];
}

RtFloat JRiPoint::b(){
	return pt[2];
}

RtFloat JRiPoint::a(){
	return pt[3];
}

RtVoid JRiPoint::MoveToScreen(){
	Mult(&(RiCurrentContext -> CurrentTransform));
	Mult(&(RiCurrentContext -> ViewTransform));
	Mult(&(RiCurrentContext -> ScreenTransform));

	for(int i = 0;i < 3;i++){
		pt[i] = pt[i]/(pt[3] + 0.000001);
	}
	return;
}

RtVoid JRiPoint::Mult(RtMatrix* mat){
	RtHpoint npt = {0,0,0,0};
	for(int j = 0;j < 4;j++){
		for(int i = 0;i < 4;i++){
			npt[j] += (*mat)[i][j] * pt[i];
		}
	}
	
	for(int i = 0;i < 4;i++){
		pt[i] = npt[i];
	}
}

RtVoid JRiPoint::DumpToScreen(RtFloat r,RtFloat g,RtFloat b){
	if(pt[3] < -(RiCurrentContext -> Near) && pt[0] >= 0 && pt[1] >= 0 && (RtInt)pt[0] < RiCurrentContext -> XResolution && (RtInt)pt[1] < RiCurrentContext -> YResolution){
		//RiCurrentContext -> FrameBuffer[(RtInt)pt[0]][(RtInt)pt[1]].r = 255;
		WriteFrameBuffer(pt[0],pt[1],r*255,g*255,b*255,0,pt[3]);
	}
}

RtVoid JRiPoint::Set(RtFloat a,RtFloat b,RtFloat c,RtFloat d){
	pt[0] = a;
	pt[1] = b;
	pt[2] = c;
	pt[3] = d;
	return;
}
RtVoid JRiPoint::Normalize(){
	RtFloat len = sqrt(pow(pt[0],2) + pow(pt[1],2) + pow(pt[2],2));
	pt[0] = pt[0]/len;
	pt[1] = pt[1]/len;
	pt[2] = pt[2]/len;
	return;
}

//Vertex
JRiVertex::JRiVertex(RtFloat x,RtFloat y,RtFloat z,RtFloat nx,RtFloat ny,RtFloat nz,RtFloat r,RtFloat g,RtFloat b,RtFloat a,RtFloat tx,RtFloat ty,RtFloat du,RtFloat dv){
	pos = new JRiPoint(x,y,z,1);	
	norm = new JRiPoint(nx,ny,nz,1);
	norm->Normalize();	
	col = new JRiPoint(r,g,b,a);	
	texpos = new JRiPoint(tx,ty,du,dv);	
}

JRiVertex::~JRiVertex(){
	delete(pos);
	delete(norm);
	delete(col);
	delete(texpos);
}

RtVoid JRiVertex::Set(RtFloat x,RtFloat y,RtFloat z,RtFloat nx,RtFloat ny,RtFloat nz,RtFloat r,RtFloat g,RtFloat b,RtFloat a,RtFloat tx,RtFloat ty,RtFloat du,RtFloat dv){
	pos->Set(x,y,z,1);	
	norm->Set(nx,ny,nz,1);
	norm->Normalize();	
	col->Set(r,g,b,a);	
	texpos->Set(tx,ty,du,dv);	
	return;
}

RtVoid JRiVertex::MoveToScreen(){
	pos->MoveToScreen();
	return;
}

RtVoid JRiVertex::Draw(){
	//pos->DumpToScreen(col->r(),col->g(),col->b());
	
	return;
}
JRiPoint* JRiVertex::GetPos(){
	return pos;
}
JRiPoint* JRiVertex::GetNorm(){
	return norm;
}
JRiPoint* JRiVertex::GetCol(){
	return col;
}
JRiPoint* JRiVertex::GetTexPos(){
	return texpos;
}

RtVoid JRiVertex::CopyNorm(JRiPoint* onorm){
	norm->Set(onorm->x(),onorm->y(),onorm->z(),onorm->w());
	return;
}

//Mesh
JRiMesh::JRiMesh(RtInt w,RtInt h){
	width = w;//w;
	height = h;//h;
	mesh = new JRiVertex**[width];
	for(int i = 0;i < width;i++){
		mesh[i] = new JRiVertex*[height];
		for(int j = 0;j < height;j++){
			mesh[i][j] = new JRiVertex(0,0,0,0,0,0,0,0,0,0,0,0,0,0);
		}
	}
}

JRiMesh::~JRiMesh(){
	for(int i = 0;i < width;i++){
		for(int j = 0;j < height;j++){
			delete(mesh[i][j]);
		}
		delete(mesh[i]);
	}
	delete(mesh);
}

RtVoid JRiMesh::Set(RtInt mx,RtInt my,RtFloat x,RtFloat y,RtFloat z,RtFloat nx,RtFloat ny,RtFloat nz,RtFloat r,RtFloat g,RtFloat b,RtFloat a,RtFloat tx,RtFloat ty,RtFloat du,RtFloat dv){
	mesh[mx][my] -> Set(x,y,z,nx,ny,nz,r,g,b,a,tx,ty,du,dv);
	return;
}

bool JRiMesh::SampleInsideMicrotriangle(JRiPixel* px,JRiPoint* a,JRiPoint* b,JRiPoint* c){
	//Determine if a point is in the triangle via barycentric coords
	//2d Vectors
	RtFloat vx_x = b->x() - a->x();
	RtFloat vx_y = b->y() - a->y();
	RtFloat vy_x = c->x() - a->x();
	RtFloat vy_y = c->y() - a->y();
	RtFloat vp_x = px->u - a->x();
	RtFloat vp_y = px->v - a->y();

	//Dot Products
	RtFloat dotxx = vx_x * vx_x + vx_y * vx_y;	
	RtFloat dotxy = vx_x * vy_x + vx_y * vy_y;	
	RtFloat dotxp = vx_x * vp_x + vx_y * vp_y;	
	RtFloat dotyy = vy_x * vy_x + vy_y * vy_y;	
	RtFloat dotyp = vy_x * vp_x + vy_y * vp_y;

	//Compute new coords
	RtFloat divisor = dotxx * dotyy - dotxy*dotxy;
	RtFloat nu = (dotyy*dotxp - dotxy*dotyp)/divisor;
	RtFloat nv = (dotxx*dotyp - dotxy*dotxp)/divisor;
		
	//Do the check
	return ((nu >= 0) && (nv >= 0) && (nu + nv < 1));
}

RtVoid JRiMesh::DrawMicropolygon(JRiVertex* ul,JRiVertex* ur,JRiVertex* ll,JRiVertex* lr){
	RtFloat uarr[4] = {ul->GetPos()->x(),ur->GetPos()->x(),ll->GetPos()->x(),lr->GetPos()->x()};
	RtFloat varr[4] = {ul->GetPos()->y(),ur->GetPos()->y(),ll->GetPos()->y(),lr->GetPos()->y()};

	//Bound	
	RtFloat minu = uarr[0];
	RtFloat maxu = uarr[0];	
	RtFloat minv = varr[0];
	RtFloat maxv = varr[0];
	for(int i = 1;i < 4;i++){
		if(uarr[i] < minu) minu = uarr[i];
		if(uarr[i] > maxu) maxu = uarr[i];
		if(varr[i] < minv) minv = varr[i];
		if(varr[i] > maxv) maxv = varr[i];
	}

	//Loop over relevant screen pixels
	for(int j = std::max(0,(int)minv);j <= std::min(RiCurrentContext -> YResolution - 1,(int)maxv);j++){
		for(RtInt i = std::max(0,(RtInt)minu);i <= std::min(RiCurrentContext -> XResolution - 1,(RtInt)maxu);i++){
			//Loop over samples
			for(RtInt l = 0;l < RiCurrentContext -> YSamples;l++){
				for(RtInt k = 0;k < RiCurrentContext -> XSamples;k++){
					//Check if sample is in quad
					JRiPixel* px = RiCurrentContext -> FrameBuffer[i][j][k][l];
					if(SampleInsideMicrotriangle(px,ul->GetPos(),ur->GetPos(),ll->GetPos()) || SampleInsideMicrotriangle(px,ur->GetPos(),ll->GetPos(),lr->GetPos())){
						//Color the sample
						if(px->z > (ul->GetPos()->w())){
							px->r = ul->GetCol()->r();
							px->g = ul->GetCol()->g();
							px->b = ul->GetCol()->b();
							px->a = ul->GetCol()->a();
							px->z = ul->GetPos()->w();
						}
					}

				}
			}
		}
	}

	return;
}

/*
extern RtColor _Cs;//color of a point
extern RtColor _Os;//opacity of a point
extern RtPoint _P;//3d position of a point
extern RtVector _dPdu;//Derivative of position of a point along u
extern RtVector _dPdv;//Derivative of position of a point along v
extern RtNormal _N;//The normal of point
extern RtFloat _U;//texture coord x
extern RtFloat _V;//texture coord v
extern RtFloat _dU;//derivative of surface params
extern RtFloat _dV;//derivative of surface params
*/

RtVoid JRiMesh::CalcVertexValsForShader(RtInt x,RtInt y){
	//calculate normals and everything
	//normals at every point ill be normal of triangle of it and point to right and down
	//normals of right and bottom edge points will be the same as opposite edge
	if(x == width - 1){
		if(y == height - 1){
			mesh[x][y]->CopyNorm(mesh[0][0]->GetNorm());
		} else {
			mesh[x][y]->CopyNorm(mesh[0][y]->GetNorm());
		}
	} else if(y == height - 1){
		mesh[x][y]->CopyNorm(mesh[x][0]->GetNorm());
	} else {
		JRiPoint *p1 = mesh[x][y]->GetPos();
		JRiPoint *p2 = mesh[x+1][y]->GetPos();
		JRiPoint *p3 = mesh[x][y+1]->GetPos();
		
		RtFloat a[3] = {p1->x() - p2->x(),p1->y() - p2->y(),p1->z() - p2->z()};	
		RtFloat b[3] = {p1->x() - p3->x(),p1->y() - p3->y(),p1->z() - p3->z()};
		
		mesh[x][y]->GetNorm()->Set(a[1]*b[2] - a[2]*b[1],a[2]*b[0] - a[0]*b[2],a[0]*b[1] - a[1]*b[0],1);
		mesh[x][y]->GetNorm()->Normalize();	
	}
}

RtVoid JRiMesh::SetShaderVals(RtInt x,RtInt y){
	_Cs[0] = mesh[x][y]->GetCol()->r();
	_Cs[1] = mesh[x][y]->GetCol()->g();
	_Cs[2] = mesh[x][y]->GetCol()->b();
	_Os[0] = mesh[x][y]->GetCol()->a();
	_Os[1] = mesh[x][y]->GetCol()->a();
	_Os[2] = mesh[x][y]->GetCol()->a();
	_P[0] = mesh[x][y]->GetPos()->x();
	_P[1] = mesh[x][y]->GetPos()->y();
	_P[2] = mesh[x][y]->GetPos()->z();
	_N[0] = mesh[x][y]->GetNorm()->x();
	_N[1] = mesh[x][y]->GetNorm()->y();
	_N[2] = mesh[x][y]->GetNorm()->z();
	_U = mesh[x][y]->GetTexPos()->x();	
	_V = mesh[x][y]->GetTexPos()->y();	
	_dU = mesh[x][y]->GetTexPos()->z();	
	_dV = mesh[x][y]->GetTexPos()->w();	


	_dPdu[0] = 0;
	_dPdu[1] = 0;
	_dPdu[2] = 0;
	_dPdv[0] = 0;
	_dPdv[1] = 0;
	_dPdv[2] = 0;

	return;
}

RtVoid JRiMesh::SetVertexFromShaderVals(RtInt x,RtInt y){
	//Set(RtInt mx,RtInt my,
	//RtFloat x,RtFloat y,RtFloat z,
	//RtFloat nx,RtFloat ny,RtFloat nz,
	//RtFloat r,RtFloat g,RtFloat b,
	//RtFloat a,
	//RtFloat tx,RtFloat ty,RtFloat du,RtFloat dv)
	Set(	x,y,
		_P[0],_P[1],_P[2],
		_N[0],_N[1],_N[2],
		_Cs[0],_Cs[1],_Cs[2],
		_Os[0],
		_U,_V,_dU,_dV);
	
	return;
}

RtVoid JRiMesh::Draw(){
	//Run displacement shader
	for(int j = 0;j < height;j++){
		for(int i = 0;i < width;i++){
			CalcVertexValsForShader(i,j);
			SetShaderVals(i,j);
			RunDisplacementShader();
			SetVertexFromShaderVals(i,j);
		}
	}
	//Run surface shader
	for(int j = 0;j < height;j++){
		for(int i = 0;i < width;i++){
			CalcVertexValsForShader(i,j);
			SetShaderVals(i,j);
			RunSurfaceShader();
			SetVertexFromShaderVals(i,j);
		}
	}
	//Project all vertices to screen space
	for(int j = 0;j < height;j++){
		for(int i = 0;i < width;i++){
			mesh[i][j] -> MoveToScreen();
		}
	}
	//Draw each micropolygon
	for(int j = 0;j < height - 1;j++){
		for(int i = 0;i < width - 1;i++){
			//DrawMicropolygon(mesh[i][j],mesh[(i + 1)%width][j],mesh[i][(j + 1)%height],mesh[(i + 1)%width][(j + 1)%height]);
			DrawMicropolygon(mesh[i][j],mesh[(i + 1)][j],mesh[i][(j + 1)],mesh[(i + 1)][(j + 1)]);
		}
	}
	return;
}


RtInt JRiMesh::GetWidth(){
	return width;
}

RtInt JRiMesh::GetHeight(){
	return height;
}
