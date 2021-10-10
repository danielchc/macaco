#include "lexical_analyzer.h"




typedef enum{
	NAT_UNK,
	NAT_ZEROSTART,
	NAT_BIN,
	NAT_INT,
	NAT_DEC,
	NAT_HEX,
	NAT_OCT,
	NAT_EXP,
	NAT_EXP_UNSIGNED,
	NAT_IMAGINARY 
} _numeric_at_st;


int lines;

char _get_next_char(){
	char c;
	c=next_char();
	if(c=='\n')lines++;
	return c;
}


void _numeric_at(lexcomp_t* lexcomp){
	int end=0;
	_numeric_at_st state=NAT_UNK;
	previous_char();
	char c;
	int p=0;
	do{
		c=_get_next_char();
		lexcomp->keyword[p]=c;
		c=tolower(c);
		switch (state){
			case NAT_ZEROSTART:
				if(c=='x') state=NAT_HEX;
				else if(c=='o') state=NAT_OCT;
				else if(c=='b') state=NAT_BIN;
				else if(c=='e') state=NAT_EXP;
				else if (c>='1' && c<='9') state=NAT_INT;
				else if (c=='.') state=NAT_DEC;
				else end=1;
				break;
			case NAT_BIN:
				if (!(c=='0' || c=='1' || c=='_')) end=1;
				break;
			case NAT_INT:
				if (c=='.') state=NAT_DEC;
				else if (c=='e') state=NAT_EXP_UNSIGNED;
				else if (c=='j') state=NAT_IMAGINARY;
				else if (!(c>='0' && c<='9') && (c!='_')) end=1;
				break;
			case NAT_DEC:
				if (c=='e') state=NAT_EXP_UNSIGNED;
				else if (c=='j') state=NAT_IMAGINARY;
				else if(!(c>='0' && c<='9') && (c!='_')) end=1;
				break;
			case NAT_EXP_UNSIGNED:
				if ((c=='-') || (c=='+') || (c>='0' && c<='9')) state=NAT_EXP;
				break;
			case NAT_EXP:
				if (c=='j') state=NAT_IMAGINARY;
				else if (!(c>='0' && c<='9')) end=1;
				break;
			case NAT_HEX:
				if (!((c>='0' && c<='9') || (c>='a' && c<='f'))) end=1;
				break;
			case NAT_OCT:
				if (!(c>='0' && c<='7')) end=1;
				break;
			case NAT_IMAGINARY:
				end=1;
				break;
			case NAT_UNK:
				if(c=='0') state=NAT_ZEROSTART;
				else if (c>='1'&& c<='9') state=NAT_INT;
				else if(c=='.')state=NAT_DEC;
				break;
		}
		p++;
	}while(!end);
	lexcomp->keyword[p-1]='\0';
	lexcomp->value=_NUMERIC;
	//printf("[LINEA %d] %s de tipo %d\n",lines,lexcomp->keyword,numeric_type);
}

void _alphanumeric_at(lexcomp_t* lexcomp){
	previous_char();
	char c;
	int p=0;
	do{
		c=_get_next_char();
		lexcomp->keyword[p]=c;
		p++;
	}while(isalnum(c) || c == '_');
	
	lexcomp->keyword[p-1]='\0';
	lexcomp->value=_ID;
}


typedef enum{
	QAT_UNK,
	QAT_SIMPLE_QUOTE,
	QAT_DOUBLE_QUOTE,
	QAT_CONTENT
}_quotes_at_st;

void _quotes_at(lexcomp_t* lexcomp){
	previous_char();
	int double_quote=0;
	int end=0;
	char c;
	int p=0;
	_quotes_at_st state=QAT_UNK;
	do{
		c=_get_next_char();
		lexcomp->keyword[p]=c;
		switch (state){
			case QAT_UNK:
				if (c=='\'') state=QAT_SIMPLE_QUOTE;
				else if (c=='\"') {
					state=QAT_DOUBLE_QUOTE;
					double_quote++;
				}
				break;
			case QAT_SIMPLE_QUOTE:
				if(c=='\'') end=1;
				break;
			case QAT_DOUBLE_QUOTE:
				if(c=='\"') double_quote++;
				else state=QAT_CONTENT;
				break;
			case QAT_CONTENT:
				if(c=='\"') double_quote--;
				if(double_quote==0)end=1;
				break;
		}
		printf("ESTADO [%d] %c %d \n",state,c,double_quote);
		p++;
	}while(!end);
	lexcomp->keyword[p]='\0';
	lexcomp->value=1000;
}


void next_lexcomp(){
	lines=1;
	char c;
	lexcomp_t current_lex;
	do{
		c = _get_next_char();
		switch (c){
			case '0' ... '9':
			case '.':
				_numeric_at(&current_lex);
				printf("current lex_comp[%d] : %s \n",current_lex.value,current_lex.keyword);
				previous_char();
				break;
			case 'A' ... 'Z':
			case 'a' ... 'z':
			case '_':
				_alphanumeric_at(&current_lex);
				printf("current lex_comp[%d] : %s \n",current_lex.value,current_lex.keyword);
				previous_char();
				break;
			case '\"':
			case '\'':
				_quotes_at(&current_lex);
				printf("current lex_comp[%d] : %s \n",current_lex.value,current_lex.keyword);
				break;

		}
	}while(c!=EOF);
	
}