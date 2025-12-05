#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree_functions.h"
#include "tree_dump.h"
#include "differentiator.h"

int main()
{
    Tree* loaded_tree = (Tree*)calloc(1, sizeof(Tree));
    if (!loaded_tree)
    {
        printf("Ошибка выделения памяти для дерева!\n");
        return 1;
    }
    
    TreeErr_t load_result = TreeLoadFromFile(loaded_tree, "tree_save.txt");
    
    if (load_result != TREE_OK)
    {
        printf("Ошибка загрузки дерева из файла!\n");
        free(loaded_tree);
        return 1;
    }
    
    TreeDump(loaded_tree, "function_loaded", 0);
    
    TreeNode* deriv_root = Differentiate(loaded_tree->root, 'x');
    
    if (!deriv_root)
    {
        printf("Error: Не удалось взять производную!\n");
        DestroyTree(loaded_tree);
        return 1;
    }
    
    Tree* deriv_tree = CreateTree(deriv_root);
    TreeDump(deriv_tree, "derivative", 1);
    
    TreeSaveToFile(deriv_tree, "derivative_save.txt");
    
    DestroyTree(loaded_tree);
    DestroyTree(deriv_tree);
    
    return 0;
}


/*
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
    
    printf("Сохранение дерева в файл 'tree_save.txt'...\n");
    TreeErr_t save_result = TreeSaveToFile(func_tree, "tree_save.txt");
    
    if (save_result != TREE_OK)
    {
        printf("Ошибка сохранения дерева в файл!\n");
        DestroyTree(func_tree);
        return 1;
    }
    
    Tree* loaded_tree = (Tree*)calloc(1, sizeof(Tree));
    if (!loaded_tree)
    {
        printf("Ошибка выделения памяти для дерева!\n");
        DestroyTree(func_tree);
        return 1;
    }
    
    TreeErr_t load_result = TreeLoadFromFile(loaded_tree, "tree_save.txt");
    
    if (load_result != TREE_OK)
    {
        printf("Ошибка загрузки дерева из файла!\n");
        DestroyTree(func_tree);
        free(loaded_tree);
        return 1;
    }
    
    TreeDump(loaded_tree, "function_loaded", 0);
    
    TreeNode* deriv_root = Differentiate(loaded_tree->root, 'x');
    
    if (!deriv_root)
    {
        printf("Error: Не удалось взять производную!\n");
        DestroyTree(func_tree);
        DestroyTree(loaded_tree);
        return 1;
    }
    
    Tree* deriv_tree = CreateTree(deriv_root);
    TreeDump(deriv_tree, "derivative", 1);
    
    TreeSaveToFile(deriv_tree, "derivative_save.txt");
    
    DestroyTree(func_tree);
    DestroyTree(loaded_tree);
    DestroyTree(deriv_tree);
    
    return 0;
}
    */