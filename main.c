#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef enum {
    IDENTIFIER, // var identifier
    KEYWORD,    // keyword
    PLUS,       // +
    INCREMENT,	// ++
    MINUS,      // -
    DECREMENT,	// --
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
    FLAG,       // ::flag_name
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
#define numKw 16
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
    "pow",		//pw
    "let",	 	//lt
    "jump_depth",	//jh
    "continue"		//ce
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


StrBuf* initStrBuf(){
	StrBuf* str=malloc(sizeof(StrBuf));
	str->len=0;
	str->ptr=malloc(1);
	if (str->ptr == NULL){
		printf("\033[31mCouldn't allocate pointer for str\033[0m\n");
		exit(5);
	}
	return str;
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
Token* initToken(TokType type, int idx_start, int col_start, int ln_start, char* expr){
	Token* ptr=malloc(sizeof(Token));
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
	return ptr;
}
void printToken(Token* ptr){
	printf("[Type:%d, Val:%s]",*ptr->type,ptr->expr);
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
Tokens* initTokenCollection(){
	Tokens* ptr=malloc(sizeof(Tokens));
	ptr->len=malloc(sizeof(size_t));
	*ptr->len=0;
	ptr->tokens=malloc(1);
	return ptr;
	}
void appendToken(Tokens* ptr, Token* tok){
	(*ptr->len)+=1;
	ptr->tokens=realloc(ptr->tokens,(*ptr->len+1)*sizeof(Token*));
	ptr->tokens[*ptr->len-1]=tok;

}
void printTokens(Tokens* ptr){
	printf("[\n");
	for(int i=0; i<*ptr->len;i++){
		printf("\t");
		printToken(ptr->tokens[i]);
		printf("\n");
	}
	printf("]\n");
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
Lexer* initLexer(char* expr){
	Lexer* ptr=malloc(sizeof(Lexer));
	ptr->expr=malloc(strLen(expr)+1);
	sprintf(ptr->expr,"%s",expr);
	ptr->index=malloc(sizeof(int));
	ptr->line=malloc(sizeof(int));
	ptr->col=malloc(sizeof(int));
	ptr->current=malloc(sizeof(char));
	ptr->toks=initTokenCollection();
	*ptr->line=0;
	*ptr->col=0;
	*ptr->index=0;
	*ptr->current=ptr->expr[0];
	return ptr;
}
void advanceLexer(Lexer* lexer){
	*lexer->index=*lexer->index+1;
	*lexer->col=*lexer->col+1;
	if(*lexer->index>=strLen(lexer->expr)){
		*lexer->current='\0';
	}
	*lexer->current=lexer->expr[*lexer->index];
}

int isAlphabet(char c){
	return((c>='a'&&c<='z')||(c>='A'&&c<='Z')||(c=='_'));
}
int isDigit(char c){
	return(c>='0'&&c<='9');
}
void parseNameLexer(Lexer* lexer){
	StrBuf* a=initStrBuf();
	int start=*lexer->index;
	int col=*lexer->col;
	int line=*lexer->line;
	int c=1;
	while(c){
		if(isDigit(*lexer->current)||isAlphabet(*lexer->current)){
			writeChar(a, *lexer->current);
			advanceLexer(lexer);
		}
		else{
			c=0;
		}
	}
	
	appendToken(lexer->toks,initToken(matchKw(a->ptr)?KEYWORD:IDENTIFIER, start, col, line, a->ptr));
	freeStrBuf(a);
}
void parseNumLexer(Lexer* lexer){
	
	StrBuf* a=initStrBuf();
	int start=*lexer->index;
	int col=*lexer->col;
	int line=*lexer->line;
	int isFloat=0;
	int c=1;
	while(c){
		if(isDigit(*lexer->current)){
			writeChar(a, *lexer->current);
			advanceLexer(lexer);
		}
		else if (*lexer->current=='.') {
			if(isFloat){printf("Illegal char \'.\' at line %d, column %d\n",*lexer->line,*lexer->col);}
			isFloat=1;
			writeChar(a, '.');
			advanceLexer(lexer);
		}
		else{
			c=0;
		}
	}
	
	appendToken(lexer->toks,initToken(isFloat?FLOAT:INTEGER, start, col, line, a->ptr));
	freeStrBuf(a);
}


void parseLexer(Lexer* lexer){
	int c=1;
	int start,ln,col;
	while(c){
		printf("%c",*lexer->current);
		switch(*lexer->current){
			case '\n':
				appendToken(lexer->toks,initToken(NEWLINE, *lexer->index, *lexer->col, *lexer->line, "NEWLINE"));
				*lexer->line=*lexer->line+1;
				*lexer->col=0;
				advanceLexer(lexer);
				break;
			case '+':
				start=*lexer->index;
				ln=*lexer->line;
				col=*lexer->col;
				advanceLexer(lexer);
				if(*lexer->current=='+'){
					appendToken(lexer->toks,initToken(INCREMENT, start, col, ln, "++"));
					advanceLexer(lexer);
				}
				else appendToken(lexer->toks,initToken(PLUS, start,col,ln,"+"));
				break;
			case '-':
				start=*lexer->index;
				ln=*lexer->line;
				col=*lexer->col;
				advanceLexer(lexer);
				if(*lexer->current=='-'){
					appendToken(lexer->toks,initToken(DECREMENT, start, col, ln, "--"));
					advanceLexer(lexer);
				}
				else if(*lexer->current=='>'){
					appendToken(lexer->toks,initToken(RARROW, start, col, ln, "->"));
					advanceLexer(lexer);
				}
				else appendToken(lexer->toks,initToken(MINUS, start,col,ln,"-"));
				break;
			case '/':
				appendToken(lexer->toks,initToken(DIV, *lexer->index, *lexer->col, *lexer->line, "/"));
				advanceLexer(lexer);
				break;
			case '*':
				appendToken(lexer->toks,initToken(MUL, *lexer->index, *lexer->col, *lexer->line, "*"));
				advanceLexer(lexer);
				break;
			case '(':
				appendToken(lexer->toks,initToken(LPAREN, *lexer->index, *lexer->col, *lexer->line, "("));
				advanceLexer(lexer);
				break;
			case ')':
				appendToken(lexer->toks,initToken(RPAREN, *lexer->index, *lexer->col, *lexer->line, ")"));
				advanceLexer(lexer);
				break;
			case '{':
				appendToken(lexer->toks,initToken(LCURL, *lexer->index, *lexer->col, *lexer->line, "{"));
				advanceLexer(lexer);
				break;
			case '}':
				appendToken(lexer->toks,initToken(RCURL, *lexer->index, *lexer->col, *lexer->line, "}"));
				advanceLexer(lexer);
				break;
			case '[':
				appendToken(lexer->toks,initToken(LBRACKET, *lexer->index, *lexer->col, *lexer->line, "["));
				advanceLexer(lexer);
				break;
			case ']':
				appendToken(lexer->toks,initToken(RBRACKET, *lexer->index, *lexer->col, *lexer->line, "]"));
				advanceLexer(lexer);
				break;
			case '|':
				appendToken(lexer->toks,initToken(OR, *lexer->index, *lexer->col, *lexer->line, "|"));
				advanceLexer(lexer);
				break;
			case '&':
				appendToken(lexer->toks,initToken(AND, *lexer->index, *lexer->col, *lexer->line, "&"));
				advanceLexer(lexer);
				break;
			case '^':
				appendToken(lexer->toks,initToken(XOR, *lexer->index, *lexer->col, *lexer->line, "^"));
				advanceLexer(lexer);
				break;
			case '<':
				start=*lexer->index;
				ln=*lexer->line;
				col=*lexer->col;
				advanceLexer(lexer);
				if(*lexer->current=='-'){
					appendToken(lexer->toks,initToken(LARROW, start, col, ln, "<-"));
					advanceLexer(lexer);
				}
				else if(*lexer->current=='='){
					appendToken(lexer->toks, initToken(LTE,start,col,ln,"<="));
					advanceLexer(lexer);
				}
				else appendToken(lexer->toks,initToken(LT, start,col,ln,"<"));				
				break;
			case '>':
				start=*lexer->index;
				ln=*lexer->line;
				col=*lexer->col;
				advanceLexer(lexer);
				if(*lexer->current=='='){
					appendToken(lexer->toks,initToken(GTE, start, col, ln, ">="));
					advanceLexer(lexer);
				}
				else appendToken(lexer->toks,initToken(GT, start,col,ln,">"));				
				break;
			case '=':
				start=*lexer->index;
				ln=*lexer->line;
				col=*lexer->col;
				advanceLexer(lexer);
				if(*lexer->current=='='){
					appendToken(lexer->toks,initToken(EE, start, col, ln, "=="));
					advanceLexer(lexer);
				}
				else appendToken(lexer->toks,initToken(EQ, start,col,ln,"="));				
				break;
			case ':':
				start=*lexer->index;
				ln=*lexer->line;
				col=*lexer->col;
				advanceLexer(lexer);
				if(*lexer->current==':'){
					appendToken(lexer->toks,initToken(FLAG, start, col, ln, "::"));
					advanceLexer(lexer);
				}
				else appendToken(lexer->toks,initToken(COLON, start,col,ln,":"));				
				break;
			case ';':
				appendToken(lexer->toks,initToken(SEMICOLON, *lexer->index, *lexer->col, *lexer->line, ";"));
				advanceLexer(lexer);
				break;
			case '.':
				appendToken(lexer->toks,initToken(DOT, *lexer->index, *lexer->col, *lexer->line, "."));
				advanceLexer(lexer);
				break;
			case ',':
				appendToken(lexer->toks,initToken(COMMA, *lexer->index, *lexer->col, *lexer->line, ","));
				advanceLexer(lexer);
				break;
			case '!':
				start=*lexer->index;
				ln=*lexer->line;
				col=*lexer->col;
				advanceLexer(lexer);
				if(*lexer->current=='='){

					appendToken(lexer->toks,initToken(NE, start, col, ln, "!="));
					advanceLexer(lexer);
				}
				else appendToken(lexer->toks,initToken(NOT, start,col,ln,"!"));				
				break;
			case ' ':
				advanceLexer(lexer);
				break;
			case '\0':
				appendToken(lexer->toks,initToken(TEOF, *lexer->index, *lexer->col, *lexer->line, "EOF"));
				c=0;
				break;
			default:
				if(isDigit(*lexer->current)){
					parseNumLexer(lexer);
				}
				else if(isAlphabet(*lexer->current)){
					parseNameLexer(lexer);
				}
				break;
		}
	}
	printf("\n");
}

typedef struct{
	Token* self;
	struct LinkedListToken* next;
	struct LinkedListToken* prev;
} LinkedListToken;
LinkedListToken* initLLT(Token* tok){
	LinkedListToken* llt=malloc(sizeof(LinkedListToken));
	llt->self=tok;
	llt->next=NULL;
	llt->prev=NULL;
	return llt;
}
void pushToken(LinkedListToken* LLT, Token* tok){
	if(LLT->next!=NULL){
		pushToken(((LinkedListToken *)LLT->next),tok);
	}
	else{
		LLT->next=((struct LinkedListToken *)initLLT(tok));
		((LinkedListToken *)LLT->next)->prev=((struct LinkedListToken*)LLT);
	}
}
LinkedListToken* fromCollection(Tokens* toks){
	LinkedListToken* llt=initLLT(toks->tokens[0]);
	for(int i=1; i<*toks->len;i++){
		pushToken(llt, toks->tokens[i]);
	}
	return llt;
}

typedef enum {
	ROOT_NODE,
	NUMBER_NODE,
	TYPE_NODE,
	TYPECHAIN_NODE,
	BIN_OP_NODE,
	UN_OP_NODE,
	VAR_ASSIGN_NODE,
	VAR_ACCESS_NODE,
	IF_NODE,
	FOR_NODE,
	WHILE_NODE,
	FLAG_NODE,
	JUMP_NODE,
	JUMP_IF_NODE,
	FUNC_DEF_NODE,
	CALL_NODE
} NodeType;
typedef struct{struct Node* node;} Root;
typedef struct{Token* token;} Number;
typedef struct{
	struct Node* left;
	Token* token; 
	struct Node* right;
} Binop;
typedef struct{
	struct Node* node;
	Token* token;
} Unop;
typedef struct{
	struct Node* node;
	Token* token;
	//Type* type;
} VarAssign;
typedef struct{
	Token* token;
} VarAccess;
typedef struct{
	struct Node* condition;
	struct Node* expression;
} IfSet;
typedef struct{
	IfSet** cases;
	struct Node* elsecase;
} IfNode;
typedef struct{
	struct Node* condition;
	struct Node* body;
} WhileNode;
typedef struct{
	Token* variable;
	struct Node* start;
	struct Node* end;
	struct Node* step;
	struct Node* body;
} ForNode;
typedef struct{Token* flag_name;} Flag;
typedef struct{Token* flag_name;} Jump;
typedef struct{
	Token* flag_name;
	struct Node* condition;
} JumpIf;
typedef struct{
	Token* name;
	//Type* type;
} ArgSet;
typedef struct{
	Token* var_name;
	ArgSet** args;
	//Type* return_type;
	struct Node* body;
} FuncDef;
typedef struct{
	struct Node* target;
	struct Node** args;
} Call;
typedef union{
	Root* root;
	Number* number;
	struct{}* type;
	struct{}* typechain;
	Binop* binop;
	Unop* unop;
	VarAssign* var_assign;
	VarAccess* var_access;
	IfNode* if_node;
	WhileNode* while_node;
	ForNode* for_node;
	Flag* flag;
	Jump* jump;
	JumpIf* jump_if;
	FuncDef* func_def;
	Call* call;
} NodeWrapper;
typedef struct{
	NodeWrapper* node;
	NodeType* type;
} Node;
Node* initNode(NodeType type, NodeWrapper* node){
	Node* n=malloc(sizeof(Node));
	n->type=malloc(sizeof(NodeType));
	n->node=node;
	*n->type=type;
	return n;
}

typedef struct{
	Node** roots;
	size_t* len;
} AST;
AST* initAST(){
	AST* ptr=malloc(sizeof(AST));
	ptr->len=malloc(sizeof(size_t));
	*ptr->len=0;
	ptr->roots=malloc(1);
	return ptr;
}

void AddRoot(AST* ptr, Node* node){
	(*ptr->len)+=1;
	ptr->roots=realloc(ptr->roots,(*ptr->len+1)*sizeof(Node*));
	ptr->roots[*ptr->len-1]=node;
}

typedef struct{
	LinkedListToken* LLT;
	AST* ast;
} Parser;

Parser* initParser(Tokens* toks){
	Parser* parser=malloc(sizeof(Parser));
	parser->ast=initAST();
	parser->LLT=fromCollection(toks);
	return parser;

}
void advanceParser(Parser* parser){
	parser->LLT=(LinkedListToken *)parser->LLT->next;
}
Node* parseTokens(Parser* parser, char path, char p1, char p2, TokType* tType, int tokc, char* expr){
	if(path=='a'){
		TokType tt=*parser->LLT->self->type;
		if(tt==FLOAT||tt==INTEGER){
			Number* num=malloc(sizeof(Number));
			num->token=malloc(sizeof(Token));
			*num->token=*parser->LLT->self;
			advanceParser(parser);
			NodeWrapper* nw=malloc(sizeof(NodeWrapper));
			nw->number=num;
			return(initNode(NUMBER_NODE,nw));
		}
		else if(tt==LPAREN){
			advanceParser(parser);
			Node* ret=parseTokens(parser,'e', ' ', ' ', NULL, 0, "");
			if(*parser->LLT->self->type==RPAREN){
				advanceParser(parser);
				return ret;
			}
			printf("Expected \')\' at line: %d, column:%d. Got %s\n", *parser->LLT->self->ln_start, *parser->LLT->self->col_start,parser->LLT->self->expr);
			exit(1);
		}
		else if(tt==IDENTIFIER){
			VarAccess* va=malloc(sizeof(VarAccess));
			va->token=malloc(sizeof(Token));
			*va->token=*parser->LLT->self;
			advanceParser(parser);
			NodeWrapper* nw=malloc(sizeof(NodeWrapper));
			nw->var_access=va;
			return(initNode(VAR_ACCESS_NODE, nw));
		}
		else if(tt==KEYWORD){
			if(strcmp("if", parser->LLT->self->expr)==0){
				return parseTokens(parser, 'i', ' ', ' ', NULL, 0, "");
			}
			else if(strcmp("for", parser->LLT->self->expr)==0){
				return parseTokens(parser, 'f', ' ', ' ', NULL, 0, "");
			}
			else if(strcmp("while", parser->LLT->self->expr)==0){
				return parseTokens(parser, 'w', ' ', ' ', NULL, 0, "");
			}
			else if(strcmp("func", parser->LLT->self->expr)==0){
				return parseTokens(parser, 'F', ' ', ' ', NULL, 0, "");
			}

		}
		printf("Invalid token:%s at line:%d, column:%d\n",parser->LLT->self->expr,*parser->LLT->self->ln_start, *parser->LLT->self->col_start);
		exit(1);
	}
	if(path=='e'){
		TokType tt=*parser->LLT->self->type;
		if(tt==KEYWORD){
			if(strcmp("not", parser->LLT->self->expr)==0){
				Unop* un=malloc(sizeof(Unop));
				un->token=malloc(sizeof(Token));
				*un->token=*parser->LLT->self;
				advanceParser(parser);
				Node* expr=parseTokens(parser,'e', ' ', ' ', NULL, 0, "");
				un->node=(struct Node*)expr;
				NodeWrapper* nw=malloc(sizeof(NodeWrapper));
				nw->unop=un;
				return initNode(UN_OP_NODE, nw);
			}
			else if(strcmp("let", parser->LLT->self->expr)==0){
				VarAssign* va=malloc(sizeof(VarAssign));
				advanceParser(parser);
				va->token=malloc(sizeof(Token));
				if(*parser->LLT->self->type!=IDENTIFIER){
					printf("Invalid token:%s at line:%d, column:%d, expected identifier\n",parser->LLT->self->expr,*parser->LLT->self->ln_start, *parser->LLT->self->col_start);
					exit(1);
				}
				*va->token=*parser->LLT->self;
				advanceParser(parser);
				va->token=malloc(sizeof(Token));
				if(*parser->LLT->self->type!=EQ&&*parser->LLT->self->type!=LARROW){
					printf("Invalid token:%s at line:%d, column:%d, expected '=' or '<-'\n",parser->LLT->self->expr,*parser->LLT->self->ln_start, *parser->LLT->self->col_start);
					exit(1);
				}
				advanceParser(parser);
				Node* expr=parseTokens(parser,'e', ' ', ' ', NULL, 0, "");
				NodeWrapper* nw=malloc(sizeof(NodeWrapper));
				va->node=(struct Node*)expr;
				nw->var_assign=va;
				return(initNode(VAR_ASSIGN_NODE, nw));
			}
		}
	}
	if(path=='w'){
		
	}
	if(path=='f'){

	}
	if(path=='F'){

	}
	if(path=='i'){

	}
	if(path=='b'){
		Node* left=parseTokens(parser, p1, ' ', ' ', NULL, 0, "");
		int c=1;
		while(c){
			int p=0;
			for(int i=0; i<tokc; i++){
				if(*parser->LLT->self->type==tType[i]) p=1;
			}
			if(*parser->LLT->self->type==TEOF) return left;
			if(!p) return left;
			Binop* bin=malloc(sizeof(Binop));
			bin->token=malloc(sizeof(Token));
			*bin->token=*parser->LLT->self;
			advanceParser(parser);
			Node* right=parseTokens(parser, p2, ' ', ' ', NULL, 0, "");
			bin->left=(struct Node*)left;
			bin->right=(struct Node*)right;
			NodeWrapper* nw=malloc(sizeof(NodeWrapper));
			nw->binop=bin;
			left=initNode(BIN_OP_NODE, nw);
			p=0;
			for(int i=0; i<tokc; i++){
				if(*parser->LLT->self->type==tType[i]) p=1;
			}
			if(!p) c=0;

		}
		return left;
	}
	if(path=='A'){
		TokType* tts=malloc(2*sizeof(TokType));
		tts[0]=PLUS;
		tts[1]=MINUS;
		Node* n=parseTokens(parser, 'b', 't', 't', tts, 2, "");
		free(tts);
		return n;

	}
	if(path=='C'){
		
	}
	if(path=='p'){
		TokType* kw=malloc(sizeof(TokType));
		kw[0]=KEYWORD;
		Node* expr=parseTokens(parser, 'b', 'c', 'f', kw, 1, "pow");
		free(kw);
		return expr;
	}
	if(path=='t'){
		TokType* tts=malloc(2*sizeof(TokType));
		tts[0]=DIV;
		tts[1]=MUL;
		Node* n=parseTokens(parser, 'b', 'f', 'f', tts, 2, "");
		free(tts);
		return n;
	}
	if(path=='f'){
		TokType tt=*parser->LLT->self->type;
		if(tt==PLUS||tt==MINUS){
			Unop* un=malloc(sizeof(Unop));
			un->token=malloc(sizeof(Token));
			*un->token=*parser->LLT->self;
			advanceParser(parser);
			Node* node=parseTokens(parser, 'f' , ' ', ' ', NULL, 0, "");
			un->node=(struct Node*)node;
			NodeWrapper* nw=malloc(sizeof(NodeWrapper));
			nw->unop=un;
			return initNode(UN_OP_NODE, nw);
		}
		return(parseTokens(parser, 'p', ' ', ' ', NULL, 0, ""));
	}
	if(path=='c'){
		Node** args=malloc(1);
		int lenNodes=0;
		Node* atom=parseTokens(parser, 'a',  ' ', ' ', NULL, 0, "");
		if(*parser->LLT->self->type==LPAREN){
			advanceParser(parser);
			if(*parser->LLT->self->type==RPAREN){
				advanceParser(parser);
			} else {
				Node* arg=parseTokens(parser, 'e', ' ', ' ', NULL, 0, "");
				args=realloc(args, (lenNodes+2)*sizeof(Node*));
				lenNodes++;
				args[lenNodes-1]=arg;
				if(*parser->LLT->self->type==COMMA){
					int c=1;
					while(c){
						advanceParser(parser);
						arg=parseTokens(parser, 'e', ' ', ' ', NULL, 0, "");
						args=realloc(args, (lenNodes+2)*sizeof(Node*));
						lenNodes++;
						args[lenNodes-1]=arg;
						if(*parser->LLT->self->type!=COMMA) c=0;
					}
				}
				if(*parser->LLT->self->type!=RPAREN){
					printf("Invalid token:%s at line:%d, column:%d, expected ')'\n",parser->LLT->self->expr,*parser->LLT->self->ln_start, *parser->LLT->self->col_start);
					exit(1);
				}
				advanceParser(parser);
			}
			Call* cn=malloc(sizeof(Call));
			cn->target=(struct Node*)atom;
			cn->args=(struct Node**)args;
			NodeWrapper* nw=malloc(sizeof(NodeWrapper));
			nw->call=cn;
			return initNode(CALL_NODE,nw);
		}
		return atom;
	}
	if(path=='B'){

	}
	return NULL;
	
}


int main() {
	printf("Hellooo\n");
	StrBuf* str=initStrBuf();

	printf("Hellooo\n");
	printf("Hellooo\n");
	writeChar(str,'H');
	printf("Hellooo\n");
	printf("%d\n",matchKw("jump_depth"));
	printf("%d\n",matchKw("jump_deptt"));

	Token* tok=initToken(LCURL, 0, 1, 1, "aaaaa");
	Tokens* toks=initTokenCollection();
	printf("Helloo00o\n");
	appendToken(toks, tok);
	printf("%ld||\n",*toks->len);
	printf("%d\n",*(toks->tokens[0]->type));	
	printf("%d\n%s\n",LCURL,str->ptr);
	writeChars(str,"ello World!");
	printf("%s\n",str->ptr);
	freeStrBuf(str);
	freeTokens(toks);
	Lexer* lex=initLexer("1+1.0\n->1 1\nlet jump aaaaa a90AAa Ak wd_wd w_09 _00");
	printf("Helloo00o\n");
	parseLexer(lex);
	printf("Helloo00o\n");
	printTokens(lex->toks);
	Tokens* a=initTokenCollection();
	appendToken(a, initToken(FLOAT,0,0,0,"1.0"));
	Node* no=parseTokens(initParser(a), 'a', ' ', ' ', NULL, 0, "");
	printToken(no->node->number->token);

}
