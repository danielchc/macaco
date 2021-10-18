#include "lexical_analyzer.h"



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


numeric_t _numeric_type(char firstChar){
	_numeric_at_st state=NAT_UNK;
	numeric_t type=NT_INTEGER;
	char c=tolower(firstChar);
	do{
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
		//Rompo aqui o bucle para asegurarme de que procesa o EOF
		if(c==EOF) break;
		c=next_char();
		c=tolower(c);
	}while(1);
	return type;
}

quote_t _quote_type(char firstChar){
	//Collo o primeiro carácter
	char c=firstChar;
	//Establezco o estado do automata o estado inicial
	_quotes_at_st state=QAT_INITIAL;
	do{
		switch (state){
			case QAT_INITIAL:
				//Se é unha comilla simple cambio de estado de comillas simples, para intentar buscar texto.
				if (c=='\'') state=QAT_SIMPLE_QUOTE;
				//Se é unha comilla doble, cambio de estado para intentar buscar texto ou outra comilla
				else if (c=='\"') state=QAT_DOUBLE_QUOTE_1;
				break;
			case QAT_SIMPLE_QUOTE:
				//Se é unha comilla simple, continuo lendo caracteres ata atopar unha comilla simple de novo, se atopo devolvo que a cadea é un string. Exemplo 'hola'
				if(c=='\'') return QT_STRING;
				break;
			case QAT_DOUBLE_QUOTE_1:
				//Se atopo unha doble comilla, significa hai dúas dobles comillas "" (a primeira que atopou no estado QAT_INITIAL e esta)
				if(c=='\"')	state=QAT_DOUBLE_QUOTE_2;
				//Se atopo outra cousa significa que hai texto
				else state=QAT_CONTENT;
				break;
			case QAT_CONTENT:
				//Busco ata atopar outra comilla que cerre a cadea Exemplo: "hola"
				if (c=='\"') return QT_STRING;
				break;
			case QAT_DOUBLE_QUOTE_2:
				//Se atopo unha terceira comilla significa que estou nun comentario
				if(c=='\"') state=QAT_COMMENT;
				//Se non atopo unha comilla significa que é unha cadea vacia Exemplo:""
				else return QT_STRING;
				break;
			case QAT_COMMENT:
				//Se atopo unha comilla significa que pode que esté terminando o comentario. Exemplo """hola "
				if(c=='\"') state=QAT_DOUBLE_QUOTE_3;
				break;
			case QAT_DOUBLE_QUOTE_3:
				//Se atopo outra comilla, significa que casi acabei o comentario. Exemplo: """hola ""
				if(c=='\"') state=QAT_DOUBLE_QUOTE_4;
				//Se atopo outra cousa significa que esa comilla non estaba cerrando o comentario,senon que pertencia o contido """hola "Pablo
				else state=QAT_COMMENT;
				break;
			case QAT_DOUBLE_QUOTE_4:
				//Se atopo outra comilla rematei de cerrar o comentario. Exemplo: """hola """
				if(c=='\"') return QT_COMMENT;
				//Se atopo outra cousa significa que o comentario ainda non rematou. """hola "Pablo" que tal
				else state=QAT_COMMENT;
				break;
		}
		//Rompo o bucle cando me atopo un fin de ficheiro, fagoo aqui para asegurarme que procesa o EOF
		if(c==EOF) break;
		c=next_char();
	}while(1);
	//Se chega ata aquí houbo un erro, unha cadea está sen cerrar
	return QT_ERROR;
}

at_state_t _numeric_at(char firstChar,lexcomp_t* lexcomp){
	//Volvo para atrás para comprobar toda a cadena de novo
	//Chamo a función para saber que tipo de número é
	numeric_t type=_numeric_type(firstChar);
	// Se o automáta da algún erro devolvo un erro a función principal
	if(type==NT_ERROR){
		//Xestiono o error no xestor de erros
		handle_lexical_error(ERR_NUMERIC);
		//Volvo para atrás para poder procesar o último carácter
		previous_char();
		//Consumo o compeñente léxico para poder ler o seguinte
		get_lexcomp();
		return AT_ERROR;
	} 

	//Movome o caracter anterior xa que non pertence o lexema
	previous_char();
	// Copio o compoñente léxico a estrucutura
	strcpy(lexcomp->keyword,get_lexcomp());
	/*
		Se o resultado da función é DECIMAL, garda o tipo decimal na estructura do compoñente léxico, 
		en caso contrario ten que ser un número enteiro
	*/
	lexcomp->value=(type==NT_DECIMAL)?_DECIMAL:_INTEGER;

	// Se chegou ata aquí todo foi ben, entón informo a función principal 
	return AT_OK;
}

at_state_t _quotes_at(char firstChar,lexcomp_t* lexcomp){
	//Chamo a función para saber que tipo de número é
	quote_t type=_quote_type(firstChar);
	// Se o automáta da algún erro devolvo un erro a función principal
	if(type==QT_ERROR){
		handle_lexical_error(ERR_QUOTE);
		previous_char();
		return AT_ERROR;
	}

	// Copio o compoñente léxico a estrucutura
	strcpy(lexcomp->keyword,get_lexcomp());
	/*
		Se o resultado da función é QT_STRING, establezco na estructura que un string, no caso contrario é un comentario
	*/
	lexcomp->value=(type==QT_STRING)?_STRING:_COMMENT;
	// Se chegou ata aquí todo foi ben, entón informo a función principal 
	return AT_OK;
}

at_state_t _alphanumeric_at(char firstChar,lexcomp_t* lexcomp){
	char c=firstChar;
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

at_state_t _comments_at(char firstChar,lexcomp_t* lexcomp){
	char c;
	//Comprobo que o primeiro carácter e un #, senon devolvo erro 
	if( firstChar!='#' ) return AT_ERROR;
	//Recorro toda a cadena ata atopar 
	do{
		c=next_char();
	}while(c!='\n');


	previous_char();
	// Copio o compoñente léxico a estrucutura
	strcpy(lexcomp->keyword,get_lexcomp());
	//Establezco o tipo de dato na estrucuta
	lexcomp->value=_COMMENT;
	// Se chegou ata aquí todo foi ben, entón informo a función principal
	return AT_OK;
}

at_state_t _token_at(char firstChar,lexcomp_t* lexcomp){;
	char c2,c3;
	if(!_is_token(firstChar)) 
		return AT_ERROR;
	

	c2=next_char();
	if(!_is_token(c2)){
		previous_char();
		strcpy(lexcomp->keyword,get_lexcomp());
		lexcomp->value=firstChar;
		return AT_OK;
	}

	int i;
	for(i=0;i<sizeof(validTokens)/sizeof(_token_at_t);i++){
		if((firstChar==validTokens[i].c1) && (c2==validTokens[i].c2)){
			if(validTokens[i].c3.c==_NOP){
				strcpy(lexcomp->keyword,get_lexcomp());
				lexcomp->value=validTokens[i].value;
				return AT_OK;
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
	lexcomp->value=firstChar;
	return AT_OK;
}

at_state_t _dot_at(char firstChar,lexcomp_t* lexcomp){
	if(firstChar!='.') return AT_ERROR;
	char c;
	c=next_char();
	if(isdigit(c)){
		return _numeric_at(c,lexcomp);
	}else{
		previous_char();
		strcpy(lexcomp->keyword,get_lexcomp());
		lexcomp->value=c;
		return AT_OK;
	}

}

at_state_t _newline_at(char firstChar,lexcomp_t* lexcomp){
	if(firstChar!='\n') return AT_ERROR;
	strcpy(lexcomp->keyword,get_lexcomp());
	lexcomp->value=_NEWLINE;
	return AT_OK;
}

at_state_t next_lexcomp(lexcomp_t* current_lex){
	char c;
	at_state_t state=AT_UNK;
	c = next_char();
	switch (c){
		case '0' ... '9':
			state=_numeric_at(c,current_lex);
			break;
		case 'A' ... 'Z':
		case 'a' ... 'z':
		case '_':
			state=_alphanumeric_at(c,current_lex);
			if(state==AT_OK){
				save_lexcomp(current_lex->keyword,current_lex->value);
			}
			break;
		case '\"':
		case '\'':
			state=_quotes_at(c,current_lex);
			break;
		case '#':
			state=_comments_at(c,current_lex);
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
			state=_token_at(c,current_lex);
			break;
		case '.':
			state=_dot_at(c,current_lex);
			break;
		case '\n':
			state=_newline_at(c,current_lex);
			break;
		case EOF:
			strcpy(current_lex->keyword,get_lexcomp());
			current_lex->value=_EOF;
			break;
		case 32:
		default:
			get_lexcomp();
			return AT_NOLEX;
			break;
	}
	return state;
}