#include "byte_stream.hh"

using namespace std;

ByteStream::ByteStream( uint64_t capacity ) : capacity_( capacity ), buffer_() {}

bool Writer::is_closed() const
{
  return this->close_;
}

void Writer::push( string data )
{
  uint64_t can_push = min( available_capacity(), data.length() );
  this->buffer_.append( data.substr( 0, can_push ) );
  this->bytes_push_ += can_push;
  (void)data;
}

void Writer::close()
{

  this->close_ = true;
}

uint64_t Writer::available_capacity() const
{
  return this->capacity_ - this->buffer_.length();

}

uint64_t Writer::bytes_pushed() const
{
  return this->bytes_push_;

}

bool Reader::is_finished() const
{

  return this->buffer_.empty() && this->close_;
}

uint64_t Reader::bytes_popped() const
{
  return this->bytes_pop_;

}

string_view Reader::peek() const
{

  if ( buffer_.empty() )
    return {};
  return std::string_view( buffer_ );

}

void Reader::pop( uint64_t len )
{

  uint64_t can_pop = min( len, buffer_.length() );
  this->buffer_.erase( 0, can_pop );
  this->bytes_pop_ += can_pop;

  (void)len;
}

uint64_t Reader::bytes_buffered() const
{

  return (uint64_t)this->buffer_.length();

}
