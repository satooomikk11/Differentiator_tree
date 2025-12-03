#include "tree_functions.h"

// создание числового узла
TreeNode* CreateNumberNode(double value)
{
    TreeNode* node = (TreeNode*)calloc(1, sizeof(TreeNode));
    if (!node) return NULL;
    
    node->type = NODE_NUMBER;
    node->value.number = value;
    node->left  = NULL;
    node->right = NULL;
    
    return node;
}

// создание узла переменной
TreeNode* CreateVariableNode(char variable)
{
    TreeNode* node = (TreeNode*)calloc(1, sizeof(TreeNode));
    if (!node) return NULL;
    
    node->type = NODE_VARIABLE;
    node->value.variable = variable;
    node->left  = NULL;
    node->right = NULL;
    
    return node;
}

// создание узла с опрератором (типа плюс, минус)
TreeNode* CreateOperatorNode(Operator op, TreeNode* left, TreeNode* right)
{
    TreeNode* node = (TreeNode*)calloc(1, sizeof(TreeNode));
    if (!node) return NULL;
    
    node->type = NODE_OPERATOR;
    node->value.op = op;
    node->left  = left;
    node->right = right;
    
    return node;
}

// создание узла с функцией
TreeNode* CreateFunctionNode(const char* func_name, TreeNode* arg)
{
    TreeNode* node = (TreeNode*)calloc(1, sizeof(TreeNode));
    if (!node) return NULL;
    
    node->type = NODE_FUNCTION;
    node->value.func_name = strdup(func_name);
    node->left  = arg;  // например синус Х, там будет Х
    node->right = NULL;
    
    return node;
}

// создание дерева
Tree* CreateTree(TreeNode* root)
{
    Tree* tree = (Tree*)calloc(1, sizeof(Tree));
    if (!tree) return NULL;
    
    tree->root = root;
    tree->size = 1;
    
    return tree;
}

TreeNode* CopySubtree(TreeNode* node)
{
    if (!node) return NULL;
    
    TreeNode* new_node = (TreeNode*)calloc(1, sizeof(TreeNode));
    if (!new_node) return NULL;
    
    new_node->type = node->type;
    
    switch (node->type)
    {
        case NODE_NUMBER:
            new_node->value.number = node->value.number;
            break;
        case NODE_VARIABLE:
            new_node->value.variable = node->value.variable;
            break;
        case NODE_OPERATOR:
            new_node->value.op = node->value.op;
            break;
        case NODE_FUNCTION:
            new_node->value.func_name = strdup(node->value.func_name);
            break;
        default:
            free(new_node);
            return NULL;
    }
    
    new_node->left  = CopySubtree(node->left);
    new_node->right = CopySubtree(node->right);
    
    return new_node;
}

void DestroyTreeNode(TreeNode* node)
{
    if (!node) return;
    
    DestroyTreeNode(node->left);
    DestroyTreeNode(node->right);
    
    if (node->type == NODE_FUNCTION && node->value.func_name)
    {
        free(node->value.func_name);
    }
    
    free(node);
}

void DestroyTree(Tree* tree)
{
    if (!tree) return;
    
    DestroyTreeNode(tree->root);
    free(tree);
}

int VerifyTree(Tree* tree)
{
    // todo: написать нормальный верификатор
    return tree && tree->root;
}