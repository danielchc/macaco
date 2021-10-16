#include "lexical_analyzer.h"
#include "input.h"

int lines=0;
const _token_at_t validTokens[]={
	{'<','<',_SHIFTL,{'=',_SHTLEQ}},
	{'>','>',_SHIFTR,{'=',_SHTREQ}},
	{'*','*',_POW,{'=',_POWEQ}},
	{'/','/',_FDIV,{'=',_FDIVEQ}},
	{'-','>',_FUNA,{_NOP}},
	{'<','=',_LEQ,{_NOP}},
	{'>','=',_GEQ,{_NOP}},
	{'=','=',_EQ,{_NOP}},
	{'!','=',_NEQ,{_NOP}},
	{'+','=',_ADDEQ,{_NOP}},
	{'-','=',_SUBEQ,{_NOP}},
	{'*','=',_MULEQ,{_NOP}},
	{'/','=',_DIVEQ,{_NOP}},
	{'%','=',_PEREQ,{_NOP}},
	{'&','=',_IAND,{_NOP}},
	{'|','=',_IOR,{_NOP}},
	{'^','=',_IXOR,{_NOP}},
};


int _is_token(char c){
	return (
		(c == '+') || (c == '-') || (c == '*') || (c == '/') || (c == '%') || 	\
		(c == '@') || (c == '&') || (c == '|') || (c == '^') || (c == '~') ||   \
		(c == '(') || (c == ')') || (c == '[') || (c == ']') || (c == '{') || 	\
		(c == '}') || (c == ',') || (c == ':') || (c == '.') || (c == ';') || 	\
		(c == '=') || (c == '>') || (c == '<')									\
	);
}


numeric_t _numeric_type(){
	previous_char();
	_numeric_at_st state=NAT_UNK;
	numeric_t type=NT_INTEGER;
	char c;
	do{
		c=next_char();
		c=tolower(c);
		switch (state){
			case NAT_ZEROSTART:
				if(c=='x') state=NAT_HEX_1;
				else if(c=='o') state=NAT_OCT_1;
				else if(c=='b') state=NAT_BIN_1;
				else if(c=='e') state=NAT_EXP;
				else if (c>='1' && c<='9') state=NAT_INT;
				else if (c=='.') state=NAT_DEC;
				else return NT_INTEGER;
				break;
			case NAT_INT:
				if (c=='.') state=NAT_DEC;
				else if (c=='e') state=NAT_EXP_UNSIGNED;
				else if (c=='j') state=NAT_IMAGINARY;
				else if (!(c>='0' && c<='9') && (c!='_')) return NT_INTEGER;
				break;
			case NAT_DEC:
				type=NT_DECIMAL;
				if (c=='e') state=NAT_EXP_UNSIGNED;
				else if (c=='j') state=NAT_IMAGINARY;
				else if(!(c>='0' && c<='9') && (c!='_')) return NT_DECIMAL;
				break;
			case NAT_EXP_UNSIGNED:
				if ((c=='-') || (c=='+') || (c>='0' && c<='9')) state=NAT_EXP;
				break;
			case NAT_EXP:
				if (c=='j') state=NAT_IMAGINARY;
				else if (!(c>='0' && c<='9')) return type;
				break;
			case NAT_BIN_1:
				if (!(c=='0' || c=='1' || c=='_')) return NT_ERROR;
				else state=NAT_BIN_2;
				break;
			case NAT_BIN_2:
				if (!(c=='0' || c=='1' || c=='_')) return NT_INTEGER;
				break;
			case NAT_HEX_1:
				if (!((c>='0' && c<='9') || (c>='a' && c<='f'))) return NT_ERROR;
				else state=NAT_HEX_2;
				break;			
			case NAT_HEX_2:
				if (!((c>='0' && c<='9') || (c>='a' && c<='f'))) return NT_INTEGER;
				break;
			case NAT_OCT_1:
				if (!(c>='0' && c<='7')) return NT_ERROR;
				else state=NAT_OCT_2;
				break;			
			case NAT_OCT_2:
				if (!(c>='0' && c<='7')) return NT_INTEGER;
				break;
			case NAT_IMAGINARY:
				return type;
				break;
			case NAT_UNK:
				if(c=='0') state=NAT_ZEROSTART;
				else if (c>='1'&& c<='9') state=NAT_INT;
				else if(c=='.') state=NAT_DEC;
				break;
		}
	}while(c!=EOF);
	return type;
}

quote_t _quote_type(){
	char c;
	_quotes_at_st state=QAT_UNK;
	do{
		c=next_char();
		switch (state){
			case QAT_UNK:
				if (c=='\'') state=QAT_SIMPLE_QUOTE;
				else if (c=='\"') state=QAT_DOUBLE_QUOTE_1;
				break;
			case QAT_SIMPLE_QUOTE:
				if(c=='\'') return QT_STRING;
				break;
			case QAT_DOUBLE_QUOTE_1:
				if(c=='\"')	state=QAT_DOUBLE_QUOTE_2;
				else state= QAT_CONTENT;
				break;
			case QAT_CONTENT:
				if (c=='\"') return QT_STRING;
				break;
			case QAT_DOUBLE_QUOTE_2:
				if(c=='\"') state=QAT_COMMENT;
				else return QT_STRING;
				break;
			case QAT_COMMENT:
				if(c=='\"') state=QAT_DOUBLE_QUOTE_3;
				break;
			case QAT_DOUBLE_QUOTE_3:
				if(c=='\"') state=QAT_DOUBLE_QUOTE_4;
				else state=QAT_COMMENT;
				break;
			case QAT_DOUBLE_QUOTE_4:
				if(c=='\"') return QT_COMMENT;
				else state=QAT_COMMENT;
				break;
		}
		if(c==EOF)break;
	}while(1);
	return QT_ERROR;
}

at_state_t _numeric_at(lexcomp_t* lexcomp){
	//Volvo para atrás para comprobar toda a cadena de novo
	//Chamo a función para saber que tipo de número é
	numeric_t type=_numeric_type();
	// Se o automáta da algún erro devolvo un erro a función principal
	if(type==NT_ERROR){
		printf("errorrrrrrrrrrrr \n");
		return AT_ERROR;
	} 

	previous_char();
	//
	// Copio o compoñente léxico a estrucutura
	strcpy(lexcomp->keyword,get_lexcomp());
	/*
		Se o resultado da función é DECIMAL, garda o tipo decimal na estructura do compoñente léxico, 
		en caso contrario ten que ser un número enteiro
	*/
	lexcomp->value=(type==NT_DECIMAL)?_DECIMAL:_INTEGER;

	//printf("GET %s %d\n",lexcomp->keyword,lexcomp->value);
	// Se chegou ata aquí todo foi ben, entón informo a función principal 

	return AT_OK;
}

at_state_t _quotes_at(lexcomp_t* lexcomp){
	//Volvo para atrás para comprobar toda a cadena de novo
	previous_char();
	//Chamo a función para saber que tipo de número é
	quote_t type=_quote_type();
	// Se o automáta da algún erro devolvo un erro a función principal
	if(type==QT_ERROR) return AT_ERROR;

	// Copio o compoñente léxico a estrucutura
	strcpy(lexcomp->keyword,get_lexcomp());
	/*
		Se o resultado da función é QT_STRING, establezco na estructura que un string, no caso contrario é un comentario
	*/
	lexcomp->value=(type==QT_STRING)?_STRING:_COMMENT;
	// Se chegou ata aquí todo foi ben, entón informo a función principal 
	return AT_OK;
}


at_state_t _alphanumeric_at(lexcomp_t* lexcomp){
	//Volvo para atrás para comprobar toda a cadena de novo
	previous_char();
	char c;
	do{
		c=next_char();
	}while(isalnum(c) || c == '_');

	previous_char();
	// Copio o compoñente léxico a estrucutura
	strcpy(lexcomp->keyword,get_lexcomp());
	//Establezco o tipo de dato na estrucuta
	lexcomp->value=_ID;
	// Se chegou ata aquí todo foi ben, entón informo a función principal
	return AT_OK;
}

at_state_t _comments_at(lexcomp_t* lexcomp){
	//Volvo para atrás para comprobar toda a cadena de novo
	previous_char();
	char c;
	c=next_char();
	//Comprobo que o primeiro carácter e un #, senon devolvo erro 
	if( c!='#' ) return AT_ERROR;
	//Recorro toda a cadena ata atopar 
	do{
		c=next_char();
	}while(c!='\n');

	// Copio o compoñente léxico a estrucutura
	strcpy(lexcomp->keyword,get_lexcomp());
	//Establezco o tipo de dato na estrucuta
	lexcomp->value=_COMMENT;
	// Se chegou ata aquí todo foi ben, entón informo a función principal
	return AT_OK;
}



at_state_t _token_at(lexcomp_t* lexcomp){
	previous_char();
	char c1,c2,c3;
	c1=next_char();
	c2=next_char();

	if(!_is_token(c1)) 
		return AT_ERROR;
	

	if(!_is_token(c2)){
		previous_char();
		strcpy(lexcomp->keyword,get_lexcomp());
		lexcomp->value=c1;
		return AT_OK;
	}

	int i;
	for(i=0;i<sizeof(validTokens)/sizeof(_token_at_t);i++){
		if((c1==validTokens[i].c1) && (c2==validTokens[i].c2)){
			if(validTokens[i].c3.c==_NOP){
				return validTokens[i].value;
			}else{
				c3=next_char();
				if(!_is_token(c3)){
					previous_char();
					strcpy(lexcomp->keyword,get_lexcomp());
					lexcomp->value=validTokens[i].value;
					return AT_OK;
				}else{
					if(validTokens[i].c3.c==c3){
						strcpy(lexcomp->keyword,get_lexcomp());
						lexcomp->value=validTokens[i].c3.value;
						return AT_OK;
					}
				}
			}
		} 
	}
	previous_char();
	strcpy(lexcomp->keyword,get_lexcomp());
	lexcomp->value=c1;
	return AT_OK;
}


lexcomp_t next_lexcomp(){
	lexcomp_t* currentlex=malloc(sizeof(lexcomp_t));
	char c;


	c = next_char();
	//printf("PRIMEIRO %c\n",c);
	switch (c){
		case '0' ... '9':
			_numeric_at(currentlex);
			break;
		case 'A' ... 'Z':
		case 'a' ... 'z':
		case '_':
			_alphanumeric_at(currentlex);
			break;
		case '\"':
		case '\'':
			_quotes_at(currentlex);
			// if(t2==QT_ERROR){
			// 	printf("Error comillas mal definidas\n");
			// 	return;
			// }
			break;
		case '#':
			_comments_at(currentlex);
			previous_char();
			break;
		case '+':
		case '-':
		case '*':
		case '/':
		case '%':
		case '&':
		case '|':
		case '^':
		case '~':
		case '(': 
		case ')': 
		case '[': 
		case ']': 
		case '{': 
		case '}': 
		case ',': 
		case ':': 
		case ';': 
		case '@': 
		case '=':			
		case '<': 
		case '>':
			_token_at(currentlex);
			break;
		case EOF:
			strcpy(currentlex->keyword,"EOF");
			currentlex->value=_EOF;
			get_lexcomp();
			break;		
		case '\n':
			lines++;
			strcpy(currentlex->keyword,"CR");
			currentlex->value=_NEWLINE;
			get_lexcomp();
			break;
		default:
			get_lexcomp();
			break;
	}

	return *currentlex;
}