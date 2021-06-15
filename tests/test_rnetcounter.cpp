#include <stdio.h>
#include "RNetCounter.hpp"



int main(int argc, char** argv) {

	rnet::RNetCounter* counter1;
	rnet::RNetCounter* counter2;

	printf("Increment counter instance 1...\n");
	counter1->Instance().Increment();
	printf("Current counter (from instance 1): %u\n", counter1->Instance().Get());
	
	printf("Increment counter instance 2...\n");
	counter2->Instance().Increment();
	printf("Current counter (from instance 2): %u\n", counter2->Instance().Get());
	

	printf("Increment counter instance 1...\n");
	counter1->Instance().Increment();
	printf("Current counter (from instance 1): %u\n", counter1->Instance().Get());
	printf("Current counter (from instance 2): %u\n", counter2->Instance().Get());

	return 0;
}
