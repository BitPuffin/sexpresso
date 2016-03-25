// Author: Isak Andersson 2016 bitpuffin dot com

#include <vector>
#include <string>
#include <cstdint>
#include "sexpresso.hpp"

#include <cctype>
#include <stack>
#include <algorithm>
#include <sstream>

namespace sexpresso {
	Sexp::Sexp() {
		this->kind = SexpValueKind::SEXP;
	}
	Sexp::Sexp(std::string const& strval) {
		this->kind = SexpValueKind::STRING;
		this->value.str = strval;
	}
	Sexp::Sexp(Sexp const& sexp)  {
		this->kind = sexp.kind;
		switch(sexp.kind) {
		case SexpValueKind::SEXP:
			this->value.sexp = std::vector<Sexp>{sexp.value.sexp};
			break;
		case SexpValueKind::STRING:
			this->value.str = sexp.value.str;
			break;
		}
	}

	Sexp::Sexp(Sexp&& sexp) {
		this->kind = sexp.kind;
		switch(sexp.kind) {
		case SexpValueKind::SEXP:
			this->value.sexp = std::move(sexp.value.sexp);
			break;
		case SexpValueKind::STRING:
			this->value.str = std::move(sexp.value.str);
			break;
		}
	}

	auto Sexp::addChild(Sexp sexp) -> void {
		if(this->kind == SexpValueKind::STRING) {
			this->kind = SexpValueKind::SEXP;
			this->value.sexp.push_back(Sexp{std::move(this->value.str)});
		}
		this->value.sexp.push_back(sexp);
	}

	auto Sexp::addChild(std::string str) -> void {
		this->addChild(Sexp{str});
	}

	auto Sexp::getChildByPath(std::string const& path) -> Sexp* {
		if(this->kind == SexpValueKind::STRING) return nullptr;

		auto paths = std::vector<std::string>{};
		{
			auto start = path.begin();
			for(auto i = path.begin()+1; i != path.end(); ++i) {
				if(*i == '/') {
					paths.push_back(std::string{start, i});
					start = i + 1;
				}
			}
			paths.push_back(std::string{start, path.end()});
		}
		auto teststr = std::string{};
		for(auto&& path : paths) {
			teststr = teststr + path + std::string{"/"};
		}

		auto* cur = this;
		for(auto i = paths.begin(); i != paths.end();) {
			auto start = i;
			for(auto& child : cur->value.sexp) {
				switch(child.kind) {
				case SexpValueKind::STRING:
					if(i == paths.end() - 1 && child.value.str == *i) return &child;
					else continue;
				case SexpValueKind::SEXP:
					if(child.value.sexp.size() == 0) continue;
					auto& fst = child.value.sexp[0];
					switch(fst.kind) {
					case SexpValueKind::STRING:
						if(fst.value.str == *i) {
							cur = &child;
							++i;
							break;
						}
					case SexpValueKind::SEXP: continue;
					}
				}
			}
			if(i == start) return nullptr;
			if(i == paths.end()) return cur;
		}
		return nullptr;
	}

	static auto stringValToString(std::string const& s) -> std::string {
		if(s.size() == 0) return std::string{"\"\""};
		if(std::find(s.begin(), s.end(), ' ') == s.end()) return s;
		else return ('"' + s + '"');
	}

 static auto toStringImpl(Sexp const& sexp, std::ostringstream& ostream) -> void {
		switch(sexp.kind) {
		case SexpValueKind::STRING:
			ostream << stringValToString(sexp.value.str);
			break;
		case SexpValueKind::SEXP:
			switch(sexp.value.sexp.size()) {
			case 0:
				ostream << "()";
				break;
			case 1:
				ostream << '(';
				toStringImpl(sexp.value.sexp[0], ostream);
				ostream <<  ')';
				break;
			default:
				ostream << '(';
				for(auto i = sexp.value.sexp.begin(); i != sexp.value.sexp.end(); ++i) {
					toStringImpl(*i, ostream);
					if(i != sexp.value.sexp.end()-1) ostream << ' ';
				}
				ostream << ')';
			}
		}
	}

	auto Sexp::toString() -> std::string {
		auto ostream = std::ostringstream{};
		// outer sexp does not get surrounded by ()
		switch(this->kind) {
		case SexpValueKind::STRING:
			ostream << stringValToString(this->value.str);
			break;
		case SexpValueKind::SEXP:
			for(auto i = this->value.sexp.begin(); i != this->value.sexp.end(); ++i) {
				toStringImpl(*i, ostream);
				if(i != this->value.sexp.end()-1) ostream << ' ';
			}
		}
		return ostream.str();
	}

	auto Sexp::isString() -> bool {
		return this->kind == SexpValueKind::STRING;
	}

	auto Sexp::isSexp() -> bool {
		return this->kind == SexpValueKind::SEXP;
	}

	auto parse(std::string const& str, std::string& err) -> Sexp {
		auto sexprstack = std::stack<Sexp>{};
		sexprstack.push(Sexp{}); // root
		auto nextiter = str.begin();
		for(auto iter = nextiter; iter != str.end(); iter = nextiter) {
			nextiter = iter + 1;
			if(std::isspace(*iter)) continue;
			auto& cursexp = sexprstack.top();
			switch(*iter) {
			case '(':
				sexprstack.push(Sexp{});
				break;
			case ')': {
				auto topsexp = std::move(sexprstack.top());
				sexprstack.pop();
				if(sexprstack.size() == 0) {
					err = std::string{"too many ')' characters detected, closing sexprs that don't exist, no good."};
					return Sexp{};
				}
				auto& top = sexprstack.top();
				top.addChild(std::move(topsexp));
				break;
			}
			case '"': {
				// TODO: handle escape sequences
				auto i = iter;
				for(; i != str.end(); ++i) {
					if(*i == '"') break;
					if(*i == '\n') {
						err = std::string{"Unexpected newline in string literal"};
						return Sexp{};
					}
				}
				if(i == str.end()) {
					err = std::string{"Unterminated string literal"};
					return Sexp{};
				}
				sexprstack.top().addChild(Sexp{std::string{iter+1, i}});
				nextiter = i + 1;
			}
			default:
				auto symend = std::find_if(iter, str.end(), [](char const& c) { return std::isspace(c) || c == ')'; });
				auto& top = sexprstack.top();
				top.addChild(Sexp{std::string{iter, symend}});
				nextiter = symend;
			}
		}
		if(sexprstack.size() != 1) {
			err = std::string{"not enough s-expressions were closed by the end of parsing"};
			return Sexp{};
		}
		return std::move(sexprstack.top());
	}
}
