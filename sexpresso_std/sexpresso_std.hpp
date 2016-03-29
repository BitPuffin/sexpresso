// Author: Isak Andersson 2016 bitpuffin dot com

// If you don't opt out, copy and paste dependencies before include
// Still gotta include sexpresso.hpp first though
#ifdef SEXPRESSO_OPT_OUT_PIKESTYLE
#ifndef SEXPRESSO_STD_HEADER
#define SEXPRESSO_STD_HEADER
#include <ostream>
// #include "sexpresso_std.hpp"
#endif
#endif

namespace sexpresso_std {
	auto operator<<(std::ostream& ostream, sexpresso::Sexp const& sexp) -> std::ostream&;
}
