#include <stdio.h>
#include <stdlib.h>

typedef struct Edge {
  int to;
  int len;
  struct Edge* next;
} Edge;

typedef struct Candidate {
  int city;
  int dist;
} Candidate;

void addCandidate(Candidate** candidates, int* count, int* capacity, int state,
                  int city, int dist) {
  if (*capacity == 0) {
    *capacity = 4;
    candidates[state] = malloc(*capacity * sizeof(Candidate));
    if (candidates[state] == NULL)
      return;
  }

  if (*count == *capacity) {
    *capacity *= 2;
    candidates[state] =
        realloc(candidates[state], *capacity * sizeof(Candidate));
    if (candidates[state] == NULL)
      return;
  }

  candidates[state][*count].city = city;
  candidates[state][*count].dist = dist;
  (*count)++;
}

void addEdge(Edge** graph, int from, int to, int len) {
  Edge* newEdge = malloc(sizeof(Edge));
  newEdge->to = to;
  newEdge->len = len;
  newEdge->next = graph[from];
  graph[from] = newEdge;
}

int main() {
  int n, m;
  if (scanf("%d %d", &n, &m) != 2) {
    return -1;
  }

  Edge** graph = malloc(n * sizeof(Edge*));
  if (graph == NULL) {
    return -1;
  }

  for (int i = 0; i < n; i++) {
    graph[i] = NULL;
  }

  for (int edge = 0; edge < m; edge++) {
    int u, v, len;
    if (scanf("%d %d %d", &u, &v, &len) != 3) {
      return -1;
    }
    addEdge(graph, u, v, len);
    addEdge(graph, v, u, len);
  }

  int k;
  if (scanf("%d", &k) != 1) {
    return -1;
  }

  int* capitals = malloc(k * sizeof(int));
  for (int i = 0; i < k; i++) {
    if (scanf("%d", &capitals[i]) != 1) {
      return -1;
    }
  }

  int* owner = malloc(n * sizeof(int));
  for (int i = 0; i < n; i++) {
    owner[i] = -1;
  }
  for (int i = 0; i < k; i++) {
    owner[capitals[i]] = i;
  }

  Candidate** candidates = malloc(k * sizeof(Candidate*));
  int* candidates_count = malloc(k * sizeof(int));
  int* candidates_capacity = malloc(k * sizeof(int));

  for (int i = 0; i < k; i++) {
    candidates[i] = NULL;
    candidates_count[i] = 0;
    candidates_capacity[i] = 0;
  }

  int total_occupied = k;
  int current_state = 0;

  for (int state = 0; state < k; state++) {
    int capital = capitals[state];

    Edge* current_edge = graph[capital];
    while (current_edge != NULL) {
      int neighbour = current_edge->to;
      int distance = current_edge->len;

      if (owner[neighbour] == -1) {
        addCandidate(candidates, &candidates_count[state],
                     &candidates_capacity[state], state, neighbour, distance);
      }

      current_edge = current_edge->next;
    }
  }

  while (total_occupied < n) {
    if (candidates_count[current_state] > 0) {

      int best_idx = 0;
      for (int i = 1; i < candidates_count[current_state]; i++) {
        if (candidates[current_state][i].dist <
            candidates[current_state][best_idx].dist) {
          best_idx = i;
        }
      }
      int chosen_city = candidates[current_state][best_idx].city;

      for (int i = best_idx; i < candidates_count[current_state] - 1; i++) {
        candidates[current_state][i] = candidates[current_state][i + 1];
      }
      candidates_count[current_state]--;

      if (owner[chosen_city] == -1) {
        owner[chosen_city] = current_state;
        total_occupied++;

        Edge* current_edge = graph[chosen_city];
        while (current_edge != NULL) {
          int neighbour = current_edge->to;
          int distance = current_edge->len;

          if (owner[neighbour] == -1) {
            addCandidate(candidates, &candidates_count[current_state],
                         &candidates_capacity[current_state], current_state,
                         neighbour, distance);
          }
          current_edge = current_edge->next;
        }
      }
    }
    current_state = (current_state + 1) % k;
  }

  for (int state = 0; state < k; state++) {
    printf("State %d: ", state);
    for (int city = 0; city < n; city++) {
      if (owner[city] == state) {
        printf("%d ", city);
      }
    }
    printf("\n");
  }

  for (int i = 0; i < n; i++) {
    Edge* current = graph[i];
    while (current != NULL) {
      Edge* temp = current;
      current = current->next;
      free(temp);
    }
  }
  free(graph);

  for (int i = 0; i < k; i++) {
    free(candidates[i]);
  }
  free(candidates);
  free(candidates_count);
  free(candidates_capacity);
  free(capitals);
  free(owner);

  return 0;