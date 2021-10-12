#include "lexical_analyzer.h"




typedef enum{
	NAT_UNK,
	NAT_ZEROSTART,
	NAT_BIN_1,
	NAT_BIN_2,
	NAT_HEX_1,
	NAT_HEX_2,
	NAT_OCT_1,
	NAT_OCT_2,
	NAT_INT,
	NAT_DEC,
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


int _numeric_at(lexcomp_t* lexcomp){
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
				if(c=='x') state=NAT_HEX_1;
				else if(c=='o') state=NAT_OCT_1;
				else if(c=='b') state=NAT_BIN_1;
				else if(c=='e') state=NAT_EXP;
				else if (c>='1' && c<='9') state=NAT_INT;
				else if (c=='.') state=NAT_DEC;
				else end=1;
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
			case NAT_BIN_1:
				if (!(c=='0' || c=='1' || c=='_')) return -1;
				else state=NAT_BIN_2;
				break;
			case NAT_BIN_2:
				if (!(c=='0' || c=='1' || c=='_')) end=1;
				break;
			case NAT_HEX_1:
				if (!((c>='0' && c<='9') || (c>='a' && c<='f'))) return -1;
				else state=NAT_HEX_2;
				break;			
			case NAT_HEX_2:
				if (!((c>='0' && c<='9') || (c>='a' && c<='f'))) end=1;
				break;
			case NAT_OCT_1:
				if (!(c>='0' && c<='7')) return -1;
				else state=NAT_OCT_2;
				break;			
			case NAT_OCT_2:
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
	QAT_DOUBLE_QUOTE_1,
	QAT_DOUBLE_QUOTE_2,
	QAT_CONTENT,
	QAT_COMMENT
}_quotes_at_st;

int _quotes_at(lexcomp_t* lexcomp){
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
				else if (c=='\"') state=QAT_DOUBLE_QUOTE_1;
				break;
			case QAT_SIMPLE_QUOTE:
				if(c=='\'') end=1;
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
					end=1;
				}
				break;
			case QAT_COMMENT:
				if(c=='\"') double_quote--;
				if(double_quote==0) end=1;
				break;
			case QAT_CONTENT:
				if(c=='\"') end=1;
				break;
		}
		if(c==EOF)break;
		p++;
	}while(!end);

	if(!end) return -1;

	lexcomp->keyword[p]='\0';
	lexcomp->value=1000;
}

int _comments_at(lexcomp_t* lexcomp){
	previous_char();
	char c;
	int p=0;
	do{
		c=_get_next_char();
		lexcomp->keyword[p]=c;
		p++;
	}while(c!='\n');
	
	lexcomp->keyword[p-1]='\0';
	lexcomp->value=900;
}

//TODO : Mirar como arreglar detectar cadenas e números EOF
void next_lexcomp(){
	lines=1;
	char c;
	lexcomp_t current_lex;
	current_lex.value=-1;
	do{
		c = _get_next_char();
		switch (c){
			case '0' ... '9':
			case '.':
				if(_numeric_at(&current_lex)==-1){
					printf("Error\n");
					return;
				}
				printf("current lex_comp(%d)[%d] : %s \n",lines,current_lex.value,current_lex.keyword);
				previous_char();
				break;
			case 'A' ... 'Z':
			case 'a' ... 'z':
			case '_':
				_alphanumeric_at(&current_lex);
				printf("current lex_comp(%d) [%d] : %s \n",lines,current_lex.value,current_lex.keyword);
				previous_char();
				break;
			case '\"':
			case '\'':
				if(_quotes_at(&current_lex)==-1){
					printf("Error\n");
					return;
				}
				printf("current lex_comp(%d) [%d] : %s \n",lines,current_lex.value,current_lex.keyword);
				break;
			case '#':
				_comments_at(&current_lex);
				printf("current lex_comp(%d) [%d] : %s \n",lines,current_lex.value,current_lex.keyword);
				break;

		}
	}while(c!=EOF);
	
}