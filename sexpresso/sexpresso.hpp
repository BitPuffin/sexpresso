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
		SexpValueKind kind;
		struct { std::vector<Sexp> sexp; std::string str; } value;
		auto addChild(Sexp sexp) -> void;
		auto addChild(std::string str) -> void;
		auto childCount() const -> size_t;
		auto getChildByPath(std::string const& path) -> Sexp*; // unsafe! careful to not have the result pointer outlive the scope of the Sexp object
		auto toString() const -> std::string;
		auto isString() const -> bool;
		auto isSexp() const -> bool;
		auto isNil() const -> bool;
		auto equal(Sexp const& other) const -> bool;
	};

	auto parse(std::string const& str, std::string& err) -> Sexp;
}
