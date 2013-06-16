#include <time.h>

#include <iostream>
#include <vector>

#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/unordered/unordered_set.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/variate_generator.hpp>

namespace
{

typedef std::vector< int > int_vec;

typedef boost::unordered_set< int > int_set;

std::ostream & 
operator << ( std::ostream & os, const int_set & ints )
{
    boost::format hex4( "%04x" );
    os << std::hex;
    for ( int_set::const_iterator i( ints.begin() ), end; i != end; ++i )
        os << " " << hex4 % *i;
    return os;
}

}

int
main( int argc, char * argv [] )
{
    int n = 20;
    if ( argc > 1 )
        n = boost::lexical_cast< int >( argv[1] );

    // generate random integers to put into sets

    boost::mt19937 rng( time( 0 ) );
    boost::uniform_int< int > uint16_dist( 0x0, 0xffff );
    boost::variate_generator< boost::mt19937 &,
                              boost::uniform_int< int > >
      uint16_gen( rng, uint16_dist );

    int_vec vals;
    for ( int i = 0; i < n; ++i )
        vals.push_back( uint16_gen() );

    int_set fwd( vals.begin(), vals.end() );
    std::cout << "fwd:" << fwd << std::endl;

    int_set rev( vals.rbegin(), vals.rend() );
    std::cout << "rev:" << rev << std::endl;

    std::cout << "equal: " << ( fwd == rev ? "yes" : "no" ) << std::endl;

    return 0;
}
