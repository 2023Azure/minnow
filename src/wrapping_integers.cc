#include "wrapping_integers.hh"

using namespace std;

Wrap32 Wrap32::wrap( uint64_t n, Wrap32 zero_point )
{
  uint32_t res = n + zero_point.raw_value_ ;
  (void)n;
  (void)zero_point;
  return Wrap32 { res };
}

uint64_t Wrap32::unwrap( Wrap32 zero_point, uint64_t checkpoint ) const
{
  uint64_t mod32 = ( 1ULL << 32 );
  uint64_t num = this->raw_value_ - zero_point.raw_value_;
  uint64_t k = checkpoint / mod32;
  num += k * mod32;
  if ( ( checkpoint < num ) && ( ( num - checkpoint ) > mod32/2 ) && num >= mod32 ) num -= mod32;
  if ( ( checkpoint > num ) && ( checkpoint - num ) > mod32/2 ) num += mod32;
  (void)zero_point;
  (void)checkpoint;
  return num;
}
