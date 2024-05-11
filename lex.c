#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MOVE_BACK_ONE_CHARACTER fseek(fd, -1, SEEK_CUR)

enum tokenType {
    KEYWORD,
    IDENTIFIER,
    CHARACTER,
    STRING,
    SPECIAL_SYMBOL,
    OPERADOR_ARITMETICO,
    OPERADOR_LOGICO,
    OPERADOR_RELACIONAL,
    OPERADOR_DE_ATRIBUICAO,
    OUTRO_OPERADOR,
    NUMERO,
    COMENTARIO,
    FIM_DE_ARQUIVO,
    TIPO_DE_DADO
};

char strType[14][100] = {
    "PALAVRA_CHAVE",
    "IDENTIFICADOR",
    "CARACTERE",
    "STRING",
    "SIMBOLO_ESPECIAL",
    "OPERADOR_ARITMETICO",
    "OPERADOR_LOGICO",
    "OPERADOR_RELACIONAL",
    "OPERADOR_DE_ATRIBUICAO",
    "OUTRO_OPERADOR",
    "NUMERO",
    "COMENTARIO",
    "FIM_DE_ARQUIVO",
    "TIPO_DE_DADO"
};

char darr[11][30] = {
    "char",
    "double",
    "enum",
    "float",
    "int",
    "long",
    "short",
    "signed",
    "struct",
    "unsigned",
    "void"
};

typedef struct token {
    char lex[100];
    enum tokenType type;
} token;

FILE *fd;

void preprocess(char *filename) {
    FILE *fa = fopen(filename, "r");
    if (!fa) {
        printf("Erro ao abrir o arquivo\n");
        exit(1);
    }
    fd = fa;
}

int isSpecialSymbol(int ca) {
    return (ca == '[' || ca == ']' || ca == '(' || ca == ')' || ca == '{' || ca == '}' || ca == ',' || ca == ';' || ca == ':' || ca == '?');
}

int isOperator(int ca) {
    return (ca == '=' || ca == '+' || ca == '-' || ca == '*' || ca == '/' || ca == '%' || ca == '<' || ca == '>' || ca == '!' || ca == '&' || ca == '|' || ca == '^' || ca == '~' || ca == '.');
}

token getNextToken() {
    char keywords[32][300] = { 
        "auto", "break", "case", "char", "const", "continue", "default", "do", "double", "else", "enum", "extern", "float", "for", "goto", "if", "int", "long", "register", "return", "short", "signed", "sizeof", "static", "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while" 
    };

    int ca;
    int j = 0;
    token var;

    ca = getc(fd);
    memset(var.lex, 0, sizeof(var.lex));
    
    if (ca == EOF) {
        var.type = FIM_DE_ARQUIVO;
        return var;
    }

    switch (ca) {
        case '\n':
            return getNextToken();
        case '"':
            j = 0;
            while ((ca = getc(fd)) != '"' && ca != EOF) {
                var.lex[j++] = ca;
            }
            var.type = STRING;
            return var;
        case '/':
            ca = getc(fd);
            if (ca == '/') {
                j = 0;
                var.type = COMENTARIO;
                while ((ca = getc(fd)) != '\n' && ca != EOF) {
                    var.lex[j++] = ca;
                }
                return var;
            }
            else if (ca == '*') {
                j = 0;
                var.type = COMENTARIO;
                while (1) {
                    var.lex[j++] = ca;
                    while ((ca = getc(fd)) != '*') {
                        if (ca == EOF) {
                            printf("Erro: Comentário não terminado\n");
                            exit(1);
                        }
                        var.lex[j++] = ca;
                    }
                    var.lex[j++] = ca;
                    if ((ca = getc(fd)) == '/') {
                        var.lex[j++] = ca;
                        break;
                    }
                }
                return var;
            }
            else {
                var.type = SPECIAL_SYMBOL;
                if (ca == '=') {
                    var.type = OPERADOR_DE_ATRIBUICAO;
                }
                else {
                    MOVE_BACK_ONE_CHARACTER;
                }
                var.lex[0] = '/';
                return var;
            }
        case '\'':
            var.lex[0] = ca;
            if ((ca = getc(fd)) == '\\') {
                var.lex[1] = ca;
                ca = getc(fd);
            }
            var.lex[1] = ca;
            ca = getc(fd);
            var.type = CHARACTER;
            return var;
        default:
            if (isdigit(ca)) {
                j = 0;
                while (!isSpecialSymbol(ca) && !isOperator(ca) && ca != ' ' && ca != '\n' && ca != '\t' && ca != '\r' && ca != EOF) {
                    var.lex[j++] = ca;
                    ca = getc(fd);
                }
                MOVE_BACK_ONE_CHARACTER;
                var.type = NUMERO;
                return var;
            }
            else if (isalpha(ca) || ca == '_') {
                while (!isSpecialSymbol(ca) && !isOperator(ca) && ca != ' ' && ca != '\n' && ca != '\t' && ca != '\r' && ca != EOF) {
                    var.lex[j++] = ca;
                    ca = getc(fd);
                }
                MOVE_BACK_ONE_CHARACTER;
                var.type = IDENTIFIER;
                for (int i = 0; i < 32; i++) {
                    if (strcmp(var.lex, keywords[i]) == 0) {
                        var.type = KEYWORD;
                        break;
                    }
                }
                for (int j = 0; j < 11; j++) {
                    if (strcmp(var.lex, darr[j]) == 0) {
                        var.type = TIPO_DE_DADO;
                        break;
                    }
                }
                return var;                           
            }
            else {
                var.lex[0] = ca;
                switch (ca) {
                    case '=':
                    case '!':
                        if ((ca = getc(fd)) == '=') {
                            var.lex[j++] = ca;
                            var.type = OPERADOR_RELACIONAL;
                        }
                        else {
                            MOVE_BACK_ONE_CHARACTER;
                            var.type = OPERADOR_ARITMETICO;
                        }
                        return var;
                    case '<':
                    case '>':
                        if ((ca = getc(fd)) == '=') {
                            var.lex[j++] = ca;
                            var.type = OPERADOR_RELACIONAL;
                        }
                        else {
                            MOVE_BACK_ONE_CHARACTER;
                            var.type = OPERADOR_RELACIONAL;
                        }
                        return var;
                    case '*':
                    case '%':
                        if ((ca = getc(fd)) == '=') {
                            var.lex[j++] = ca;
                            var.type = OPERADOR_DE_ATRIBUICAO;
                        }
                        else {
                            MOVE_BACK_ONE_CHARACTER;
                            var.type = OPERADOR_ARITMETICO;
                        }
                        return var;
                    case '+':
                    case '-':
                        if ((ca = getc(fd)) == '=') {
                            var.lex[j++] = ca;
                            var.type = OPERADOR_DE_ATRIBUICAO;
                        }
                        else {
                            MOVE_BACK_ONE_CHARACTER;
                            var.type = OPERADOR_ARITMETICO;
                        }
                        return var;
                    case '|':
                    case '&':
                        if ((ca = getc(fd)) == ca) {
                            var.lex[j++] = ca;
                            var.type = OPERADOR_LOGICO;
                        }
                        else {
                            MOVE_BACK_ONE_CHARACTER;
                            var.type = OPERADOR_ARITMETICO;
                        }
                        return var;
                    case '^':
                    case '~':
                    case '.':
                        var.type = OUTRO_OPERADOR;
                        return var;
                    default:
                        return getNextToken();
                }
            }
    }
}

// Função principal
int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s <nome_do_arquivo>\n", argv[0]);
        return 1;
    }

    // Pré-processamento do arquivo de entrada
    preprocess(argv[1]);

    token temp;

    printf("Token\t\tTipo\n");

    // Obtém e imprime os tokens do arquivo de entrada até o final do arquivo
    while ((temp = getNextToken()).type != FIM_DE_ARQUIVO) {
        printf("%s\t%s\n", strType[temp.type], temp.lex);
    }

    // Fecha o arquivo de entrada
    fclose(fd);

    return 0;
}
