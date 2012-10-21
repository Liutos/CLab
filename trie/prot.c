/*
 * prot.c
 *
 * A try of implementing the Trie trees.
 *
 * Copyright (C) 2012-10-19 liutos <mat.liutos@gmail.com>
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

enum BOOL {
    TRUE,
    FALSE,
};
typedef struct trieNode {
    int value;
    char key;
    enum BOOL is_used;
    struct trieNode *child;
    struct trieNode *brother;
    struct trieNode *parent;
} *Trie, *TrieNode;

int (*value_modifier)(int, int);

TrieNode make_trie_node(char key, int value, enum BOOL is_used)
{
    TrieNode node = malloc(sizeof(struct trieNode));

    node->key = key;
    node->is_used = is_used;
    node->value = value;
    node->parent = node->child = node->brother = NULL;

    return node;
}

Trie make_init_trie(char *whole_key, int value)
{
    if (NULL == whole_key ||
        '\0' == *whole_key)
        return NULL;
    else if ('\0' == whole_key[1])
        return make_trie_node(whole_key[0], value, TRUE);
    else {
        TrieNode root = make_trie_node(whole_key[0], 0, FALSE);

        root->child = make_init_trie(whole_key + 1, value);
        if (root->child != NULL)
            root->child->parent = root;

        return root;
    }
}

char *make_node_label(TrieNode node)
{
    char *label = malloc(100 * sizeof(char));

    sprintf(label, "<c>|{key: %c| value: %d}|<b>",
            node->key, TRUE == node->is_used? node->value: 0);

    return label;
}

void print_trie_core(Trie tree, FILE *fp)
{
    if (NULL == tree)
        return;
    fprintf(fp, "\tnode%p [label = \"%s\"];\n",
            tree, make_node_label(tree));
    if (tree->child != NULL) {
        print_trie_core(tree->child, fp);
        fprintf(fp, "\tnode%p:c -> node%p;\n",
                tree, tree->child);
    }
    if (tree->brother != NULL) {
        print_trie_core(tree->brother, fp);
        fprintf(fp, "\tnode%p:b -> node%p;\n",
                tree, tree->brother);
    }
}

void print_trie_compile(Trie tree)
{
    FILE *fp = fopen("trie.dot", "w");

    if (NULL == fp) {
        printf("Can not open file `trie.dot'.\n");
        exit(1);
    }
    fprintf(fp, "digraph G {\n\tnode [shape = record];\n");
    print_trie_core(tree, fp);
    fprintf(fp, "}");
}

int modify_value(int old_value, int new_value)
{
    return old_value + 1;
}

Trie insert_core(char *whole_key, int value, Trie tree)
{
    if (NULL == tree || '\0' == *whole_key)
        return make_init_trie(whole_key, value);
    if ('\0' == whole_key[1] && whole_key[0] == tree->key) {
        if (TRUE == tree->is_used) {
            printf("The value of the node with key %c increasing.\n", tree->key);
            tree->value = value_modifier(tree->value, value);
        } else {
            tree->is_used = TRUE;
            tree->value = value;
        }

        return tree;
    }
    if (whole_key[0] == tree->key) {
        tree->child = insert_core(whole_key + 1, value, tree->child);
        if (tree->child != NULL)
            tree->child->parent = tree;
    } else {
        tree->brother = insert_core(whole_key, value, tree->brother);
        if (tree->brother != NULL)
            tree->brother->parent = tree;
    }

    return tree;
}

Trie insert_string_int(char *whole_key, int value, Trie tree)
{
    return insert_core(whole_key, value, tree);
}

char **read_ips(void)
{
    FILE *fp;
    char **ips = malloc(40 * sizeof(char *));
    int i;

    if (NULL == (fp = fopen("ips.txt", "r"))) {
        printf("Can not read file `ips.txt'.\n");
        exit(1);
    }
    for (i = 0; i < 40; i++) {
        char *line = malloc(20 * sizeof(char));
        size_t n = 20;
        int tmp;
        tmp = getline(&line, &n, fp);
        if ('\n' == line[tmp - 1])
            line[tmp - 1] = '\0';
        ips[i] = line;
    }

    return ips;
}

TrieNode find_max_value(Trie tree)
{
    return NULL;
}

int main(int argc, char *argv[])
{
    Trie tree = NULL;
    int i;
    /* int len = 40; */
    /* char **keys = read_ips(); */
    char *keys[] = {
        "abc",
        "b",
        "abd",
        "bcd",
        "abcd",
        "efg",
        "hii",
    };
    int len = sizeof(keys) / sizeof(char *);

    value_modifier = modify_value;
    for (i = 0; i < len; i++)
        tree = insert_string_int(keys[i], 1, tree);
    print_trie_compile(tree);

    return 0;
}
