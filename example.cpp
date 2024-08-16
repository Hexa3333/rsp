#include "src/rsp.h"
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
  rsp hexa = CreateRspFromFile("rsp/foo");
  char* rep = GetReply(hexa, "hello");

  // Hello, Hello to you too, Hi
  printf("%s\n", rep);
  
  free(rep);
  FreeRsp(hexa);
}
