#pragma once

#include <cstdint>
#include <llvm/Support/SMLoc.h>
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
};
