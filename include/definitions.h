#ifndef DEFINITIONS
#define DEFINITIONS

//Palabras clave
#define _FALSE 	    	301			/*	False	  */
#define _AWAIT 	    	302         /*	await	  */
#define _ELSE        	303         /*	else	  */
#define _IMPORT      	304         /*	import    */
#define _PASS        	305         /*	pass	  */
#define _NONE        	306         /*	None	  */
#define _BREAK       	307         /*	break	  */
#define _EXCEPT      	308         /*	except    */
#define _IN          	309         /*	in	      */
#define _RAISE 	     	310         /*	raise	  */
#define _TRUE        	311         /*	True	  */
#define _CLASS       	312         /*	class	  */
#define _FINALLY     	313         /*	finally   */
#define _IS 	     	314         /*	is	      */
#define _RETURN     	315         /*	return    */
#define _AND        	316         /*	and	      */
#define _CONTINUE 		317         /*	continue  */
#define _FOR 	    	318         /*	for	      */
#define _LAMBDA 	    319         /*	lambda    */
#define _TRY 	    	320         /*	try	      */
#define _AS 	        321         /*	as	      */
#define _DEF 	    	322         /*	def		  */
#define _FROM 	    	323         /*	from      */
#define _NONLOCAL 		324         /*	nonlocal  */	
#define _WHILE 	    	325         /*	while	  */
#define _ASSERT 	    326         /*	assert    */
#define _DEL 	    	327         /*	del       */
#define _GLOBAL 	    328         /*	global	  */
#define _NOT 	    	329         /*	not       */
#define _WITH 	    	330         /*	with	  */
#define _ASYNC 	    	331         /*	async     */
#define _ELIF 	    	332         /*	elif      */
#define _IF 	        333         /*	if        */
#define _OR 	        334         /*	or	      */
#define _YIELD 	    	335         /*	yield	  */



#define _ID				400
#define _INTEGER		401
#define _DECIMAL		401
#define _STRING			402
#define _COMMENT		403

//Salto de linea e fin de ficheiro
#define _EOF			    450
#define _NEWLINE			451
#define _TERROR				452

//Operadores simples

#define _ADD		43 		/*  +  */
#define _SUB		45		/*  -  */
#define _MUL		42		/*  *  */
#define _DIV		47		/*  /  */
#define _PER		37		/*  %  */
#define _AT			64		/*  @  */
#define _OPAND		38		/*  &  */
#define _OPOR 		124		/*  |  */
#define _LT			60		/*  <  */
#define _GT			62		/*  >  */
#define _DIA		94		/*  ^  */
#define _TILDE		126		/*  ~  */


//Operadores dobles
#define _SHIFTL 	500 		/*  <<  */
#define _SHIFTR 	501 		/*  >>  */
#define _POW	 	502 		/*  **  */
#define _FDIV 		503 		/*  //  */
#define _LEQ 		504 		/*  <=  */
#define _GEQ		505 		/*  >=  */
#define _EQ			506 		/*  ==  */
#define _NEQ		507 		/*  !=  */
#define _FUNA		508			/*  ->  */
#define _ADDEQ		509			/*  += 	*/
#define _SUBEQ		510			/*  -= 	*/
#define _MULEQ		511			/*  *= 	*/
#define _DIVEQ		512			/*  /=	*/
#define _PEREQ		513			/*  %=	*/
#define _IAND		514			/*  &=	*/
#define _IOR		515			/*  |=	*/
#define _IXOR		516			/*  ^=	*/

//Operadores triples
#define _SHTLEQ		517			/*	>>=	*/    
#define _SHTREQ		518			/*	<<=	*/    
#define _POWEQ		519			/*	**=	*/
#define _FDIVEQ		520			/*	//=	*/


#define _NOP		-1			/*	No operador	*/

#endif


