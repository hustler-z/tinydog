// @Hustler's Project

#ifndef _TINY_CONSOLE_H_
#define _TINY_CONSOLE_H_

// ####################################################

#include <type.h>

#define BUILD_USER           __stringify(hustler)

#define BUILD_HOST           __stringify(embedded)

#define SHELL_VERSION        __stringify(1)

#define USER_REPO_VERSION    __stringify(0)

#define USER_PROMPT          __stringify(hustle)

#define PROMPT               USER_PROMPT" => "

#ifndef LINE_BUFF_SIZE
#define LINE_BUFF_SIZE       32
#endif

#define MAX_ARG_COUNT        (LINE_BUFF_SIZE / 2)

#ifndef NUM_HISTORY_ENTRIES
#define NUM_HISTORY_ENTRIES  2
#endif

#define END_OF_LINE          '\0'
#define SPACE                ' '
#define TAB                  '\t'
#define NEW_LINE             '\n'
#define CARRIAGE_RETURN      '\r'
#define BACK_SPACE           '\b'
#define DELETE               '\177'
#define ESCAPE               '\33'
#define SQUARE_BRACKET_OPEN  '\133'
#define UP_ARROW             '\101'

#define ECHO_INIT_VALUE      1

// ####################################################

int exec(char *cmd_str);

typedef int (*xmd)(int argc, char **argv);

typedef struct {
    const char *name;
    const char *help;
    xmd xcall;
} cmd_t;

#define PUSH_CMD_LIST(_name, _help, _func) \
    __attribute__((__section__(".cmd_list"))) \
    cmd_t _var##_func = { #_name, _help, &_func }

#define PUSH_CMD_AUTO(_name, _help, _func) \
    __attribute__((__section__(".cmd_auto"))) \
    cmd_t _var##_func = { #_name, _help, &_func }

xmd xcall_addr(char *cmd_str);

int console_init(void);

// ####################################################

#endif /* _TINY_CONSOLE_H_ */
