

#include <stdio.h>
#include "src/core/frixia.h"

int main(int argc, char *argv[])
{	
	int val = frixia_start();
	printf("%d\n",val);
	val = frixia_start_http_listening();
	printf("%d\n",val);
	val = frixia_stop();
	printf("%d\n",val);
}