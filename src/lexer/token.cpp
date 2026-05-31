#include <lexer/token.hpp>
#include <utils.hpp>

static const char *ttstr[] = {
#define X(Variant, Str) Str,
  KEYWORDS LITERALS
#undef X
};

#define max_tok_buf_size 32

std::string
tt2str (TokenType tt) {
  if (tt < TokenType::FirstReserved) {
    if (std::isprint ((unsigned char) tt) != 0) {
      return { static_cast<char> ((unsigned char) tt) };
    }
    return std::format ("\\{}", (unsigned) tt);
  }

  if (tt > TokenType::KeywordsStart && tt < TokenType::KeywordsEnd) {
    // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-constant-array-index)
    return ttstr[(unsigned) tt - (unsigned) TokenType::KeywordsStart - 1];
  }
  if (tt == TokenType::Eof) {
    return "EOF";
  }

  return "Unknown";
}
