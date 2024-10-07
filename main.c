#include <stdio.h>
typedef enum {
    IDENTIFIER, // var identifier
    KEYWORD,    // keyword
    PLUS,       // +
    MINUS,      // -
    MUL,        // *
    DIV,        // /
    DECIMAL,    // regex /[0-9]*(.[0-9]*)?/
    LPAREN,     // (
    RPAREN,     // )
    LBRACKET,   // [
    RBRACKET,   // ]
    LCURL,      // {
    RCURL,      // }
    FLAG,       // ::flag_name::
    COMMA,      // ,
    COLON,      // :
    SEMICOLON,  // ;
    DOT,        // .
    LARROW,     // <-
    RARROW,     // ->
    EQ,         // =
    EE,         // ==
    GT,         // >
    GTE,        // >=
    LT,         // <
    LTE,        // <=
    NE,         // !=
    NEWLINE,    // NEWLINE
    TEOF        // EOF
} TokType;

char* keywords[]={
    "if", 
    "else", 
    "elif", 
    "jump_if", 
    "jump", 
    "func", 
    "not", 
    "and", 
    "or", 
    "xor", 
    "let", 
    "jump_depth"
};


typedef enum {
    Integer,
    Float,
    Boolean
} Type;

typedef struct {
	void* valPtr;
	int size;
	Type type;
} Value;



int main() {
printf("Hellooo");
printf("%d",LCURL);
}
