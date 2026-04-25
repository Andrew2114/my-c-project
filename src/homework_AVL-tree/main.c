#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CODE 4
#define MAX_NAME 256

// Структура узла АВЛ-дерева
typedef struct Node {
  char code[MAX_CODE];
  char name[MAX_NAME];
  struct Node* left;
  struct Node* right;
  int height;
} Node;

// Вспомогательная функция: максимум двух чисел
int max(int a, int b) {
  return a > b ? a : b;
}

// Высота узла
int getHeight(Node* n) {
  return n ? n->height : 0;
}

// Обновить высоту
void updateHeight(Node* n) {
  if (n) {
    n->height = 1 + max(getHeight(n->left), getHeight(n->right));
  }
}

// Правый поворот
Node* rotateRight(Node* y) {
  Node* x = y->left;
  Node* T2 = x->right;

  x->right = y;
  y->left = T2;

  updateHeight(y);
  updateHeight(x);

  return x;
}

// Левый поворот
Node* rotateLeft(Node* x) {
  Node* y = x->right;
  Node* T2 = y->left;

  y->left = x;
  x->right = T2;

  updateHeight(x);
  updateHeight(y);

  return y;
}

// Балансировка узла
Node* balance(Node* n) {
  if (!n)
    return NULL;

  updateHeight(n);
  int balanceFactor = getHeight(n->left) - getHeight(n->right);

  // Левый перекос
  if (balanceFactor > 1) {
    if (getHeight(n->left->right) > getHeight(n->left->left)) {
      n->left = rotateLeft(n->left);
    }
    return rotateRight(n);
  }

  // Правый перекос
  if (balanceFactor < -1) {
    if (getHeight(n->right->left) > getHeight(n->right->right)) {
      n->right = rotateRight(n->right);
    }
    return rotateLeft(n);
  }

  return n;
}

// Создать новый узел
Node* createNode(const char* code, const char* name) {
  Node* new = (Node*)malloc(sizeof(Node));
  strcpy(new->code, code);
  strcpy(new->name, name);
  new->left = NULL;
  new->right = NULL;
  new->height = 1;
  return new;
}

// Вставка
Node* insert(Node* root, const char* code, const char* name) {
  if (!root)
    return createNode(code, name);

  int cmp = strcmp(code, root->code);

  if (cmp < 0) {
    root->left = insert(root->left, code, name);
  } else if (cmp > 0) {
    root->right = insert(root->right, code, name);
  } else {
    // Уже существует
    return root;
  }

  return balance(root);
}

// Поиск
Node* find(Node* root, const char* code) {
  if (!root)
    return NULL;

  int cmp = strcmp(code, root->code);
  if (cmp < 0)
    return find(root->left, code);
  if (cmp > 0)
    return find(root->right, code);

  return root;
}

// Найти самый левый узел
Node* findMin(Node* root) {
  while (root->left)
    root = root->left;
  return root;
}

// Удаление
Node* delete(Node* root, const char* code) {
  if (!root)
    return NULL;

  int cmp = strcmp(code, root->code);

  if (cmp < 0) {
    root->left = delete (root->left, code);
  } else if (cmp > 0) {
    root->right = delete (root->right, code);
  } else {
    // Нашли узел
    if (!root->left || !root->right) {
      // 0 или 1 ребенок
      Node* temp = root->left ? root->left : root->right;
      free(root);
      return temp;
    } else {
      // 2 ребенка
      Node* temp = findMin(root->right);
      strcpy(root->code, temp->code);
      strcpy(root->name, temp->name);
      root->right = delete (root->right, temp->code);
    }
  }

  return balance(root);
}

// Сохранение (обход по порядку)
void save(Node* root, FILE* f) {
  if (!root)
    return;
  save(root->left, f);
  fprintf(f, "%s:%s\n", root->code, root->name);
  save(root->right, f);
}

// Освобождение памяти
void freeTree(Node* root) {
  if (!root)
    return;
  freeTree(root->left);
  freeTree(root->right);
  free(root);
}

// Подсчет узлов
int countNodes(Node* root) {
  if (!root)
    return 0;
  return 1 + countNodes(root->left) + countNodes(root->right);
}

// Удалить пробелы в начале и конце
void trim(char* str) {
  int i = 0, j = strlen(str) - 1;
  while (isspace(str[i]))
    i++;
  while (j >= i && isspace(str[j]))
    j--;

  int len = j - i + 1;
  for (int k = 0; k < len; k++) {
    str[k] = str[i + k];
  }
  str[len] = '\0';
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    printf("Использование: %s <файл>\n", argv[0]);
    return 1;
  }

  // Загрузка базы
  FILE* f = fopen(argv[1], "r");
  if (!f) {
    printf("Ошибка открытия файла\n");
    return 1;
  }

  Node* root = NULL;
  char line[512];
  int count = 0;

  while (fgets(line, sizeof(line), f)) {
    line[strcspn(line, "\n")] = '\0';

    char* colon = strchr(line, ':');
    if (!colon)
      continue;

    *colon = '\0';
    char* code = line;
    char* name = colon + 1;

    if (strlen(code) == 3) { // Только трёхбуквенные коды
      root = insert(root, code, name);
      count++;
    }
  }
  fclose(f);

  printf("Загружено %d аэропортов. Система готова к работе.\n\n", count);

  // Обработка команд
  char cmd[20], arg[300];

  while (1) {
    printf("> ");
    if (!fgets(line, sizeof(line), stdin))
      break;
    line[strcspn(line, "\n")] = '\0';

    // Парсим команду
    if (sscanf(line, "%19s %299[^\n]", cmd, arg) < 1)
      continue;

    // Обработка команд
    if (strcmp(cmd, "find") == 0) {
      if (strlen(arg) == 0) {
        printf("Использование: find <код>\n");
      } else {
        trim(arg);
        Node* found = find(root, arg);
        if (found) {
          printf("%s → %s\n", found->code, found->name);
        } else {
          printf("Аэропорт с кодом '%s' не найден в базе.\n", arg);
        }
      }
    } else if (strcmp(cmd, "add") == 0) {
      if (strlen(arg) == 0) {
        printf("Использование: add <код>:<название>\n");
      } else {
        char* colon = strchr(arg, ':');
        if (!colon) {
          printf("Неверный формат. Нужно: add SVO:Шереметьево\n");
        } else {
          *colon = '\0';
          char* code = arg;
          char* name = colon + 1;
          trim(code);
          trim(name);

          if (strlen(code) == 3 && strlen(name) > 0) {
            if (find(root, code)) {
              printf("Аэропорт с кодом '%s' уже существует.\n", code);
            } else {
              root = insert(root, code, name);
              count++;
              printf("Аэропорт '%s' добавлен в базу.\n", code);
            }
          } else {
            printf("Неверный формат кода или названия.\n");
          }
        }
      }
    } else if (strcmp(cmd, "delete") == 0) {
      if (strlen(arg) == 0) {
        printf("Использование: delete <код>\n");
      } else {
        trim(arg);
        if (!find(root, arg)) {
          printf("Аэропорт с кодом '%s' не найден.\n", arg);
        } else {
          root = delete (root, arg);
          count--;
          printf("Аэропорт '%s' удалён из базы.\n", arg);
        }
      }
    } else if (strcmp(cmd, "save") == 0) {
      f = fopen(argv[1], "w");
      if (f) {
        save(root, f);
        fclose(f);
        printf("База сохранена: %d аэропортов.\n", count);
      } else {
        printf("Ошибка сохранения.\n");
      }
    } else if (strcmp(cmd, "quit") == 0) {
      break;
    } else {
      printf("Команды: find, add, delete, save, quit\n");
    }
    printf("\n");
  }

  freeTree(root);
  return 0;
}
