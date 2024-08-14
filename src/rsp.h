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
char** GetResponsesFromHeader(rsp_data in, const char* header);
char* GetReply(rsp in, const char* header);
void FreeRsp(rsp in);

#endif // RSP_H_
