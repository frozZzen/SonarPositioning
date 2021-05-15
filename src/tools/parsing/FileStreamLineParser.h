#pragma once

#include "StreamLineParser.h"

#include <filesystem>
#include <fstream>

namespace sp::tools::parsing
{
  template <typename OutputData>
  class FileStreamLineParser : private std::unique_ptr<std::ifstream>, public StreamLineParser<OutputData>
  {
    using Base = StreamLineParser<OutputData>;

  public:
    FileStreamLineParser(const std::filesystem::path& filePath_, typename Base::LineParser parser_)
      : std::unique_ptr<std::ifstream>(std::make_unique<std::ifstream>())
      , StreamLineParser<OutputData>(*this->get(), std::move(parser_))
    {
      if (!std::filesystem::exists(filePath_))
      {
        throw std::runtime_error{"File not found!"};
      }
      this->get()->open(filePath_);
    }

    ~FileStreamLineParser()
    {
      this->get()->close();
    }
  };
}
