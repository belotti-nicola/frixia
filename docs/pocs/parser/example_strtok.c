/* strtok example */





/* USES STATIC VARIABLES TO KEEP TRACK OF ITS INTERNAL STATE */



#include <stdio.h>
#include <string.h>

int main ()
{
  char str[] ="- This, a sample string.";
  char * pch;
  printf ("Splitting string \"%s\" into tokens:\n",str);
  pch = strtok (str," ,.-");
  while (pch != NULL)
  {
    //printf ("1:%s\n",pch);
    pch = strtok (NULL, " ,.-");
    printf ("2:%s\n",pch);
  }
  return 0;
}