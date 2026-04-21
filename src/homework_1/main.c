#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int is_number(const char *str) {
    if (str == NULL || *str == '\0') {
        return 0;
    }
    
    int i = 0;
    if (str[0] == '-') {
        i++;
     
        if (str[i] == '\0') return 0;
    }
    
    int has_dot = 0;
    int has_digit = 0; 
    
    for (; str[i] != '\0'; i++) {
        if (str[i] == '.') {
        
            if (has_dot || i == 0 || (i == 1 && str[0] == '-')) {
                return 0;
            }
            has_dot = 1;
        } else if (isdigit(str[i])) {
            has_digit = 1; 
        } else {
            return 0;
        }
    }
    
    return has_digit;
}





typedef struct {
    int rows;
    int cols;
    char ***data;
} Table;


Table* createTable() {
    Table *t = malloc(sizeof(Table));
    t->data = NULL;
    t->rows = 0;
    t->cols = 0;
    return t;
}


int addRow(Table *t, char *line) {
    int new_row_count = t->rows + 1;

    char ***new_data = realloc(t->data, sizeof(char**) * new_row_count);
    if (new_data == NULL) return -1;
    t->data = new_data;

    char temp[1024];
    strcpy(temp, line);

    int col_count = 0;
    char *token = strtok(temp, ",\n");
    while (token != NULL) {
        col_count++;
        token = strtok(NULL, ",\n");
    }

    t->data[t->rows] = malloc(sizeof(char*)*col_count);

    strcpy(temp, line);
    token = strtok(temp, ",\n");
    int col_index = 0;
    while (token != NULL) {
        char *field = malloc(strlen(token)+1);
        strcpy(field,token);
        t->data[t->rows][col_index] = field;

        col_index++;
        token = strtok(NULL, ",\n");
    }

    if (t->cols == 0) {
        t->cols = col_count;
    }

    t->rows = new_row_count;
    return 0;
}

void freeTable(Table *t) {
    if (t == NULL) { 
        return;
    }

    for (int i = 0; i < t->rows; i++){
        for (int j = 0; j < t->cols; j++){
            free(t->data[i][j]);
        }
        free(t->data[i]);
    }

    free(t->data);
    free(t);
}


int printTableToFile(Table *t, const char *filename){
    FILE *f = fopen(filename, "w");
    if (f == NULL) {
        return -1;
    }

    int *col_width = malloc(sizeof(int) * t->cols);

    for (int i = 0; i < t->cols; i++){
        col_width[i] = 0;
    }

    for (int i = 0; i < t->rows; i++) {
        for (int j = 0; j < t->cols; j++){
            int len = strlen(t->data[i][j]);
            if (len > col_width[j]) col_width[j] = len;
        }
    }

    fprintf(f, "+");
    for (int j = 0; j < t->cols; j++) {
        for (int k = 0; k < col_width[j]; k++) {
            fprintf(f, "=");
        }
        fprintf(f,"+");
    }
    fprintf(f, "\n");

    
    fprintf(f, "|");
    for (int j = 0; j < t->cols; j++){
        fprintf(f, "%-*s|", col_width[j], t->data[0][j]);
    }
    fprintf(f,"\n");

    fprintf(f, "+");
    for (int j = 0; j < t->cols; j++) {
        for (int k = 0; k < col_width[j]; k++) {
            fprintf(f, "=");
        }
        fprintf(f, "+");
    }
    fprintf(f, "\n");


    for (int i = 1; i < t->rows; i++) {
        fprintf(f, "|");
        for (int j = 0; j < t->cols; j++) {
            if (is_number(t->data[i][j])) {
                fprintf(f, "%*s|", col_width[j], t->data[i][j]);
            } 
            else {
                fprintf(f,"%-*s|",col_width[j], t->data[i][j]);
            }
        }
        fprintf(f, "\n");


        if (i < t->rows - 1){
            fprintf(f,"+");
            for (int j = 0; j < t->cols; j++) {
                for (int k = 0; k < col_width[j]; k++) {
                    fprintf(f, "-");
                }
                fprintf(f,"+");
            }
            fprintf(f, "\n");
        }
    }
    free(col_width);
    fclose(f);
    return 0;
}

#ifndef TEST_MODE
int main(){
    const char *file = "input.csv";
    FILE *f = fopen(file, "r");
    if (f == NULL) {
        printf("Canot open file %s\n", file);
        return -1;
    }

    Table *t = createTable();
    char buffer[1024];
    
    while (fgets(buffer, sizeof(buffer),f) != NULL) {
        buffer[strcspn(buffer, "\n")] = 0;

        if (addRow(t,buffer) != 0) {
            printf("Error adding row\n");
            freeTable(t);
            fclose(f);
            return 1;
        }
    } 

    fclose(f);
    printTableToFile(t, "output.txt");
    freeTable(t);

    return 0;
}
#endif
    
    
    


