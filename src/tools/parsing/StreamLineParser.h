#pragma once

#include <cwctype>
#include <functional>
#include <istream>
#include <optional>
#include <sstream>
#include <string>

namespace sp::tools::parsing
{
  template <typename OutputData>
  class StreamLineParser
  {
  public:
    using LineParser = std::function<std::optional<OutputData> (std::istringstream&)>;

    StreamLineParser(std::istream& stream_, LineParser parser_);

    std::optional<OutputData> parseNextLine();

  private:
    inline bool isValidLine(const std::string& line_) const
    {
      const auto it = std::find_if(_lineBuffer.begin(), _lineBuffer.end(), [](char ch_) { return !std::iswspace(ch_); });
      return it != _lineBuffer.end() && *it != '#';
    }

    std::istream& _stream;
    LineParser _parser;
    std::string _lineBuffer;
  };

  template <typename OutputData>
  StreamLineParser<OutputData>::StreamLineParser(std::istream& stream_, LineParser parser_)
    : _stream(stream_)
    , _parser(std::move(parser_))
  {
    _lineBuffer.reserve(1024);
  }

  template <typename OutputData>
  std::optional<OutputData> StreamLineParser<OutputData>::parseNextLine()
  {
    while (!_stream.eof())
    {
      _lineBuffer.clear();
      std::getline(_stream, _lineBuffer);
      if (!_stream.eof() || !_lineBuffer.empty())
      {
        if (isValidLine(_lineBuffer))
        {
          std::istringstream isstream{_lineBuffer};
          auto record = _parser(isstream);
          if (record)
          {
            return record;
          }
        }
      }
    }
    return std::nullopt;
  }
}
