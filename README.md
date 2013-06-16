ajf-boost-samples
=================

Small C++ programs, most demonstrating various bits of Boost.

While investigating Boost features for myself, or while helping others
(especially on the Boost mailing lists and IRC channels), I've ended
up writing a quite a few small C++ programs.

Instead of hosting them in various personal directories, I thought I'd
put them here on GitHub for others to use.  (Putting them on a more
public service also reduces the chance they'll be lost if I migrate or
otherwise rearrange my personal pages elsewhere.)

Programs
--------

[regex-token-iter.cpp](regex-token-iter.cpp) - Uses the Boost.Regex
iterator to scan through text in an HTML-like markup language.

[unordered-set.cpp](unordered-set.cpp) - Demonstrates that storage
order within unordered_set is indeed influenced by insertion order.
Written after [Daniel James corrected me]
(http://article.gmane.org/gmane.comp.lib.boost.user/78730) on the
boost-users list.
