/* Win32 later */
/* Lexing is shit but this is my first time after all.
 * There is no trimming and stuff like that +
 * There is no way of checking errors in code; write proper rsp files */

/* TODO: Change line-per to multi line */

#include <stdio.h>
#include <stdlib.h>
#include "rsp.h"

enum struct SPECIAL_HEADER
{};


/* TODO: Shared headers:
 * [hi, hello] */

int main(void)
{
  rsp hexa = CreateRspFromFile("../rsp/hexa3333");

  char* reply = GetReply(hexa, "allah yok");
  printf("%s\n", reply);
  free(reply);

  FreeRsp(hexa);
  return 0;
}
