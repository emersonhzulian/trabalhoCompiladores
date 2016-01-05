/* 
 * @<parser.c>::
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "tokens.h"
#include "lexer.h"
#include "keywords.h"
#include "mypas.h"

/**
 * Top-down recursive parser emulating an EBNF for the simplified
 * Pascal (MyPas):
 *
 * mypas -> PROGRAM ID ; body .
 * 
 * body -> { VAR ID { , ID } : type ; { ID { , ID } : type ; } }
 *       | { PROCEDURE ID parmlist ; body ; | FUNCTION ID parmlist : type ; body ; }
 */

void mypas();
void block();
void heading();
void parameterList();
void type();
void simpleType();
void statement();
void expression();
void simpleExpression();
void term();
void factor();
void constant();
 
void mypas (void)
{
	match(PROGRAM);       
	  
	// Nome do programa.  
	match(ID);
	
	// Se o programa tiver parametros, que nada mais eh do que uma lista de identificadores.
	if (lookahead == '(')            
	{
		match('(');
		
			if (lookahead == '(')            
			{
                loop_idList:
				match(ID);
				
				if(lookahead == ',')
				{
                    match(',');      	
				    goto loop_idList;
                }
			}
		match(')');
	}

	match(';');
	
	block(); 
    
    match('.');    
}   
   
void block()
{
    if(lookahead == LABEL)
    {
	    match(LABEL);
	
		loop_label:     
			match(UNSIGNEDINTEGER);    
			
			if (lookahead == ',')
			{
				match(',');
				goto loop_label;
			}
			
		match(';');
    }
    
    if(lookahead == CONST)
    {        
		match(CONST);
	
		loop_const:
			if (lookahead == ID)
			{
				match(ID);
				match('=');
				constant();
				match(';');
				
				goto loop_const;
			}	
    }
    
    if(lookahead == TYPE)
    {
		match(TYPE);
	
		loop_type:
			if (lookahead == ID)
			{
				match(ID);
				match('=');
				type();
				match(';');
				
				goto loop_type;
			}	
    }
    
    if(lookahead == VAR)
    {
        match(VAR);						
		
		loop_var:
			if (lookahead == ID)
			{
				match(ID);
			
				while (lookahead == ',')	
				{			
					match(',');
					match(ID);
				}
					
				match(':');
				type();
				match(';');
				
				goto loop_var;
			}	
    }
	// Um programa pode ter zero ou mais procedures ou functions
	// Entao, no final dos cases da PROCEDURE e da FUNCTION, ele retorna para a label loop_functionProcedure.
	loop_functionProcedure:
        
    if(lookahead == PROCEDURE)	
    {
		match(PROCEDURE);
		match(ID);
		parameterList();
		match(';');				
		block();
		match(';');
		goto loop_functionProcedure;
    }
    
    if(lookahead == FUNCTION)	
    {
		match(FUNCTION);
		match(ID);
		parameterList();
		match(':');
		type();
		match(';');				
		block();
		match(';');
		goto loop_functionProcedure;				
    }
    
    if(lookahead == BEGIN)	
    {
    	match(BEGIN);
    	
    	// Um bloco pode ter um ou mais statements.
		loop_statement:
			statement();
			
			if (lookahead == ';')
			{
				match(';');
				goto loop_statement;
			}
		
		match(END);
    }
}

void heading()
{
	switch(lookahead)
	{
		case PROCEDURE:
			match(PROCEDURE);
			match(ID);
			parameterList();
			break;
		
		case FUNCTION:
			match(FUNCTION);
			match(ID);
			parameterList();
			match(':');
			type();	
			break;
	}
		
}

void parameterList()
{
    if(lookahead == ('('))
    {
        match('(');  
        
        loop_parameterList:
        if(lookahead == VAR)
        {
            match(VAR);
            loop_parameterList_idList:
			match(ID);
			
			if(lookahead == ',')
			{
                match(',');      	
			    goto loop_parameterList_idList;
            }
            
            match(':');
            
            type();
        }
        else
        {
            heading();
        }
        
        if(lookahead == ';')
        {
            match(';');   
            goto loop_parameterList;
        }
        
        match(')');
        
    }
}

void type()
{
    //Alterar esse ID para tipo
    if(lookahead == ID)
    {
        match(ID);
    }
    
	if (lookahead == ARRAY)
	{
		match(ARRAY);
		match('[');	
		
		loop_array:
			simpleType();
			
			if (lookahead == ',')
			{
				match(',');
				goto loop_array;
			}
		
		match(']');
		match(OF);
		type();
	}
	
	// simpleType();
	
	// type identifier
	
	// NAO PRECISA FAZER: PACKED e SET / RECORD / FILE
}

void simpleType()
{
	if (lookahead == '(')
	{
		match('(');
		match(ID);
	
		while (lookahead == ',')	
		{			
			match(',');
			match(ID);
		}
		
		match(')');
	}	
	
	// type identifier
	
	// constant();
	// match('.');
	// match('.');
	// constant();
}

void statement()
{
	if (lookahead == UNSIGNEDINTEGER)
	{
		match(UNSIGNEDINTEGER);
		match(':');
	}
	
	/* //Parte do VARIABLE e FUNCTION IDENTIFIER.
	if (variable? || function Identifier?)
	{
		if (variable?)
			variable();
		else
			if (function Identifier?)
				match(ID); 
		
		match(':');
		match('=');
		expression();
		
		return;
	}
	*/
	
	/* // Parte do PROCEDURE IDENTIFIER
	match(ID);
	if (lookahead == '(')
	{
		match('(');
		
		loop_procedureIdentifier:		
			if (expression?)
				expression();
			else
				match(ID);
				
			if (lookahead == ',')
			{
				match(',');
				goto loop_procedureIdentifier;
			}
			
		match(')');
		
		return;
	}	
	*/
	
	switch(lookahead)
	{
		case(BEGIN):
			match(BEGIN);
	    	
	    	// Um statement pode ter um ou mais statements.
			loop_statementBegin:
				statement();
				
				if (lookahead == ';')
				{
					match(';');
					goto loop_statementBegin;
				}
			
			match(END);
			break;
			
		case(IF):
			match(IF);
			expression();
			match(THEN);
			statement();
			
			if(lookahead == ELSE)
			{
				match(ELSE);
				statement();
			}
			
			break;
			
		case(REPEAT):
			match(REPEAT);
			
			// Um repeat pode ter um ou mais statements.
			loop_statementRepeat:
				statement();
				
				if (lookahead == ';')
				{
					match(';');
					goto loop_statementRepeat;
				}
			
			match(UNTIL);
			expression();
			break;
			
		case(WHILE):
			match(WHILE);
			expression();
			match(DO);
			statement();
			break;
			
		case(FOR):
			/*
			//variable identifier
			match(':');
			match('=');
			expression();
			
			if (lookahead == TO)
				match(TO);
			else
				match(DOWNTO);
				
			expression();
			match(DO);
			statement();
			*/
			
			break;
			
		case(CASE):
			match(CASE);
			expression();
			match(OF);
			
			/*
			if (constant?)
			{
				loop_case:
					constant();
					
					while (lookahead == ',')
					{
						match(',');
						constant();
					}
					
					match(':');
					statement();
					
					if (lookahead == ';')
					{
						match(';');
						goto loop_case;
					}
			}
			*/
			match(END);
			break;
			
		case(WITH):
			/*
			match(WITH);
			
			variable();
			
			while(lookahead == ',')
			{
				match(',');
				variable();
			}
			
			match(DO);
			statement();		
			*/
			break;
			
		case(GOTO):
			match(GOTO);
			match(UNSIGNEDINTEGER);
			break;
	}
}

void expression()
{
	simpleExpression();
	
	switch(lookahead)
	{
		case('<'): // Caso lookahead seja < ou <= ou <>
			match('<');
			
			if (lookahead == '=')
				match('=');	
			else
				if (lookahead == '>')
					match('>');	
			
			simpleExpression();			
			break;
		
		case('>'): // Caso lookahead seja > ou >=
			match('>');
			
			if (lookahead == '=')
				match('=');						
			
			simpleExpression();
			break;
		
		case('='):
			match('=');
			simpleExpression();
			break;
			
		case(IN):
			match(IN);
			simpleExpression();
	}
}

void simpleExpression()
{
	if (lookahead == '+')
		match('+');	
	else
		if (lookahead == '-')
			match('-');	
			
	term();
	
	switch(lookahead)
	{		
		case('+'):
			match('+');
			term();
			break;
			
		case('-'):
			match('-');
			term();
			break;
			
		case(OR):
			match(OR);
			term();
	}
}

void term()
{
	factor();
	
	switch(lookahead)
	{		
		case('*'):
			match('*');
			factor();
			break;
			
		case('/'):
			match('/');
			factor();
			break;
			
		case(DIV):
			match(DIV);
			factor();
			break;
			
		case(MOD):
			match(MOD);
			factor();
			break;
				
		case(AND):
			match(AND);
			factor();
	}	
}

void factor()
{
	// Unsigned Constant
	
	// Variable
	
	//Function Identifier
	/* 
	match(ID);
	
	if (lookahead == '(')
	{
		match('(');
		expression();
		
		while (lookahead == ',')
		{
			match(',');
			expression();
		}
		
		match(')');
	}
	*/
	
	switch(lookahead)
	{
		case('('):
			match('(');
			expression();
			match(')');
			break;
			
		case(NOT):
			match(NOT);
			factor();
			break;
		
		case('['):
			match('[');			
			/*
			if (expression?)
			{
				loop_factorExpression:
					expression();
					
					if (lookahead == '.')
					{
						match('.');
						match('.');
						expression();
					}
					
					if (lookahead == ',')
					{
						match(',');
						goto loop_factorExpression;
					}
				
			}
			*/			
			match(']');
	}
}

void variable()
{
	loop_variable:
		switch(lookahead)
		{
			case('['):
				match('[');
				expression();
		
				while (lookahead == ',')
				{
					match(',');
					expression();
				}
				
				match(']');
				goto loop_variable;
				
			//case():
		}
}

void constant()
{
    switch(lookahead)
	{
		case('+'): 
            match('+');
		case('-'):
            match('-');
        //Constant Identifier
        case(ID): 
            match(ID);
        break;
        //Unisiggned number
        case(UNSIGNEDINTEGER): 
            match(UNSIGNEDINTEGER);
        break;
        default:
            match('\'');
            while (lookahead == CHAR)	
    		{			
    			match(CHAR);
    		}          
            match('\'');
    }
        
	// Character
	/*
	if (lookahead == "'")
	{
		match("'");
		
		loop_constCharacter:
			if (lookahead == character)
			{
				match(character);
				goto loop_constCharacter;
			}
			
		match("'");
		
		return;
	}
	*/	
	
//	if (lookahead == '+')
//		match('+');
//	else
//		if (lookahead == '-')
//			match('-');
//			
//	if (lookahead == ID)
//		match(ID);

}


/** lexer-to-parser interface **/

int lookahead;

void match(int predicted)
{    
    printf("\n Recebeu : %d, Lookahead: %d" , predicted, lookahead);
     
    if (predicted == lookahead) {
        if (lookahead != EOF) {
            lookahead = gettoken(tape);
        }
    } else {
        fprintf(stderr, "\n\ntoken mismatch... exiting\n\n");
        system("pause");
        exit(-1);
    }
}

