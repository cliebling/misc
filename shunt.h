#ifndef _SHUNT_H
#define _SHUNT_H

#define MAX_BUFFER_LENGTH 255U

extern "C" float c_postfixEvaluator(char *postfix);
extern "C" int c_infix2postfix(char *infix, char *postfix, size_t buffer_length);

#endif /* _SHUNT_H */
