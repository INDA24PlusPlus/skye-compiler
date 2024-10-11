#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
typedef enum {
    IDENTIFIER, // var identifier
    KEYWORD,    // keyword
    PLUS,       // +
    MINUS,      // -
    MUL,        // *
    DIV,        // /
    INTEGER,	// regex /[0-9]+/i
    FLOAT,	// regex /[0-9]*(.[0-9]*)?/
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
    AND,	// &
    OR,		// |
    XOR,	// ^
    NOT,	// !
    NEWLINE,    // NEWLINE
    TEOF        // EOF
} TokType;

#define longestkw 10
#define shortestkw 2
#define numKw 14
char* keywords[]={
    "if", 		//if
    "else", 		//ee
    "elif", 		//ef
    "jump_if",		//jf 
    "jump", 		//jp
    "func", 		//fc
    "not", 		//nt
    "malloc",		//mc
    "free",		//fe
    "and", 		//ad
    "or", 		//or
    "xor", 		//xr
    "let",	 	//lt
    "jump_depth"	//jh
};

typedef struct {
	char* ptr;
	size_t len;
} StrBuf;
int strLen(char* str){
	int ln=0;
	while(1){
		if(str[ln]=='\0'){
			return ln;
		}
		ln++;
	}
}

int matchKw(char* str){
	if(strLen(str)>(longestkw)) return 0;
	if(strLen(str)<shortestkw) return 0;
	for(int i=0; i<numKw;i++){
		if(strLen(keywords[i])!=strLen(str)) continue;
		int q=1;
		for(int j=0; j<strLen(str); j++){
			if(!q) continue;
			if(str[j]!=keywords[i][j]) q=0;
		}
		if(q) return 1;
		
	}
	return 0;
}

void initStrBuf(StrBuf* str){
	str->len=0;
	str->ptr=malloc(1);
	if (str->ptr == NULL){
		printf("\033[31mCouldn't allocate pointer for str\033[0m\n");
		exit(5);
	}
}

void writeChar(StrBuf* str, char chr){	
	str->len+=1;
	str->ptr=realloc(str->ptr,str->len+1);
	if (str->ptr == NULL){
		printf("\033[31mCouldn't reallocate pointer of size %lu for str\033[0m\n", str->len+1);
		exit(5);
	}
	str->ptr[str->len-1]=chr;
	str->ptr[str->len]='\0';
}

void writeChars(StrBuf* str, char* chrs){
	int ln=0;
	while(1){
		if(chrs[ln]=='\0'){
			return;
		}
		writeChar(str, chrs[ln]);
		ln++;
	}

}
void freeStrBuf(StrBuf* ptr){
	free(ptr->ptr);
	free(ptr);
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
void freeValue(Value* ptr){
	free(ptr->valPtr);
	free(ptr);
}

typedef struct{
	char* expr;
	int* idx_start;
	int* col_start;
	int* ln_start;
	TokType* type;
} Token;
void freeToken(Token* ptr){
	free(ptr->expr);
	free(ptr->type);
	free(ptr->ln_start);
	free(ptr->idx_start);
	free(ptr->col_start);
	free(ptr);
}
void initToken(Token* ptr, TokType type, int idx_start, int col_start, int ln_start, char* expr){
	ptr->idx_start=malloc(sizeof(idx_start));
	*ptr->idx_start=idx_start;
	ptr->col_start=malloc(sizeof(col_start));
	*ptr->col_start=col_start;
	ptr->ln_start=malloc(sizeof(ln_start));
	*ptr->ln_start=ln_start;
	ptr->type=malloc(sizeof(TokType));
	*ptr->type=type;
	ptr->expr=malloc(strLen(expr)+1);
	sprintf(ptr->expr,"%s",expr);
}

typedef struct{
	Token** tokens;
	size_t* len;
} Tokens;
void freeTokens(Tokens* ptr){
	for(int i=0; i<*ptr->len;i++){
		freeToken(ptr->tokens[i]);
	}
	free(ptr->tokens);
	free(ptr);
}
void initTokenCollection(Tokens* ptr){
	ptr->len=malloc(sizeof(size_t));
	*ptr->len=0;
	ptr->tokens=malloc(1);
	}
void appendToken(Tokens* ptr, Token* tok){
	(*ptr->len)+=1;
	ptr->tokens=realloc(ptr->tokens,(*ptr->len+1));
	ptr->tokens[*ptr->len-1]=tok;

}

typedef struct {
	char* expr;
	int* index;
	int* line;
	int* col;
	char* current;
	Tokens* toks;


} Lexer;
void freeLexer(Lexer* ptr){
	free(ptr->expr);
	freeTokens(ptr->toks);
	free(ptr->col);
	free(ptr->line);
	free(ptr->index);
	free(ptr->current);
	free(ptr);
}
void initLexer(Lexer* ptr, char* expr){
	ptr->index=malloc(sizeof(int));
	ptr->line=malloc(sizeof(int));
	ptr->col=malloc(sizeof(int));
	ptr->current=malloc(sizeof(char));
	ptr->toks=malloc(sizeof(Tokens));
	*ptr->line=0;
	*ptr->col=0;
	*ptr->index=0;
	*ptr->current='\'';
	initTokenCollection(ptr->toks);
}
void advanceLexer(Lexer* lexer){
	lexer->index++;
	lexer->col++;
	if(*lexer->index>=strLen(lexer->expr)){
		*lexer->current='\0';
	}
	*lexer->current=lexer->expr[*lexer->index];
	if(*lexer->current=='\n'){
		Token* tok=malloc(sizeof(Token));
		initToken(tok, NEWLINE, *lexer->index, *lexer->col, *lexer->line, "\n");
		appendToken(lexer->toks,tok);
		lexer->line++;
		lexer->col=0;
	}
}
void parseNameLexer(Lexer* lexer){
	regex_t strRegCompiled;
	StrBuf* a=malloc(sizeof(StrBuf));
	regcomp(&strRegCompiled, "[A-Za-z0-9_]", REG_EXTENDED);
	regfree(&strRegCompiled);
	free(a);
	
}
void parseNumLexer(Lexer* lexer){
	regex_t numRegCompiled;
	
	StrBuf* a=malloc(sizeof(StrBuf));
	char* b=malloc(2);
	regcomp(&numRegCompiled,"[0-9]", REG_EXTENDED);
	b[0]=*lexer->current;
	b[1]='\0';
	int c=1;
	regmatch_t matches[1];
	while(c){
		if(regexec(&numRegCompiled,b,1,matches,0)==0){
		
		}
	}
	regfree(&numRegCompiled);
	// "(0|[1-9][0-9]*)?\.(0|[0-9]*[1-9])";

	// "(0|[1-9][0-9]*)";
	free(b);
	free(a);
	}
void parseLexer(Lexer* lexer){}
int main() {
	printf("Hellooo\n");
	StrBuf* str=malloc(sizeof(StrBuf));
	printf("Hellooo\n");
	initStrBuf(str);
	printf("Hellooo\n");
	writeChar(str,'H');
	printf("Hellooo\n");
	printf("%d\n",matchKw("jump_depth"));
	printf("%d\n",matchKw("jump_deptt"));

	Token* tok=malloc(sizeof(Token));
	initToken(tok, LCURL, 0, 1, 1, "aaaaa");
	Tokens* toks=malloc(sizeof(Tokens));
	initTokenCollection(toks);
	printf("Helloo00o\n");
	appendToken(toks, tok);
	printf("%ld||\n",*toks->len);
	printf("%d\n",*(toks->tokens[0]->type));	
	printf("%d\n%s\n",LCURL,str->ptr);
	writeChars(str,"ello World!");
	printf("%s\n",str->ptr);
	freeStrBuf(str);
	freeTokens(toks);
}
