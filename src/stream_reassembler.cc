#include <iostream>
#include <tuple>
#include "stream_reassembler.hh"

// Dummy implementation of a stream reassembler.

// For Lab 1, please replace with a real implementation that passes the
// automated checks run by `make check_lab1`.

// You will need to add private members to the class declaration in `stream_reassembler.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&.../* unused */) {}

using namespace std;

StreamReassembler::StreamReassembler(const size_t capacity) : _output(capacity),
                                                              _capacity(capacity),
                                                              ack(0),
                                                              _eof(false),
                                                              index_of_eof(-1)
{
}
void StreamReassembler::push_substring(const std::string &data, const uint64_t index, const bool eof)
{
    size_t unacc_ind = ack + _output.remaining_capacity(); //first index out of the window
    if (eof)
    {
        index_of_eof = index + data.size();
    }
    if (index + data.size() <= ack)
    {
        if (index_of_eof == ack)
        {
            _output.end_input();
        }
        return;
    }
    if (index <= ack) // If new index is less than or equal to acknowledgment index (means in order) then push in bytesteam
    {
        ack += _output.write(data.substr(ack - index)); //using substr to handle overlapping bytes
    }
    else
    {
        int it = 0;
        while (it < data.size() && it < unacc_ind)
        { // Write only those bytes to buffer which fit inside the bytestream 
            _buf[it + index] = data[it];
            ++it;
        }
    }
    //Pushing the unassembled bytes stored in the buffer into output bytestream
    for (auto it = _buf.begin(); it != _buf.end();)
    {
        if (it->first < ack)
        {
            it = _buf.erase(it);
        }
        else if (it->first == ack && _output.remaining_capacity() != 0)
        {
            string s = "";
            s.push_back(it->second);
            _output.write(s);
            it = _buf.erase(it);
            ++ack;
        }
        else
        {
            break;
        }
    }
    if (index_of_eof == ack)
    {
        _output.end_input();
    }
}
size_t StreamReassembler::unassembled_bytes() const
{
    return _buf.size();
}
bool StreamReassembler::empty() const
{
    return _buf.size() == 0;
}
size_t StreamReassembler::ack_index() const
{
    return ack;
}

//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
