#include "byte_stream.hh"
#include <algorithm>

ByteStream::ByteStream(const size_t capacity)
    : capacity(capacity), End_input(false), ERROR(false), bytesWritten(0), bytesRead(0) {
    buffer.clear();
}

size_t ByteStream::write(const std::string &data) {
    if (End_input || ERROR){
        return 0;
    }

    size_t bytesToWrite = std::min(capacity - buffer.size(), data.size());
    for (size_t i = 0; i < bytesToWrite; ++i) {
        buffer.push_back(data[i]);
    }

    bytesWritten += bytesToWrite;

    return bytesToWrite;
}

size_t ByteStream::remaining_capacity() const {
    return capacity - buffer.size();
}

void ByteStream::end_input() {
    End_input = true;
}

void ByteStream::set_error() {
    ERROR = true;
}

std::string ByteStream::peek_output(const size_t len) const {
    size_t bytesToPeek = std::min(len, buffer.size());
    return std::string(buffer.begin(), buffer.begin() + bytesToPeek);
}

void ByteStream::pop_output(const size_t len) {
    size_t bytesToPop = std::min(len, buffer.size());
    buffer.erase(buffer.begin(), buffer.begin() + bytesToPop);
    bytesRead += bytesToPop;
}

std::string ByteStream::read(const size_t len) {
    std::string data = peek_output(len);
    pop_output(len);
    return data;
}

bool ByteStream::input_ended() const {
    return End_input;
}

bool ByteStream::error() const {
    return ERROR;
}

size_t ByteStream::buffer_size() const {
    return buffer.size();
}

bool ByteStream::buffer_empty() const {
    return buffer.empty();
}

bool ByteStream::eof() const {
    return End_input && buffer_empty();
}

size_t ByteStream::bytes_written() const {
    return bytesWritten;
}

size_t ByteStream::bytes_read() const {
    return bytesRead;
}

