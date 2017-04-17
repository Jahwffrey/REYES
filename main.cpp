//John Thomason
//ID 3058344

#include "Ri.h"
#include <iostream>

int main(){
	RiBegin(RI_NULL);
		RiFormat(20,10,1);
		//JohnPrint();
		RiIdentity();
		//RtMatrix ident = {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
		//RtMatrix tmpmat = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
		//RtMatrix tmpmat2 = {1,2,3,1,2,3,1,2,3,1,2,3,1,2,3,1};
		//RiConcatTransform(tmpmat2);	
		//RiConcatTransform(tmpmat);	
		//RiConcatTransform(ident);
		//std::cout << "\n";
		
		/*JohnPrintMat();
		std::cout << "\n";
		RiTranslate(3,4,5);
		JohnPrintMat();
		std::cout << "\n";
		RiTranslate(2,0,0);
		JohnPrintMat();*/
		
		/*RtHpoint p1 = {5.0,5.0,2.0,1.0};
		RtHpoint p2 = {5.0,5.0,2.0,1.0};
		RtHpoint p3 = {5.0,5.0,2.0,1.0};
		JohnPoint(p1);
		JohnPrint();
		std::cout << "\n";
		JohnPrintMat();
		std::cout << "\n";
		RiTranslate(2,0,0);
		JohnPoint(p2);
		JohnPrint();
		std::cout << "\n";
		JohnPrintMat();
		std::cout << "\n";
		RiTranslate(-3,-3,2);
		JohnPoint(p3);
		JohnPrint();
		std::cout << "\n";
		JohnPrintMat();
		std::cout << "\n";*/
	
		/*for(RtFloat i = 0.0;i < 6.15;i+=0.01){
			RtHpoint p1 = {5,5,2,1};
			RiRotate(0.1,0,0,1);
			JohnPoint(p1);
		}
		JohnPrint();*/

	RiEnd();
}
