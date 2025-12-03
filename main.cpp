#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree_functions.h"
#include "tree_dump.h"
#include "differentiator.h"

TreeNode* ParseExpression();

// todo: нормальный парсинг
TreeNode* ParseExpression()
{
    // 2*x^4
    TreeNode* two = CreateNumberNode(2);
    TreeNode* x1 = CreateVariableNode('x');
    TreeNode* three1 = CreateNumberNode(4);
    TreeNode* x_pow3 = CreateOperatorNode(OP_POW, x1, three1);
    TreeNode* term1  = CreateOperatorNode(OP_MUL, two, x_pow3);
    
    // 5*x^2
    TreeNode* five = CreateNumberNode(5);
    TreeNode* x2 = CreateVariableNode('x');
    TreeNode* two2 = CreateNumberNode(2);
    TreeNode* x_pow2 = CreateOperatorNode(OP_POW, x2, two2);
    TreeNode* term2  = CreateOperatorNode(OP_MUL, five, x_pow2);
    
    // 3*x
    TreeNode* three2 = CreateNumberNode(3);
    TreeNode* x3 = CreateVariableNode('x');
    TreeNode* term3 = CreateOperatorNode(OP_MUL, three2, x3);
    
    // 7
    TreeNode* seven = CreateNumberNode(7);
    
    // ((2*x^3) + (5*x^2)) - (3*x)) + 7
    TreeNode* sum1 = CreateOperatorNode(OP_ADD, term1, term2);
    TreeNode* sum2 = CreateOperatorNode(OP_SUB, sum1, term3);
    TreeNode* result = CreateOperatorNode(OP_ADD, sum2, seven);
    
    return result;
}

int main()
{
    TreeNode* func_root = ParseExpression();
    Tree* func_tree = CreateTree(func_root);
    
    if (!func_tree || !func_tree->root)
    {
        printf("Error: Не получилось создать дерево выражения!\n");
        return 1;
    }
    
    // сначала дамп начального выражения
    TreeDump(func_tree, "function", 0);
    
    TreeNode* deriv_root = Differentiate(func_root, 'x');
    
    if (!deriv_root)
    {
        printf("Error: Не удалось чота производную взять!\n");
        DestroyTree(func_tree);
        return 1;
    }
    
    // создаем дерево производной
    Tree* deriv_tree = CreateTree(deriv_root);
    
    if (!deriv_tree)
    {
        printf("Error: Не получилось создать дерево производной!\n");
        DestroyTree(func_tree);
        return 1;
    }
    
    // дампим результат
    TreeDump(deriv_tree, "derivative_raw", 1);

    // Освобождаем память
    DestroyTree(func_tree);
    DestroyTree(deriv_tree);
    
    return 0;
}