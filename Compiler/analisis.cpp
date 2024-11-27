#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

char auxiliary[100];
// Definición de tipos y estructuras
typedef enum {
    TOKEN_KEYWORD,
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_UNKNOWN
} TokenType;

typedef struct {//Define la longitud y tipo de token
    TokenType tipo;
    char value[100];
} Token;

typedef enum {//Identifica el tipo de numero
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_UNKNOWN
} DataType;

typedef struct Symbol {//Crea una variable para el tipo de dato guardado
    char name[100];
    DataType type;
    struct Symbol* next;
} Symbol;

// Tabla de símbolos (lista enlazada)
Symbol* symbol_table = NULL;

// Lista de palabras clave
const char* keywords[] = { "vezof" /* si */ , "noreth" /* else */ , "kash" /* while */ , "rhaeshisar" /* return */ , 
"rhaeshis" /* int */ , "dothraki" /* float */ ,"saul" /* for */ ,"mauricioemilianovelazquezcaudillo" /* { */ , NULL };

// Función para verificar si una cadena es una palabra clave
int is_keyword(const char* str) {
    for (int i = 0; keywords[i] != NULL; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

// Función para buscar una variable en la tabla de símbolos
Symbol* find_symbol(const char* name) {
    Symbol* current = symbol_table;
    while (current) {
        if (strcmp(current->name, name) == 0) {
            return current;
        }
        current = current->next; // Avanzar al siguiente nodo
    }
    return NULL;
}

void free_symbol_table() {
    Symbol* current = symbol_table;
    while (current) {
        Symbol* temp = current;
        current = current->next;
        free(temp);
    }
    symbol_table = NULL;
}

// Función para agregar una variable a la tabla de símbolos
void add_symbol(const char* name, DataType type) {
    if (find_symbol(name)) {
        printf("Error semántico: La variable '%s' ya está declarada.\n", name);
        exit(1);
    }
    Symbol* new_symbol = (Symbol*)malloc(sizeof(Symbol));
    if (!new_symbol) {
        printf("Error: No se pudo asignar memoria para la nueva variable.\n");
        exit(1);
    }

    // Copiar manualmente el nombre a la tabla de símbolos
    size_t len = strlen(name);
    if (len >= sizeof(new_symbol->name)) {
        printf("Error semántico: El nombre de la variable '%s' es demasiado largo.\n", name);
        exit(1);
    }
    for (size_t i = 0; i <= len; i++) {
        new_symbol->name[i] = name[i];
    }

    new_symbol->type = type;
    new_symbol->next = symbol_table;
    symbol_table = new_symbol;
}

// Función para obtener el siguiente token
Token get_next_token(const char* input, int* pos) {
    Token token;
    token.tipo = TOKEN_UNKNOWN;
    int i = 0;

    // Saltar espacios en blanco
    while (isspace(input[*pos])) (*pos)++;

    // Fin de cadena
    if (input[*pos] == '\0') {
        token.value[0] = '\0';
        return token;
    }

    // Detectar y saltar comentarios 
    if (input[*pos] == '/') {
        while (input[*pos] != '\0' && input[*pos] != '/') {
            (*pos)++;
        }
        return get_next_token(input, pos);  // Obtener el siguiente token después del comentario
    }

    // Identificadores o palabras clave
    if (isalpha(input[*pos]) || input[*pos] == '_') {
        while (isalnum(input[*pos]) || input[*pos] == '_') {
            token.value[i++] = input[*pos];
            (*pos)++;
        }
        token.value[i] = '\0';

        if (is_keyword(token.value)) {
            token.tipo = TOKEN_KEYWORD;
        }
        else {
            token.tipo = TOKEN_IDENTIFIER;
        }
        return token;
    }

    // Números
    if (isdigit(input[*pos]) || input[*pos] == '.') {
        int has_dot = 0;
        while (isdigit(input[*pos]) || (input[*pos] == '.' && !has_dot)) {
            if (input[*pos] == '.') has_dot = 1;
            token.value[i++] = input[*pos];
            (*pos)++;
        }
        token.value[i] = '\0';
        token.tipo = TOKEN_NUMBER;
        return token;
    }

    // Operadores (#, $, &, %)
    if (strchr("#$&%<>"/* + - * / < > */, input[*pos])) {
        token.value[i++] = input[*pos];
        token.value[i] = '\0';
        token.tipo = TOKEN_OPERATOR;
        (*pos)++;
        return token;
    }

    // Caracter desconocido
    token.value[0] = input[*pos];
    token.value[1] = '\0';
    (*pos)++;
    return token;
}

void parse_expression(const char* input, int* pos);
void parse_statement(const char* input, int* pos);
void parse_program(const char* input, int* pos);

// Función para analizar una declaración de variable
void parse_declaration(const char* input, int* pos) {
    Token token;
    size_t j;
    for (j = 0; j < sizeof(auxiliary) - 1 && token.value[j] != '\0'; j++) {
        token.value[j] = auxiliary[j];
    }
    token.value[j] = '\0';
    DataType type = TYPE_UNKNOWN;

    // Determinar el tipo de variable
    if (strcmp(token.value, "rhaeshis") == 0) {
        type = TYPE_INT;
    }
    else if (strcmp(token.value, "dothraki") == 0) {
        type = TYPE_FLOAT;
    }

    token = get_next_token(input, pos);

    // Obtener el identificador
    if (token.tipo != TOKEN_IDENTIFIER) {
        printf("Error de sintaxis: Se esperaba un identificador.\n");
        exit(1);
    }

    // Agregar a la tabla de símbolos
    add_symbol(token.value, type);

    // Verificar '='
    token = get_next_token(input, pos);
    if (token.value[0] != '=') {
        printf("Error de sintaxis: Se esperaba '=' después del identificador.\n");
        exit(1);
    }

    // Analizar expresión
    parse_expression(input, pos);

    // Verificar ';'
    size_t i;
    for (i = 0; i < sizeof(auxiliary) - 1 && token.value[i] != '\0'; i++) {
        token.value[i] = auxiliary[i];
    }
    token.value[i] = '\0';
    if (token.value[0] != ';') {
        printf("Error de sintaxis: Se esperaba ';' al final de la declaración.\n");
        exit(1);
    }
}

// Función para analizar expresiones aritméticas
void parse_expression(const char* input, int* pos) {
    Token token = get_next_token(input, pos);

    if (token.tipo == TOKEN_NUMBER || token.tipo == TOKEN_IDENTIFIER) {
        if (token.tipo == TOKEN_IDENTIFIER) {
            if (!find_symbol(token.value)) {
                printf("Error semántico: La variable '%s' no está declarada.\n", token.value);
                exit(1);
            }
        }

        token = get_next_token(input, pos);

        if (token.tipo != TOKEN_OPERATOR && token.value[0] != ';') {
            printf("Error de sintaxis: Se esperaba un ';'.\n");
            exit(1);
        }

        while (token.tipo == TOKEN_OPERATOR) {
            token = get_next_token(input, pos);

            if (token.tipo != TOKEN_NUMBER && token.tipo != TOKEN_IDENTIFIER) {
                printf("Error de sintaxis en expresión.\n");
                exit(1);
            }

            if (token.tipo == TOKEN_IDENTIFIER && !find_symbol(token.value)) {
                printf("Error semántico: La variable '%s' no está declarada.\n", token.value);
                exit(1);
            }

            token = get_next_token(input, pos);
        }
    }
    else {
        printf("Error de sintaxis: Se esperaba un número o identificador.\n");
        exit(1);
    }

    size_t i;//evita perder los tokens
    for (i = 0; i < sizeof(auxiliary) - 1 && token.value[i] != '\0'; i++) {
        auxiliary[i] = token.value[i];
    }
    auxiliary[i] = '\0';
}

void parse_identifier(const char* input, int* pos) {
    Token token = get_next_token(input, pos);
    if (token.value[0] != '=') {
        printf("Error de sintaxis: Se esperaba un símbolo '='");
        exit(1);
    }
    parse_expression(input, pos);
    size_t i;
    for (i = 0; i < sizeof(auxiliary) - 1 && token.value[i] != '\0'; i++) {
        token.value[i] = auxiliary[i];
    }
    token.value[i] = '\0';
    //token = get_next_token(input, pos);
    if (token.value[0] != ';') {
        printf("Error de sintaxis: Se esperaba ';' después de la condición.\n");
        exit(1);
    }
}

void parse_keys(const char* input, int* pos) {
    Token token = get_next_token(input, pos);
    if (strcmp(token.value, "mauricioemilianovelazquezcaudillo") != 0) {
        printf("Error de sintaxis: se esperaba 'mauricioemilianovelazquezcaudillo'.\n");
        exit(1);
    }
    int aux = *pos;

    token = get_next_token(input, pos);

    if (strcmp(token.value, "mauricioemilianovelazquezcaudillo") != 0) {
        *pos = aux;
        parse_statement(input, pos);

        token = get_next_token(input, pos);

        if (strcmp(token.value, "mauricioemilianovelazquezcaudillo") != 0) {
            printf("Error de sintaxis: se esperaba 'mauricioemilianovelazquezcaudillo'.\n");
            exit(1);
        }
    }
}

// Función para analizar una sentencia if
void parse_if_statement(const char* input, int* pos) {
    Token token = get_next_token(input, pos);
    if (token.value[0] != '(') {
        printf("Error de sintaxis: Se esperaba '('.\n");
        exit(1);
    }
    parse_expression(input, pos);

    size_t k;
    for (k = 0; k < sizeof(auxiliary) - 1 && token.value[k] != '\0'; k++) {
        token.value[k] = auxiliary[k];
    }
    token.value[k] = '\0';

    if (token.value[0] != ')') {
        printf("Error de sintaxis: Se esperaba ')'.\n");
        exit(1);
    }
    parse_keys(input, pos);
}

void saul_expression(const char* input, int* pos) {
    Token token = get_next_token(input, pos);

    // Verificar apertura de paréntesis
    if (token.value[0] != '(') {
        printf("Error de sintaxis: Se esperaba '('.\n");
        exit(1);
    }

    token = get_next_token(input, pos);

    if (!find_symbol(token.value) && token.tipo != TOKEN_KEYWORD) {
        printf("Error semántico: La variable '%s' no está declarada.\n", token.value);
        exit(1);
    }

    size_t j;
    for (j = 0; j < sizeof(auxiliary) - 1 && token.value[j] != '\0'; j++) {
        auxiliary[j] = token.value[j];
    }
    auxiliary[j] = '\0';

    // Analizar la declaración inicial (debe ser una declaración de variable)
    parse_declaration(input, pos);

    // Analizar la condición del bucle
    parse_expression(input, pos);

    // Verificar punto y coma tras la condición
    size_t i;
    for (i = 0; i < sizeof(auxiliary) - 1 && token.value[i] != '\0'; i++) {
        token.value[i] = auxiliary[i];
    }
    token.value[i] = '\0';
    //token = get_next_token(input, pos);
    if (token.value[0] != ';') {
        printf("Error de sintaxis: Se esperaba ';' después de la condición.\n");
        exit(1);
    }

    // Analizar la expresión de incremento
    parse_expression(input, pos);

    size_t k;
    for (k = 0; k < sizeof(auxiliary) - 1 && token.value[k] != '\0'; k++) {
        token.value[k] = auxiliary[k];
    }
    token.value[k] = '\0';

    // Verificar cierre de paréntesis
    if (token.value[0] != ')') {
        printf("Error de sintaxis: Se esperaba ')'.\n");
        exit(1);
    }

    // Analizar el cuerpo del bucle
    parse_keys(input, pos);
}

// Función para analizar cualquier sentencia
void parse_statement(const char* input, int* pos) {
    Token token = get_next_token(input, pos);

    if (token.tipo == TOKEN_KEYWORD) {
        if (strcmp(token.value, "rhaeshis") == 0 || strcmp(token.value, "dothraki") == 0) {
            parse_declaration(input, pos);
        }
        else if (strcmp(token.value, "vezof") == 0) {
            parse_if_statement(input, pos);
        }
        else if (strcmp(token.value, "rhaeshisar") == 0) {
            parse_expression(input, pos);
        }
        else if (strcmp(token.value, "saul") == 0) {
            saul_expression(input, pos);
        }
    }
    else if (find_symbol(token.value)) {
        parse_identifier(input, pos);
    }else{
        printf("Error de sintaxis: Token inesperado '%s'.\n", token.value);
    }
}

// Función principal para analizar el programa completo
void parse_program(const char* input, int* pos) {
    while (input[*pos] != '\0') {
        parse_statement(input, pos);
    }
}

int main() {
    char input[1024]; // Buffer para la entrada del usuario
    int pos = 0;

    printf("Introduce el código fuente (máximo 1023 caracteres):\n");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error al leer la entrada.\n");
        return 1;
    }

    // Eliminar el carácter de nueva línea si está presente
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }

    printf("\nCódigo fuente ingresado:\n%s\n", input);

    printf("\nAnálisis:\n");
    parse_program(input, &pos);

    printf("\nEl programa es semánticamente correcto.\n");
    return 0;
}
