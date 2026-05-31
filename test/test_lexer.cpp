#include <catch2/catch_test_macros.hpp>
#include <filesystem>
#include <lexer/lexer.hpp>
#include <lexer/src.hpp>
#include <llvm/Support/SourceMgr.h>
#include <print>

static std::string
testpath (const std::string &file) {
  std::filesystem::path p = PROJ_SOURCE_DIR;
  p /= "test";
  p /= "lexer";
  p /= file;

  return p.string ();
}

#define init_lexer(filepath)                                              \
  llvm::SourceMgr mgr;                                                    \
  unsigned        buff_id = Src::FromFile (mgr, testpath (filepath));     \
                                                                          \
  Lexer  lexer (mgr, buff_id);                                            \
  size_t littest_id = 0;

#define deftokens(...)                                                    \
  using enum TokenType;                                                   \
  const TokenType desired_tokens[] = { __VA_ARGS__, Eof }

static void
tokenizeToVector (Lexer &lexer, std::vector<Token> &vec) {
  for (Token tok = lexer.Tokenize ();; tok = lexer.Tokenize ()) {
    vec.push_back (tok);
    if (tok.Type == TokenType::Eof) {
      break;
    }
  }
}

#define tokenize_all()                                                    \
  std::vector<Token> vec;                                                 \
  tokenizeToVector (lexer, vec);

#define arrlen(arr) (sizeof (arr) / sizeof ((arr)[0]))

static void
checkAllFun (std::vector<Token> &vec, const TokenType *arr, size_t len) {
  REQUIRE (vec.size () == len);
  for (size_t i = 0; i < len; ++i) {
    if (vec[i].Type != arr[i]) {
      std::println (
              stderr,
              "Tokenized type: {}, expected type: {}",
              tt2str (vec[i].Type),
              tt2str (arr[i]));
    }

    REQUIRE (vec[i].Type == arr[i]);
  }
}

#define check_all()                                                       \
  SECTION ("checking all tokens for types") {                             \
    checkAllFun (vec, desired_tokens, arrlen (desired_tokens));           \
  }

static void
printAll (std::vector<Token> &vec) {
  for (auto &t : vec) {
    std::print (stderr, "{} ", t.ToString ());
  }

  std::println (stderr, "");
}

#define print_all() printAll (vec);

static void
checkLiteral (std::vector<Token> &vec, size_t index, Literal expected) {
  const Token &tok    = vec[index];
  auto         actual = tok.Literal;

  if (actual != expected) {
    std::println (
            stderr,
            "Expected {}, Got {}.",
            tok.LiteralString (),
            Token::LiteralString (expected));
  }

  REQUIRE (actual == expected);
}

#define check_lit(v) checkLiteral (vec, littest_id++, v)

static void
ignore (void *v) {
}

#define ignore_lit() ignore (&littest_id);

TEST_CASE ("basic lexing", "{lexer}") {
  init_lexer ("basic.ct");
  ignore_lit ();
  deftokens (Fn);

  tokenize_all ();
  print_all ();
  check_all ();
}

TEST_CASE ("literals lexing", "{lexer}") {
  init_lexer ("literals.ct");
  deftokens (Literal, Literal, Literal);

  tokenize_all ();
  print_all ();
  check_all ();

  check_lit (123);
  check_lit (123.56);
  // check_lit (2, 'a');
  // check_lit (3, '\n');
  // check_lit (4, "Hello, World!\n");
}
