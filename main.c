#include <math.h>
#include <stddef.h>
#include <dirent.h>
#include <unistd.h>
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
#define numKw 14	//22
char* keywords[]={
	"if",	 	//if
	"else", 	//ee
	"elif", 	//ef
	"jump_if",	//jf
	"jump", 	//jp
	"int",		//it
	"float",	//ft
	"func", 	//fc
	"not", 		//nt
	//"malloc",	//mc
	//"free",	//fe
	//"and", 	//ad
	//"or", 	//or
	//"xor", 	//xr
	//"pow",	//pw
	"jump_depth",	//jh
	//"continue",	//ce
	"return",	//rn
    	"for",		//fr
    	"while",	//we
	//"println",	//pn
	"print"		//pt
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
char* ttfe(TokType tt){
	switch (tt) {
		case IDENTIFIER:
			return "IDENTIFIER";
		case KEYWORD:
			return "KEYWORD";
		case PLUS:
			return "PLUS";
		case INCREMENT:
			return "INCREMENT";
		case MINUS:
			return "MINUS";
		case DECREMENT:
			return "DECREMENT";
		case MUL:
			return "MUL";
		case DIV:
			return "DIV";
		case INTEGER:
			return "INTEGER";
		case FLOAT:
			return "FLOAT";
		case LPAREN:
			return "LPAREN";
		case RPAREN:
			return "RPAREN";
		case LBRACKET:
			return "LBRACKET";
		case RBRACKET:
			return "RBRACKET";
		case LCURL:
			return "LCURL";
		case RCURL:
			return "RCURL";
		case FLAG:
			return "FLAG";
		case COMMA:
			return "COMMA";
		case COLON:
			return "COLON";
		case SEMICOLON:
			return "SEMICOLON";
		case DOT:
			return "DOT";
		case LARROW:
			return "LARROW";
		case RARROW:
			return "RARROW";
		case EQ:
			return "EQ";
		case EE:
			return "EE";
		case GT:
			return "GT";
		case GTE:
			return "GTE";
		case LT:
			return "LT";
		case LTE:
			return "LTE";
		case NE:
			return "NE";
		case AND:
			return "AND";
		case OR:
			return "OR";
		case XOR:
			return "XOR";
		case NOT:
			return "NOT";
		case NEWLINE:
			return "NEWLINE";
		case TEOF:
			return "TEOF";
	}
}
void printToken(Token* ptr){
	printf("[Type:%s, Val:'%s'; Line:%d, Column:%d]",ttfe(*ptr->type),ptr->expr,*ptr->ln_start,*ptr->col_start);
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
		switch(*lexer->current){
			case '\n':
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
			case '\t':
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
	NUMBER_NODE,
	CURLY_NODE,
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
	CALL_NODE,
	PRINT_NODE,
	RETURN_NODE
} NodeType;
typedef struct{Token* token;} Number;
typedef struct{
	struct Node* left;
	Token* token; 
	struct Node* right;
} Binop;
typedef struct{
	struct Node** nodes;
	int* len;
} CurlyBlock;
typedef struct{
	struct Node* node;
	Token* token;
} Unop;
typedef struct{
	struct Node* node;
	Token* token;
	Type* type;
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
	int* casenum;
	struct Node* elsecase;
} IfNode;
typedef struct{
	struct Node* expr;
} PrintNode;
typedef struct{
	struct Node* expr;
} ReturnNode;
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
	Type* type;
} ArgSet;
typedef struct{
	Token* var_name;
	ArgSet** args;
	int* argc;
	Type* return_type;
	struct Node* body;
} FuncDef;
typedef struct{
	Token* target;
	struct Node** args;
	int* argc;
} Call;
typedef union{
	CurlyBlock* curly_block;
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
	PrintNode* print;
	ReturnNode* return_node;
} NodeWrapper;
typedef struct{
	NodeWrapper* node;
	NodeType* type;
} Node;
void printNode(Node* node){
	switch (*node->type) {
		case NUMBER_NODE:
			printf("{NumberNode|Token:");
			printToken(node->node->number->token);
			printf("}");
			break;
		case CURLY_NODE:
			printf("{CurlyNode|NumNodes:%d|Nodes:(",*node->node->curly_block->len);
			for(int i=0; i<*node->node->curly_block->len;i++){
				if(i!=0) printf(",");
				printNode((Node*)node->node->curly_block->nodes[i]);
			}
			printf(")}");
			break;
		case TYPE_NODE:
			break;
		case TYPECHAIN_NODE:
			break;
		case BIN_OP_NODE:
			printf("{BinopNode|Left:");
			printNode((Node*)node->node->binop->left);
			printf("|Token:");
			printToken(node->node->binop->token);
			printf("|Right:");
			printNode((Node*)node->node->binop->right);
			printf("}");
			break;
		case UN_OP_NODE:
			printf("{UnopNode|Token:");
			printToken(node->node->unop->token);
			printf("|Node:");
			printNode((Node*)node->node->unop->node);
			printf("}");			
			break;
		case VAR_ASSIGN_NODE:
			printf("{VarAssignNode|Node:");
			printNode((Node*)node->node->var_assign->node);
			printf("|Token:");
			printToken(node->node->var_assign->token);
			printf("}");
			break;
		case VAR_ACCESS_NODE:
			printf("{VarAccessNode|Token:");
			printToken(node->node->var_access->token);
			printf("}");
			break;
		case IF_NODE:
			break;
		case FOR_NODE:
			break;
		case WHILE_NODE:
			break;
		case FLAG_NODE:
			printf("{FlagNode|Flag Name:");
			printToken(node->node->flag->flag_name);
			printf("}");
			break;
		case JUMP_NODE:
			printf("{JumpNode|Flag Name:");
			printToken(node->node->jump->flag_name);
			printf("}");
			break;
		case JUMP_IF_NODE:
			break;
		case FUNC_DEF_NODE:
			break;
		case CALL_NODE:
			break;
		default:
			break;
	
	}
}
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
Node* parseTokens(Parser* parser, char path, char p1, char p2, TokType* tType, int tokc, char* tExpr){
	if(path=='P'){	// Parse
		Node* node=parseTokens(parser, 'e', ' ', ' ', NULL, 0, "");
		if((*parser->LLT->self->type)!=SEMICOLON&&*((LinkedListToken *)parser->LLT->prev)->self->type!=RCURL){
			
			printf("P:Invalid token:%s at line:%d, column:%d, expected ';'\n",parser->LLT->self->expr,*parser->LLT->self->ln_start, *parser->LLT->self->col_start);
			exit(1);
		}
		if(*parser->LLT->self->type==SEMICOLON)advanceParser(parser);
		if(strcmp(tExpr,"isRoot")==0){
			AddRoot(parser->ast, node);
			if((*parser->LLT->self->type)!=TEOF){
				parseTokens(parser,'P',' ',' ',NULL,0,"isRoot");
			}
			return node;	
		}
		return node;	
	}
	if(path=='a'){	// Atom
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
			if(*((LinkedListToken*)parser->LLT->next)->self->type==LPAREN) return parseTokens(parser, 'c', ' ', ' ', NULL, 0, "");
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
				return parseTokens(parser, 'F', ' ', ' ', NULL, 0, "");
			}
			else if(strcmp("while", parser->LLT->self->expr)==0){
				return parseTokens(parser, 'w', ' ', ' ', NULL, 0, "");
			}			
		}
		printf("Invalid token:%s at line:%d, column:%d\n",parser->LLT->self->expr,*parser->LLT->self->ln_start, *parser->LLT->self->col_start);
		exit(1);
	}
	if(path=='e'){	// Expr
		TokType tt=*parser->LLT->self->type;
		if(tt==FLAG){
			Node* flag=parseTokens(parser, 'L', ' ', ' ', NULL, 0, "");
			return flag;
		}
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
			else if(strcmp("int", parser->LLT->self->expr)==0){
				VarAssign* va=malloc(sizeof(VarAssign));
				advanceParser(parser);
				va->token=malloc(sizeof(Token));
				if(*parser->LLT->self->type!=IDENTIFIER){
					printf("Invalid token:%s at line:%d, column:%d, expected identifier\n",parser->LLT->self->expr,*parser->LLT->self->ln_start, *parser->LLT->self->col_start);
					exit(1);
				}
				*va->token=*parser->LLT->self;
				advanceParser(parser);
				if(*parser->LLT->self->type!=EQ&&*parser->LLT->self->type!=LARROW){
					printf("Invalid token:%s at line:%d, column:%d, expected '=' or '<-'\n",parser->LLT->self->expr,*parser->LLT->self->ln_start, *parser->LLT->self->col_start);
					exit(1);
				}
				advanceParser(parser);
				Node* expr=parseTokens(parser,'e', ' ', ' ', NULL, 0, "");
				NodeWrapper* nw=malloc(sizeof(NodeWrapper));
				va->node=(struct Node*)expr;
				va->type=malloc(sizeof(Type));
				*va->type=Integer;
				nw->var_assign=va;
				return(initNode(VAR_ASSIGN_NODE, nw));
			}
			else if(strcmp("float", parser->LLT->self->expr)==0){
				VarAssign* va=malloc(sizeof(VarAssign));
				advanceParser(parser);
				va->token=malloc(sizeof(Token));
				if(*parser->LLT->self->type!=IDENTIFIER){
					printf("Invalid token:%s at line:%d, column:%d, expected identifier\n",parser->LLT->self->expr,*parser->LLT->self->ln_start, *parser->LLT->self->col_start);
					exit(1);
				}
				*va->token=*parser->LLT->self;
				advanceParser(parser);
				if(*parser->LLT->self->type!=EQ&&*parser->LLT->self->type!=LARROW){
					printf("Invalid token:%s at line:%d, column:%d, expected '=' or '<-'\n",parser->LLT->self->expr,*parser->LLT->self->ln_start, *parser->LLT->self->col_start);
					exit(1);
				}
				advanceParser(parser);
				Node* expr=parseTokens(parser,'e', ' ', ' ', NULL, 0, "");
				NodeWrapper* nw=malloc(sizeof(NodeWrapper));
				va->node=(struct Node*)expr;
				va->type=malloc(sizeof(Type));
				*va->type=Float;
				nw->var_assign=va;
				return(initNode(VAR_ASSIGN_NODE, nw));
			}
			else if(strcmp("func", parser->LLT->self->expr)==0){
				return parseTokens(parser, 'D', ' ', ' ', NULL, 0, "");
			}
			else if(strcmp("jump", parser->LLT->self->expr)==0){
				Node* node=parseTokens(parser, 'J', ' ', ' ', NULL, 0, "");
				return node;
			}
			else if(strcmp("jump_if", parser->LLT->self->expr)==0){
				Node* node=parseTokens(parser, 'j', ' ', ' ', NULL, 0, "");
				return node;
			}
			else if(strcmp("print", parser->LLT->self->expr)==0){
				Node* node=parseTokens(parser, 'R', ' ', ' ', NULL, 0, "");
				return node;
			}
			else if(strcmp("return", parser->LLT->self->expr)==0){
				Node* node=parseTokens(parser, 'Q', ' ', ' ', NULL, 0, "");
				return node;
			}
		}
		TokType* tts=malloc(sizeof(TokType)*3);
		tts[0]=AND;
		tts[1]=OR;
		tts[2]=XOR;
		Node* node=parseTokens(parser, 'b', 'C', 'C', tts, 3, "");
		free(tts);
		return node;

	}
	if(path=='w'){	// While
		WhileNode* wn=malloc(sizeof(WhileNode));
		advanceParser(parser);
		if(*parser->LLT->self->type!=LPAREN){
			printf("Invalid token:%s at line:%d, column:%d, expected '('\n",parser->LLT->self->expr,*parser->LLT->self->ln_start, *parser->LLT->self->col_start);
			exit(1);
		}
		advanceParser(parser);
		wn->condition=(struct Node*)parseTokens(parser, 'e', ' ', ' ', NULL, 0, "");
		if(*parser->LLT->self->type!=RPAREN){
			printf("Invalid token:%s at line:%d, column:%d, expected ')'\n",parser->LLT->self->expr,*parser->LLT->self->ln_start, *parser->LLT->self->col_start);
			exit(1);
		}
		advanceParser(parser);
		wn->body=(struct Node*)parseTokens(parser, 'B', ' ', ' ', NULL, 0, "");
		NodeWrapper* nw=malloc(sizeof(NodeWrapper));
		nw->while_node=wn;
		return initNode(WHILE_NODE, nw);


	}
	if(path=='F'){	// For
		ForNode* fn=malloc(sizeof(ForNode));
		advanceParser(parser);
		if(*parser->LLT->self->type!=LPAREN){
			printf("Invalid token:%s at line:%d, column:%d, expected '('\n",parser->LLT->self->expr,*parser->LLT->self->ln_start, *parser->LLT->self->col_start);
			exit(1);
		}
		advanceParser(parser);
		if(strcmp(parser->LLT->self->expr, "int")!=0){
			printf("Invalid token:%s at line:%d, column:%d, expected 'let'\n",parser->LLT->self->expr,*parser->LLT->self->ln_start, *parser->LLT->self->col_start);
			exit(1);
		}
		advanceParser(parser);
		if(*parser->LLT->self->type!=IDENTIFIER){
			printf("Invalid token:%s at line:%d, column:%d, expected identifier\n",parser->LLT->self->expr,*parser->LLT->self->ln_start, *parser->LLT->self->col_start);
			exit(1);
		}
		fn->variable=malloc(sizeof(Token));
		*fn->variable=*parser->LLT->self;
		advanceParser(parser);
		if(*parser->LLT->self->type!=EQ&&*parser->LLT->self->type!=LARROW){
			printf("Invalid token:%s at line:%d, column:%d, expected '=' or '<-'\n",parser->LLT->self->expr,*parser->LLT->self->ln_start, *parser->LLT->self->col_start);
			exit(1);
		}
		advanceParser(parser);
		fn->start=(struct Node*)parseTokens(parser, 'e', ' ', ' ', NULL, 0, "");
		if(*parser->LLT->self->type!=RARROW){
			printf("Invalid token:%s at line:%d, column:%d, expected '->'\n",parser->LLT->self->expr,*parser->LLT->self->ln_start, *parser->LLT->self->col_start);
			exit(1);
		}
		advanceParser(parser);
		fn->end=(struct Node*)parseTokens(parser, 'e', ' ', ' ', NULL, 0, "");
		if(*parser->LLT->self->type!=COLON){
			printf("Invalid token:%s at line:%d, column:%d, expected ':'\n",parser->LLT->self->expr,*parser->LLT->self->ln_start, *parser->LLT->self->col_start);
			exit(1);
		}
		advanceParser(parser);
		fn->step=(struct Node*)parseTokens(parser, 'e', ' ', ' ', NULL, 0, "");
		if(*parser->LLT->self->type!=RPAREN){
			printf("Invalid token:%s at line:%d, column#define MAX 255:%d, expected ')'\n",parser->LLT->self->expr,*parser->LLT->self->ln_start, *parser->LLT->self->col_start);
			exit(1);
		}
		advanceParser(parser);
		fn->body=(struct Node*)parseTokens(parser, 'B', ' ', ' ', NULL, 0, "");
		NodeWrapper* nw=malloc(sizeof(NodeWrapper));
		nw->for_node=fn;
		return initNode(FOR_NODE, nw);
	}
	if(path=='D'){	// Function Def
		Token* funcName;
		ArgSet** args=malloc(1);
		int argLen=0;
		advanceParser(parser);
		FuncDef* fd=malloc(sizeof(FuncDef));
		funcName=parser->LLT->self;
		advanceParser(parser);
		if(*parser->LLT->self->type!=LPAREN){
			printf("Invalid token:%s at line:%d, column:%d, expected ')'\n",parser->LLT->self->expr,*parser->LLT->self->ln_start, *parser->LLT->self->col_start);
			exit(1);
		}
		advanceParser(parser);
		if(*parser->LLT->self->type!=RPAREN){
			int p=1;
			while(p){
				if(*parser->LLT->self->type==KEYWORD){
					char* tExpr=parser->LLT->self->expr;
					if(strcmp(tExpr,"int")==0){
						advanceParser(parser);
						if(*parser->LLT->self->type!=IDENTIFIER){
							printf("Invalid token:%s at line:%d, column:%d, expected Identifier\n",parser->LLT->self->expr,*parser->LLT->self->ln_start, *parser->LLT->self->col_start);
							exit(1);
						}
						ArgSet* arg=malloc(sizeof(ArgSet));
						arg->type=malloc(sizeof(Type));
						*arg->type=Integer;
						arg->name=malloc(sizeof(Token));
						*arg->name=*parser->LLT->self;
						args=realloc(args, ((argLen)+2)*sizeof(ArgSet*));
						args[argLen]=arg;
						argLen+=1;
						advanceParser(parser);
						if(*parser->LLT->self->type==RPAREN) p=0;
						else if(*parser->LLT->self->type==COMMA) advanceParser(parser);
						else {
							printf("Invalid token:%s at line:%d, column:%d, expected ',' or ')'\n",parser->LLT->self->expr,*parser->LLT->self->ln_start, *parser->LLT->self->col_start);
							exit(1);
						}
					}
					else if(strcmp("float",tExpr)==0){
						advanceParser(parser);
						if(*parser->LLT->self->type!=IDENTIFIER){
							printf("Invalid token:%s at line:%d, column:%d, expected Identifier\n",parser->LLT->self->expr,*parser->LLT->self->ln_start, *parser->LLT->self->col_start);
							exit(1);
						}
						ArgSet* arg=malloc(sizeof(ArgSet));
						arg->type=malloc(sizeof(Type));
						*arg->type=Float;
						arg->name=malloc(sizeof(Token));
						*arg->name=*parser->LLT->self;
						args=realloc(args, ((argLen)+2)*sizeof(ArgSet*));
						args[argLen]=arg;
						argLen+=1;
						advanceParser(parser);
						if(*parser->LLT->self->type==RPAREN) p=0;
						else if(*parser->LLT->self->type==COMMA) advanceParser(parser);
						else {
							printf("Invalid token:%s at line:%d, column:%d, expected ',' or ')'\n",parser->LLT->self->expr,*parser->LLT->self->ln_start, *parser->LLT->self->col_start);
							exit(1);
						}
					}
				}
				else{
						printf("Invalid token:%s at line:%d, column:%d, expected 'int' or float'\n",parser->LLT->self->expr,*parser->LLT->self->ln_start, *parser->LLT->self->col_start);
						exit(1);

				}
			}
			

		}
		advanceParser(parser);
		int g=1;
		fd->return_type=malloc(sizeof(Type));
		if(*parser->LLT->self->type==KEYWORD){
			if(strcmp(parser->LLT->self->expr,"int")==0){
				g=0;
				*fd->return_type=Integer;
			}
			if(strcmp(parser->LLT->self->expr,"float")==0){
				g=0;
				*fd->return_type=Float;
			}
		}
		if(g){
			printf("Invalid token:%s at line:%d, column:%d, expected 'int' or float' as return type\n",parser->LLT->self->expr,*parser->LLT->self->ln_start, *parser->LLT->self->col_start);
			exit(1);
		}
		advanceParser(parser);
		Node* body=parseTokens(parser, 'B',  ' ', ' ', NULL, 0, "");
		fd->argc=malloc(sizeof(int));
		*fd->argc=argLen;
		fd->body=(struct  Node*)body;
		fd->args=args;
		fd->var_name=funcName;
		NodeWrapper* nw=malloc(sizeof(NodeWrapper));
		nw->func_def=fd;
		return initNode(FUNC_DEF_NODE, nw);
	}
	if(path=='R'){
		advanceParser(parser);
		if(*parser->LLT->self->type!=LPAREN){
			printf("Invalid token:%s at line:%d, column:%d, expected '('\n",parser->LLT->self->expr,*parser->LLT->self->ln_start, *parser->LLT->self->col_start);
			exit(1);
		}
		advanceParser(parser);
		Node* body=parseTokens(parser, 'e', ' ', ' ', NULL, 0, "");
		if(*parser->LLT->self->type!=RPAREN){
			printf("Invalid token:%s at line:%d, column:%d, expected ')'\n",parser->LLT->self->expr,*parser->LLT->self->ln_start, *parser->LLT->self->col_start);
			exit(1);
		}
		advanceParser(parser);
		PrintNode* pn=malloc(sizeof(PrintNode));
		pn->expr=(struct Node*)body;
		NodeWrapper* nw=malloc(sizeof(NodeWrapper));
		nw->print=pn;
		return initNode(PRINT_NODE, nw);
	}
	if(path=='Q'){ // Return node
		advanceParser(parser);
		if(*parser->LLT->self->type!=LPAREN){
			printf("Invalid token:%s at line:%d, column:%d, expected '('\n",parser->LLT->self->expr,*parser->LLT->self->ln_start, *parser->LLT->self->col_start);
			exit(1);
		}
		advanceParser(parser);
		Node* body=parseTokens(parser, 'e', ' ', ' ', NULL, 0, "");
		if(*parser->LLT->self->type!=RPAREN){
			printf("Invalid token:%s at line:%d, column:%d, expected ')'\n",parser->LLT->self->expr,*parser->LLT->self->ln_start, *parser->LLT->self->col_start);
			exit(1);
		}
		advanceParser(parser);
		ReturnNode* rn=malloc(sizeof(ReturnNode));
		rn->expr=(struct Node*)body;
		NodeWrapper* nw=malloc(sizeof(NodeWrapper));
		nw->return_node=rn;
		return initNode(RETURN_NODE, nw);
	}

	if(path=='i'){	// If Expr
		IfSet** cases=malloc(1);
		int nSets=0;
		if(strcmp(parser->LLT->self->expr, "if")!=0){
			printf("Invalid token:%s at line:%d, column:%d, expected 'if'\n",parser->LLT->self->expr,*parser->LLT->self->ln_start, *parser->LLT->self->col_start);
			exit(1);
		}
		int p=1;
		advanceParser(parser);		
		IfSet* set=malloc(sizeof(IfSet));
		set->condition=(struct Node*)parseTokens(parser, 'e', ' ', ' ', NULL, 0, "");
		set->expression=(struct Node*)parseTokens(parser, 'B', ' ', ' ', NULL, 0, "");
		cases=realloc(cases, ((nSets)+2)*sizeof(IfSet*));
		cases[nSets]=set;
		nSets++;
		if(strcmp(parser->LLT->self->expr,"elif")!=0){
			p=0;
		}
		while(p){
			advanceParser(parser);		
			IfSet* set=malloc(sizeof(IfSet));
			set->condition=(struct Node*)parseTokens(parser, 'e', ' ', ' ', NULL, 0, "");
			set->expression=(struct Node*)parseTokens(parser, 'B', ' ', ' ', NULL, 0, "");
			cases=realloc(cases, ((nSets)+2)*sizeof(IfSet*));
			cases[nSets]=set;
			nSets++;
			if(strcmp(parser->LLT->self->expr,"elif")!=0){
				p=0;
			}
		}
		if(strcmp(parser->LLT->self->expr,"else")!=0){
			printf("Invalid token:%s at line:%d, column:%d, expected 'else' or 'elif'\n",parser->LLT->self->expr,*parser->LLT->self->ln_start, *parser->LLT->self->col_start);
			exit(1);
		}
		advanceParser(parser);
		Node* elsecase=parseTokens(parser, 'B', ' ', ' ', NULL, 0, "");

		IfNode* in=malloc(sizeof(IfNode));
		in->cases=cases;
		in->elsecase=(struct Node*)elsecase;
		in->casenum=malloc(sizeof(int));
		*in->casenum=nSets;
		NodeWrapper* nw=malloc(sizeof(NodeWrapper));
		nw->if_node=in;
		return initNode(IF_NODE, nw);
	}
	if(path=='J'){ 	// Jump
		advanceParser(parser);
		Jump* jump=malloc(sizeof(Jump));
		if(*parser->LLT->self->type!=IDENTIFIER){
			printf("Invalid token:%s at line:%d, column:%d, expected Identifier\n",parser->LLT->self->expr,*parser->LLT->self->ln_start,*parser->LLT->self->col_start);
			exit(1);
		}
		jump->flag_name=malloc(sizeof(Token));
		*jump->flag_name=*parser->LLT->self;
		advanceParser(parser);
		NodeWrapper* nw=malloc(sizeof(NodeWrapper));
		nw->jump=jump;
		return initNode(JUMP_NODE,nw);

	}
	if(path=='j'){	// Jump If
		advanceParser(parser);
		JumpIf* jumpi=malloc(sizeof(JumpIf));
		if(*parser->LLT->self->type!=LPAREN){
			printf("Invalid token:%s at line:%d, column:%d, expected '('\n",parser->LLT->self->expr,*parser->LLT->self->ln_start, *parser->LLT->self->col_start);
			exit(1);
		}
		advanceParser(parser);
		jumpi->condition=(struct Node*)parseTokens(parser, 'e', ' ', ' ', NULL, 0, "");
		if(*parser->LLT->self->type!=RPAREN){
			printf("Invalid token:%s at line:%d, column:%d, expected ')'\n",parser->LLT->self->expr,*parser->LLT->self->ln_start, *parser->LLT->self->col_start);
			exit(1);
		}
		advanceParser(parser);

		if(*parser->LLT->self->type!=IDENTIFIER){
			printf("Invalid token:%s at line:%d, column:%d, expected Identifier\n",parser->LLT->self->expr,*parser->LLT->self->ln_start,*parser->LLT->self->col_start);
			exit(1);
		}
		jumpi->flag_name=malloc(sizeof(Token));
		*jumpi->flag_name=*parser->LLT->self;
		advanceParser(parser);
		NodeWrapper* nw=malloc(sizeof(NodeWrapper));
		nw->jump_if=jumpi;
		return initNode(JUMP_IF_NODE,nw);

	}
	if(path=='L'){	// Flag Def
		advanceParser(parser);
		Flag* flag=malloc(sizeof(Flag));
		if(*parser->LLT->self->type!=IDENTIFIER){
			printf("Invalid token:%s at line:%d, column:%d, expected Identifier\n",parser->LLT->self->expr,*parser->LLT->self->ln_start,*parser->LLT->self->col_start);
			exit(1);
		}
		flag->flag_name=malloc(sizeof(Token));
		*flag->flag_name=*parser->LLT->self;
		advanceParser(parser);
		NodeWrapper* nw=malloc(sizeof(NodeWrapper));
		nw->flag=flag;
		return initNode(FLAG_NODE,nw);
		
	}
	if(path=='b'){	// Binop
		Node* left=parseTokens(parser, p1, ' ', ' ', NULL, 0, "");
		int c=1;
		while(c){
			int p=0;
			for(int i=0; i<tokc; i++){
				if(*parser->LLT->self->type==tType[i]) p=1;
				if(*parser->LLT->self->type==KEYWORD&&*parser->LLT->self->type==tType[i]){
					if(strcmp(parser->LLT->self->expr,tExpr)!=0) p=0;
				}
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
	if(path=='C'){	// Comp Expr
		TokType* tts=malloc(sizeof(TokType)*6);
		tts[0]=GT;
		tts[1]=GTE;
		tts[2]=EE;
		tts[3]=NE;
		tts[4]=LTE;
		tts[5]=LT;
		Node* node=parseTokens(parser, 'b', 'A', 'A', tts, 6, "");
		free(tts);
		return node;
	}
	if(path=='p'){	// Power
		TokType* kw=malloc(sizeof(TokType));
		kw[0]=KEYWORD;
		Node* expr=parseTokens(parser, 'b', 'c', 'f', kw, 1, "pow");
		free(kw);
		return expr;
	}
	if(path=='t'){	// Term
		TokType* tts=malloc(2*sizeof(TokType));
		tts[0]=DIV;
		tts[1]=MUL;
		Node* n=parseTokens(parser, 'b', 'f', 'f', tts, 2, "");
		free(tts);
		return n;
	}
	if(path=='f'){	// Factor
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
	if(path=='c'){	// Call
		int lenNodes=0;
		Call* cn=malloc(sizeof(Call));
		if(*parser->LLT->self->type!=IDENTIFIER==0&&*((LinkedListToken*)parser->LLT->next)->self->type==LPAREN){
			cn->target=malloc(sizeof(Token));
			*cn->target=*parser->LLT->self;
			advanceParser(parser);
		if(*parser->LLT->self->type==LPAREN){
			cn->args=malloc(1);
			advanceParser(parser);
			if(*parser->LLT->self->type==RPAREN){
				advanceParser(parser);
			} else {
				Node* arg=parseTokens(parser, 'e', ' ', ' ', NULL, 0, "");
				cn->args=realloc(cn->args, (lenNodes+2)*sizeof(Node*));
				lenNodes++;
				cn->args[lenNodes-1]=(struct Node*)arg;
				if(*parser->LLT->self->type==COMMA){
					int c=1;
					while(c){
						advanceParser(parser);
						arg=parseTokens(parser, 'e', ' ', ' ', NULL, 0, "");
						cn->args=realloc(cn->args, (lenNodes+2)*sizeof(Node*));
						lenNodes++;
						cn->args[lenNodes-1]=(struct Node*)arg;
						if(*parser->LLT->self->type!=COMMA) c=0;
					}
				}
				if(*parser->LLT->self->type!=RPAREN){
					printf("Invalid token:%s at line:%d, column:%d, expected ')'\n",parser->LLT->self->expr,*parser->LLT->self->ln_start, *parser->LLT->self->col_start);
					exit(1);
				}
				advanceParser(parser);
			}
			cn->argc=malloc(sizeof(int));
			*cn->argc=lenNodes;
			NodeWrapper* nw=malloc(sizeof(NodeWrapper));
			nw->call=cn;
			return initNode(CALL_NODE,nw);
			}
			exit(1);
		}
		return parseTokens(parser, 'a', ' ', ' ', NULL, 0, "");
	}
	if(path=='B'){	// Block
		TokType tt=*parser->LLT->self->type;
		if(tt==LCURL){
			advanceParser(parser);
			CurlyBlock* cNode=malloc(sizeof(CurlyBlock));
			cNode->len=malloc(sizeof(int));
			*cNode->len=0;
			cNode->nodes=malloc(1);
			Node* expr=parseTokens(parser, 'P', ' ', ' ', NULL, 0, "");
			cNode->nodes=realloc(cNode->nodes, ((*cNode->len)+2)*sizeof(struct Node*));
			cNode->nodes[(*cNode->len)]=(struct Node *)expr;
			*cNode->len+=1;
			int p=1;
			if(*parser->LLT->self->type==RCURL){
				p=0;
			}
			while(p){
				Node* expr=parseTokens(parser, 'P', ' ', ' ', NULL, 0, "");
				cNode->nodes=realloc(cNode->nodes, ((*cNode->len)+2)*sizeof(struct Node*));
				cNode->nodes[(*cNode->len)]=(struct Node *)expr;
				*cNode->len+=1;
				if(*parser->LLT->self->type==RCURL){
					p=0;
				}
			}
			advanceParser(parser);
			NodeWrapper* nw=malloc(sizeof(NodeWrapper));
			nw->curly_block=cNode;
			return(initNode(CURLY_NODE, nw));
		}
		printf("Invalid token:%s at line:%d, column:%d, expected '{'\n",parser->LLT->self->expr,*parser->LLT->self->ln_start, *parser->LLT->self->col_start);
		exit(1);
	}
	return NULL;
}
typedef struct{
	Token* name;
	Node* value;
	int* isConstant;
	int* evalsToFloat;
} CTimeVariable;

CTimeVariable* initVar(Node* node){
	CTimeVariable* ctv=malloc(sizeof(CTimeVariable));
	ctv->evalsToFloat=malloc(1);
	*ctv->evalsToFloat=*node->node->var_assign->type==Float;
	ctv->isConstant=malloc(1);
	ctv->name=node->node->var_assign->token;
	ctv->value=(Node*)node->node->var_assign->node;
	return ctv;
}
typedef struct{
	Token* name;
	Node* flag;
} CTimeFlag;
CTimeFlag* initFlag(Node* node){
	CTimeFlag* ctf=malloc(sizeof(CTimeFlag));
	ctf->name=node->node->flag->flag_name;
	ctf->flag=node;
	return ctf;
}
typedef struct{
	Token* name;
	Node* defNode; 
} CTimeFunction;
CTimeFunction* initFunct(Node* node){
	CTimeFunction* ctf=malloc(sizeof(CTimeFunction));
	ctf->name=node->node->func_def->var_name;
	ctf->defNode=node;
	return ctf;
}
typedef struct{
	CTimeVariable** vars;
	int* nvars;
	CTimeFlag** flags;
	int* nflags;
	CTimeFunction** funcs;
	int* nfuncs;
	int* dpth;
	int* nILVarNames;
} CTimeContext;

CTimeContext* goDeeper(CTimeContext* ctx){
	CTimeContext* c=malloc(sizeof(CTimeContext));
	c->vars=malloc(1);
	c->funcs=malloc(1);
	c->flags=malloc(1);
	c->nvars=malloc(sizeof(int));
	*c->nvars=0;
	c->nflags=malloc(sizeof(int));
	*c->nflags=0;
	c->nfuncs=malloc(sizeof(int));
	*c->nfuncs=0;
	c->dpth=malloc(sizeof(int));
	*c->dpth=ctx==NULL?0:*ctx->dpth+1;
	c->nILVarNames=malloc(sizeof(int));
	*c->nILVarNames=ctx==NULL?0:*ctx->nILVarNames;
	return c;	
}
int isConstant(Node* node){
	return 1;	
}
char* constantFold(Node* node){
	return "";	
}

void addVar(CTimeContext* ctx, Node* node){
	ctx->vars=realloc(ctx->vars,(*ctx->nvars+2)*sizeof(CTimeVariable*));
	ctx->vars[*ctx->nvars]=initVar(node);
	*ctx->nvars+=1;
}
void addFlag(CTimeContext* ctx, Node* node){
	ctx->flags=realloc(ctx->flags,(*ctx->nflags+2)*sizeof(CTimeFlag*));
	ctx->flags[*ctx->nflags]=initFlag(node);
	*ctx->nflags+=1;
}
void addFunc(CTimeContext* ctx, Node* node){
	ctx->funcs=realloc(ctx->funcs,(*ctx->nfuncs+2)*sizeof(CTimeFunction*));
	ctx->funcs[*ctx->nfuncs]=initFunct(node);
	*ctx->nfuncs+=1;
}

int flagExists(CTimeContext* ctx, Token* flagname){
	for(int i=0; i<*ctx->nflags;i++){
		if(strcmp(ctx->flags[i]->name->expr,flagname->expr)==0) return 1;
	}
	return 0;
}
int varExists(CTimeContext* ctx, Token* varname){
	for(int i=0; i<*ctx->nvars;i++){
		if(strcmp(ctx->vars[i]->name->expr,varname->expr)==0) return 1;
	}
	return 0;
}
int funcExists(CTimeContext* ctx, Token* funcname){
	for(int i=0; i<*ctx->nfuncs;i++){
		if(strcmp(ctx->funcs[i]->name->expr,funcname->expr)==0) return 1;
	}
	return 0;
}
Node* getVar(CTimeContext* ctx, Token* varname){
	for(int i=0; i<*ctx->nvars;i++){
		if(strcmp(ctx->vars[i]->name->expr,varname->expr)==0) return ctx->vars[i]->value;
	}
	return NULL;
}
int varIsFloat(CTimeContext* ctx, Token* varname){
	for(int i=0; i<*ctx->nvars;i++){
		if(strcmp(ctx->vars[i]->name->expr,varname->expr)==0) return *ctx->vars[i]->evalsToFloat;
	}
	return 0;

}
Node* getFunc(CTimeContext* ctx, Token* funcname){
	for(int i=0; i<*ctx->nfuncs;i++){
		if(strcmp(ctx->funcs[i]->name->expr,funcname->expr)==0) return ctx->funcs[i]->defNode;
	}
	exit(20);
}
int evalsToFloat(CTimeContext* ctx, Node* node){
	if(*node->type==NUMBER_NODE) return *node->node->number->token->type==FLOAT;
	if(*node->type==UN_OP_NODE) return evalsToFloat(ctx,(Node*)node->node->unop->node);
	if(*node->type==VAR_ACCESS_NODE){
		return (varIsFloat(ctx, node->node->var_access->token));
	}
	if(*node->type==CALL_NODE){
		Node* cFunc=getFunc(ctx,node->node->call->target);
		return(*cFunc->node->func_def->return_type==Float);
	}
	if(*node->type==BIN_OP_NODE){
		return (evalsToFloat(ctx,(Node*)node->node->binop->left)||evalsToFloat(ctx,(Node*)node->node->binop->right));
	}

	return 0;
}


typedef struct{
	char* IL;
	char* varName;
} CompilerRetValue;

char* getILFlagNames(CTimeContext* ctx){
	char* a="abcdefghijklmno";
	int p=*ctx->nILVarNames;
	char* retval=malloc(7);
	retval[6]='\0';
	retval[0]='@';
	for(int i=5; i>0;i--){
		retval[i]=a[p%16];
		p=(int)floor((double)p/16.0);
	}
	*ctx->nILVarNames+=1;
	return retval;
}

char* getILVarNames(CTimeContext* ctx){
	char* a="abcdefghijklmno";
	int p=*ctx->nILVarNames;
	char* retval=malloc(7);
	retval[6]='\0';
	retval[0]='%';
	for(int i=5; i>0;i--){
		retval[i]=a[p%16];
		p=(int)floor((double)p/16.0);
	}
	*ctx->nILVarNames+=1;
	return retval;
}
char* BinOpTtHelper(Token* tok,int isFloats){
	TokType tt=*tok->type;
	return 
			(tt==DIV?"div":
			(tt==MUL?"mul":
			 (tt==PLUS?"add":
			  (tt==MINUS?"sub":
			   (tt==AND?"and":
			    (tt==OR?"or":
			     (tt==XOR?"xor":
			      (tt==EE?(isFloats?"ceqd":"ceqw"):
			       (tt==NE?(isFloats?"cned":"cnew"):
				(tt==LT?(isFloats?"cltd":"csltw"):
				 (tt==LTE?(isFloats?"cled":"cslew"):
				  (tt==GT?(isFloats?"cgtd":"csgtw"):
				   (tt==GTE?(isFloats?"cged":"csgew"):"-------")
				  )
				 )
				)
			       )
			      )
			     )
			    )
			   )
			  )
			 )
			)
			);
}
CompilerRetValue* IlFromNode(Node* node, CTimeContext* ctx){
	switch (*node->type) {
		case NUMBER_NODE:
			if(!evalsToFloat(ctx,node)){
				CompilerRetValue* nV=malloc(sizeof(CompilerRetValue));
				nV->varName=node->node->number->token->expr;
				nV->IL="";
				return nV;
			}
			CompilerRetValue* nV=malloc(sizeof(CompilerRetValue));
			nV->varName=malloc(strLen(node->node->number->token->expr)+sizeof("d_"));
			sprintf(nV->varName,"d_%s",node->node->number->token->expr);
			nV->IL="";
			return nV;
			break;
		case CURLY_NODE:
			StrBuf* cA=initStrBuf();
			for(int i=0; i<*node->node->curly_block->len;i++){
				writeChars(cA, IlFromNode((Node*)node->node->curly_block->nodes[i], ctx)->IL);
			}
			CompilerRetValue* cnV=malloc(sizeof(CompilerRetValue));
			cnV->varName="";
			cnV->IL=cA->ptr;
			return cnV;
			break;
		case TYPE_NODE:
			break;
		case TYPECHAIN_NODE:
			break;
		case PRINT_NODE:
			CompilerRetValue* pnExp=IlFromNode((Node*)node->node->print->expr,ctx);
			char* pnVar=getILVarNames(ctx);
			int etf=evalsToFloat(ctx,(Node*)node->node->print->expr);
			CompilerRetValue* pnV=malloc(sizeof(CompilerRetValue));
			pnV->IL=malloc(sizeof("= copy \ncall $printf(l $, ..., )\n")+2+4+strLen(pnVar)*2+strLen(pnExp->varName)+strLen(pnExp->IL));			
			sprintf(pnV->IL,"%s%s=%s copy %s\ncall $printf(l $%s, ...,%s %s)\n",pnExp->IL,pnVar,etf?"d":"w",pnExp->varName,etf?"fmtd":"fmtw",etf?"d":"w",pnVar);
			pnV->varName="";
			return pnV;
			break;
		case BIN_OP_NODE:
			CompilerRetValue* left=IlFromNode((Node*)node->node->binop->left, ctx);
			CompilerRetValue* right=IlFromNode((Node*)node->node->binop->right, ctx);
			CompilerRetValue* bnV=malloc(sizeof(CompilerRetValue));
			bnV->varName=getILVarNames(ctx);
			bnV->IL = malloc(sizeof(" =  , \n")+6+5+1+strLen(left->IL)+strLen(left->varName)+strLen(right->IL)+strLen(right->varName));
			TokType tt=*node->node->binop->token->type;
			sprintf(bnV->IL,"%s%s%s =%s %s %s, %s\n",left->IL,right->IL,bnV->varName,(tt!=EE&&tt!=NE&&tt!=LTE&&tt!=LT&&tt!=GTE&&tt!=GT)&&(evalsToFloat(ctx,(Node*)node->node->binop->left)||evalsToFloat(ctx,(Node*)node->node->binop->right))?"d":"w",BinOpTtHelper(node->node->binop->token,evalsToFloat(ctx,node)),left->varName,right->varName);
			return bnV;	
			break;
		case UN_OP_NODE:
			if(*node->node->unop->token->type==MINUS){
				if(*((Node*)node->node->unop->node)->type==UN_OP_NODE){
					if(*((Node*)node->node->unop->node)->node->unop->token->type==MINUS){
						return IlFromNode((Node*)((Node*)node->node->unop->node)->node->unop->node,ctx);
					}
				}
				if(*((Node*)node->node->unop->node)->type==NUMBER_NODE){
					if(*((Node*)node->node->unop->node)->node->number->token->type==FLOAT){
						CompilerRetValue* unV=malloc(sizeof(CompilerRetValue));
						unV->varName=malloc(strLen(((Node*)node->node->unop->node)->node->number->token->expr)+sizeof("d_-"));
						sprintf(unV->varName,"d_-%s",((Node*)node->node->unop->node)->node->number->token->expr);
						unV->IL="";
						return unV;
					}
					CompilerRetValue* unV=malloc(sizeof(CompilerRetValue));
					unV->varName=malloc(strLen(((Node*)node->node->unop->node)->node->number->token->expr)+sizeof("-"));
					sprintf(unV->varName,"-%s",((Node*)node->node->unop->node)->node->number->token->expr);
					unV->IL="";
					return unV;

				}
				CompilerRetValue* prev=IlFromNode((Node*)node->node->unop->node, ctx);
				CompilerRetValue* unV=malloc(sizeof(CompilerRetValue));
				unV->varName=getILVarNames(ctx);
				unV->IL= malloc(sizeof(" = sub 0, \n")+6+1+strLen(prev->IL)+strLen(prev->varName));
				sprintf(unV->IL,"%s%s =%s sub 0, %s\n",prev->IL,unV->varName,evalsToFloat(ctx,(Node*)node->node->unop->node)?"d":"w",prev->varName);
				return unV;

			}
			if(*node->node->unop->token->type==NOT){
				if(*((Node*)node->node->unop->node)->type==UN_OP_NODE){
					if(*((Node*)node->node->unop->node)->node->unop->token->type==NOT){
						return IlFromNode((Node*)((Node*)node->node->unop->node)->node->unop->node,ctx);
					}
				}
				CompilerRetValue* prev=IlFromNode((Node*)node->node->unop->node, ctx);
				CompilerRetValue* unV=malloc(sizeof(CompilerRetValue));
				unV->varName=getILVarNames(ctx);
				unV->IL= malloc(sizeof(" = not \n")+6+1+strLen(prev->IL)+strLen(prev->varName));
				sprintf(unV->IL,"%s%s =%s not %s\n",prev->IL,unV->varName,evalsToFloat(ctx,(Node*)node->node->unop->node)?"d":"w",prev->varName);
				return unV;

			}		
			break;
		case VAR_ASSIGN_NODE:
			addVar(ctx, node);
			CompilerRetValue* vasExpr=IlFromNode((Node*)node->node->var_assign->node,ctx);
			CompilerRetValue* vasV=malloc(sizeof(CompilerRetValue));
			vasV->varName=malloc(sizeof("%")+strLen(node->node->var_assign->token->expr));
			sprintf(vasV->varName+1,"%s",node->node->var_assign->token->expr);
			vasV->varName[0]='%';
			vasV->IL=malloc(sizeof("= copy \n")+1+strLen(vasV->varName)+strLen(vasExpr->IL)+strLen(vasExpr->varName));
			sprintf(vasV->IL,"%s%s=%s copy %s\n", vasExpr->IL,vasV->varName,*node->node->var_assign->type==Float?"d":"w",vasExpr->varName);
			return vasV;
			break;
		case VAR_ACCESS_NODE:
			CompilerRetValue* vacV=malloc(sizeof(CompilerRetValue));
			vacV->varName=malloc(sizeof("%")+strLen(node->node->var_access->token->expr));
			sprintf(vacV->varName+1, "%s", node->node->var_access->token->expr);
			vacV->varName[0]='%';
			vacV->IL="";
			return vacV;
			printf("{VarAccessNode|Token:");
			printToken(node->node->var_access->token);
			printf("}");
			break;
		case IF_NODE:
			StrBuf* iBody=initStrBuf();
			StrBuf* iIL=initStrBuf();
			StrBuf* iHead=initStrBuf();
			StrBuf* iA=initStrBuf();
			char* iEndFlag=getILFlagNames(ctx);
			for(int i=0; i<*node->node->if_node->casenum;i++){
				CompilerRetValue* ifcCond=IlFromNode((Node*)node->node->if_node->cases[i]->condition, ctx);
				CompilerRetValue* ifcBody=IlFromNode((Node*)node->node->if_node->cases[i]->expression, ctx);
				writeChars(iIL, ifcCond->IL);
				char* ifcFlag=getILFlagNames(ctx);
				char* ifcSkipFlag=getILFlagNames(ctx);
				char* ijmpExpr=malloc(sizeof("jnz ,,\n\n")+strLen(ifcCond->varName)+strLen(ifcFlag)+strLen(ifcSkipFlag)*2);
				sprintf(ijmpExpr,"jnz %s,%s,%s\n%s\n",ifcCond->varName,ifcFlag,ifcSkipFlag,ifcSkipFlag);
				writeChars(iHead, ijmpExpr);
				writeChars(iBody, ifcFlag);
				writeChar(iBody,'\n');
				writeChars(iBody, ifcBody->IL);
				writeChars(iBody,"jmp ");
				writeChars(iBody, iEndFlag);
				writeChar(iBody, '\n');
			}
			CompilerRetValue* iecBody=IlFromNode((Node*)node->node->if_node->elsecase, ctx);
			char* iecFlag=getILFlagNames(ctx);
			char* ijmpExpr=malloc(sizeof("jmp \n")+strLen(iecFlag));
			sprintf(ijmpExpr,"jmp %s\n",iecFlag);
			writeChars(iHead, ijmpExpr);
			writeChars(iBody, iecFlag);
			writeChar(iBody,'\n');
			writeChars(iBody, iecBody->IL);			
			writeChars(iBody, iEndFlag);
			writeChar(iBody, '\n');
			writeChars(iA, iIL->ptr);
			writeChars(iA, iHead->ptr);
			writeChars(iA, iBody->ptr);
			freeStrBuf(iHead);
			freeStrBuf(iIL);
			freeStrBuf(iBody);
			CompilerRetValue* ifV=malloc(sizeof(CompilerRetValue));
			ifV->varName="";
			ifV->IL=iA->ptr;
			return ifV;
			break;
		case FOR_NODE:
			char* flStartFlag=getILFlagNames(ctx);
			char* flEndFlag=getILFlagNames(ctx);
			CompilerRetValue* flV=malloc(sizeof(CompilerRetValue));
			CompilerRetValue* flBody=IlFromNode((Node*)node->node->for_node->body, ctx);
			CompilerRetValue* flIncExpr=IlFromNode((Node*)node->node->for_node->step,ctx);
			CompilerRetValue* flStart=IlFromNode((Node*)node->node->for_node->start,ctx);
			CompilerRetValue* flEnd=IlFromNode((Node*)node->node->for_node->end,ctx);
			char* flVarStr=malloc(strLen(node->node->for_node->variable->expr)+2);
			char* flCond=getILVarNames(ctx);
			sprintf(flVarStr+1,"%s",node->node->for_node->variable->expr);
			flVarStr[0]='%';
			flV->IL=malloc(sizeof("=w copy \n\n=w add , \n=w csltw , \njnz , , \n\n")+strLen(flEndFlag)*2+strLen(flBody->IL)+strLen(flIncExpr->IL)+strLen(flIncExpr->varName)+strLen(flStart->IL)+strLen(flStart->varName)+strLen(flEnd->IL)+strLen(flEnd->varName)+strLen(flStartFlag)*2+strLen(flVarStr)*4+strLen(flCond)*2);
			sprintf(flV->IL,"%s%s%s%s=w copy %s\n%s\n%s%s=w add %s, %s\n%s=w csltw %s, %s\njnz %s, %s, %s\n%s\n",flIncExpr->IL, flStart->IL, flEnd->IL,flVarStr,flStart->varName,flStartFlag,flBody->IL,flVarStr,flVarStr,flIncExpr->varName,flCond,flVarStr,flEnd->varName,flCond,flStartFlag,flEndFlag,flEndFlag);
			flV->varName="";
			return flV;
			break;
		case WHILE_NODE:
			char* wlStartFlag=getILFlagNames(ctx);
			char* wlEndFlag=getILFlagNames(ctx);
			CompilerRetValue* wlV=malloc(sizeof(CompilerRetValue));
			CompilerRetValue* wlBody=IlFromNode((Node*)node->node->while_node->body, ctx);
			CompilerRetValue* wlCond=IlFromNode((Node*)node->node->while_node->condition,ctx);
			wlV->IL=malloc(sizeof("\njnz , , \n\n")+strLen(wlEndFlag)*2+strLen(wlBody->IL)+strLen(wlStartFlag)*2+strLen(wlCond->IL)+strLen(wlCond->varName));
			sprintf(wlV->IL,"%s\n%s%sjnz %s, %s, %s\n%s\n",wlStartFlag,wlBody->IL,wlCond->IL,wlCond->varName,wlStartFlag,wlEndFlag,wlEndFlag);
			wlV->varName="";
			return wlV;

			break;
		case FLAG_NODE:
			CompilerRetValue* fV=malloc(sizeof(CompilerRetValue));
			fV->varName="";
			fV->IL=malloc(sizeof("@\n")+strLen(node->node->flag->flag_name->expr));
			sprintf(fV->IL,"@%s\n",node->node->flag->flag_name->expr);
			addFlag(ctx, node);
			return fV;
			break;
		case JUMP_NODE:
			*ctx->dpth+=1;
			CompilerRetValue* jmV=malloc(sizeof(CompilerRetValue));
			jmV->IL=malloc(sizeof("jmp @\n")+strLen(node->node->jump->flag_name->expr));
			sprintf(jmV->IL,"jmp @%s\n", node->node->jump->flag_name->expr);
			jmV->varName="";
			return jmV;
			break;

			printf("{JumpNode|Flag Name:");
			printToken(node->node->jump->flag_name);
			printf("}");
			break;
		case JUMP_IF_NODE:
			CompilerRetValue* jiCon=IlFromNode((Node*)node->node->jump_if->condition,ctx);
			*ctx->dpth+=1;
			CompilerRetValue* jiV=malloc(sizeof(CompilerRetValue));
			jiV->varName=getILFlagNames(ctx);
			jiV->IL=malloc(sizeof("jnz , @, \n\n")+strLen(jiCon->varName)+strLen(jiCon->IL)+strLen(jiV->varName)*2+strLen(node->node->jump_if->flag_name->expr));
			sprintf(jiV->IL,"%sjnz %s, @%s, %s\n%s\n", jiCon->IL, jiCon->varName, node->node->jump_if->flag_name->expr, jiV->varName, jiV->varName);
			return jiV;
			break;
		case FUNC_DEF_NODE:
			StrBuf* fdA=initStrBuf();
			writeChars(fdA,"export function ");
			addFunc(ctx,node);
			if(*node->node->func_def->return_type==Integer){
				writeChars(fdA,"w $");
			}
			else {
				writeChars(fdA, "d $");
			}
			writeChars(fdA, node->node->func_def->var_name->expr);
			writeChar(fdA, '(');
			int commacheck=0;
			for(int i=0; i<*node->node->func_def->argc;i++){
				if(commacheck){
					writeChars(fdA, ", ");
				}
				else commacheck=1;
				if(*node->node->func_def->args[i]->type==Integer){
					writeChars(fdA,"w %");
				}
				else {
					writeChars(fdA, "d %");
				}
				writeChars(fdA, node->node->func_def->args[i]->name->expr);
			}
			writeChars(fdA, ") {\n@start\n");
			CompilerRetValue* fdeBody=IlFromNode((Node*)node->node->func_def->body, ctx);
			writeChars(fdA, fdeBody->IL);
			CompilerRetValue* fdV=malloc(sizeof(CompilerRetValue));
			writeChars(fdA, "}\n");
			fdV->IL=fdA->ptr;
			fdV->varName="";
			return fdV;
			break;
		case CALL_NODE:
			StrBuf* cnA=initStrBuf();
			if(!funcExists(ctx,node->node->call->target)) exit(100);
			Node* fNode=getFunc(ctx, node->node->call->target);
			char* cnVarName=getILVarNames(ctx);
			

			StrBuf* cnILs=initStrBuf();
			StrBuf* cnCaller=initStrBuf();
			writeChars(cnCaller, cnVarName);
			if(*fNode->node->func_def->return_type==Integer) writeChars(cnCaller, "=w call $");
			else writeChars(cnCaller, "=d call $");
			writeChars(cnCaller, node->node->call->target->expr);
			writeChar(cnCaller, '(');
			int cccheck=0;
			for(int i=0;i<*node->node->call->argc;i++){
				if(cccheck)writeChars(cnCaller, ", ");
				else cccheck=1;
				CompilerRetValue* cnArgIL=IlFromNode((Node*)node->node->call->args[i],ctx);
				writeChars(cnILs, cnArgIL->IL);
				if(*fNode->node->func_def->args[i]->type==Integer) writeChars(cnCaller, "w ");
				else writeChars(cnCaller, "d ");
				writeChars(cnCaller, cnArgIL->varName);
			}
			writeChars(cnCaller, ")\n");
			writeChars(cnA,cnILs->ptr);
			writeChars(cnA, cnCaller->ptr);
			CompilerRetValue* cnVa=malloc(sizeof(CompilerRetValue));
			cnVa->varName=cnVarName;
			cnVa->IL=cnA->ptr;
			return cnVa;
			break;
		case RETURN_NODE:
			CompilerRetValue* rnExp=IlFromNode((Node*)node->node->return_node->expr,ctx);
			char* rnVar=getILVarNames(ctx);
			int rEtf=evalsToFloat(ctx,(Node*)node->node->print->expr);
			CompilerRetValue* rnV=malloc(sizeof(CompilerRetValue));
			rnV->IL=malloc(sizeof("= copy \nret \n")+1+strLen(rnVar)*2+strLen(rnExp->varName)+strLen(rnExp->IL));			
			sprintf(rnV->IL,"%s%s=%s copy %s\nret %s\n",rnExp->IL,rnVar,rEtf?"d":"w",rnExp->varName,rnVar);
			rnV->varName="";
			return rnV;
			break;
		default:
			break;
	
	}
	return NULL;
}
char* CompileAST(Parser* parser){
	StrBuf* strb=initStrBuf();
	CTimeContext* ctx=goDeeper(NULL);
	//writeChars(strb, "export function w $main() {\n@start\n");
	int progmeter=0;
	for(int i=0;i<*parser->ast->len; i++){
		for(int q=0; q<progmeter;q++) printf("\b");
		printf("%d/%lu%n",i+1, *parser->ast->len, &progmeter);
		writeChars(strb, IlFromNode(parser->ast->roots[i], ctx)->IL);
	}
	writeChars(strb, "\ndata $fmtw = { b \"\%d\\n\", b 0 }\ndata $fmtd = { b \"\%lf\\n\", b 0 }");


	return strb->ptr;
}
int main(int argc, char **argv) {
	FILE* progIn;
	StrBuf* prog=initStrBuf();
	progIn=fopen(argv[1], "r");
	char ch;
	while((ch = fgetc(progIn)) != EOF){
		writeChar(prog, ch);
	}
	fclose(progIn);
	Lexer* lex=initLexer(prog->ptr);
	parseLexer(lex);
	Parser* par=initParser(lex->toks);
	parseTokens(par, 'P', ' ', ' ', NULL, 0, "isRoot");
	char* IL=CompileAST(par);
	FILE* progOut;
	int p=0;
	for(int i=0; i<strLen(argv[1]); i++){
		if(argv[1][i]=='.') p=i;
	}
	char* outStr=malloc(p+sizeof(".o"));
	for(int i=0; i<p;i++){
		outStr[i]=argv[1][i];
	}
	outStr[p+2]='\0';
	outStr[p]='.';
	outStr[p+1]='o';
	progOut = fopen(outStr, "w");
	fprintf(progOut,"%s", IL);
	fclose(progOut);

	char wd[512];
	wd[511] = '\0';
	if(getcwd(wd, 511) == NULL) {
		printf ("Can not get current working directory\n");
	}
	char* outStr2=malloc(p+sizeof(".s"));
	for(int i=0; i<p;i++){
		outStr2[i]=argv[1][i];
	}
	outStr2[p+2]='\0';
	outStr2[p]='.';
	outStr2[p+1]='s';
	
	char* outStr3=malloc(p+sizeof(""));
	for(int i=0; i<p;i++){
		outStr3[i]=argv[1][i];
	}
	outStr3[p]='\0';
	char* command1=malloc(sizeof("cd ")+strLen(wd));
	sprintf(command1, "cd %s", wd);
	system(command1);
	char* command2=malloc(sizeof("qbe  -o ")+strLen(outStr)+strLen(outStr2));
	sprintf(command2, "qbe %s -o %s", outStr, outStr2);
	char* command3=malloc(sizeof("cc  -o ")+strLen(outStr2)+strLen(outStr3));
	sprintf(command3, "cc %s -o %s", outStr2, outStr3);
	system(command2);
	system(command3);
	return 0;
}
