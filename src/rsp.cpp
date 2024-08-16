#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "rsp.h"

#define REPLY_MAXLEN 512

static void LTrimWhitespace(char* str)
{
  if (!str)
    return;

  char* it = str;
  while (*it == ' ')
    ++it;

  memmove(str, it, strlen(it)+1);
}

static void RTrimWhitespace(char* str)
{
  if (!str)
    return;

  int count = 0;
  int str_length = strlen(str);
  char* it = str + str_length - 1;
  while(*it == ' ')
  {
    count++;
    --it;
  }

  str[str_length-count] = 0;
}

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
    if (c == ' ')
      continue;

    if (c == TOKEN_POUND) /* Comment */
    {
      char _trash[512];
      fgets(_trash, 512, fp);
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
      LTrimWhitespace(header);
      if (header[0] == 0)
        ret.data.headers[ret.data.headerCount] = strdup("_DEFAULT_");
      else
        ret.data.headers[ret.data.headerCount] = strdup(header);

      ret.data.headerStartIndexes[ret.data.headerCount] = ftell(fp);
      ret.data.headerReplyCount[ret.data.headerCount] = 0;
      ++ret.data.headerCount;
    }

    /* TODO: there must be a better way: */
    /* ++ header reply count */
    if (c == TOKEN_QUOTE)
    {
      /* -1 because the block before this modifies the headerCount */
      ++ret.data.headerReplyCount[ret.data.headerCount-1];
      /* clear line */
      char _trash[REPLY_MAXLEN+3];
      fgets(_trash, REPLY_MAXLEN+3, fp);
    }
  }

  fclose(fp);
  return ret;
}

char* GetReply(rsp in, const char* header)
{
  char ret[REPLY_MAXLEN] = {};

  /* Add trailing .rsp */
  char fileName[128] = {};
  strcpy(fileName, in.userName);
  strcpy(&fileName[strlen(in.userName)], ".rsp");

  /* Find the header index */
  int headi = 0;
  int found = 1;
  for (headi = 0; headi < in.data.headerCount; ++headi)
  {
    /* found one */
    if ((found = strcmp(in.data.headers[headi], header)) == 0)
    {
      break;
    }
  }

  /* No such header exists */
  if (!found)
  {
    fprintf(stderr, "No such header exists in %s's entries: %s\n", in.userName, header);    
  }

  FILE* fp = fopen(fileName, "r");
  fseek(fp, in.data.headerStartIndexes[headi], SEEK_SET);

  srand(time(0));
  unsigned int callIndex = rand() % in.data.headerReplyCount[headi];
  char buffer[512] = {};
  while (fgets(buffer, 512, fp) != NULL)
  {
    LTrimWhitespace(buffer);
    RTrimWhitespace(buffer);

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
