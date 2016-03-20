#pragma once

#include <tuple>

namespace etu {
namespace http {

struct Request;

/// Parser for incoming requests.
class RequestParser
{
public:
  /// Construct ready to parse the request method.
  RequestParser();

  /// Reset to initial parser State.
  void reset();

  /// Result of parse.
  enum Result { GOOD, BAD, INDETERMINATE };

  /// Parse some data. The enum return value is good when a complete request has
  /// been parsed, bad if the data is invalid, indeterminate when more data is
  /// required. The InputIterator return value indicates how much of the input
  /// has been consumed.
  template <typename InputIterator>
  std::tuple<Result, InputIterator> parse(Request& req,
      InputIterator begin, InputIterator end)
  {
    while (begin != end)
    {
      Result result = consume(req, *begin++);
      if (result == GOOD || result == BAD)
        return std::make_tuple(result, begin);
    }
    return std::make_tuple(INDETERMINATE, begin);
  }

private:
  /// handle the next character of input.
  Result consume(Request& req, char input);

  /// Check if a byte is an HTTP character.
  static bool isChar(int c);

  /// Check if a byte is an HTTP control character.
  static bool isCtl(int c);

  /// Check if a byte is defined as an HTTP tspecial character.
  static bool isTspecial(int c);

  /// Check if a byte is a digit.
  static bool isDigit(int c);

  /// The current State of the parser.
  enum class State
  {
    METHOD_START,
    METHOD,
    URI,
    HTTP_VERSION_H,
    HTTP_VERSION_T1,
    HTTP_VERSION_T2,
    HTTP_VERSION_P,
    HTTP_VERSION_SLASH,
    HTTP_VERSION_MAJOR_START,
    HTTP_VERSION_MAJOR,
    HTTP_VERSION_MINOR_START,
    HTTP_VERSION_MINOR,
    EXPECTING_NEW_LINE1,
    HEADER_LINE_START,
    HEADER_LWS,
    HEADER_NAME,
    SPACE_BEFORE_HEADER_VALUE,
    HEADER_VALUE,
    EXPECTING_NEW_LINE2,
    EXPECTING_NEW_LINE3
  };
};

}
}

