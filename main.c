#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
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


typedef struct {
	char* ptr;
	size_t len;
} StrBuf;
int strLen(char* str){
	int ln=0;
	while(1){
		if(str[ln]=='\0'){
			return ln-1;
		}
		ln++;
	}
}

void initStrBuf(StrBuf* str){
	str->len=0;
	str->ptr=malloc(1);
	if (str->ptr == NULL){
		printf("\033[31mCouldn't init pointer for str\033[0m\n");
		exit(5);
	}	
}

void writeChar(StrBuf* str, char chr){
	str->len+=1;
	str->ptr=realloc(str->ptr,str->len+1);
	str->ptr[str->len-1]=chr;
	str->ptr[str->len]='\0';
}

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


typedef struct {
	char* expr;
	int index;
	char* current;
} Lexer;
int main() {
printf("Hellooo\n");
StrBuf* str;
initStrBuf(str);
writeChar(str,'a');
printf("%d\n%s\n",LCURL,str->ptr);
writeChar(str,'a');
printf("%s\n",str->ptr);
}
