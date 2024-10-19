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
#define numKw 19
char* keywords[]={
	"if", 		//if
	"else", 	//ee
	"elif", 	//ef
	"jump_if",	//jf
	"jump", 	//jp
	"func", 	//fc
	"not", 		//nt
	"malloc",	//mc
	"free",		//fe
	"and", 		//ad
	"or", 		//or
	"xor", 		//xr
	"pow",		//pw
	"let",	 	//lt
	"jump_depth",	//jh
	"continue",	//ce
	"return",	//rn
    	"for",		//fr
    	"while"		//we
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
	CALL_NODE
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
	int* casenum;
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
		if((*parser->LLT->self->type)!=SEMICOLON){
			printf("Invalid token:%s at line:%d, column:%d, expected ';'\n",parser->LLT->self->expr,*parser->LLT->self->ln_start, *parser->LLT->self->col_start);
			exit(1);
		}
		advanceParser(parser);
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
			else if(strcmp("func", parser->LLT->self->expr)==0){
				return parseTokens(parser, 'D', ' ', ' ', NULL, 0, "");
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
			else if(strcmp("jump", parser->LLT->self->expr)==0){
				Node* node=parseTokens(parser, 'J', ' ', ' ', NULL, 0, "");
				return node;
			}
			else if(strcmp("jump_if", parser->LLT->self->expr)==0){
				Node* node=parseTokens(parser, 'j', ' ', ' ', NULL, 0, "");
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
		if(strcmp(parser->LLT->self->expr, "let")!=0){
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
			printf("Invalid token:%s at line:%d, column:%d, expected ')'\n",parser->LLT->self->expr,*parser->LLT->self->ln_start, *parser->LLT->self->col_start);
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
				if(*parser->LLT->self->type==IDENTIFIER){
					ArgSet* arg=malloc(sizeof(ArgSet));
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
				else{
						printf("Invalid token:%s at line:%d, column:%d, expected Identifier\n",parser->LLT->self->expr,*parser->LLT->self->ln_start, *parser->LLT->self->col_start);
						exit(1);

				}
			}
			

		}
		advanceParser(parser);
		Node* body=parseTokens(parser, 'B',  ' ', ' ', NULL, 0, "");
		FuncDef* fd=malloc(sizeof(FuncDef));
		fd->body=(struct  Node*)body;
		fd->args=args;
		fd->var_name=funcName;
		NodeWrapper* nw=malloc(sizeof(NodeWrapper));
		nw->func_def=fd;
		return initNode(FUNC_DEF_NODE, nw);
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
		Node* atom=parseTokens(parser, 'a',  ' ', ' ', NULL, 0, "");
		if(*parser->LLT->self->type==LPAREN){
			Node** args=malloc(1);
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
	Lexer* lex=initLexer("2*(1+1);\nfor(let a<-0->15:1){\na;\n};\nwhile(true){\nlet b=b+1;\n};\n::aaa;\njump aaa;\njump_if(1==1) aaa;");
	printf("Helloo00o\n");
	parseLexer(lex);
	printf("Helloo00o\n");
	printTokens(lex->toks);
	Parser* par=initParser(lex->toks);

	Node* no=parseTokens(par, 'P', ' ', ' ', NULL, 0, "isRoot");
	printNode(par->ast->roots[0]);

}
