/* Win32 later */
/* Lexing is shit but this is my first time after all.
 * There is no trimming and stuff like that +
 * There is no way of checking errors in code; write proper rsp files */

/* TODO: Change line-per to multi line */

/* TODO: special headers should be documented
 * _DEFAULT_: default header that is empty ([]) */

/* TODO: ADD TRIM FUNCTIONS */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rsp.h"

enum struct SPECIAL_HEADER
{};


/* TODO: Shared headers:
 * [hi, hello] */



int main(void)
{
  rsp hexa = CreateRspFromFile("../rsp/hexa3333");
  /*
  for (int i = 0; i < hexa.data.headerCount; i++)
  {
    printf("%s\n", hexa.data.headers[i]);
  } */

  char* reply = GetReply(hexa, "allah yok");
  printf("%s\n", reply);
  free(reply);

  FreeRsp(hexa);
  return 0;
}
