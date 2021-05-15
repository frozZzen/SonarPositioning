#include "../../pch.h"

#include <tools/parsing/StreamLineParser.h>

#include <sstream>

using namespace sp::tools::parsing;

TEST(Parser, ParseStringStream)
{
  struct Record
  {
    std::string _tag;
    int _value;

    bool operator==(const Record& o_) const { return _tag == o_._tag && _value == o_._value; };
  };

  std::istringstream sstream;
  sstream.str(" #Comment\nTag1 123\n Tag2 456");

  Record expectedRecord1{"Tag1", 123};
  Record expectedRecord2{"Tag2", 456};

  StreamLineParser<Record> parser{sstream, [](std::istringstream& stream_) {
    Record record;
    stream_ >> record._tag >> record._value;
    return record;
  }};

  const auto record1 = parser.parseNextLine();
  const auto record2 = parser.parseNextLine();
  const auto record3 = parser.parseNextLine();

  ASSERT_TRUE(record1);
  ASSERT_TRUE(record2);
  EXPECT_FALSE(record3);

  EXPECT_EQ(record1.value(), expectedRecord1);
  EXPECT_EQ(record2.value(), expectedRecord2);
}
