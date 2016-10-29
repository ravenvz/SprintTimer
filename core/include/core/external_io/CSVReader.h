/********************************************************************************
**
** Copyright (C) 2016 Pavel Pavlov.
**
**
** This file is part of PROG_NAME.
**
** PROG_NAME is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** PROG_NAME is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with PROG_NAME.  If not, see <http://www.gnu.org/licenses/>.
**
*********************************************************************************/

#ifndef CSVREADER_H_RH5EZUKL
#define CSVREADER_H_RH5EZUKL

#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace CSV {


class StreamReadError : public std::runtime_error {
public:
    StreamReadError(const std::string& message);
};


class FormatError : public std::runtime_error {
public:
    FormatError(const std::string& message);
};


using Row = std::vector<std::string>;


class State;


/* Reads Comma-Separated Values (CSV) from input stream.

As there is no formal specification of CSV files,
this implementation understands CSV files as described in
IETF RFC 4180 (https://tools.ietf.org/html/rfc4180).

Note, that this CSV reader is implemented with simplicity in mind.
Unfortunately, that means that not all RFC 4180 rules are enforced.
In other words, it is client's responsibility to ensure that input
data conforms to RFC 4180 specification, otherwise parsing is not
guaranteed to be correct.

Throws StreamReadError when input stream is invalid.

When encountered a line with incorrect number of values,
FormatError is thrown, indicating the number of this line.
"Correct" number of values per line is deduced from the first
line of the input data.

Copy from RFC 4180 definition of CSV format:

1.  Each record is located on a separate line, delimited by a line
    break (CRLF).  For example:

    aaa,bbb,ccc CRLF
    zzz,yyy,xxx CRLF

2.  The last record in the file may or may not have an ending line
    break.  For example:

    aaa,bbb,ccc CRLF
    zzz,yyy,xxx

3.  There maybe an optional header line appearing as the first line
    of the file with the same format as normal record lines.  This
    header will contain names corresponding to the fields in the file
    and should contain the same number of fields as the records in
    the rest of the file (the presence or absence of the header line
    should be indicated via the optional "header" parameter of this
    MIME type).  For example:

    field_name,field_name,field_name CRLF
    aaa,bbb,ccc CRLF
    zzz,yyy,xxx CRLF

4.  Within the header and each record, there may be one or more
    fields, separated by commas.  Each line should contain the same
    number of fields throughout the file.  Spaces are considered part
    of a field and should not be ignored.  The last field in the
    record must not be followed by a comma.  For example:

    aaa,bbb,ccc

5.  Each field may or may not be enclosed in double quotes (however
    some programs, such as Microsoft Excel, do not use double quotes
    at all).  If fields are not enclosed with double quotes, then
    double quotes may not appear inside the fields.  For example:

    "aaa","bbb","ccc" CRLF
    zzz,yyy,xxx

6.  Fields containing line breaks (CRLF), double quotes, and commas
    should be enclosed in double-quotes.  For example:

    "aaa","b CRLF
    bb","ccc" CRLF
    zzz,yyy,xxx

7.  If double-quotes are used to enclose fields, then a double-quote
    appearing inside a field must be escaped by preceding it with
    another double quote.  For example:

    "aaa","b""bb","ccc"
*/
class CSVReader {
    friend class State;
    friend class ParseValue;
    friend class ParseQuotedValue;
    using row_t = std::vector<Row>;

public:
    using const_iterator = row_t::const_iterator;
    CSVReader(std::istream& stream, char separator = ',');
    const_iterator begin() const;
    const_iterator end() const;
    const_iterator cbegin() const;
    const_iterator cend() const;

private:
    std::istream& file;
    const char separator;
    State* state;
    std::vector<Row> rows;
    std::string valueBuffer;
    Row rowBuffer;
    size_t numValuesPerRow{0};
    size_t lineNumber{0};

    void parseFile();
    void parseChar(char ch);
    void skipNextChar();
    void pushValueBuffer(char ch);
    void flushBuffers();
    void flushValueBuffer();
    void flushRowBuffer();
};


class State {
public:
    virtual void parseChar(CSVReader& reader, char ch) = 0;
};


class ParseValue : public State {
public:
    void parseChar(CSVReader& reader, char ch) override;
};


class ParseQuotedValue : public State {
public:
    void parseChar(CSVReader& reader, char ch) override;
};

} // namespace ExternalIO

#endif /* end of include guard: CSVREADER_H_RH5EZUKL */
