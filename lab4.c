#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *left, *right;
} Node;

/* Equal values go to the right; unique mode skips them. */
static Node *insert(Node *root, int data, int unique)
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
    if (data < root->data)
        root->left = insert(root->left, data, unique);
    else if (data > root->data || !unique)
        root->right = insert(root->right, data, unique);
    return root;
}

Node *CreateTree(Node *root, int data)
{
    return insert(root, data, 0);
}

Node *CreateTreeUnique(Node *root, int data)
{
    return insert(root, data, 1);
}

const Node *Search(const Node *root, int value)
{
    while (root != NULL && root->data != value)
        root = value < root->data ? root->left : root->right;
    return root;
}

int CountOccurrences(const Node *root, int value)
{
    if (root == NULL) return 0;
    if (value < root->data)
        return CountOccurrences(root->left, value);
    if (value > root->data)
        return CountOccurrences(root->right, value);
    return 1 + CountOccurrences(root->right, value);
}

void print_tree(const Node *root, int level)
{
    if (root == NULL) return;
    print_tree(root->right, level + 1);
    for (int i = 0; i < level; ++i) printf("    ");
    printf("%d\n", root->data);
    print_tree(root->left, level + 1);
}

void FreeTree(Node *root)
{
    if (root == NULL) return;
    FreeTree(root->left);
    FreeTree(root->right);
    free(root);
}

int main(void)
{
    Node *root = NULL;
    int mode, data, value;
    puts("Mode: 1 = allow duplicates, 2 = unique values");
    if (scanf("%d", &mode) != 1 || (mode != 1 && mode != 2)) {
        fputs("Invalid mode.\n", stderr);
        return EXIT_FAILURE;
    }
    puts("Enter integers; -1 finishes tree construction:");
    for (;;) {
        if (scanf("%d", &data) != 1) {
            fputs("Invalid tree input.\n", stderr);
            FreeTree(root);
            return EXIT_FAILURE;
        }
        if (data == -1) break;
        root = mode == 1 ? CreateTree(root, data)
                         : CreateTreeUnique(root, data);
    }
    puts("Tree (right subtree above the root):");
    if (root == NULL) puts("(empty)");
    else print_tree(root, 0);
    puts("Enter the value to search:");
    if (scanf("%d", &value) != 1) {
        fputs("Invalid search input.\n", stderr);
        FreeTree(root);
        return EXIT_FAILURE;
    }
    printf("Search: %s\n", Search(root, value) ? "found" : "not found");
    printf("Occurrences: %d\n", CountOccurrences(root, value));
    FreeTree(root);
    return EXIT_SUCCESS;
}
