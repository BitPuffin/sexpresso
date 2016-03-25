// Author: Isak Andersson 2016 bitpuffin dot com


#include <vector>
#include <string>
#include <cstdint>
#include <ostream>
#include "sexpresso.hpp"
#include "sexpresso_std.hpp"

namespace sexpresso_std {
	auto operator<<(std::ostream& ostream, sexpresso::Sexp const& sexp) -> std::ostream& {
		ostream << sexp.toString();
		return ostream;
	}
}
