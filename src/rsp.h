/* Lexing is shit but this is my first time after all.
 * There is no trimming and stuff like that +
 * There is no way of checking errors in code; write proper rsp files */

/* TODO: Change line-per to multi line */
/* TODO: Shared headers:
 * [hi, hello] */

#ifndef RSP_H_
#define RSP_H_

enum TOKEN
{
  TOKEN_POUND = '#',
  TOKEN_LBRACK = '[',
  TOKEN_RBRACK = ']',
  TOKEN_QUOTE = '\"'
};

struct rsp_data
{
  int headerCount;
  char** headers;
  int* headerStartIndexes;
  int* headerReplyCount;
};

struct rsp
{
  char userName[32];
  rsp_data data;
};

rsp CreateRspFromFile(const char* username);
char* GetReply(rsp in, const char* header);
void FreeRsp(rsp in);

#endif // RSP_H_
