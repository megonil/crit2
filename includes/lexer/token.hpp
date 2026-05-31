#pragma once

#include <cstdint>
#include <iomanip>
#include <llvm/Support/SMLoc.h>
#include <sstream>
#include <string>
#include <utility>
#include <variant>

#define LITERALS                                                          \
  X (Literal, "<literal>")                                                \
  X (Id, "<id>")

#define KEYWORDS                                                          \
  X (Fn, "fn")                                                            \
  X (Let, "let")                                                          \
  X (Ret, "ret")

// clang-format off
#define X(Variant, Str) Variant,

enum class TokenType : unsigned char {
	FirstReserved = INT8_MAX,
  KeywordsStart,
  KEYWORDS
	LITERALS
	KeywordsEnd,
	Eof
};

#undef X
// clang-format on

std::string
tt2str (TokenType tt);

using Literal = std::variant<int64_t, double, char, std::string>;

struct LiteralToStringVisitor {
  std::string
  operator() (int64_t v) {
    return std::to_string (v);
  }

  std::string
  operator() (double v) {
    return std::to_string (v);
  }

  std::string
  operator() (char c) {
    std::string s (c, 1);
    return std::format ("{:?}", s);
  }

  std::string
  operator() (const std::string &s) {
    return std::format ("{:?}", s);
  }
};

struct Token {
  TokenType   Type{ TokenType::Eof };
  ::Literal   Literal;
  llvm::SMLoc Start, End;

  /// useful for testing
  Token () = default;

  Token (TokenType type, llvm::SMLoc start, llvm::SMLoc end)
      : Type (type), Start (start), End (end) {
  }

  Token (TokenType   type,
         ::Literal   literal,
         llvm::SMLoc start,
         llvm::SMLoc end)
      : Type (type),
        Literal (std::move (literal)),
        Start (start),
        End (end) {
  }

  explicit Token (TokenType type) : Type (type) {
  }

  static std::string
  LiteralString (const ::Literal &literal) {
    return std::visit (LiteralToStringVisitor{}, literal);
  }

  std::string
  LiteralString () const {
    return LiteralString (Literal);
  }

  std::string
  ToString () const {
    std::stringstream ss;
    ss << tt2str (Type);

    if (Type == TokenType::Literal) {
      ss << '(' << LiteralString () << ')';
    }

    return ss.str ();
  }
};
