

#include <stdio.h>
#include "src/core/frixia.h"

int main(int argc, char *argv[])
{	
	int val = frixia_start();
	printf("start ended: %d\n",val);
}