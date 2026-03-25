#include "reassembler.hh"

using namespace std;

void Reassembler::insert( uint64_t first_index, string data, bool is_last_substring )
{
  if ( !output_.writer().is_closed() ) {
    uint64_t start = max ( first_index, first_unassembled_ );
    uint64_t end = min ( output_.writer().available_capacity() + first_unassembled_, first_index + data.length() );
    for ( uint64_t i = start; i < end; i ++ ) {
      uint64_t j = i - first_index;
      if (i >= buffer_.size()) {
        buffer_.resize(i + 1, 0);
        buffered_.resize(i + 1, false);
      }
      if (buffered_[i] == true)  continue;
      buffer_[i] = data[j];
      buffered_[i] = true;
      bytes_buffered_ ++;
    }
    
    while ( first_unassembled_ < buffer_.size() && buffered_[first_unassembled_] == true ) {
      output_.writer().push( string ( 1, buffer_[first_unassembled_] ) );
      buffer_[first_unassembled_] = 0;
      buffered_[first_unassembled_] = false;
      first_unassembled_ ++;
      bytes_buffered_ --;
    }
  }

  if ( is_last_substring ) {
    is_eof = true;
    eof_index = first_index + data.length();
  } 
  if ( is_eof && first_unassembled_ == eof_index) {
    output_.writer().close();
  }
  (void)first_index;
  (void)data;
  (void)is_last_substring;
}

uint64_t Reassembler::bytes_pending() const
{
  return bytes_buffered_ ;
}
