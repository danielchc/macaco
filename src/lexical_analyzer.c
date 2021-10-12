#include "lexical_analyzer.h"
#include "input.h"



int lines;

char _get_next_char(){
	char c;
	c=next_char();
	if(c=='\n')lines++;
	return c;
}

int is_operator(char c){
	return (c=='+' || c=='-' || c=='*' || c== '/' || c=='%' || c=='>' || c=='<' || c=='=');
}

int _alphanumeric_at(){
	previous_char();
	char c;
	do{
		c=_get_next_char();
	}while(isalnum(c) || c == '_');
	return 0;
}

int _comments_at(){
	previous_char();
	char c;
	do{
		c=_get_next_char();
	}while(c!='\n');
	return 0;
}

numeric_t _numeric_at(){
	numeric_t type=NT_INTEGER;
	_numeric_at_st state=NAT_UNK;
	previous_char();
	char c;
	do{
		c=_get_next_char();
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
	}while(1);
	return NT_ERROR;
}

quote_t _quotes_at(){
	previous_char();
	int double_quote=0;
	char c;
	_quotes_at_st state=QAT_UNK;
	do{
		c=_get_next_char();
		switch (state){
			case QAT_UNK:
				if (c=='\'') state=QAT_SIMPLE_QUOTE;
				else if (c=='\"') state=QAT_DOUBLE_QUOTE_1;
				break;
			case QAT_SIMPLE_QUOTE:
				if(c=='\'') return QT_STRING;
				break;
			case QAT_DOUBLE_QUOTE_1:
				if(c=='\"') state=QAT_DOUBLE_QUOTE_2;
				else state=QAT_CONTENT;
				break;
			case QAT_DOUBLE_QUOTE_2:
				if(c=='\"'){ 
					state=QAT_COMMENT;
					double_quote=3;
				}else {
					//A cadena vacia recoñecese co fin de liña, polo cal hai que volver un paso atrás
					previous_char();
					return QT_STRING;
				}
				break;
			case QAT_COMMENT:
				if(c=='\"') double_quote--;
				if(double_quote==0) return QT_COMMENT;
				break;
			case QAT_CONTENT:
				if(c=='\"') return QT_STRING;
				break;
		}
		if(c==EOF)break;
	}while(1);

	return QT_ERROR;
}

typedef struct{
	char c1;
	char c2;
	int value;
} tw_t;

tw_t paco[]={
	{'<','<',2000},
	{'<','=',2001},
	{'>','>',2002},
	{'>','=',2000},
	{'=','=',2000},
	{'*','*',2000},
	{'*','=',2000},
	{'+','=',2000},
	{'-','=',2000},
	{'/','=',2000},
	{'%','=',2000},
};

int _operator_at(){
	previous_char();
	char c1,c2;
	c1=_get_next_char();

	if(is_operator(c1)){
		c2=_get_next_char();
		if(!is_operator(c2)){
			previous_char();
			return c1; //DEVOLVE O VALOR ASCII DO OPERADOR
		}else{
			int i;
			for(i=0;i<sizeof(paco);i++){
				if((c1==paco[i].c1) && (c2==paco[i].c2)) return paco[i].value;
			}
		}

	}
	return -1;
}



//TODO : Mirar como arreglar detectar cadenas e números EOF
void next_lexcomp(){
	char* currentlex;
	lines=1;
	char c;
	numeric_t tx;
	int tipo;

	do{
		c = _get_next_char();
		switch (c){
			case '0' ... '9':
			case '.':
				tx=_numeric_at();
				if(tx==NT_ERROR){
					printf("Error número mal definido\n");
					return;
				}
				//printf("TIPO %d",tx);
				previous_char();
				break;
			case 'A' ... 'Z':
			case 'a' ... 'z':
			case '_':
				_alphanumeric_at();
				previous_char();
				break;
			case '\"':
			case '\'':
				if(_quotes_at()==QT_ERROR){
					printf("Error\n");
					return;
				}
				
				break;
			case '#':
				_comments_at();
				break;
			case '+':
			case '-':
			case '*': 
			case  '/': 
			case '%': 
			case '>': 
			case '<': 
			case '=':
				tipo=_operator_at();
				break;
		}
		currentlex=get_lexcomp();
		if(currentlex[0]==32 || currentlex[0]=='\n')continue;
		printf("COMPONENTE LEXICO [%d] %s\n",lines,currentlex);
		
	}while(c!=EOF);
	
}