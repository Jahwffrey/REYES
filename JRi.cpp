//John Thomason
//ID 3058344

#include "JRi.h"
#include "Ri.h"
#include <math.h>
#include <algorithm>
#include <iostream>
#include <vector>

std::vector<JRiImage*> images;

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

RtVoid JRiPoint::Transform(){
	Mult(&(RiCurrentContext -> CurrentTransform));//Move in world
	Mult(&(RiCurrentContext -> ViewTransform));//Move to within camera view
	return;
}

RtVoid JRiPoint::MoveToScreen(){
	Mult(&(RiCurrentContext -> ScreenTransform));//Perspective transform and moving on the screen

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

RtVoid JRiPoint::Set(RtFloat a,RtFloat b,RtFloat c,RtFloat d){
	pt[0] = a;
	pt[1] = b;
	pt[2] = c;
	pt[3] = d;
	return;
}
RtVoid JRiPoint::Normalize(){
	RtFloat len = (float)sqrt(pow(pt[0],2) + pow(pt[1],2) + pow(pt[2],2));
	pt[0] = pt[0]/len;
	pt[1] = pt[1]/len;
	pt[2] = pt[2]/len;
	return;
}

//Vertex
JRiVertex::JRiVertex(RtFloat x,RtFloat y,RtFloat z,RtFloat nx,RtFloat ny,RtFloat nz,RtFloat r,RtFloat g,RtFloat b,RtFloat ar,RtFloat ag,RtFloat ab,RtFloat tx,RtFloat ty,RtFloat du,RtFloat dv){
	pos = new JRiPoint(x,y,z,1);	
	norm = new JRiPoint(nx,ny,nz,1);
	norm->Normalize();	
	col = new JRiPoint(r,g,b,1);	
	opa = new JRiPoint(ar,ag,ab,1);	
	texpos = new JRiPoint(tx,ty,du,dv);	
}

JRiVertex::~JRiVertex(){
	delete(pos);
	delete(norm);
	delete(col);
	delete(opa);
	delete(texpos);
}

RtVoid JRiVertex::Set(RtFloat x,RtFloat y,RtFloat z,RtFloat nx,RtFloat ny,RtFloat nz,RtFloat r,RtFloat g,RtFloat b,RtFloat ar,RtFloat ag,RtFloat ab,RtFloat tx,RtFloat ty,RtFloat du,RtFloat dv){
	pos->Set(x,y,z,1);	
	norm->Set(nx,ny,nz,1);
	norm->Normalize();	
	col->Set(r,g,b,1);	
	opa->Set(ar,ag,ab,1);	
	texpos->Set(tx,ty,du,dv);	
	return;
}

RtVoid JRiVertex::Transform(){
	pos->Transform();
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
JRiPoint* JRiVertex::GetOpa(){
	return opa;
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
			mesh[i][j] = new JRiVertex(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);
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

RtVoid JRiMesh::Set(RtInt mx,RtInt my,RtFloat x,RtFloat y,RtFloat z,RtFloat nx,RtFloat ny,RtFloat nz,RtFloat r,RtFloat g,RtFloat b,RtFloat ar,RtFloat ag,RtFloat ab,RtFloat tx,RtFloat ty,RtFloat du,RtFloat dv){
	mesh[mx][my] -> Set(x,y,z,nx,ny,nz,r,g,b,ar,ag,ab,tx,ty,du,dv);
	return;
}

RtVoid JRiMesh::GetBarycentricCoords(JRiPixel* px,JRiPoint* a,JRiPoint* b,JRiPoint* c,RtFloat* uv){
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
	uv[0] = (dotyy*dotxp - dotxy*dotyp)/divisor;
	uv[1] = (dotxx*dotyp - dotxy*dotxp)/divisor;
	return;
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
					RtFloat uv1[2];
					RtFloat uv2[2];
					GetBarycentricCoords(px,ul->GetPos(),ur->GetPos(),ll->GetPos(),uv1);
					GetBarycentricCoords(px,ur->GetPos(),ll->GetPos(),lr->GetPos(),uv2);
					bool inside = false;
					JRiVertex* a;
					JRiVertex* b;
					JRiVertex* c;
					RtFloat uu;
					RtFloat vv;
					RtFloat ww;
					
					if((uv1[0] >= 0) && (uv1[1] >= 0) && (uv1[0] + uv1[1] < 1)){
						inside = true;
						c = ul;
						a = ur;
						b = ll;
						uu = uv1[0];
						vv = uv1[1];
					} else if((uv2[0] >= 0) && (uv2[1] >= 0) && (uv2[0] + uv2[1] < 1)){
						inside = true;
						c = ur;
						a = ll;
						b = lr;
						uu = uv2[0];
						vv = uv2[1];
					}
					ww = 1 - uu - vv;

					if(inside){
					//Color the sample
						//if(px->z > (ul->GetPos()->w())){
							RtFloat sr = a->GetCol()->r() * uu + b->GetCol()->r() * vv + c->GetCol()->r() * ww;
							RtFloat sg = a->GetCol()->g() * uu + b->GetCol()->g() * vv + c->GetCol()->g() * ww;
							RtFloat sb = a->GetCol()->b() * uu + b->GetCol()->b() * vv + c->GetCol()->b() * ww;
							RtFloat sar = a->GetOpa()->r() * uu + b->GetOpa()->r() * vv + c->GetOpa()->r() * ww;
							RtFloat sag = a->GetOpa()->g() * uu + b->GetOpa()->g() * vv + c->GetOpa()->g() * ww;
							RtFloat sab = a->GetOpa()->b() * uu + b->GetOpa()->b() * vv + c->GetOpa()->b() * ww;
							RtFloat sz = a->GetPos()->w() * uu + b->GetPos()->w() * vv + c->GetPos()->w() * ww;
							px->AddSample(sr,sg,sb,sar,sag,sab,sz);
						//}
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
	//IM GANA TRY REVERSING THE DIRECTION! IF ITS AN EDGE ONE ITS INSTEAD DEFINED BY THE ONES UP AND LEFT, NOT RIGHT AND DOWN!!
	//calculate normals and everything
	//normals at every point will be normal of triangle of it and point to right and down
	//normals of right and bottom edge points will be the same as opposite edge
	JRiPoint* p1;
	JRiPoint* p2;
	JRiPoint* p3;
	/*if(x == width - 1){
		if(y == height - 1){
			//mesh[x][y]->CopyNorm(mesh[x-1][y-1]->GetNorm());
			p1 = mesh[0][0]->GetPos();
			p2 = mesh[0][1]->GetPos();
			p3 = mesh[1][0]->GetPos();
		} else {
			p1 = mesh[0][y]->GetPos();
			p2 = mesh[1][y]->GetPos();
			p3 = mesh[0][y+1]->GetPos();
			//mesh[x][y]->CopyNorm(mesh[x-1][y]->GetNorm());
		}
	} else if(y == height - 1){
		p1 = mesh[x][0]->GetPos();
		p2 = mesh[x+1][0]->GetPos();
		p3 = mesh[x][1]->GetPos();
		//mesh[x][y]->CopyNorm(mesh[x][y-1]->GetNorm());
	} else {*/
		p1 = mesh[x][y]->GetPos();
		p2 = mesh[(x+1)%width][y]->GetPos();
		//p2 = mesh[(x+1)][y]->GetPos();
		p3 = mesh[x][(y+1)%height]->GetPos();
		//p3 = mesh[x][(y+1)]->GetPos();
	//}	

	float a[3] = {p1->x() - p2->x(),p1->y() - p2->y(),p1->z() - p2->z()};	
	float b[3] = {p1->x() - p3->x(),p1->y() - p3->y(),p1->z() - p3->z()};

	mesh[x][y]->GetNorm()->Set((a[1]*b[2] - a[2]*b[1]),(a[2]*b[0] - a[0]*b[2]),(a[0]*b[1] - a[1]*b[0]),1);
	mesh[x][y]->GetNorm()->Normalize();

	//if(x == width - 1 || y == height - 1){
	//	mesh[x][y]->GetNorm()->Set(0,0,1,1);
	//}
	
}

RtVoid JRiMesh::SetShaderVals(RtInt x,RtInt y){
	_Cs[0] = mesh[x][y]->GetCol()->r();
	_Cs[1] = mesh[x][y]->GetCol()->g();
	_Cs[2] = mesh[x][y]->GetCol()->b();
	_Os[0] = mesh[x][y]->GetOpa()->r();
	_Os[1] = mesh[x][y]->GetOpa()->g();
	_Os[2] = mesh[x][y]->GetOpa()->b();
	_P[0] = mesh[x][y]->GetPos()->x();
	_P[1] = mesh[x][y]->GetPos()->y();
	_P[2] = mesh[x][y]->GetPos()->z();
	_N[0] = mesh[x][y]->GetNorm()->x();
	_N[1] = mesh[x][y]->GetNorm()->y();
	_N[2] = mesh[x][y]->GetNorm()->z();
	_U = mesh[x][y]->GetTexPos()->x();	
	_V = mesh[x][y]->GetTexPos()->y();	
	
	RtInt right_ind = (x + 1) % width;
	RtInt left_ind = x - 1;
	if(left_ind == -1) left_ind = width - 1;
	RtInt down_ind = (y + 1) % height;
	RtInt up_ind = y - 1;
	if(up_ind == -1) up_ind = height - 1;
	
	//Central Difference
	_dU = mesh[right_ind][y]->GetTexPos()->x() - mesh[left_ind][y]->GetTexPos()->x();	
	_dV = mesh[x][down_ind]->GetTexPos()->y() - mesh[x][up_ind]->GetTexPos()->y();	


	_dPdu[0] = mesh[right_ind][y]->GetPos()->x() - mesh[left_ind][y]->GetPos()->x();
	_dPdu[1] = mesh[right_ind][y]->GetPos()->y() - mesh[left_ind][y]->GetPos()->y();
	_dPdu[2] = mesh[right_ind][y]->GetPos()->z() - mesh[left_ind][y]->GetPos()->z();
	_dPdv[0] = mesh[x][down_ind]->GetPos()->x() - mesh[x][up_ind]->GetPos()->x();
	_dPdv[1] = mesh[x][down_ind]->GetPos()->y() - mesh[x][up_ind]->GetPos()->y();
	_dPdv[2] = mesh[x][down_ind]->GetPos()->z() - mesh[x][up_ind]->GetPos()->z();

	return;
}

RtVoid JRiMesh::SetVertexFromShaderVals(RtInt x,RtInt y){
	//Set(RtInt mx,RtInt my,
	//RtFloat x,RtFloat y,RtFloat z,
	//RtFloat nx,RtFloat ny,RtFloat nz,
	//RtFloat r,RtFloat g,RtFloat b,
	//RtFloat ar,RtFloat ag,RtFloat ab,
	//RtFloat tx,RtFloat ty,RtFloat du,RtFloat dv)
	//std::cout << _Cs[0] << "," << _Cs[1] << "," << _Cs[2] << "\n";
	Set(	x,y,
		_P[0],_P[1],_P[2],
		_N[0],_N[1],_N[2],
		_Cs[0],_Cs[1],_Cs[2],
		_Os[0],_Os[1],_Os[2],
		_U,_V,_dU,_dV);
	
	return;
}

RtVoid JRiMesh::Draw(){

	//Move Points to the correct place in world
	for(int j = 0;j < height;j++){
		for(int i = 0;i < width;i++){
			mesh[i][j]->Transform();
		}
	}

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
			//std::cout << mesh[i][j]->GetPos()->x() <<","<< mesh[i][j]->GetPos()->y()<< "\n";
		}
	}
	//Draw each micropolygon
	for(int j = 1;j < height - 1;j++){
		for(int i = 1;i < width - 1;i++){
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


//Image
/*class JRiImage {
	private: 
		RtFloat*** img;
		RtInt width;
		RtInt height;
	public:
		RtVoid GetUVPoint(RtFloat u,RtFloat v,RtFloat* vals);
		RtInt GetWidth();
		RtInt GetHeight();
		RtVoid SetImageVal(RtInt x,RtInt y,RtFloat* vals);
		JRiImage(int w,int h);
		~JRiImage();
}*/

RtVoid JRiImage::GetUVPoint(RtFloat u,RtFloat v,RtFloat* vals){
	int x = (int)(u*width) % width;
	int y = (int)(v*height) % height;
	if(x < 0) x = 0;
	if(y < 0) y = 0;	

	RtFloat* pt = img[x][y];
	for(int i = 0;i < 6;i++){
		vals[i] = pt[i];
	}	
	return;
}

RtInt JRiImage::GetWidth(){
	return width;
}

RtInt JRiImage::GetHeight(){
	return height;
}

RtVoid JRiImage::SetImageVal(RtInt x,RtInt y,RtFloat *vals){
	for(int i = 0;i < 6;i++){
		img[x][y][i] = vals[i];
	}
}

JRiImage::JRiImage(RtInt w,RtInt h){
	width = w;
	height = h;
	img = new RtFloat**[w];
	for(RtInt i = 0;i < w;i++){
		img[i] = new RtFloat*[h];
		for(RtInt j = 0;j < h;j++){
			img[i][j] = new RtFloat[6];
		}
	}
}

JRiImage::~JRiImage(){
	for(RtInt i = 0;i < width;i++){
		for(RtInt j = 0;j < height;j++){
			delete(img[i][j]);
		}
		delete(img[i]);
	}
	delete(img);
}
