/*
 * prototype.c
 *
 *
 *
 * Copyright (C) 2012-10-17 liutos mat.liutos@gmail.com
 */
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

typedef struct node {
    void *value;
    /* char *value; */
    int height;
    int bf;                     /* balance factor */
    struct node *parent, *left, *right;
} *AVLTree;

int (*node_comparator)(struct node *node1, struct node *node2); /* Compares two nodes. If equal, returns zero; If node1 is greater then node2, returns a positive integer; otherwise, returns a negative integer. */
char *(*node_name_maker)(struct node *);                        /* For generating a string represents a node. */
char *(*node_value_maker)(struct node *);                       /* For generating a string represents the value of a node. */

struct node *make_node(void *value)
{
    struct node *node = malloc(sizeof(struct node));

    node->value = value;
    node->height = 0;
    node->parent = node->left = node->right = NULL;

    return node;
}

void free_node(struct node *node)
{
    free(node);
}

int get_tree_height(AVLTree tree)
{
    return NULL == tree? -1: tree->height;
}

int get_max(int a, int b)
{
    return a > b? a: b;
}

int compute_tree_height(AVLTree tree)
{
    return get_max(get_tree_height(tree->left),
                   get_tree_height(tree->right)) + 1;
}

int compute_tree_bf(AVLTree tree)
{
    return get_tree_height(tree->left) - get_tree_height(tree->right);
}

AVLTree left_rotate(AVLTree tree)
{
    AVLTree tmp;

    tmp = tree->right;
    tree->right = tmp->left;
    tmp->left = tree;
    tmp->bf = 0;
    tree->bf = 0;
    tree->height = compute_tree_height(tree); /* Update the tree->height first. */
    tmp->height = compute_tree_height(tmp);   /* Update the height of the root of
                                                 the new tree. */

    return tmp;
}

AVLTree right_rotate(AVLTree tree)
{
    AVLTree tmp;

    tmp = tree->left;
    tree->left = tmp->right;
    tmp->right = tree;
    tmp->bf = 0;
    tree->bf = 0;
    tree->height = compute_tree_height(tree); /* The same as the left_rotate. */
    tmp->height = compute_tree_height(tmp);

    return tmp;
}

AVLTree insert_node(struct node *node, AVLTree tree)
{
    if (NULL == tree)
        return node;
    if (0 == node_comparator(node, tree)) {
        printf("%s already exists.\n", node_value_maker(node));

        return tree;
    }
    if (node_comparator(node, tree) > 0) {
        /* Insert into the right subtree. */
        tree->right = insert_node(node, tree->right);
        tree->right->parent = tree;
        tree->height = compute_tree_height(tree);
        tree->bf = compute_tree_bf(tree);
        /* If both tree->bf and tree->right->bf are negative, it's the case of RR. */
        if (-2 == tree->bf && -1 == tree->right->bf)
            tree = left_rotate(tree);
        /* If tree->bf is negative and tree->right->bf is positive, it's the case of RL. */
        else if (-2 == tree->bf && 1 == tree->right->bf) {
            tree->right = right_rotate(tree->right);
            tree = left_rotate(tree);
        }
    } else if (node_comparator(node, tree) < 0 ) {
        /* Insert into the left subtree. */
        tree->left = insert_node(node, tree->left);
        tree->left->parent = tree;
        tree->height = compute_tree_height(tree);
        tree->bf = compute_tree_bf(tree);
        /* If both tree->bf and tree->left->bf are positive, it's the case of LL. */
        if (2 == tree->bf && 1 == tree->left->bf)
            tree = right_rotate(tree);
        /* If tree->bf is positive and tree->left->bf is negative, it's the case of LR. */
        else if (2 == tree->bf && -1 == tree->left->bf) {
            tree->left = left_rotate(tree->left);
            tree = right_rotate(tree);
        }
    }

    return tree;
}

/* Free the memory of a AVLTree in postorder. */
void free_tree(AVLTree tree)
{
    struct node *node;

    while (tree != NULL) {
        free_tree(tree->left);
        node = tree;
        tree = tree->right;
        free_node(node);
    }
}

void draw_tree_core(AVLTree tree, FILE *fp)
{
    if (NULL)
        return;
    if (tree->left != NULL) {
        fprintf(fp, "\t%s:l -> %s;\n", node_name_maker(tree), node_name_maker(tree->left));
        draw_tree_core(tree->left, fp);
    }
    if (tree->right != NULL) {
        fprintf(fp, "\t%s:r -> %s;\n", node_name_maker(tree), node_name_maker(tree->right));
        draw_tree_core(tree->right, fp);
    }
}

void gen_node_description(struct node *node, FILE *fp)
{
    fprintf(fp, "\t%s [label = \"<l> |{<v> value: %s| height: %d| bf: %d}|<r>\"];\n", node_name_maker(node), node_value_maker(node), node->height, node->bf);
}

void gen_tree_description(AVLTree tree, FILE *fp)
{
    if (NULL == tree) return;
    gen_tree_description(tree->left, fp);
    gen_tree_description(tree->right, fp);
    gen_node_description(tree, fp);
}

void draw_tree(AVLTree tree)
{
    FILE *fp = fopen("tree.dot", "w");

    if (NULL == fp) {
        printf("Can not open file `tree.dot'.\n");
        exit(1);
    }
    fprintf(fp, "digraph G {\n\tnode [shape = record];\n");
    gen_tree_description(tree, fp);
    draw_tree_core(tree, fp);
    fprintf(fp, "}");
}

/* Used when the value is of type `int'. */
int node_num_compare(struct node *node1, struct node *node2)
{
    return node1->value - node2->value;
}

/* The string version. */
int node_str_compare(struct node *node1, struct node *node2)
{
    return strcmp(node1->value, node2->value);
}

/* Used when the value is of type `int'. */
char *node_num_label(struct node *node)
{
    char *label = malloc(10 * sizeof(char));

    sprintf(label, "node%d", (int)node->value);

    return label;
}

/* The string version. */
char *node_str_label(struct node *node)
{
    return (char *)node->value;
}

/* The string version. */
char *node_str_value(struct node *node)
{
    return (char *)node->value;
}

/* Used when the value is of type `int'. */
char *node_num_value(struct node *node)
{
    char *value = malloc(10 * sizeof(char));

    sprintf(value, "%d", (int)node->value);

    return value;
}

void preorder_traversal_avl(AVLTree tree)
{
    if (NULL == tree)
        return;
    else {
        printf("%d", (int)tree->value);
        preorder_traversal_avl(tree->left);
        preorder_traversal_avl(tree->right);
    }
}

void inorder_traversal_avl(AVLTree tree)
{
    if (NULL == tree)
        return;
    else {
        inorder_traversal_avl(tree->left);
        printf("%d", (int)tree->value);
        inorder_traversal_avl(tree->right);
    }
}

int main(int argc, char *argv[])
{
    /* char *datum[] = { */
    /*     "ls", */
    /*     "mkdir", */
    /*     "rm", */
    /*     "dot", */
    /*     "sbcl", */
    /*     "fg", */
    /*     "bg", */
    /*     "ghci", */
    /*     "ocamlc", */
    /* }; */
    int datum[] = {1, 2, 3, 4, 5, 8, 7, 6, 9};
    int datum_len = sizeof(datum) / sizeof(int);
    int i;
    AVLTree tree = NULL;
    struct node *node;

    node_comparator = node_num_compare;
    node_name_maker = node_num_label;
    node_value_maker = node_num_value;

    /* node_comparator = node_str_compare; */
    /* node_name_maker = node_str_label; */
    /* node_value_maker = node_str_value; */
    for (i = 0; i < datum_len; i++) {
        node = make_node((void *)datum[i]);
        tree = insert_node(node, tree);
    }
    /* draw_tree(tree); */
    inorder_traversal_avl(tree);
    printf("\n");
    free_tree(tree);

    return 0;
}
