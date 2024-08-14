#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "rsp.h"

rsp CreateRspFromFile(const char* username)
{
  rsp ret = {};
  strcpy(ret.userName, username);
  ret.data.headers = (char**)malloc(sizeof(char**));
  ret.data.headerStartIndexes = (int*)malloc(sizeof(int*));
  ret.data.headerReplyCount = (int*)malloc(sizeof(int*));

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
    if (c == TOKEN_POUND) /* Comment */
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
      /* if empty == _DEFAULT_ */
      if (header[0] == 0)
        ret.data.headers[ret.data.headerCount] = strdup("_DEFAULT_");
      else
        ret.data.headers[ret.data.headerCount] = strdup(header);


      ret.data.headerStartIndexes[ret.data.headerCount] = ftell(fp);
      ret.data.headerReplyCount[ret.data.headerCount] = 0;
      ++ret.data.headerCount;
    }

    /* ++ header reply count */
    if (c == TOKEN_QUOTE)
    {
      /* TODO: there must be a better way: */
      /* -1 because the block before this modifies the headerCount */
      ++ret.data.headerReplyCount[ret.data.headerCount-1];
      /* clear line */
      char _trash[512];
      fgets(_trash, 512, fp);
    }
  }

  fclose(fp);
  return ret;
}

char** GetResponsesFromHeader(rsp_data in, const char* header)
{
  return NULL;
}

char* GetReply(rsp in, const char* header)
{
  char ret[512] = {};

  /* Add trailing .rsp */
  char fileName[128] = {};
  strcpy(fileName, in.userName);
  strcpy(&fileName[strlen(in.userName)], ".rsp");

  /* Find the header index */
  int headi = 0;
  for (headi = 0; headi < in.data.headerCount; ++headi)
  {
    /* found one */
    if (strcmp(in.data.headers[headi], header) == 0)
    {
      break;
    }
  }

  /* TODO: Maybe a better random gen ? - Might not be necessary though */

  FILE* fp = fopen(fileName, "r");
  fseek(fp, in.data.headerStartIndexes[headi], SEEK_SET);

  srand(time(0));
  unsigned int callIndex = rand() % in.data.headerReplyCount[headi];
  char buffer[512] = {};
  while (fgets(buffer, 512, fp) != NULL)
  {
    if (buffer[0] == TOKEN_POUND ||
        buffer[0] == '\n') continue;
    
    /* if the quote has no enclosing counterpart... */
    if (callIndex == 0 &&
        buffer[0] == TOKEN_QUOTE)
    {
      /* trim and save */
      strncpy(ret, buffer+1, strlen(buffer)-3);
    }
    
    --callIndex;
  }

  fclose(fp);
  return strdup(ret);
}

void FreeRsp(rsp in)
{
  for (int i = 0; i < in.data.headerCount; i++)
  { free(in.data.headers[i]); }
  free(in.data.headers);
  free(in.data.headerStartIndexes);
  free(in.data.headerReplyCount);
}
