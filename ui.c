/*
 * @file ui.c
 * @brief User interface layer
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "ui.h"
#include "vect.h"
#include "storage.h"

#define LINE_LEN   256
#define MAX_TOKENS 8
#define UI_CONTINUE 0
#define UI_QUIT     1

typedef struct {
    int is_scalar;   // 1 for dot product
    double s;
    vect v;
} result;

/* Display help message and prints User info*/

void ui_help(void) {
    printf("minimat - interactive 3D vector calculator\n\n");
    printf("  a = 1 2 3        store a vector (commas or spaces; z optional)\n");
    printf("  a                display a vector\n");
    printf("  a + b            add\n");
    printf("  a - b            subtract\n");
    printf("  a * 2 | 2 * a    scalar multiply\n");
    printf("  a . b            dot product\n");
    printf("  a x b            cross product\n");
    printf("  c = a + b        operate and store the result in c\n");
    printf("  list             show all stored vectors\n");
    printf("  clear            erase all stored vectors\n");
    printf("  quit             exit\n");
    printf("Put spaces around =, +, -, *, . and x. Up to %d vectors.\n", MAX_VECTS);
    printf("WElCOME TO THE MINIMAT VECTOR CALCULATOR! Type 'quit' to exit.\n");
}


static void print_vect(const char *name, const vect *v) {
    printf("%s = %g %g %g\n", name, v->x, v->y, v->z);
}

// Turn commas into spaces, then split on spaces. Returns token count or -1.
static int tokenize(char *line, char *tok[], int max) {
    for (char *p = line; *p; p++) {
        if (*p == ',' || *p == '\t') {
            *p = ' ';
        }
    }

    int n = 0;
    for (char *t = strtok(line, " "); t; t = strtok(NULL, " ")) {
        if (n == max) {
            return -1;
        }
        tok[n++] = t;
    }
    return n;

}

// 1 if the whole token is a valid double
static int is_number(const char *s, double *out) {
    char *end;
    double d = strtod(s, &end);
    if (end == s || *end != '\0') {
        return 0;
    }
    *out = d;
    return 1;
}

static int is_op(const char *s) {
    return strlen(s) == 1 && strchr("+-*.x", s[0]) != NULL;
}

static int valid_name(const char *s) {
    if (strlen(s) >= NAME_LEN || !isalpha((unsigned char)s[0])) {
        return 0;
    }
    for (const char *p = s; *p; p++) {
        if (!isalnum((unsigned char)*p) && *p != '_') {
            return 0;
        }
    }
    return strcmp(s, "list") && strcmp(s, "clear") && strcmp(s, "quit");
}

// Classify an operand: 1 = stored vector (in *v), 2 = number (in *d), 0 = neither
static int operand(const char *tok, vect *v, double *d) {
    if (findvect(tok, v) == 0) {
        return 1;
    }
    if (is_number(tok, d)) {
        return 2;
    }
    return 0;
}

// Returns 0 on success (result filled in), -1 on error (message printed)
static int evaluate(const char *a, char op, const char *b, result *r) {
    vect va, vb;
    double da = 0, db = 0;
    int ka = operand(a, &va, &da);
    int kb = operand(b, &vb, &db);

    if (!ka) { 
        printf("Error: '%s' is not a stored vector or a number\n", a); 
        return -1; 
    }
    if (!kb) { 
        printf("Error: '%s' is not a stored vector or a number\n", b); 
        return -1; 
    }

    r->is_scalar = 0;
    switch (op) {
    case '+':
    case '-':
        if (ka != 1 || kb != 1) { 
            printf("Error: '%c' needs two vectors\n", op); 
            return -1; 
        }
        r->v = (op == '+') ? add(va, vb) : sub(va, vb);
        break;
    case '*':
        if (ka == 1 && kb == 2) {
            r->v = scale(va, db);
        } else if (ka == 2 && kb == 1) {
            r->v = scale(vb, da);
        } else {
            printf("Error: '*' needs one vector and one number (use '.' for dot product)\n");
            return -1;
        }
        break;
    case 'x':
        if (ka != 1 || kb != 1) { 
            printf("Error: cross product needs two vectors\n"); 
            return -1; 
        }
        r->v = cross(va, vb);
        break;
    case '.':
        if (ka != 1 || kb != 1) { 
            printf("Error: dot product needs two vectors\n"); 
            return -1; 
        }
        r->is_scalar = 1;
        r->s = dot(va, vb);
        break;
    }
    return 0;
}

static void store_and_show(const char *name, vect v) {
    strcpy(v.name, name);
    if (addvect(v) != 0) {
        printf("Error: memory full (%d vectors max). Use 'clear' to free space.\n", MAX_VECTS);
        return;
    }
    print_vect(name, &v);
}

static void do_list(void) {
    vect v;
    int count = 0;
    for (int i = 0; i < MAX_VECTS; i++) {
        if (vect_at(i, &v) == 0) {
            print_vect(v.name, &v);
            count++;
        }
    }
    if (count == 0) printf("No vectors stored.\n");
}

static int handle_line(const char *line) {
    char buf[LINE_LEN];                             // buffer so that we can tokanize our code and input.
    char *tok[MAX_TOKENS];
    strncpy(buf, line, sizeof buf - 1);
    buf[sizeof buf - 1] = '\0';

    int n = tokenize(buf, tok, MAX_TOKENS);
    if (n < 0) { 
        printf("Error: expression too long\n"); 
        return UI_CONTINUE; 
    }
    if (n == 0) {
        return UI_CONTINUE;
    }

    // One word: command or display
    if (n == 1) {
        vect v;
        if (strcmp(tok[0], "quit") == 0) {
            return UI_QUIT;
        }
        if (strcmp(tok[0], "list") == 0)  { 
            do_list(); 
            return UI_CONTINUE;
     }
        if (strcmp(tok[0], "clear") == 0) { 
            clearvects(); 
            printf("Vector memory cleared.\n"); 
            return UI_CONTINUE; 
        }
        if (findvect(tok[0], &v) == 0) {
            print_vect(v.name, &v);
        } else {
            printf("Vector '%s' does not exist.\n", tok[0]);
        }
        return UI_CONTINUE;
    }

    // a op b  -> display as ans
    if (n == 3 && is_op(tok[1])) {
        result r;
        if (evaluate(tok[0], tok[1][0], tok[2], &r) == 0) {
            if (r.is_scalar) {
                printf("ans = %g\n", r.s);
            } else {
                print_vect("ans", &r.v);
            }
        }
        return UI_CONTINUE;
    }

    // name = ...
    if (n >= 3 && strcmp(tok[1], "=") == 0) {
        if (!valid_name(tok[0])) {
            printf("Error: invalid vector name '%s' (letters, digits, _; must start with a letter)\n", tok[0]);
            return UI_CONTINUE;
        }

        // c = a op b
        if (n == 5 && is_op(tok[3])) {
            result r;
            if (evaluate(tok[2], tok[3][0], tok[4], &r) != 0) {
                return UI_CONTINUE;
            }
            if (r.is_scalar) {
                printf("Error: dot product gives a scalar and can't be stored in a vector (ans = %g)\n", r.s);
            } else {
                store_and_show(tok[0], r.v);
            }
            return UI_CONTINUE;
        }

        // c = a  (copy)
        if (n == 3) {
            vect v;
            if (findvect(tok[2], &v) != 0) {
                printf("Error: vector '%s' does not exist\n", tok[2]);
            } else {
                store_and_show(tok[0], v);
            }
            return UI_CONTINUE;
        }

        // c = x y [z]
        if (n == 4 || n == 5) {
            vect v = {"", 0, 0, 0};
            double vals[3] = {0, 0, 0};
            int count = n - 2;
            for (int i = 0; i < count; i++) {
                if (!is_number(tok[2 + i], &vals[i])) {
                    printf("Error: '%s' is not a number\n", tok[2 + i]);
                    return UI_CONTINUE;
                }
            }
            v.x = vals[0]; v.y = vals[1]; v.z = vals[2];
            store_and_show(tok[0], v);
            return UI_CONTINUE;
        }
    }

    printf("Error: invalid expression. Type -h at launch for usage.\n");
    return UI_CONTINUE;
}

int ui_run(void) {
    char line[LINE_LEN];
    while (1) {
        printf("minimat> ");
        // Looked up with Google how to read lines better so I can parse them
        if (fgets(line, sizeof line, stdin) == NULL) {
            printf("\n");
            break;
        }

        // Looked up how to remove stuff from strings in C
        line[strcspn(line, "\n")] = '\0';
        if (handle_line(line) == UI_QUIT) break;
    }
    return 0;
}