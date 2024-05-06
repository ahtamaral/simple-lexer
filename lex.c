// Trabalho 01 Linguagens formais 2024.1

// Artur Henrique Teixeira do Amaral
// Tales Coutinho Moreira

#include <stdio.h>
#include <stdlib.h>

// var a = 1 + 2

#define MAX_TOKEN_SIZE 1024

typedef struct _charBuffer
{
    char cur;
    char next;
} charBuffer;

typedef struct _flags
{
    int isNumber;
    int isName;
    int isSymbol;
    int isComment;
} flags;

typedef char tokenBuffer[MAX_TOKEN_SIZE];

typedef struct _lexer
{
    charBuffer charBuffer;
    flags flags;
    tokenBuffer tokenBuffer;
} Lexer;

// Vão retornar flags que indicam o tipo que o token atual pode assumir, dada a leitura de seus caracteres.
// int isNumber(charBuffer b)
// int isName(charBuffer b)
// int isComment(charBuffer b)
// int isSymbol(charBuffer b)

// int isReservedWord()
// int isSpace()

// Checa as flags do Lexer. Caso uma, e apenas uma, seja verdadeira, sabe-se inequivocamente qual é o tipo do token 
// sendo lido no momento.
int isCurrentTokenAmbiguous(flags flags)
{
    int ret = flags.isNumber + flags.isName + flags.isSymbol + flags.isComment;
    return ret == 1 ? 1 : 0; 
}


int main()
{
    Lexer lexer;

    lexer.charBuffer.cur = getchar();

    if (lexer.charBuffer.cur == EOF) { exit (1); }

    while(1) {

        lexer.charBuffer.next = getchar();

        if (lexer.charBuffer.next == EOF ) { exit (1); }

        printf("%c%c\n", lexer.charBuffer.cur, lexer.charBuffer.next);

        lexer.charBuffer.cur = lexer.charBuffer.next;
    }   

    return 0;
}
