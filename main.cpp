//John Thomason
//ID 3058344

#include "Ri.h"
#include <iostream>

int main(){
	RiBegin(RI_NULL);
		RiFormat(20,10,1);
		JohnPrint();
		RiIdentity();
		RtMatrix ident = {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
		RtMatrix tmpmat = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
		RtMatrix tmpmat2 = {1,2,3,1,2,3,1,2,3,1,2,3,1,2,3,1};
		//RiConcatTransform(tmpmat2);	
		RiConcatTransform(tmpmat);	
		//RiConcatTransform(ident);	
		std::cout << "\n";
		JohnPrintMat();
	RiEnd();
}
