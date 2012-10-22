/*
 * prototype.c
 *
 * Search a binary tree and tell the result and its depth.
 *
 * Copyright (C) 2012-10-22 liutos <mat.liutos@gmail.com>
 */
#include <stdlib.h>
#include <stdio.h>

typedef struct treeNode {
    char value;
    struct treeNode *left, *right;
} *TreeNode, *Tree;

typedef struct queue {
    int head_index;
    int tail_index;
    TreeNode content[10];
} *Queue;

TreeNode make_node(char value, TreeNode left, TreeNode right)
{
    TreeNode node = malloc(sizeof(struct treeNode));

    node->value = value;
    node->left = left;
    node->right = right;

    return node;
}

void print_tree_aux(Tree tree)
{
    if (NULL == tree)
        return;
    print_tree_aux(tree->left);
    putchar(tree->value);
    print_tree_aux(tree->right);
}

void print_tree(Tree tree)
{
    print_tree_aux(tree);
    putchar('\n');
}

Queue make_queue(void)
{
    Queue queue = malloc(sizeof(struct queue));

    queue->head_index = -1;
    queue->tail_index = 0;

    return queue;
}

void enqueue(Queue queue, TreeNode node)
{
    queue->content[queue->tail_index] = node;
    queue->tail_index++;
}

TreeNode dequeue(Queue queue)
{
    queue->head_index++;

    return queue->content[queue->head_index];
}

void print_queue(Queue queue)
{
    int i;

    for (i = queue->head_index + 1; i < queue->tail_index; i++)
        putchar(queue->content[i]->value);
    putchar('\n');
}

int is_queue_empty(Queue queue)
{
    return queue->head_index >= queue->tail_index;
}

TreeNode dfs_search_tree_aux(char target, int depth, int *nd, Queue queue, int current_count, int next_count)
{
    TreeNode node;

    if (is_queue_empty(queue))
        return NULL;
    node = dequeue(queue);
    current_count--;            /* Decrease the number of the rest nodes in
                                   the same depth. */
    if (node->value == target) {
        *nd = depth;

        return node;
    }
    if (0 == current_count) {
        depth++;
        current_count = next_count;
        next_count = 0;
    }
    if (node->left != NULL) {
        enqueue(queue, node->left);
        next_count++;
    }
    if (node->right != NULL) {
        enqueue(queue, node->right);
        next_count++;
    }

    return dfs_search_tree_aux(target, depth, nd, queue, current_count, next_count);
}

TreeNode dfs_search_tree(Tree tree, char target, int *depth)
{
    Queue queue;

    if (NULL == tree)
        return NULL;
    queue = make_queue();
    enqueue(queue, tree);

    return dfs_search_tree_aux(target, 0, depth, queue, 1, (NULL == tree->left? 0: 1) + (NULL == tree->right? 0: 1));
}

int main(int argc, char *argv[])
{
    TreeNode n5 = make_node('3', NULL, NULL);
    TreeNode n4 = make_node('4', NULL, NULL);
    TreeNode n3 = make_node('2', NULL, NULL);
    TreeNode n2 = make_node('3', n4, n5);
    TreeNode n1 = make_node('1', n2, n3);
    /* Queue queue = make_queue(); */
    TreeNode result;
    int depth;

    /* print_tree(n1); */
    /* enqueue(queue, n1); */
    /* enqueue(queue, n2); */
    /* enqueue(queue, n3); */
    /* enqueue(queue, n4); */
    /* enqueue(queue, n5); */
    /* print_queue(queue); */
    result = dfs_search_tree(n1, '3', &depth);
    if (result != NULL)
        printf("%p with depth %d.\n", result, depth); /* The depth should be
                                                         1. */

    return 0;
}
