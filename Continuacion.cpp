#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <iostream>
#include <stdlib.h>

using namespace std;
char auxiliary[100];

typedef enum {
    IR_ASSIGN, // Asignación: variable = valor
    IR_ADD,    // Suma: var = var1 + var2
    IR_SUB,    // Resta: var = var1 - var2
    IR_MUL,    // Multiplicación: var = var1 * var2
    IR_DIV,    // División: var = var1 / var2
    IR_LABEL,  // Etiqueta (para saltos)
    IR_GOTO,   // Salto
    IR_IF,     // Condición if
    IR_PRINT   // Instrucción de impresión
} IR_Opcode;

typedef struct {
    IR_Opcode opcode; // Tipo de operación
    char operand1[100]; // Primer operando
    char operand2[100]; // Segundo operando (si es necesario)
    char result[100]; // Resultado de la operación
} IR_Instr;

IR_Instr ir_code[100]; // Código intermedio almacenado
int ir_index = 0; // Índice para agregar nuevas instrucciones

// Definici�n de tipos y estructuras
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
    int value;
    struct Symbol* next;
} Symbol;

// Tabla de s�mbolos (lista enlazada)
Symbol* symbol_table = NULL;

// Lista de palabras clave
const char* keywords[] = { "vezof" /* si */ , "noreth" /* else */ , "kash" /* while */ , "rhaeshisar" /* return */ , 
"rhaeshis" /* int */ , "dothraki" /* float */ ,"saul" /* for */ ,"mauricioemilianovelazquezcaudillo" /* { */ , "voktys"/*printf */, NULL };

// Funci�n para verificar si una cadena es una palabra clave
int is_keyword(const char* str) {
    for (int i = 0; keywords[i] != NULL; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

// Funcion para buscar una variable en la tabla de simbolos
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

void add_value(const char* name, int value){
    Symbol* current= find_symbol(name);
    if (current != NULL){
        current->value=value;
    }
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

// Funcion para agregar una variable a la tabla de s�mbolos
void add_symbol(const char* name, DataType type) {
    if (find_symbol(name)) {
        printf("Error sem�ntico: La variable '%s' ya est� declarada.\n", name);
        exit(1);
    }
    Symbol* new_symbol = (Symbol*)malloc(sizeof(Symbol));
    if (!new_symbol) {
        printf("Error: No se pudo asignar memoria para la nueva variable.\n");
        exit(1);
    }

    // Copiar manualmente el nombre a la tabla de simbolos
    size_t len = strlen(name);
    if (len >= sizeof(new_symbol->name)) {
        printf("Error sem�ntico: El nombre de la variable '%s' es demasiado largo.\n", name);
        exit(1);
    }
    for (size_t i = 0; i <= len; i++) {
        new_symbol->name[i] = name[i];
    }

    new_symbol->type = type;
    new_symbol->next = symbol_table;
    symbol_table = new_symbol;
}

// Funcion para obtener el siguiente token
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
        return get_next_token(input, pos);  // Obtener el siguiente token despu�s del comentario
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

    // N�meros
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
    if (strchr("#$&%"/* + - * / < > */, input[*pos])) {
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

int parse_expression(const char* input, int* pos);
void parse_statement(const char* input, int* pos);
void parse_program(const char* input, int* pos);
string temporal;
bool check = true, check_2=true;
// Funci�n para analizar una declaraci�n de variable
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

    // Agregar a la tabla de simbolos
    add_symbol(token.value, type);
    temporal = token.value;
    // Verificar '='
    token = get_next_token(input, pos);
    if (token.value[0] != '=') {
        printf("Error de sintaxis: Se esperaba '=' despu�s del identificador.\n");
        exit(1);
    }

    // Analizar expresi�n
    int n_temp = parse_expression(input, pos);
    add_value(temporal.c_str(),n_temp);

    // Verificar ';'
    size_t i;
    for (i = 0; i < sizeof(auxiliary) - 1 && token.value[i] != '\0'; i++) {
        token.value[i] = auxiliary[i];
    }
    token.value[i] = '\0';
    if (token.value[0] != ';') {
        printf("Error de sintaxis: Se esperaba ';' al final de la declaraci�n.\n");
        exit(1);
    }
}

int operation(string expresion){
    string rest;
    char n1[5],n2[5],op, cons[20];
    int N1,N2, resultado;
    for (int i= 0; expresion[i] != '\0'; i++){
        i=0;
        int j= 0;
        if(isdigit(expresion[i])){
            while(isdigit(expresion[i])){
            n1[j++] = expresion[i++];
            }
            n1[j]='\0';
            N1=atoi(n1);
        }else{
            while(!strchr("#$&%",expresion[i])){
            cons[j++]=expresion[i++];
            }
            cons[j]='\0';
            Symbol* current= find_symbol(cons);
            if(current!=NULL){N1=current->value;}else{printf("Error semántico: La variable '%s' no está declarada.\n", cons); exit(1);}
        }
        j=0;
        op = expresion[i++];
        if (!strchr("#$&%",op)){
            return resultado;
        }
        if(isdigit(expresion[i])){
            while(isdigit(expresion[i])){
            n2[j++] = expresion[i++];
            }
            n2[j]='\0';
            N2=atoi(n2);
        }else{
            while(!strchr("#$&%",expresion[i])){
            cons[j++]=expresion[i++];
            }
            cons[j]='\0';
            Symbol* current= find_symbol(cons);
            if(current!=NULL){N2=current->value;}else{printf("Error semántico: La variable '%s' no está declarada.\n", cons); exit(1);}
        }
        rest = expresion.substr(i);
        switch (op){
        case '#':resultado=N1+N2;break;
        case '$':resultado=N1-N2;break;
        case '&':resultado=N1*N2;break;
        case '%':resultado=N1/N2;break;}
        expresion = to_string(resultado) + rest;
        i=0;
    }
    return resultado;
}

string sacar(const char* nuevo) {
    string salida;
    for (int i = 0; nuevo[i] != '\0'; i++) {
        salida += nuevo[i]; // Agregar carácter al final de salida
    }
    return salida;
}
int print;

// Funci�n para analizar expresiones aritm�ticas
int parse_expression(const char* input, int* pos) {
    Token token = get_next_token(input, pos);
    string expresion;
    TokenType temp=token.tipo;
    int n_temp;
    bool amp=true;
    char valor[20];
    if (token.tipo == TOKEN_NUMBER || token.tipo == TOKEN_IDENTIFIER) {
        if (token.tipo == TOKEN_IDENTIFIER) {
            if (!find_symbol(token.value)) {
                printf("Error semántico: La variable '%s' no está declarada.\n", token.value);
                exit(1);
            }
            int i = 0;
            for (;token.value[i]!='\0'; i++){
                valor[i]=token.value[i];
            }
            valor[i]='\0';
        }
        if (token.tipo == TOKEN_NUMBER){
            n_temp = atoi(token.value);
        }

        expresion += sacar(token.value); // Concatenar el valor del token

        token = get_next_token(input, pos);

        if (token.tipo != TOKEN_OPERATOR && token.value[0] != ';' && check_2) {
            printf("Error de sintaxis: Se esperaba un ';'.\n");
            exit(1);
        }

        if (temp==TOKEN_IDENTIFIER && !check_2 && token.value[0] == ')'){
            Symbol* get_value = find_symbol(valor);
            size_t i;
            for (i = 0; i < strlen(token.value) && i < sizeof(auxiliary) - 1; i++) {
                auxiliary[i] = token.value[i];
            }
           auxiliary[i] = '\0';
            return get_value->value;
        }

        if(temp==TOKEN_NUMBER && !check_2){
            size_t i;
            for (i = 0; i < strlen(token.value) && i < sizeof(auxiliary) - 1; i++) {
                auxiliary[i] = token.value[i];
            }
           auxiliary[i] = '\0';
           return n_temp;
        }

        if (temp==TOKEN_NUMBER && token.value[0]==';'){
            size_t i;
            for (i = 0; i < strlen(token.value) && i < sizeof(auxiliary) - 1; i++) {
                auxiliary[i] = token.value[i];
            }
           auxiliary[i] = '\0';
            return n_temp;
        }

        expresion += sacar(token.value);

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

            expresion += sacar(token.value); // Concatenar el valor del token
            print = *pos;
            token = get_next_token(input, pos);
            expresion += sacar(token.value);
        }
    } else {
        printf("Error de sintaxis: Se esperaba un número o identificador.\n");
        exit(1);
    }

    if(amp){
        n_temp =operation(expresion);
        size_t i;
        for (i = 0; i < strlen(token.value) && i < sizeof(auxiliary) - 1; i++) {
            auxiliary[i] = token.value[i];
        }
        auxiliary[i] = '\0';
        return n_temp;
        }

    // Ajustar el buffer auxiliary con los datos de token.value
    return NULL;
}

void parse_identifier(const char* input, int* pos) {
    Token token = get_next_token(input, pos);
    if (token.value[0] != '=') {
        printf("Error de sintaxis: Se esperaba un s�mbolo '='");
        exit(1);
    }
    int n_temp = parse_expression(input, pos);
    add_value(temporal.c_str(),n_temp);
    size_t i;
    for (i = 0; i < sizeof(auxiliary) - 1 && token.value[i] != '\0'; i++) {
        token.value[i] = auxiliary[i];
    }
    token.value[i] = '\0';
    //token = get_next_token(input, pos);
    if (token.value[0] != ';') {
        printf("Error de sintaxis: Se esperaba ';' despu�s de la condici�n.\n");
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
        if (check){
            *pos = aux;
            parse_statement(input, pos);

            token = get_next_token(input, pos);
        }
        check=true;
        while(strcmp(token.value, "mauricioemilianovelazquezcaudillo") != 0){
            token = get_next_token(input, pos);
            if(token.value[0]=='\0'){printf("Error de sintaxis: se esperaba 'mauricioemilianovelazquezcaudillo'.\n"); exit(1);}
        }
    }
}

bool if_solve(int n1, int n2, char exp){
    switch (exp)
    {
    case '=':
        return n1 == n2 ? true : false;
        break;
    case '>':
        return n1 > n2 ? true : false;
        break;
    case '<':
        return n1 < n2 ? true : false;
        break;
    }
}

// Funci�n para analizar una sentencia if
void parse_if_statement(const char* input, int* pos) {
    Token token = get_next_token(input, pos);
    int exp1,exp2;char op[2];
    check_2=false;
    if (token.value[0] != '(') {
        printf("Error de sintaxis: Se esperaba '('.\n");
        exit(1);
    }
    exp1 = parse_expression(input, pos);

    size_t k;
    for (k = 0; k < sizeof(auxiliary) - 1 && token.value[k] != '\0'; k++) {
        token.value[k] = auxiliary[k];
        op[k] = auxiliary[k];
    }
    token.value[k] = '\0';
    if (!strchr("=<>",token.value[0])){printf("Error de sintaxis: Se esperaba '=<>'.\n"); exit(1);};

    exp2 = parse_expression(input, pos);
    check = if_solve(exp1,exp2,op[0]);
    bool revisar=check;
    size_t h = k;
    for (h = 0; h < sizeof(auxiliary) - 1 && token.value[h] != '\0'; h++) {
        token.value[h] = auxiliary[h];
    }
    token.value[h] = '\0';

    if (token.value[0] != ')') {
        printf("Error de sintaxis: Se esperaba ')'.\n");
        exit(1);
    }
    check_2=true;
    parse_keys(input, pos);
}

void incremento(const char* input, int* pos){
    Token token = get_next_token(input, pos);
    if (token.tipo == TOKEN_IDENTIFIER) {
        if (!find_symbol(token.value)) {
            printf("Error sem�ntico: La variable '%s' no est� declarada.\n", token.value);
            exit(1);
        }
    }
    
    token = get_next_token(input, pos);

    if(token.value[0] != '='){
        printf("Error de sintaxis: se esperaba '='.\n");
    }

    parse_expression(input, pos);

}

void saul_expression(const char* input, int* pos) {
    Token token = get_next_token(input, pos);

    // Verificar apertura de par�ntesis
    if (token.value[0] != '(') {
        printf("Error de sintaxis: Se esperaba '('.\n");
        exit(1);
    }

    token = get_next_token(input, pos);

    if (!find_symbol(token.value) && token.tipo != TOKEN_KEYWORD) {
        printf("Error sem�ntico: La variable '%s' no est� declarada.\n", token.value);
        exit(1);
    }

    size_t j;
    for (j = 0; j < sizeof(auxiliary) - 1 && token.value[j] != '\0'; j++) {
        auxiliary[j] = token.value[j];
    }
    auxiliary[j] = '\0';

    // Analizar la declaraci�n inicial (debe ser una declaraci�n de variable)
    parse_declaration(input, pos);

    // Analizar la condici�n del bucle
    parse_expression(input, pos);

    // Verificar punto y coma tras la condici�n
    size_t i;
    for (i = 0; i < sizeof(auxiliary) - 1 && token.value[i] != '\0'; i++) {
        token.value[i] = auxiliary[i];
    }
    token.value[i] = '\0';
    //token = get_next_token(input, pos);
    if (token.value[0] != ';') {
        printf("Error de sintaxis: Se esperaba ';' despu�s de la condici�n.\n");
        exit(1);
    }

    // Analizar la expresi�n de incremento
    incremento(input, pos);

    size_t k;
    for (k = 0; k < sizeof(auxiliary) - 1 && token.value[k] != '\0'; k++) {
        token.value[k] = auxiliary[k];
    }
    token.value[k] = '\0';

    // Verificar cierre de par�ntesis
    if (token.value[0] != ')') {
        printf("Error de sintaxis: Se esperaba ')'.\n");
        exit(1);
    }

    // Analizar el cuerpo del bucle
    parse_keys(input, pos);
}

int imp_expr(Token token, const char* input, int* pos, int* pos_aux){
    int aux = *pos;
    Token auxiliar = token;
    token=get_next_token(input, pos);
    *pos= aux;
    if((auxiliar.value[0]!=')'||token.value[0]!='\"') && token.tipo != TOKEN_OPERATOR){
        int salida;
        if(auxiliar.tipo==TOKEN_NUMBER){
            return atoi(auxiliar.value);
        }else{
            Symbol* simbolo = find_symbol(auxiliar.value);
            if(simbolo!=NULL){return simbolo->value;}else{
                printf("Error sem�ntico: La variable '%s' no est� declarada.\n", token.value);
                exit(1);
            }
        }
    }else{
        int salida = parse_expression(input, pos_aux);
        *pos = print;
        return salida;
    }
}

void parse_print(const char* input, int* pos) {
    Token token = get_next_token(input, pos);
    string imprimir;

    if (token.value[0] != '(') {
        printf("Error de sintaxis: Se esperaba '('.\n");
        exit(1);
    }

    int aux = *pos;  // Guardar posición inicial
    while (token.value[0] != ')') {
        token = get_next_token(input, pos);

        if (token.value[0] == '\0') {
            printf("Error de sintaxis: se esperaba ')'.\n");
            exit(1);
        }

        if (token.tipo == TOKEN_KEYWORD) {
            printf("Error de sintaxis: Palabra reservada.\n");
            exit(1);
        }

        if (token.value[0] == '"') {
            token = get_next_token(input, pos);
            while (token.value[0] != '"') {
                imprimir = imprimir +token.value+" ";
                token = get_next_token(input, pos);

                if (token.value[0] == '\0') {
                    printf("Error de sintaxis: se esperaba '\"'.\n");
                    exit(1);
                }
            }
            aux = *pos;
        } else if (token.value[0] != ')') {
            int salida = imp_expr(token, input, pos, &aux);  // Corregido: pasar dirección de aux
            imprimir = imprimir + to_string(salida)+ " ";
        }
    }
    printf(imprimir.c_str());
}

int rhaeshisar(const char* input, int* pos) {  //Esta es la funcion que funciona como return
    Token token = get_next_token(input, pos);

    // Verificar si se sigue con una expresión
    if (token.value[0] != ';') {
        int return_value = parse_expression(input, pos);

        // Verificar el ';' final
        token = get_next_token(input, pos);
        if (token.value[0] != ';') {
            printf("Error de sintaxis: Se esperaba ';' después del valor de retorno.\n");
            exit(1);
        }

        return return_value; // Devuelve el valor evaluado de la expresión
    } else {
        // Retorno vacío
        return 0;
    }
}

// Funcion para analizar cualquier sentencia
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
            int return_value =rhaeshisar(input, pos);   //Se maneja la funcion return
        }
        else if (strcmp(token.value, "saul") == 0) {
            saul_expression(input, pos);
        }
        else if(strcmp(token.value, "voktys") == 0) {
            parse_print(input, pos);
        }
    }
    
    else if (find_symbol(token.value)) {
        temporal=token.value;
        parse_identifier(input, pos);
    }else{
        if (token.value[0]!='\0'){printf("Error de sintaxis: Token inesperado '%s'.\n", token.value);}
    }
}

// Funci�n principal para analizar el programa completo
void parse_program(const char* input, int* pos) {
    while (input[*pos] != '\0') {
        parse_statement(input, pos);
    }
}

void saul(const char* input, int* pos) {  //Declaracion de la funcion Saul
    Token token = get_next_token(input, pos);

    // Verificar '('
    if (token.value[0] != '(') {
        printf("Error de sintaxis: Se esperaba '('.\n");
        exit(1);
    }

    // Analizar la inicialización
    token = get_next_token(input, pos);
    if (token.tipo != TOKEN_IDENTIFIER && token.tipo != TOKEN_KEYWORD) {
        printf("Error de sintaxis en la inicialización.\n");
        exit(1);
    }
    parse_declaration(input, pos);

    // Analizar la condición del bucle
    token = get_next_token(input, pos);
    if (token.value[0] != ';') {
        parse_expression(input, pos);
    } else {
        printf("Error de sintaxis: Se esperaba una condición después del punto y coma.\n");
        exit(1);
    }

    // Verificar ';'
    token = get_next_token(input, pos);
    if (token.value[0] != ';') {
        printf("Error de sintaxis: Se esperaba ';' después de la condición.\n");
        exit(1);
    }

    // Analizar el incremento
    token = get_next_token(input, pos);
    if (token.tipo == TOKEN_IDENTIFIER) {
        incremento(input, pos);
    } else {
        printf("Error de sintaxis en el incremento.\n");
        exit(1);
    }

    // Verificar ')'
    token = get_next_token(input, pos);
    if (token.value[0] != ')') {
        printf("Error de sintaxis: Se esperaba ')'.\n");
        exit(1);
    }

    // Analizar el cuerpo del bucle
    parse_keys(input, pos);
}

void add_ir_instruction(IR_Opcode opcode, const char* operand1, const char* operand2, const char* result) {
    if (ir_index >= 100) {
        printf("Error: Excedido el límite del código intermedio.\n");
        exit(1);
    }
    ir_code[ir_index].opcode = opcode;
    strcpy(ir_code[ir_index].operand1, operand1 ? operand1 : "");
    strcpy(ir_code[ir_index].operand2, operand2 ? operand2 : "");
    strcpy(ir_code[ir_index].result, result ? result : "");
    ir_index++;
}

int main() {
    char input[1024]; // Buffer para la entrada del usuario
    char full_program[8192] = ""; // Buffer acumulativo para el programa completo
    int pos = 0;

    printf("Introduce el codigo fuente (deja una linea vacia para finalizar):\n");

    while (1) {
        // Leer una linea del usuario
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Error al leer la entrada.\n");
            return 1;
        }

        // Eliminar el caracter de nueva inea si esta presente
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }

        // Finalizar si la linea esta vacia
        if (strlen(input) == 0) {
            break;
        }
        // Acumular la linea al programa completo
        strncat(full_program, input, sizeof(full_program) - strlen(full_program) - 1);
        strncat(full_program, "\n", sizeof(full_program) - strlen(full_program) - 1);
    }
    printf("\nCodigo fuente ingresado:\n%s\n", full_program);
    printf("\nAnalisis:\n");
    parse_program(full_program, &pos);
    /*printf("\nEl programa es semanticamente correcto.\n");*/
    return 0;
}

//************************************************ESTO LO AGREGASTE RECIENTEMENTE********************//
/*void parse_assignment(const char* input, int* pos) {
    // Obtener el siguiente token, que debería ser el nombre de la variable
    Token token = get_next_token(input, pos);
    if (token.tipo != TOKEN_IDENTIFIER) {
        printf("Error de sintaxis: Se esperaba un identificador.\n");
        exit(1);
    }
    char variable[100];
    strcpy(variable, token.value);

    // Verificar que la variable esté declarada
    Symbol* sym = find_symbol(variable);
    if (!sym) {
        printf("Error semántico: Variable '%s' no declarada.\n", variable);
        exit(1);
    }

    // Obtener el siguiente token, que debería ser '='
    token = get_next_token(input, pos);
    if (strcmp(token.value, "=") != 0) {
        printf("Error de sintaxis: Se esperaba '=' después del identificador '%s'.\n", variable);
        exit(1);
    }

    // Analizar la expresión y obtener el resultado
    char expr_result[100];
    parse_expression(input, pos, expr_result); // Asume que parse_expression llena expr_result con el nombre de una variable temporal o un valor

    // Verificar que el siguiente token sea ';'
    token = get_next_token(input, pos);
    if (strcmp(token.value, ";") != 0) {
        printf("Error de sintaxis: Se esperaba ';' después de la asignación a '%s'.\n", variable);
        exit(1);
    }

    // Generar una instrucción IR_ASSIGN
    add_ir_instruction(IR_ASSIGN, expr_result, NULL, variable);
}

void parse_expression(const char* input, int* pos, char* result) {
    // Implementación básica de análisis de expresión
    // Este ejemplo asume expresiones simples como 'a + b'

    Token token1 = get_next_token(input, pos);
    char operand1[100];
    strcpy(operand1, token1.value);

    Token operator_token = get_next_token(input, pos);
    char operator_char = operator_token.value[0];

    Token token2 = get_next_token(input, pos);
    char operand2[100];
    strcpy(operand2, token2.value);

    // Crear una variable temporal para el resultado
    char temp[100];
    sprintf(temp, "t%d", ir_index); // Por ejemplo, t0, t1, ...

    // Generar la instrucción correspondiente
    switch (operator_char) {
        case '+':
            add_ir_instruction(IR_ADD, operand1, operand2, temp);
            break;
        case '-':
            add_ir_instruction(IR_SUB, operand1, operand2, temp);
            break;
        case '*':
            add_ir_instruction(IR_MUL, operand1, operand2, temp);
            break;
        case '/':
            add_ir_instruction(IR_DIV, operand1, operand2, temp);
            break;
        case '>':
            add_ir_instruction(IR_IF, operand1, operand2, temp); // Simplificado
            break;
        default:
            printf("Error: Operador desconocido '%c'.\n", operator_char);
            exit(1);
    }

    // Devolver el nombre de la variable temporal
    strcpy(result, temp);
}*/

/*void print_ir_code() {
    printf("\nCódigo Intermedio Generado:\n");
    for (int i = 0; i < ir_index; i++) {
        printf("%d: ", i);
        switch (ir_code[i].opcode) {
            case IR_ASSIGN:
                printf("ASSIGN %s -> %s\n", ir_code[i].operand1, ir_code[i].result);
                break;
            case IR_ADD:
                printf("ADD %s, %s -> %s\n", ir_code[i].operand1, ir_code[i].operand2, ir_code[i].result);
                break;
            case IR_SUB:
                printf("SUB %s, %s -> %s\n", ir_code[i].operand1, ir_code[i].operand2, ir_code[i].result);
                break;
            case IR_MUL:
                printf("MUL %s, %s -> %s\n", ir_code[i].operand1, ir_code[i].operand2, ir_code[i].result);
                break;
            case IR_DIV:
                printf("DIV %s, %s -> %s\n", ir_code[i].operand1, ir_code[i].operand2, ir_code[i].result);
                break;
            // Agrega otros casos según tus necesidades
            default:
                printf("UNKNOWN OPCODE\n");
        }
    }
}*/
