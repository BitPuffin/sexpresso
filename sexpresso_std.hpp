// Author: Isak Andersson 2016 bitpuffin dot com

// additional dependencies (other than sexpresso.hpp) dependencies (copy below lines and uncomment)
// #include <ostream>
// #include "sexpresso_std.hpp"

namespace sexpresso_std {
	auto operator<<(std::ostream& ostream, sexpresso::Sexp const& sexp) -> std::ostream&;
}
