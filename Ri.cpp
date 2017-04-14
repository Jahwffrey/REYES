//John Thomason
//ID 3058344

#include "Ri.h"
#include <iostream>

RiContext* RiCurrentContext;

//Graphics States
void RiBegin(RtToken name){
	RiCurrentContext = new RiContext;
	RiIdentity();
	return;
}
void RiEnd(){
	delete(RiCurrentContext);
	return;
}

void RiIdentity(){
	for(int i = 0;i < 4;i++){
		for(int j = 0;j < 4;j++){
			if(i == j){
				RiCurrentContext -> CurrentTransform[i][j] = 1;
			} else {
				RiCurrentContext -> CurrentTransform[i][j] = 0;
			}
		}
	}
	return;
}

void JohnPrint(){
	std::cout << "Print!\n";
	return;
}
