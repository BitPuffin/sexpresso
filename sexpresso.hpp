// Author: Isak Andersson 2016 bitpuffin dot com

// dependencies (copy below lines and uncomment)
// #include <vector>
// #include <string>
// #include <cstdint>
// #include "sexpresso.hpp"

namespace sexpresso {
	enum class SexpValueKind : uint8_t { SEXP, STRING };

	struct Sexp {
		Sexp();
		Sexp(std::string const& strval);
		Sexp(std::vector<Sexp> const& sexpval);
		Sexp(Sexp const& sexp);
		Sexp(Sexp&& sexp);
		SexpValueKind kind;
		struct { std::vector<Sexp> sexp; std::string str; } value;
		auto addChild(Sexp sexp) -> void;
		auto addChild(std::string str) -> void;
		auto getChildByPath(std::string const& path) -> Sexp*; // unsafe! careful to not have the result pointer outlive the scope of the Sexp object
		auto toString() -> std::string;
		auto isString() -> bool;
		auto isSexp() -> bool;
	};

	auto parse(std::string const& str, std::string& err) -> Sexp;
}
