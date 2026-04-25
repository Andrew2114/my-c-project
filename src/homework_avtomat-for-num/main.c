#include <ctype.h>
#include <stdio.h>

typedef enum { Q0, Q1, Q2, Q3, Q4, Q5, Q6, ERR } State;

State transition(State s, char c) {
  int is_digit = isdigit(c);
  int is_dot = (c == '.');
  int is_E = (c == 'E');
  int is_sign = (c == '+' || c == '-');

  if (s == Q0) {
    if (is_digit)
      return Q1;
    return ERR;
  }
  if (s == Q1) {
    if (is_digit)
      return Q1;
    if (is_dot)
      return Q2;
    if (is_E)
      return Q4;
    return ERR;
  }
  if (s == Q2) {
    if (is_digit)
      return Q3;
    return ERR;
  }
  if (s == Q3) {
    if (is_digit)
      return Q3;
    if (is_E)
      return Q4;
    return ERR;
  }
  if (s == Q4) {
    if (is_digit)
      return Q6;
    if (is_sign)
      return Q5;
    return ERR;
  }
  if (s == Q5) {
    if (is_digit)
      return Q6;
    return ERR;
  }
  if (s == Q6) {
    if (is_digit)
      return Q6;
    return ERR;
  }
  return ERR;
}

int is_accepting(State s) {
  return (s == Q1 || s == Q3 || s == Q6);
}

int match(const char* str) {
  State cur = Q0;
  for (int i = 0; str[i] != '\0'; i++) {
    cur = transition(cur, str[i]);
    if (cur == ERR)
      return 0;
  }
  return is_accepting(cur);
}

int main() {
  const char* tests[] = {"123",     "123.456", "123E45",      "123.456E78",
                         "123E+45", "123E-45", "123.456E+78", "1.",
                         ".123",    "E123",    "123E",        "123.456E+"};
  int n = sizeof(tests) / sizeof(tests[0]);

  for (int i = 0; i < n; i++) {
    printf("%-15s -> %s\n", tests[i], match(tests[i]) ? "accept" : "reject");
  }
  return 0;
}