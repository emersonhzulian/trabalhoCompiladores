/*
 * @<main.c>::
 */

#include <stdio.h>
#include <stdlib.h>

#include "mypas.h"

FILE           *tape;

main(int argc, char *argv[])
{
//    if (argc > 1) {
//        tape = fopen(argv[1], "r");
//        if (tape == NULL) {
//            fprintf(stderr, "file not found... exiting\n");
//            exit(-2);
//        }
//    } else {
//        tape = stdin;
//    }


      
    tape = fopen("teste.txt", "r");
    
    if (tape == NULL) 
    {
       printf("Erro ao abrir o arquivo.");
    }
    else
    {
      lookahead = gettoken(tape);
  
      mypas();
  
      printf("\n\nDeu certo!\nPrograma pascal compilado com sucesso.\n\n");
    }
    
    system("pause");

    return 0;
}
