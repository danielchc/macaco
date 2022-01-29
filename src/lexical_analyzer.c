#include "lexical_analyzer.h"


/*
	OPERADORES VÁLIDOS
	Explicación dos campos por orde
	(1) c1-> Primeiro caracter
	(2) c2-> Segundo caracter
	(3) ID que lle corresponde a c1c2, por exemplo << correspondelle o identificador _SHIFTL
	(4) Array co terceiro carácter que pode formar unha expresión de existir
		- Se pon _NOP, significa que non hai ningún operador c1c2 que teña outro carácter despois
		- Se ten un simbolo, significa que con ese símbolo, c1c2c3 é un operador que ten como o último parámetro deste array
		Por exemplo primeiro <<= ten como identificador _SHTLEQ 
*/
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

/*
	_recover_lexcomp
		garda o compoñente léxico  procesado na estructura de compoñente léxico pasada por referencia no parámetro e libera a memoria
	param:
		lexcomp_t* lexcomp: compoñente léxico procesado (punteiro a estructura para gardar os datos)
*/
void _recover_lexcomp(lexcomp_t* lexcomp, int id){
	char* lex=get_lexcomp();
	strcpy(lexcomp->keyword,lex);
	lexcomp->value=id;
	free(lex);
}

/*
	_consume_lexcomp
		ignora o compoñente léxico e libera a memoria
	param:
		lexcomp_t* lexcomp: compoñente léxico procesado (punteiro a estructura para gardar os datos)
*/
void _consume_lexcomp(){
	char* lex=get_lexcomp();
	free(lex);
}

/*
	_is_token
		comproba se o carácter pasado por argumento é un delimitador ou operador
	param:
		char c: carácter a comprobar que é un delimitador ou operador
	return:
		1 sé un delimitador
		0 se non é un delimitador
*/


int _is_token(char c){
	return (
		(c == '+') || (c == '-') || (c == '*') || (c == '/') || (c == '%') || 	\
		(c == '@') || (c == '&') || (c == '|') || (c == '^') || (c == '~') ||   \
		(c == '(') || (c == ')') || (c == '[') || (c == ']') || (c == '{') || 	\
		(c == '}') || (c == ',') || (c == ':') || (c == '.') || (c == ';') || 	\
		(c == '=') || (c == '>') || (c == '<')									\
	);
}

/*
	_numeric_type
		devolve o tipo de número dunha cadea a procesar
	param:
		char firstChar: carácter inicial da cadea a procesar
	return:
		NT_ERROR: Se ocorre un erro
		NT_INTEGER: Se o número é un enteiro
		NT_DECIMAL: Se o número é decimal
*/

numeric_t _numeric_type(char firstChar){
	//Establezco os estados e tipo inicial
	_numeric_at_st state=NAT_INITIAL;
	//Gardo o primeiro cáracter nunha variable para procesar todo o número
	char c=tolower(firstChar);
	do{
		/*
			Neste bucle vou transicionando por diferentes estado ata determinar que tipo de número é, se está ben formado,
			por exemplo esta función non admite números tipo 0b, 0o xa que son tipos incompletos
		*/
		switch (state){
			//Estado inicial
			case NAT_INITIAL:
				//Se atopo un 0, pasa o estado para comprobar se é binario/octal/hexadecimal
				if(c=='0') state=NAT_ZEROSTART;
				//Se atopo un 1..9, significa que comeza por un número enteiro
				else if (c>='1'&& c<='9') state=NAT_INT;
				//Se atopo un punto é un decimal
				else if(c=='.') state=NAT_DEC;
				break;
			case NAT_ZEROSTART:
				//Comproba que é un número hexadecimal,transiciona o estado
				if(c=='x') state=NAT_HEX_1;
				//Comproba que é un número octal,transiciona o estado
				else if(c=='o') state=NAT_OCT_1;
				//Comproba que é un número binario,transiciona o estado
				else if(c=='b') state=NAT_BIN_1;
				//Comproba que é a cadea ten un "e",e un numero exponencial,transiciona o estado
				else if(c=='e') state=NAT_EXP;
				//Se atopo un 1-9, significa que comeza por un número enteiro,transiciona o estado
				else if (c>='1' && c<='9') state=NAT_INT;
				else if (c=='.') state=NAT_DEC; //Se atopo un punto é un decimal,transiciona o estado
				else return NT_INTEGER; //Se non atopo nada máis é o enteiro 0
				break;
			case NAT_INT:
				//Se atopo un punto é un decimal,transiciona o estado
				if (c=='.') state=NAT_DEC;
				//Comproba que é a cadea ten un "e",e un numero exponencial,transiciona o estado
				else if (c=='e') state=NAT_EXP_UNSIGNED;
				//Se atopo unha j, é un número imaxinario
				else if (c=='j') state=NAT_IMAGINARY;
				//Se non atopo nada máis é un enteiro
				else if (!(c>='0' && c<='9') && (c!='_')) return NT_INTEGER;
				break;
			case NAT_DEC:
				//Establezco o estado a decimal porque pasou por este estado
				//Comproba que é a cadea ten un "e",e un numero exponencial,transiciona o estado
				if (c=='e') state=NAT_EXP_UNSIGNED;
				//Se atopo unha j, é un número imaxinario
				else if (c=='j') state=NAT_IMAGINARY;
				//Se atopo outra cousa[0-9 ] é un decimal
				else if(!(c>='0' && c<='9') && (c!='_')) return NT_DECIMAL;
				break;
			case NAT_EXP_UNSIGNED:
				//Se entra aqui significa que é un exponecial, e pode ter ou números 1e9 ou pode ter un simbolo(+/-) ej 1e-(seguinte_estado)
				if ((c=='-') || (c=='+') || (c>='0' && c<='9')) state=NAT_EXP;
				break;
			case NAT_EXP:
				//Se entra aqui significa que é un exponecial
				//Se atopo unha j, é un número imaxinario
				if (c=='j') state=NAT_IMAGINARY;
				//Aqui atopo os números que se atopan despois da "e" ou dos símbolos(+/-), Exemplo: 10e-12 ou 2e32
				else if (!(c>='0' && c<='9')) return NT_DECIMAL;
				break;
			case NAT_BIN_1:
				//Se a cadea acaba como 0b, significa que o número binario está mal formado
				if (!(c=='0' || c=='1' || c=='_')) return NT_ERROR;
				//Senon é un binario correcto, transiciono
				else state=NAT_BIN_2;
				break;
			case NAT_BIN_2:
				//Se atopo 0 e 1, estou ante un número binario correcto, devolvo que é un integer
				if (!(c=='0' || c=='1' || c=='_')) return NT_INTEGER;
				break;
			case NAT_HEX_1:
				//Se a cadea acaba como 0x, significa que o número hexadecimal está mal formado
				if (!((c>='0' && c<='9') || (c>='a' && c<='f'))) return NT_ERROR;
	 			//Senon é un hexadecimal correcto 
				else state=NAT_HEX_2;
				break;			
			case NAT_HEX_2:
				//Se atopo 0-9 a-f, estou ante un número hexadecimal correcto, devolvo que é un integer
				if (!((c>='0' && c<='9') || (c>='a' && c<='f'))) return NT_INTEGER;
				break;
			case NAT_OCT_1:
				//Se a cadea acaba como 0o, significa que o número octal está mal formado
				if (!(c>='0' && c<='7')) return NT_ERROR;
				//Senon é un octal correcto 
				else state=NAT_OCT_2;
				break;			
			case NAT_OCT_2:
				//Se atopo 0-7, estou ante un número octal correcto, devolvo que é un integer
				if (!(c>='0' && c<='7')) return NT_INTEGER;
				break;
			case NAT_IMAGINARY:
				/*
					Se entra aqui é o que último caracter é un j, devolvo tipo que teño ata agora, 
					INTEGER ou DECIMAL, dependendo dos estado polos que pasara
				*/
				return NT_DECIMAL;
				break;
		}
		/*
			Rompo aqui o bucle para asegurarme de que procesa o EOF, debido a que lle paso o primeiro carácter por argumento teño que poñer o final o next_char
			e o EOF nunca se chega a procesar
			Outra opción menos eficiente de primeiras podería ser mover o punteiro con previous_char
		
			Se me atopo co final significa atopei o final do ficheiro, devolvo tipo que teño ata agora
		*/
		if(c==EOF) return NT_INTEGER;
		c=next_char();
		c=tolower(c);
	}while(1);
	//Se me atopo co final significa atopei o final do ficheiro, devolvo tipo que teño ata agora. Nunca debería chegar ata aquí
	return NT_INTEGER;
}

/*
	_quote_type
		devolve o tipo de cadea de comillas dunha cadea a procesar
	param:
		char firstChar: carácter inicial da cadea a procesar
	return:
		QT_ERROR: Se ocorre un erro
		QT_STRING: Se se atopou un string
		QT_COMMENT: Se se atopou un comentario
*/

quote_t _quote_type(char firstChar){
	//Collo o primeiro carácter
	char c=firstChar;
	//Establezco o estado do autómata o estado inicial
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
		//Se chega o EOF, houbo un erro
		if(c==EOF) return QT_ERROR;
		c=next_char();
	}while(1);

	//Se chega aquí houbo un erro. Nunca debería chegar aquí.
	return QT_ERROR;
}


/*
	_numeric_at
		garda o compoñente léxico númerico procesado na estructura 
		de compoñente léxico pasada por referencia no parámetro
	param:
		char firstChar: carácter inicial da cadea a procesar
		lexcomp_t* lexcomp: compoñente léxico procesado (punteiro a estructura para gardar os datos)
	return:
		AT_ERROR, 	Se ocorre un erro 
		AT_OK		Se todo vai ben
*/

at_state_t _numeric_at(char firstChar,lexcomp_t* lexcomp){
	//Volvo para atrás para comprobar toda a cadena de novo
	//Chamo a función para saber que tipo de número é
	numeric_t type=_numeric_type(firstChar);

	//Movome o caracter anterior xa que non pertence o lexema
	previous_char();

	/*
		Copio o compoñente léxico a estrucutura
		Se o resultado da función é DECIMAL, garda o tipo decimal na estructura do compoñente léxico, 
		en caso contrario ten que ser un número enteiro
	*/
	_recover_lexcomp(lexcomp,(type==NT_DECIMAL)?_DECIMAL:_INTEGER);

	// Se o autómata da algún erro devolvo un erro a función principal
	if(type==NT_ERROR){
		lexcomp->value=_TERROR;
		//Xestiono o error no xestor de erros
		handle_lexical_error(ERR_NUMERIC,lexcomp->keyword);
		return AT_ERROR;
	} 
	
	// Se chegou ata aquí todo foi ben, entón informo a función principal 
	return AT_OK;
}

/*
	_quotes_at
		garda o compoñente léxico de cadea de comillas procesado na estructura 
		de compoñente léxico pasada por referencia no parámetro
	param:
		char firstChar: carácter inicial da cadea a procesar
		lexcomp_t* lexcomp: compoñente léxico procesado (punteiro a estructura para gardar os datos)
	return:
		AT_ERROR, 		Se ocorre un erro 
		AT_OK			Se é un string
		AT_NOLEX		Se é un comentario
*/

at_state_t _quotes_at(char firstChar,lexcomp_t* lexcomp){
	//Chamo a función para saber que tipo de número é
	quote_t type=_quote_type(firstChar);
	// Se o autómata da algún erro devolvo un erro a función principal
	if(type==QT_ERROR){
		handle_lexical_error(ERR_QUOTE,NULL);
		previous_char();
		return AT_ERROR;
	}

	// Copio o compoñente léxico a estrucutura
	_recover_lexcomp(lexcomp,(type==QT_COMMENT)?_COMMENT:_STRING);

	// Se chegou ata aquí todo foi ben, entón informo a función principal 
	return AT_OK;
}

/*
	_alphanumeric_at
		garda o compoñente léxico de cadea alfanúmerica procesada na estructura 
		de compoñente léxico pasada por referencia no parámetro
	param:
		char firstChar: carácter inicial da cadea a procesar
		lexcomp_t* lexcomp: compoñente léxico procesado (punteiro a estructura para gardar os datos)
	return:
		AT_ERROR, 	Se ocorre un erro 
		AT_OK		Se todo vai ben
*/

at_state_t _alphanumeric_at(char firstChar,lexcomp_t* lexcomp){
	//Obteño o primeiro carácter
	char c=firstChar;
	//Recorro todos compoñentes léxicos ata atopar algo que non sexa un carácter alfanúmerico ou unha _
	do{
		c=next_char();
	}while(isalnum(c) || c == '_');

	previous_char();
	// Copio o compoñente léxico a estrucutura
	_recover_lexcomp(lexcomp,_ID);
	// Se chegou ata aquí todo foi ben, entón informo a función principal
	return AT_OK;
}

/*
	_comments_at
		garda o compoñente léxico de un comentario procesado na estructura 
		de compoñente léxico pasada por referencia no parámetro
	param:
		char firstChar: carácter inicial da cadea a procesar
		lexcomp_t* lexcomp: compoñente léxico procesado
	return:
		AT_ERROR, 	Se ocorre un erro 
		AT_NOLEX	Se todo vai ben, xa que non é un compoñente léxico
*/

at_state_t _comments_at(char firstChar,lexcomp_t* lexcomp){
	char c;
	//Comprobo que o primeiro carácter e un #, senon devolvo erro 
	if( firstChar!='#' ) return AT_ERROR;
	//Recorro toda a cadena ata atopar 
	do{
		c=next_char();
	}while(c!='\n');


	previous_char();
	// Consumo o resultado da función xa que non é un compoñente léxico
	_consume_lexcomp();
	// Se chegou ata aquí todo foi ben recoñeceu o comentario, como é non é un compoñente léxico informo a función principal
	return AT_NOLEX;
}


/*
	_token_at
		garda o compoñente léxico de un operador ou delimitador procesado na estructura 
		de compoñente léxico pasada por referencia no parámetro
	param:
		char firstChar: carácter inicial da cadea a procesar
		lexcomp_t* lexcomp: compoñente léxico procesado
	return:
		AT_ERROR, 	Se ocorre un erro 
		AT_OK		Se todo vai ben
*/

at_state_t _token_at(char firstChar,lexcomp_t* lexcomp){
	// token = operador/delimitador
	char c2,c3;
	//Se o primeiro carácter non é un token, devolvo erro
	if(!_is_token(firstChar)) 
		return AT_ERROR;
	
	//Se o segundo carácter é non é un token, é un token simple(1 caracter)
	c2=next_char();
	if(!_is_token(c2)){
		//Volvo para atrás porque o que acabo de detectar non é un token
		previous_char();
		//Gardo o valor no compoñente léxico co seu valor ASCII
		_recover_lexcomp(lexcomp,firstChar);
		return AT_OK;
	}

	//Recorro a lista de combinacións posibles de dous símbolos
	int i;
	for(i=0;i<sizeof(validTokens)/sizeof(_token_at_t);i++){
		//Se concide o primeiro é o segundo hai dúas opcións
		if((firstChar==validTokens[i].c1) && (c2==validTokens[i].c2)){

			/*
				Se non hai ningunha combinación dos símbolos anteriores cun terceiro ou
				se o terceiro non é un token
				entonces recoñecin un token doble. Exemplo +=
			*/
			c3=next_char();
			if(!_is_token(c3) || validTokens[i].c3.c==_NOP){
				previous_char();
				_recover_lexcomp(lexcomp,validTokens[i].value);
				return AT_OK;
			}
			/*
				Se existe unha combinación, devolvo o identificador desa combinación.
				Exemplo <<=
			*/
			if(validTokens[i].c3.c==c3){
				_recover_lexcomp(lexcomp,validTokens[i].c3.value);
				return AT_OK;
			}
		} 
	}

	//Se chega aqui significa que non é unha combinación válida de tokens, por exemplo "=>", devolve procesa un solo
	previous_char();
	_recover_lexcomp(lexcomp,firstChar);
	return AT_OK;
}

/*
	_dot_at
		garda o compoñente léxico dunha cadea que comeza por punto procesada na estructura 
		de compoñente léxico pasada por referencia no parámetro
	param:
		char firstChar: carácter inicial da cadea a procesar
		lexcomp_t* lexcomp: compoñente léxico procesado (punteiro a estructura para gardar os datos)
	return:
		AT_ERROR, 	Se ocorre un erro 
		AT_OK		Se todo vai ben
*/

at_state_t _dot_at(char firstChar,lexcomp_t* lexcomp){
	//Se o primeiro carácter non é un punto devolvo un erro
	if(firstChar!='.') 
		return AT_ERROR;
	char c;
	c=next_char();
	//Se o seguinte cáracter o punto é un número procesao no autómata de números
	if(isdigit(c)){
		return _numeric_at(c,lexcomp);
	}
	//Se non o é garda o punto como un compoñente léxico
	previous_char();
	_recover_lexcomp(lexcomp,firstChar);
	//Se chegou ata aqui todo foi ben
	return AT_OK;
}

/*
	_newline_at
		garda o compoñente léxico dun salto de liña  na estructura 
		de compoñente léxico pasada por referencia no parámetro
	param:
		char firstChar: carácter inicial da cadea a procesar
		lexcomp_t* lexcomp: compoñente léxico procesado (punteiro a estructura para gardar os datos)
	return:
		AT_ERROR, 	Se ocorre un erro 
		AT_OK		Se todo vai ben
*/

at_state_t _newline_at(char firstChar,lexcomp_t* lexcomp){
	//Se o primeiro carácter non é un salto de liña devolvo un erro
	if(firstChar!='\n') 
		return AT_ERROR;

	//Gardo o compoñente léxico na estructura
	_recover_lexcomp(lexcomp,_NEWLINE);

	//Se chegou ata aqui todo foi ben
	return AT_OK;
}

/*
	_eof_at
		garda o compoñente léxico do fin do ficheiro na estructura 
		de compoñente léxico pasada por referencia no parámetro
	param:
		char firstChar: carácter inicial da cadea a procesar
		lexcomp_t* lexcomp: compoñente léxico procesado (punteiro a estructura para gardar os datos)
	return:
		AT_ERROR, 	Se ocorre un erro 
		AT_OK		Se todo vai ben
*/

at_state_t _eof_at(char firstChar,lexcomp_t* lexcomp){
	//Se o primeiro carácter non é un o fin do ficheiro devolvo un erro
	if(firstChar!=EOF) 
		return AT_ERROR;

	//Gardo o compoñente léxico na estructura
	_recover_lexcomp(lexcomp,_EOF);
	//Se chegou ata aqui todo foi ben
	return AT_OK;
}

/*
	next_lexcomp
		garda o compoñente léxico na estructura de compoñente léxico pasada por referencia no parámetro
	param:
		lexcomp_t* lexcomp: compoñente léxico procesado (punteiro a estructura para gardar os datos)
	return:
		AT_ERROR, 	Se ocorre un erro 
		AT_OK		Se todo vai ben
		AT_NOLEX	Non é un compoñente léxico o último procesado
*/

at_state_t next_lexcomp(lexcomp_t* current_lex){
	char c;
	//Establezco o estado inicial do autómata principal
	at_state_t state=AT_UNK;
	c = next_char();
	switch (c){
		//Se é un número entra no función para recoñecer números
		case '0' ... '9':
			state=_numeric_at(c,current_lex);
			break;
		case 'A' ... 'Z':
		case 'a' ... 'z':
		case '_':
			//Se comeza por un carácter alfanumérico é unha cadea alfanumérica
			state=_alphanumeric_at(c,current_lex);
			//Se o estado é correcto, gardo na táboa de simbolos
			if(state==AT_OK) save_lexcomp(current_lex->keyword,&(current_lex->value));
			break;
		case '\"':
		case '\'':
			//Se comeza por un comilla proceso no función de comillas
			state=_quotes_at(c,current_lex);
			break;
		case '#':
			//Se comeza por un almohadilla proceso no función de comentarios
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
			//Se é un delimitador ou operador proceso no función de tokens
			state=_token_at(c,current_lex);
			break;
		case '.':
			//Se é un punto proceso na función de puntos
			state=_dot_at(c,current_lex);
			break;
		case '\n':
			//Se é un salto de liña proceso na función de salto de liña
			state=_newline_at(c,current_lex);
			break;
		case EOF:
			//Se é un EOF proceso na función de fin de ficheiro
			state=_eof_at(c,current_lex);
			break;
		case 32:
		default:
			//Se non é un compoñente léxico, consume a cadea que leu ata o momento, por exemplo os espacios
			_consume_lexcomp();
			return AT_NOLEX;
			break;
	}
	return state;
}