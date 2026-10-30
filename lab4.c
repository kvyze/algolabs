#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    char data;
    struct Node *left, *right;
} Node;

/* Equal values go to the right; unique mode skips them.
   Comparison is by ASCII code. */
static Node *insert(Node *root, char data, int unique)
{
    if (root == NULL) {
        Node *node = malloc(sizeof *node);
        if (node == NULL) {
            fputs("Memory allocation failed.\n", stderr);
            exit(EXIT_FAILURE);
        }
        *node = (Node){data, NULL, NULL};
        return node;
    }
    if ((unsigned char)data < (unsigned char)root->data)
        root->left = insert(root->left, data, unique);
    else if ((unsigned char)data > (unsigned char)root->data || !unique)
        root->right = insert(root->right, data, unique);
    return root;
}

Node *CreateTree(Node *root, char data)
{
    return insert(root, data, 0);
}

Node *CreateTreeUnique(Node *root, char data)
{
    return insert(root, data, 1);
}

const Node *Search(const Node *root, char value)
{
    while (root != NULL && root->data != value)
        root = (unsigned char)value < (unsigned char)root->data
                   ? root->left : root->right;
    return root;
}

int CountOccurrences(const Node *root, char value)
{
    if (root == NULL) return 0;
    if ((unsigned char)value < (unsigned char)root->data)
        return CountOccurrences(root->left, value);
    if ((unsigned char)value > (unsigned char)root->data)
        return CountOccurrences(root->right, value);
    return 1 + CountOccurrences(root->right, value);
}

void print_tree(const Node *root, int level)
{
    if (root == NULL) return;
    print_tree(root->right, level + 1);
    for (int i = 0; i < level; ++i) printf("    ");
    /* Печатаем сам символ и его ASCII-код */
    if (root->data == ' ')
        printf("' ' (%d)\n", (unsigned char)root->data);
    else if (root->data == '\n')
        printf("'\\n' (%d)\n", (unsigned char)root->data);
    else if (root->data == '\t')
        printf("'\\t' (%d)\n", (unsigned char)root->data);
    else
        printf("'%c' (%d)\n", root->data, (unsigned char)root->data);
    print_tree(root->left, level + 1);
}

void FreeTree(Node *root)
{
    if (root == NULL) return;
    FreeTree(root->left);
    FreeTree(root->right);
    free(root);
}

/* Пропускаем оставшиеся символы в строке ввода */
static void skip_line(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

int main(void)
{
    Node *root = NULL;
    int mode;
    char data, value;

    puts("Mode: 1 = allow duplicates, 2 = unique values");
    if (scanf("%d", &mode) != 1 || (mode != 1 && mode != 2)) {
        fputs("Invalid mode.\n", stderr);
        return EXIT_FAILURE;
    }
    skip_line();

    puts("Enter ASCII characters, one per line; enter '!' alone to finish:");
    for (;;) {
        int c = getchar();
        if (c == EOF) {
            fputs("Unexpected end of input.\n", stderr);
            FreeTree(root);
            return EXIT_FAILURE;
        }
        /* Считываем первый символ строки; если это '!' и за ним сразу '\n' — конец */
        if (c == '!') {
            int next = getchar();
            if (next == '\n' || next == EOF) {
                if (next == EOF) skip_line();
                break;
            }
            /* иначе '!' — обычный символ, а next — часть ввода; обработаем оба */
            data = (char)c;
            root = mode == 1 ? CreateTree(root, data) : CreateTreeUnique(root, data);
            if (next != '\n')
                ungetc(next, stdin);
            skip_line();
            continue;
        }
        /* Если перевод строки — пропускаем пустые строки */
        if (c == '\n') continue;

        data = (char)c;
        root = mode == 1 ? CreateTree(root, data) : CreateTreeUnique(root, data);
        skip_line();
    }

    puts("Tree (right subtree above the root) — char and its ASCII code:");
    if (root == NULL) puts("(empty)");
    else print_tree(root, 0);

    puts("Enter the character to search:");
    int sc = getchar();
    if (sc == EOF) {
        fputs("Invalid search input.\n", stderr);
        FreeTree(root);
        return EXIT_FAILURE;
    }
    value = (char)sc;
    skip_line();

    printf("Search: %s\n", Search(root, value) ? "found" : "not found");
    printf("Occurrences: %d\n", CountOccurrences(root, value));
    FreeTree(root);
    return EXIT_SUCCESS;
}