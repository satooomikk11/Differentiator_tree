#ifndef TREE_FUNCTIONS_H
#define TREE_FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

enum NodeType
{
    NODE_NUMBER,
    NODE_VARIABLE,
    NODE_OPERATOR,
    NODE_FUNCTION
};

enum Operator
{
    OP_ADD,     // +
    OP_SUB,     // -
    OP_MUL,     // *
    OP_DIV,     // /
    OP_POW,     // ^
    OP_SIN,     // sin
    OP_COS,     // cos
    OP_LN,      // ln
    OP_EXP      // exp
};

struct TreeNode
{
    NodeType type;
    // в зависимости от типа данных
    union
    {
        double number;
        char variable;
        Operator op;
        char* func_name;
    } value;
    struct TreeNode* left;
    struct TreeNode* right;
};

struct Tree
{
    TreeNode* root;
    size_t size;
};

// коды ошибок
typedef enum
{
    TREE_OK = 0,
    TREE_ERR_FILE,
    TREE_ERR_MEMORY,
    TREE_ERR_PARSE
} TreeErr_t;

// всё что связано с узлами
TreeNode* CreateNumberNode(double value);
TreeNode* CreateVariableNode(char variable);
TreeNode* CreateOperatorNode(Operator op, TreeNode* left, TreeNode* right);
TreeNode* CreateFunctionNode(const char* func_name, TreeNode* arg);

Tree* CreateTree(TreeNode* root);

// копирование поддерева
TreeNode* CopySubtree(TreeNode* node);

// вся фигня с файловой записью
TreeErr_t TreeSaveToFile(Tree* tree, const char* filename);
TreeErr_t TreeSaveNodeToFile(TreeNode* node, FILE* filestream);
TreeErr_t TreeLoadFromFile(Tree* tree, const char* filename);
TreeErr_t CreateTreeFromStr(Tree* tree, const char* str);
TreeNode* CreateNodeFromStr(const char** str_ptr);
void      SkipSpaces(const char** str_ptr);
size_t    CountNodes(TreeNode* node);

void DestroyTree(Tree* tree);
void DestroyTreeNode(TreeNode* node);

int VerifyTree(Tree* tree);

#endif // TREE_FUNCTIONS_H