#ifndef DIFFERENTIATOR_H
#define DIFFERENTIATOR_H

#include "tree_functions.h"

#define CREATE_MUL(left, right) CreateOperatorNode(OP_MUL, left, right)

// базовые функции дифференцирования
TreeNode* DiffNumber();
TreeNode* DiffVariable();
TreeNode* DiffAdd(TreeNode* node, char var);
TreeNode* DiffSub(TreeNode* node, char var);
TreeNode* DiffMul(TreeNode* node, char var);
TreeNode* DiffDiv(TreeNode* node, char var);
TreeNode* DiffPow(TreeNode* node, char var);
TreeNode* DiffSin(TreeNode* node, char var);
TreeNode* DiffCos(TreeNode* node, char var);
TreeNode* DiffLn (TreeNode* node, char var);
TreeNode* DiffExp(TreeNode* node, char var);

TreeNode* DiffConstMul(TreeNode* node, char var);

TreeNode* Differentiate(TreeNode* node, char var);

int    IsConstant(TreeNode* node, char var);
double GetConstantValue(TreeNode* node);

#endif // DIFFERENTIATOR_H