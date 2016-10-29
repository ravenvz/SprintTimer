#include "core/external_io/CSVReader.h"
#include <iostream>
#include <iterator>
#include <memory>
#include <sstream>

namespace CSV {


StreamReadError::StreamReadError(const std::string& message)
    : std::runtime_error{message}
{
}


FormatError::FormatError(const std::string& message)
    : std::runtime_error{message}
{
}


namespace {
    auto parseValueState = std::make_unique<ParseValue>();
    auto parseQuotedValue = std::make_unique<ParseQuotedValue>();
    constexpr char quote{'"'};
    constexpr char linebreak{'\n'};
}


CSVReader::CSVReader(std::istream& stream, char separator)
    : file{stream}
    , separator{separator}
    , state{parseValueState.get()}
{
    if (!file) {
        std::string msg{"Error accessing stream, check that it is valid."};
        throw StreamReadError(msg);
    }
    parseFile();
}


void CSVReader::parseFile()
{
    while (true) {
        char ch = file.get();
        if (!file) {
            // Handle case when last row might miss line break.
            if (!rowBuffer.empty())
                flushBuffers();
            break;
        }
        parseChar(ch);
    }
}


void CSVReader::parseChar(char ch) { state->parseChar(*this, ch); }


CSVReader::const_iterator CSVReader::begin() const { return rows.begin(); }


CSVReader::const_iterator CSVReader::end() const { return rows.end(); }


CSVReader::const_iterator CSVReader::cbegin() const { return rows.cbegin(); }


CSVReader::const_iterator CSVReader::cend() const { return rows.cend(); }


void CSVReader::pushValueBuffer(char ch) { valueBuffer.push_back(ch); }


void CSVReader::flushBuffers()
{
    flushValueBuffer();
    flushRowBuffer();
}


void CSVReader::flushValueBuffer()
{
    rowBuffer.emplace_back(valueBuffer);
    valueBuffer.clear();
}


void CSVReader::flushRowBuffer()
{
    if (numValuesPerRow == 0)
        numValuesPerRow = rowBuffer.size();
    else if (numValuesPerRow != rowBuffer.size()) {
        std::stringstream err;
        err << "Line " << lineNumber << " has wrong number of values:\n"
            << "expected: " << numValuesPerRow << ", got: " << rowBuffer.size();
        throw FormatError(err.str());
    }
    rows.emplace_back(rowBuffer);
    rowBuffer.clear();
    ++lineNumber;
}


void CSVReader::skipNextChar() { file.get(); }


void ParseValue::parseChar(CSVReader& reader, char ch)
{
    if (ch == linebreak) {
        reader.flushBuffers();
    }
    else if (ch == reader.separator) {
        reader.flushValueBuffer();
    }
    else if (ch == quote) {
        reader.state = parseQuotedValue.get();
    }
    else {
        reader.pushValueBuffer(ch);
    }
}


void ParseQuotedValue::parseChar(CSVReader& reader, char ch)
{
    if (ch == quote) {
        if (reader.file.peek() == quote) {
            reader.pushValueBuffer(ch);
            reader.skipNextChar();
        }
        else {
            reader.state = parseValueState.get();
        }
    }
    else {
        reader.pushValueBuffer(ch);
    }
}


} // namespace ExternalIO
