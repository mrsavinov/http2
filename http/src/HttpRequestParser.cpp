#include "HttpRequestParser.hpp"
#include "HttpRequest.hpp"

namespace etu {
namespace http {

RequestParser::RequestParser()
  : state_(State::METHOD_START)
{
}

void RequestParser::reset()
{
  state_ = State::METHOD_START;
}

RequestParser::Result RequestParser::consume(Request& req, char input)
{
  switch (state_)
  {
  case State::METHOD_START:
    if (!isChar(input) || isCtl(input) || isTspecial(input))
    {
      return BAD;
    }
    else
    {
      state_ = State::METHOD;
      req.method.push_back(input);
      return INDETERMINATE;
    }
  case State::METHOD:
    if (input == ' ')
    {
      state_ = State::URI;
      return INDETERMINATE;
    }
    else if (!isChar(input) || isCtl(input) || isTspecial(input))
    {
      return BAD;
    }
    else
    {
      req.method.push_back(input);
      return INDETERMINATE;
    }
  case State::URI:
    if (input == ' ')
    {
      state_ = State::HTTP_VERSION_H;
      return INDETERMINATE;
    }
    else if (isCtl(input))
    {
      return BAD;
    }
    else
    {
      req.uri.push_back(input);
      return INDETERMINATE;
    }
  case State::HTTP_VERSION_H:
    if (input == 'H')
    {
      state_ = State::HTTP_VERSION_T1;
      return INDETERMINATE;
    }
    else
    {
      return BAD;
    }
  case State::HTTP_VERSION_T1:
    if (input == 'T')
    {
      state_ = State::HTTP_VERSION_T2;
      return INDETERMINATE;
    }
    else
    {
      return BAD;
    }
  case State::HTTP_VERSION_T2:
    if (input == 'T')
    {
      state_ = State::HTTP_VERSION_P;
      return INDETERMINATE;
    }
    else
    {
      return BAD;
    }
  case State::HTTP_VERSION_P:
    if (input == 'P')
    {
      state_ = State::HTTP_VERSION_SLASH;
      return INDETERMINATE;
    }
    else
    {
      return BAD;
    }
  case State::HTTP_VERSION_SLASH:
    if (input == '/')
    {
      req.http_version_major = 0;
      req.http_version_minor = 0;
      state_ = State::HTTP_VERSION_MAJOR_START;
      return INDETERMINATE;
    }
    else
    {
      return BAD;
    }
  case State::HTTP_VERSION_MAJOR_START:
    if (isDigit(input))
    {
      req.http_version_major = req.http_version_major * 10 + input - '0';
      state_ = State::HTTP_VERSION_MAJOR;
      return INDETERMINATE;
    }
    else
    {
      return BAD;
    }
  case State::HTTP_VERSION_MAJOR:
    if (input == '.')
    {
      state_ = State::HTTP_VERSION_MINOR_START;
      return INDETERMINATE;
    }
    else if (isDigit(input))
    {
      req.http_version_major = req.http_version_major * 10 + input - '0';
      return INDETERMINATE;
    }
    else
    {
      return BAD;
    }
  case State::HTTP_VERSION_MINOR_START:
    if (isDigit(input))
    {
      req.http_version_minor = req.http_version_minor * 10 + input - '0';
      state_ = State::HTTP_VERSION_MINOR;
      return INDETERMINATE;
    }
    else
    {
      return BAD;
    }
  case State::HTTP_VERSION_MINOR:
    if (input == '\r')
    {
      state_ = State::EXPECTING_NEW_LINE1;
      return INDETERMINATE;
    }
    else if (isDigit(input))
    {
      req.http_version_minor = req.http_version_minor * 10 + input - '0';
      return INDETERMINATE;
    }
    else
    {
      return BAD;
    }
  case State::EXPECTING_NEW_LINE1:
    if (input == '\n')
    {
      state_ = State::HEADER_LINE_START;
      return INDETERMINATE;
    }
    else
    {
      return BAD;
    }
  case State::HEADER_LINE_START:
    if (input == '\r')
    {
      state_ = State::EXPECTING_NEW_LINE3;
      return INDETERMINATE;
    }
    else if (!req.headers.empty() && (input == ' ' || input == '\t'))
    {
      state_ = State::HEADER_LWS;
      return INDETERMINATE;
    }
    else if (!isChar(input) || isCtl(input) || isTspecial(input))
    {
      return BAD;
    }
    else
    {
      req.headers.push_back(Header());
      req.headers.back().name.push_back(input);
      state_ = State::HEADER_NAME;
      return INDETERMINATE;
    }
  case State::HEADER_LWS:
    if (input == '\r')
    {
      state_ = State::EXPECTING_NEW_LINE2;
      return INDETERMINATE;
    }
    else if (input == ' ' || input == '\t')
    {
      return INDETERMINATE;
    }
    else if (isCtl(input))
    {
      return BAD;
    }
    else
    {
      state_ = State::HEADER_VALUE;
      req.headers.back().value.push_back(input);
      return INDETERMINATE;
    }
  case State::HEADER_NAME:
    if (input == ':')
    {
      state_ = State::SPACE_BEFORE_HEADER_VALUE;
      return INDETERMINATE;
    }
    else if (!isChar(input) || isCtl(input) || isTspecial(input))
    {
      return BAD;
    }
    else
    {
      req.headers.back().name.push_back(input);
      return INDETERMINATE;
    }
  case State::SPACE_BEFORE_HEADER_VALUE:
    if (input == ' ')
    {
      state_ = State::HEADER_VALUE;
      return INDETERMINATE;
    }
    else
    {
      return BAD;
    }
  case State::HEADER_VALUE:
    if (input == '\r')
    {
      state_ = State::EXPECTING_NEW_LINE2;
      return INDETERMINATE;
    }
    else if (isCtl(input))
    {
      return BAD;
    }
    else
    {
      req.headers.back().value.push_back(input);
      return INDETERMINATE;
    }
  case State::EXPECTING_NEW_LINE2:
    if (input == '\n')
    {
      state_ = State::HEADER_LINE_START;
      return INDETERMINATE;
    }
    else
    {
      return BAD;
    }
  case State::EXPECTING_NEW_LINE3:
    return (input == '\n') ? GOOD : BAD;
  default:
    return BAD;
  }
}

bool RequestParser::isChar(int c)
{
  return c >= 0 && c <= 127;
}

bool RequestParser::isCtl(int c)
{
  return (c >= 0 && c <= 31) || (c == 127);
}

bool RequestParser::isTspecial(int c)
{
  switch (c)
  {
  case '(': case ')': case '<': case '>': case '@':
  case ',': case ';': case ':': case '\\': case '"':
  case '/': case '[': case ']': case '?': case '=':
  case '{': case '}': case ' ': case '\t':
    return true;
  default:
    return false;
  }
}

bool RequestParser::isDigit(int c)
{
  return c >= '0' && c <= '9';
}

}
}

