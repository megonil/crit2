#include <lexer/lexer.hpp>
#include <utils.hpp>

void inline Lexer::next () {
  c++;
}

void
Lexer::skip () {
  while (isspace ()) {
    next ();
  }
}

void
Lexer::skipComment () {
  while (!matches ("*)")) {
    next ();
  }
}

char
Lexer::peek (unsigned i) const {
  return *(c + i);
}

bool
Lexer::matches (char e) {
  bool result = *c == e;
  if (result) {
    next ();
  }

  return result;
}

bool
Lexer::matches (const char e[2]) {
  bool result = *c == e[0] && peek (1) == e[1];
  if (result) {
    next ();
    next ();
  }

  return result;
}

Token
Lexer::tokenizeNumber () {
  bool  has_dot = false;
  auto *starts  = c;

  while (isdigit () || matches ('.')) {
    if (matches ('.')) {
      if (has_dot) {
        error ("Too many dots");
      }

      has_dot = true;
    }

    next ();

    if (matches ('_')) {
      next ();
    }
  }

  auto *ends = c;

  Literal     litval;
  std::string val (starts, ends - starts);

  if (has_dot) {
    litval = std::stod (val);
  } else {
    litval = std::stoll (val);
  }

  auto start = mkloc (starts);
  auto end   = mkloc (ends);

  return mkliteral (litval, start, end);
}

Token
Lexer::tokenizeKeyIdent () {
  auto *starts = c;
  next (); // skip first alpha char

  while (isalpha () || matches ('_') || isdigit ()) {
    next ();
  }

  auto *ends = c;

  std::string val (starts, ends - starts);

  auto start = mkloc (starts);
  auto end   = mkloc (ends);

  if (_keywords.contains (val)) {
    return mktoken (_keywords[val], start, end);
  }

  return mktoken (TokenType::Id, start, end);
}

char
Lexer::readEscaped () {
  if (matches ('\\')) {
    next ();
    switch (*c) {
      case '0': return '\0';
      case 'n': return '\n';
      case 'r': return '\r';
      case 't': return '\t';
      case 'v': return '\v';
      case '\'': return '\'';
      case '"': return '"';
      default: error ("Wrong escape sequence");
    }
  }

  return *c;
}

Token
Lexer::tokenizeChar () {
  auto start = mkloc ();
  next (); // skip delimiter

  char c = readEscaped ();

  next (); // skip delimiter
  auto end = mkloc ();

  return mkliteral (c, start, end);
}

Token
Lexer::tokenizeString () {
  next (); // skip delimiter
  auto start = mkloc ();

  std::string val;

  while (!matches ('"')) {
    val += readEscaped ();
    next ();
    if (matches ('\0')) {
      error ("Unfinished string");
    }
  }

  auto end = mkloc ();

  return mkliteral (val, start, end);
}

Token
Lexer::Tokenize () {
  skip ();

  switch (*c) {
    case '\0': return mktoken (TokenType::Eof);
    case '\'': return tokenizeChar ();
    case '"': return tokenizeString ();
    default: {
      if (isdigit ()) {
        return tokenizeNumber ();
      }
      if (isalpha () || *c == '_') {
        return tokenizeKeyIdent ();
      }
      if (matches ("(*")) { // Is a comment
        skipComment ();
        return Tokenize ();
      }

      return mktoken ((TokenType) *c, mkloc (), mkloc ());
    }
  }
}
