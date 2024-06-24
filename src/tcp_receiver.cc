#include "tcp_receiver.hh"

#include <algorithm>


using namespace std;

void TCPReceiver::segment_received(const TCPSegment &seg) {
    const TCPHeader &Header = seg.header();
    string payload;
    size_t BytesWritten;

    if (Header.syn && _synReceived)//if syn already received then return
        return;

    if (Header.syn) {
        _isn = Header.seqno;
        _synReceived = true;
    }
    
    if (_synReceived) {
        _checkpoint = unwrap(Header.seqno, _isn, _checkpoint);//unwrapping 32-bit sequence number 
        payload = seg.payload().copy();
        if (Header.fin)
            _finReceived = true;
        //Pushing the paylod to reassembler rest will be handled by reassembler
        _reassembler.push_substring(payload, Header.syn ? 0 : _checkpoint - 1, _finReceived);

        BytesWritten = _reassembler.stream_out().bytes_written();

        expected_ack = BytesWritten + 1; 

        if (_finReceived && _reassembler.empty()){
            expected_ack++;
        }

        _ack = wrap(expected_ack, _isn);
    }
}

optional<WrappingInt32> TCPReceiver::ackno() const {
    if (_synReceived) {

        return _ack;
    }
    return {};
}

size_t TCPReceiver::window_size() const { return _capacity - _reassembler.stream_out().buffer_size(); }
