#include <string.h>

#include <stdlib.h>
#include "minishell.h"

Token *new_token(char *word, TokenType kind)
{
    Token* token;

    token = malloc(sizeof(*token));
    if(token == NULL)
        fatal_error("malloc");
    else
        memset(token, 0, sizeof(*token));
    token->word = word;
    token->kind = kind;
    return token;
}

bool is_blank(char c)
{
    return (c == ' ' || c == '\t' || c == '\n');
}

bool consume_blank(char **rest, char* line)
{
    if(is_blank(*line))
    {
        while(*line && is_blank(*line))
            line++;
        *rest = line;
        return true;
    }
    *rest = line;
    return false;
}

bool startswith(const char *s, const char* keyword)
{
    return (memcmp(s, keyword, strlen(keyword)) == 0);
}

bool is_operator(const char *s)
{
    static char *const operators[] = {"||", "&", "&&", ";", ";;", "(", ")", "|", "\n"};
    size_t i  = 0;

    while(i < sizeof(operators) / sizeof(*operators))
    {
        if(startswith(s, operators[i]))
            return true;
        i++;
    }
    return false;
}

bool is_metacharacter(char c)
{
    return (c && strchr("|&;()<> \t\n", c));
}

bool is_word(const char *s)
{
    return(*s && !is_metacharacter(*s));
}

Token *operator(char **rest, char *line)
{
   	static char	*const	operators[] = {"||", "&", "&&", ";", ";;", "(", ")", "|", "\n"};
    size_t i = 0;
    char *op;

    while(i < sizeof(operators)/sizeof(*operators))
    {
        if(startswith(line, operators[i]))
        {
            op = strdup(operators[i]);
            if(op == NULL)
                fatal_error("strdup");
            *rest = line + strlen(op);
            return (new_token(op, TOKEN_OP));
        }
        i++;
    }
    assert_error("Unexpected operator");
}

Token *word(char **rest, char *line)
{
    const char *start = line;
    char *word;

    while(*line && !is_metacharacter(*line))
    {
        if(*line == SINGLE_QUOTE_CHAR)
        {
            line++;
            while(*line != SINGLE_QUOTE_CHAR)
            {
                if(*line == '\0')
                    todo("Unclosed single quote");
                line++;
            }
            line++;
        }
        else if(*line == DOUBLE_QUOTE_CHAR)
        {
            line++;
            while(*line != DOUBLE_QUOTE_CHAR)
            {
                if(*line == '\0')
                    todo("Unclosed double quote");
                line++;
            }
            line++;
        }
        else
            line++;
    }
    word = strndup(start, line - start);
    if(word == NULL)
        fatal_error("strndup");
    *rest = line;
    return (new_token(word, TOKEN_WORD));
}

Token *tokenize(char *line)
{
    Token head;
    Token *token;

    head.next = NULL;
    token = &head;
    while(*line)
    {
        if(consume_blank(&line, line))
            continue;
        else if(is_operator(line))
            token = token->next = operator(&line, line);
        else if(is_word(line))
            token = token->next =word(&line, line);
        else
            assert_error("Unexpected Token");
    }
    token->next = new_token(NULL, TOKEN_EOF);
    return head.next;
}

char **tail_recursive(Token *token, int nargs, char **argv)
{
    char **new_argv;

    if (token == NULL || token->kind == TOKEN_EOF)
        return (argv);

    // 新しいサイズのメモリを確保
    new_argv = malloc((nargs + 2) * sizeof(char *));
    if (new_argv == NULL)
    {
        free(argv);  // メモリ確保が失敗した場合、元のポインタを解放
        fatal_error("malloc");
    }

    // 元のメモリ内容を新しい領域にコピー
    memcpy(new_argv, argv, nargs * sizeof(char *));
    free(argv);  // 古いメモリを解放

    argv = new_argv;

    // 次のトークンを追加
    argv[nargs] = strdup(token->word);
    if (argv[nargs] == NULL)
        fatal_error("strdup");

    argv[nargs + 1] = NULL;

    return (tail_recursive(token->next, nargs + 1, argv));
}

char **token_list_to_argv(Token *token)
{
    char **argv;

    argv = malloc(sizeof(char *));  // 1つのポインタ分のメモリを確保
    if (argv == NULL)
        fatal_error("malloc");
    memset(argv, 0, sizeof(char *));  // メモリをゼロ初期化
    return (tail_recursive(token, 0, argv));
}
