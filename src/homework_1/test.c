#define TEST_MODE
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int rows;
  int cols;
  char ***data;
} Table;

Table *createTable();
int addRow(Table *t, char *line);
void freeTable(Table *t);
int printTableToFile(Table *t, const char *filename);
int is_number(const char *str);

void create_test_csv(const char *filename, const char *content) {
  FILE *f = fopen(filename, "w");
  if (f != NULL) {
    fprintf(f, "%s", content);
    fclose(f);
  }
}

void test_is_number() {
  printf("  Testing is_number...\n");
  assert(is_number("123") == 1);
  assert(is_number("0") == 1);
  assert(is_number("-123") == 1);
  assert(is_number("123.456") == 1);
  assert(is_number("-123.456") == 1);
  assert(is_number("0.1") == 1);

  assert(is_number("abc") == 0);
  assert(is_number("123abc") == 0);
  assert(is_number("12.34.56") == 0);
  assert(is_number("") == 0);
  assert(is_number(NULL) == 0);
  assert(is_number(".") == 0);
  assert(is_number("-") == 0);

  printf("  + is_number tests passed\n");
}

void test_table_operations() {
  printf("  Testing table operations...\n");
  Table *t = createTable();
  assert(t != NULL);
  assert(t->rows == 0);
  assert(t->cols == 0);
  assert(t->data == NULL);

  freeTable(t);
  printf("  + table operations tests passed\n");
}

void test_addRow() {
  printf("  Testing addRow...\n");
  Table *t = createTable();

  int result = addRow(t, "Name,Age,City");
  assert(result == 0);
  assert(t->rows == 1);
  assert(t->cols == 3);

  assert(strcmp(t->data[0][0], "Name") == 0);
  assert(strcmp(t->data[0][1], "Age") == 0);
  assert(strcmp(t->data[0][2], "City") == 0);

  result = addRow(t, "Alice,25,New York");
  assert(result == 0);
  assert(t->rows == 2);
  assert(t->cols == 3);

  assert(strcmp(t->data[1][0], "Alice") == 0);
  assert(strcmp(t->data[1][1], "25") == 0);
  assert(strcmp(t->data[1][2], "New York") == 0);

  freeTable(t);
  printf("  + addRow tests passed\n");
}

void test_integration() {
  printf("  Testing integration...\n");
  create_test_csv("test_input.csv", "Name,Age,Score\n"
                                    "Alice,25,95.5\n"
                                    "Bob,30,87.3\n"
                                    "Charlie,35,92.8\n");

  FILE *f = fopen("test_input.csv", "r");
  assert(f != NULL);

  Table *t = createTable();
  char buffer[1024];

  while (fgets(buffer, sizeof(buffer), f) != NULL) {
    buffer[strcspn(buffer, "\n")] = 0;
    addRow(t, buffer);
  }
  fclose(f);

  assert(t->rows == 4);
  assert(t->cols == 3);

  assert(strcmp(t->data[0][0], "Name") == 0);
  assert(strcmp(t->data[0][1], "Age") == 0);
  assert(strcmp(t->data[0][2], "Score") == 0);

  assert(strcmp(t->data[1][0], "Alice") == 0);
  assert(strcmp(t->data[2][0], "Bob") == 0);
  assert(strcmp(t->data[3][0], "Charlie") == 0);

  assert(is_number(t->data[1][1]) == 1);
  assert(is_number(t->data[1][2]) == 1);
  assert(is_number(t->data[1][0]) == 0);

  printTableToFile(t, "test_output.txt");

  FILE *out = fopen("test_output.txt", "r");
  assert(out != NULL);
  fclose(out);

  freeTable(t);

  remove("test_input.csv");
  remove("test_output.txt");

  printf("   + integration tests passed\n");
}

void run_tests() {
  printf("\n=== Running Tests ===\n");
  test_is_number();
  test_table_operations();
  test_addRow();
  test_integration();
  printf("\n=== All tests passed! ===\n\n");
}

// main для тестов
int main() {
  run_tests();
  return 0;
}
