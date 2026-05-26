#pragma once

#include "token.hpp"

#include <llvm/Support/SourceMgr.h>

#define discard_const(v) (char *) v

class Lexer {
  const char                                *_end;
  std::unordered_map<std::string, TokenType> _keywords;

  /// Current char
  char *c; // NOLINT(readability-identifier-naming)

  constexpr llvm::SMLoc
  mkloc () {
    return llvm::SMLoc::getFromPointer (c);
  }

  static constexpr llvm::SMLoc
  mkloc (const char *p) {
    return llvm::SMLoc::getFromPointer (p);
  }

  static constexpr Token
  mktoken (TokenType type) {
    return Token{ type };
  }

  static constexpr Token
  mktoken (TokenType type, llvm::SMLoc start, llvm::SMLoc end) {
    return { type, start, end };
  }

  static constexpr Token
  mkliteral (const Literal &literal, llvm::SMLoc start, llvm::SMLoc end) {
    return { TokenType::Literal, literal, start, end };
  }

  constexpr bool
  isspace () const {
    return std::isspace (static_cast<unsigned char> (*c)) != 0;
  }

  constexpr bool
  isdigit () const {
    return std::isdigit (static_cast<unsigned char> (*c)) != 0;
  }

  constexpr bool
  isalpha () const {
    return std::isalpha (static_cast<unsigned char> (*c)) != 0;
  }

  void
  skip ();

  void
  skipComment ();

  bool
  matches (char e);

  bool
  matches (const char e[2]);

  char
  peek (unsigned i) const;

  void
  next ();

  Token
  tokenizeNumber ();

  Token
  tokenizeKeyIdent ();

  Token
  tokenizeChar ();

  Token
  tokenizeString ();

  char
  readEscaped ();

  // NOLINTNEXTLINE(readability-identifier-naming)
#define X(Variant, Str) { Str, TokenType::Variant },

  public:
  Lexer (llvm::SourceMgr &mgr, unsigned buffer) {
    const auto &m = mgr.getMemoryBuffer (buffer);

    c    = discard_const (m->getBufferStart ());
    _end = m->getBufferEnd ();

    _keywords = { KEYWORDS };
  }

#undef X

  Token
  Tokenize ();
};
