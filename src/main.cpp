/* Win32 later */
/* Lexing is shit but this is my first time after all.
 * There is no trimming and stuff like that +
 * There is no way of checking errors in code; write proper rsp files */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum TOKEN
{
  TOKEN_LBRACK = '[',
  TOKEN_RBRACK = ']',
  TOKEN_QUOTE = '\"',
};

struct rsp_data
{
  int headerCount;
  char** headers;

  int* responseCounts;
  int** responseIndexes;
};

struct rsp
{
  char* userName;
  rsp_data data;
};

char** GetResponsesFromHeader(rsp_data in, const char* header)
{
  return NULL;
}

/* TODO: Shared headers:
 * [sa, selaminaleykum] */
rsp CreateRspFromFile(const char* username)
{
  rsp ret = {};
  ret.data.headers = (char**)malloc(sizeof(char**));

  /* Add trailing .rsp */
  char fileName[128] = {};
  strcpy(fileName, username);
  strcpy(&fileName[strlen(username)], ".rsp");

  FILE* fp = fopen(fileName, "r");
  if (!fp)
  {
    fprintf(stderr, "There is no such file: %s\n", fileName);
    return {};
  }

  /* Get headers */
  char c;
  while((c = fgetc(fp)) != EOF)
  {
    if (c == '#') /* Comment */
    {
      char _trash[256];
      fgets(_trash, 256, fp);
    }

    if (c == TOKEN_LBRACK)
    {
      char header[64] = {};
      int i = 0;
      /* TODO: Failsafe this; if someone forgets to put a ], this is a deadlock */
      while ((c = fgetc(fp)) != TOKEN_RBRACK)
      {
        header[i] = c;
        ++i;
      }
      /* TODO: special headers should be documented */
      /* if empty == _DEFAULT_ */
      if (header[0] == 0)
        ret.data.headers[ret.data.headerCount] = strdup("_DEFAULT_");
      else
        ret.data.headers[ret.data.headerCount] = strdup(header);

      char _trash [32];
      fgets(_trash, 32, fp);

      ++ret.data.headerCount;
    }
  }
  fseek(fp, 0, SEEK_SET);

  /* Indexing algorithm */

  fclose(fp);
  return ret;
}

void FreeRsp(rsp in)
{
  for (int i = 0; i < in.data.headerCount; i++)
  { free(in.data.headers[i]); }
  free(in.data.headers);
}

/* TODO: ADD TRIM FUNCTIONS */
/* TODO: Parse/Debug the file better */

int main(void)
{
  rsp hexa = CreateRspFromFile("../rsp/hexa3333");
  for (int i = 0; i < hexa.data.headerCount; i++)
  {
    printf("%s\n", hexa.data.headers[i]);
  }

  FreeRsp(hexa);
  return 0;
}
