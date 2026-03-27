#include "tcp_receiver.hh"

using namespace std;

void TCPReceiver::receive( TCPSenderMessage message )
{
  if ( message.RST ) {
    reader().set_error();
    return;
  }
  if ( message.SYN && !syn_flag_) {  isn_ = message.seqno; syn_flag_ = true; }
  if ( !syn_flag_ ) return;
  uint64_t abs_sn = message.seqno.unwrap( isn_, reassembler_.get_first_unassembled_() );
  uint64_t stream_seq = abs_sn;
  if ( !message.SYN ) stream_seq -= 1; // 如果这个消息是SYN消息，那SYN对应的seq就是开头第一个的seq，不用减，如果不是SYN消息，那么seq因为之前SYN的存在有1位的位移，需要减
  if ( message.payload.size() > 0 || message.FIN )
    reassembler_.insert( stream_seq , message.payload, message.FIN );
  (void)message;
}

TCPReceiverMessage TCPReceiver::send() const
{
  TCPReceiverMessage message;
  if (syn_flag_) {
    uint64_t ack = reassembler_.get_first_unassembled_();
    if ( writer().is_closed() ) ack += 1;
    message.ackno = Wrap32::wrap( ack + 1 , isn_);
  }
  message.window_size = min( writer().available_capacity() , static_cast<uint64_t>(UINT16_MAX)  );
  message.RST = reader().has_error();
  return message;
}
