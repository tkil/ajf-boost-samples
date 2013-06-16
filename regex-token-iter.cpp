/* compile line:

   g++ -O2 -o regex-token-iter regex-token-iter.cpp -lboost_regex

 */


#include <string>
#include <vector>
#include <boost/regex.hpp>

int main( int argc, char * argv [] )
{
    // match <color=\w+>...</color>, <i>...</i>, or <b>...</b>.  this
    // RE actually allows "</color=foo>", but we test that in code
    // rather than making the RE any uglier.
    const boost::regex tag_re(
        "<"               // start tag
        "/?"              //   ... which might be a closing tag
        "(?:"             //   non-saved group for alternates
        "(i)|"            //     italics go in sub-match 1
        "(b)|"            //     bold goes in sub-match 2
        "(color)"         //     color goes in sub-match 3
        "(?:=(\\w+))?"    //       actual color (if any) in 4
        ")"               //   end of alternates
        ">"               // end of tag
    );

    // we want the text before the match (-1), the whole-pattern match
    // (0), and all the capturing groups (1, 2, 3, 4).
    std::vector< int > groups;
    groups.push_back( -1 ); // pre text
    groups.push_back(  0 ); // whole tag
    groups.push_back(  1 ); //   italics
    groups.push_back(  2 ); //   bold
    groups.push_back(  3 ); //   color
    groups.push_back(  4 ); //     color name

    for ( int i = 1; i < argc; ++i )
    {
        const std::string s( argv[i] );

        boost::sregex_token_iterator rti( s.begin(), s.end(), tag_re, groups );
        boost::sregex_token_iterator rti_end;

        // which group are we currently looking at?
        size_t cur_group = 0;

        // are we in a tag:
        bool in_tag = false;

        // is this a closing tag?
        bool is_close = false;

        while ( rti != rti_end )
        {
            if ( rti->matched )
            {
                switch ( groups[ cur_group ] )
                {
                case -1: std::cout << "pre:        "; break;
                case 0:
                    in_tag = true;
                    is_close = ( rti->str()[1] == '/' );
                    if ( ! is_close )
                        std::cout << "tag_open:   ";
                    else
                        std::cout << "tag_close:  ";
                    break;
                case 1:  std::cout << "ital:       "; break;
                case 2:  std::cout << "bold:       "; break;
                case 3:  std::cout << "color:      "; break;
                case 4:  std::cout << "color_name: ";
                    if ( is_close )
                        std::cerr << "ERROR: color name in closing color tag!" << std::endl;
                    break;
                }
                std::cout << "'" << rti->str() << "'" << std::endl;
            }

            // go on to next group
            ++rti;

            // increment our group index
            if ( ++cur_group >= groups.size() )
            {
                // reset to start state
                in_tag = false;
                is_close = false;
                cur_group = 0;
            }
        }
    }

    return 0;
}
