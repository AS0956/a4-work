/**
 * A tokenizer for a simple shell language.
 *
 * Tokens:
 *   ( ) < > ; |      special characters, each one is its own token
 *   "some text"      a quoted string, special characters inside it are
 *                    ignored and the quotes are not part of the token
 *   ls, foo_bar      a word: a sequence of non-special characters
 *
 * Whitespace (space, tab, newline) is not a token, it only separates tokens.
 */
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "tokens.h"

/** Is the given character whitespace (space, tab or newline)? */
int is_whitespace(char ch) {
  return ch == ' ' || ch == '\t' || ch == '\n';
}

/** Is the given character a special one-character token? */
int is_special(char ch) {
  return ch == '(' || ch == ')' || ch == '<' || ch == '>' ||
         ch == ';' || ch == '|';
}

/** Can the given character be part of a word? */
int is_word_char(char ch) {
  return ch != '\0' && ch != '"' && !is_whitespace(ch) && !is_special(ch);
}

/** Add the first len characters of start to the vector as a new token. */
void add_token(vect_t *tokens, const char *start, int len) {
  char *token = malloc(len + 1);
  assert(token != NULL);

  strncpy(token, start, len);
  token[len] = '\0';

  vect_add(tokens, token);  /* the vector makes its own copy */
  free(token);
}

/**
 * Read a word starting at input[0] and add it to tokens.
 * Returns how many characters were read.
 */
int read_word(const char *input, vect_t *tokens) {
  int len = 0;
  while (is_word_char(input[len])) {
    len++;
  }
  add_token(tokens, input, len);
  return len;
}

/**
 * Read a quoted string starting at the opening quote input[0] and add what
 * is inside the quotes to tokens. If there is no closing quote, the string
 * goes to the end of the line.
 * Returns how many characters were read, including the quotes.
 */
int read_string(const char *input, vect_t *tokens) {
  assert(input[0] == '"');

  int len = 0;
  const char *start = input + 1;  /* skip the opening quote */
  while (start[len] != '\0' && start[len] != '"') {
    len++;
  }

  if (start[len] == '"') {
    add_token(tokens, start, len);
    return len + 2;               /* both quotes + the contents */
  }

  /* no closing quote: don't include a newline at the end */
  int consumed = len + 1;         /* opening quote + the contents */
  if (len > 0 && start[len - 1] == '\n') {
    len--;
  }
  add_token(tokens, start, len);
  return consumed;
}

/** Split the given string into a vector of tokens. The caller must delete
 *  the returned vector with vect_delete. */
vect_t *tokenize(const char *input) {
  assert(input != NULL);

  vect_t *tokens = vect_new();
  int i = 0;

  while (input[i] != '\0') {
    if (is_whitespace(input[i])) {
      i++;                        /* skip whitespace */
    } else if (is_special(input[i])) {
      add_token(tokens, &input[i], 1);
      i++;
    } else if (input[i] == '"') {
      i += read_string(&input[i], tokens);
    } else {
      i += read_word(&input[i], tokens);
    }
  }

  return tokens;
}
