#include "differentiator.h"
#include <math.h>
#include <string.h>

// производная константы
TreeNode* DiffNumber()
{
    return CreateNumberNode(0);
}

// переменной
TreeNode* DiffVariable()
{
    return CreateNumberNode(1);
}

// суммы
TreeNode* DiffAdd(TreeNode* node, char var)
{
    TreeNode* left_deriv  = Differentiate(node->left, var);
    TreeNode* right_deriv = Differentiate(node->right, var);
    return CreateOperatorNode(OP_ADD, left_deriv, right_deriv);
}

// разности
TreeNode* DiffSub(TreeNode* node, char var)
{
    TreeNode* left_deriv  = Differentiate(node->left, var);
    TreeNode* right_deriv = Differentiate(node->right, var);
    return CreateOperatorNode(OP_SUB, left_deriv, right_deriv);
}

// произведения 
TreeNode* DiffMul(TreeNode* node, char var)
{
    TreeNode* left_deriv  = Differentiate(node->left, var);
    TreeNode* right_deriv = Differentiate(node->right, var);
    
    TreeNode* term1 = CREATE_MUL(left_deriv, CopySubtree(node->right));
    TreeNode* term2 = CREATE_MUL(CopySubtree(node->left), right_deriv);
    
    return CreateOperatorNode(OP_ADD, term1, term2);
}

// частного
TreeNode* DiffDiv(TreeNode* node, char var)
{
    TreeNode* left_deriv  = Differentiate(node->left, var);
    TreeNode* right_deriv = Differentiate(node->right, var);
    
    TreeNode* numerator1 = CREATE_MUL(left_deriv, CopySubtree(node->right));
    TreeNode* numerator2 = CREATE_MUL(CopySubtree(node->left), right_deriv);
    TreeNode* numerator  = CreateOperatorNode(OP_SUB, numerator1, numerator2);
    
    TreeNode* denominator = CreateOperatorNode(OP_POW, CopySubtree(node->right), CreateNumberNode(2));
    
    return CreateOperatorNode(OP_DIV, numerator, denominator);
}

// степени
TreeNode* DiffPow(TreeNode* node, char var)
{
    if (IsConstant(node->right, var))
    {
        double n = GetConstantValue(node->right);
        TreeNode* power_minus_one = CreateNumberNode(n - 1);
        TreeNode* new_power = CreateOperatorNode(OP_POW, 
                                                 CopySubtree(node->left), 
                                                 power_minus_one);
        TreeNode* multiplier = CreateNumberNode(n);
        return CREATE_MUL(multiplier, new_power);
    }
    
    // (u^v)' = u^v * (v'*ln(u) + v*u'/u)
    TreeNode* u = node->left;
    TreeNode* v = node->right;
    TreeNode* u_copy = CopySubtree(u);
    TreeNode* v_copy = CopySubtree(v);
    
    TreeNode* u_deriv = Differentiate(u, var);
    TreeNode* v_deriv = Differentiate(v, var);
    
    TreeNode* ln_u = CreateFunctionNode("ln", CopySubtree(u));
    TreeNode* v_ln_u = CREATE_MUL(v_deriv, ln_u);
    
    TreeNode* v_u_prime = CREATE_MUL(v_copy, u_deriv);
    TreeNode* v_u_prime_div_u = CreateOperatorNode(OP_DIV, v_u_prime, CopySubtree(u));
    
    TreeNode* inner_sum = CreateOperatorNode(OP_ADD, v_ln_u, v_u_prime_div_u);
    TreeNode* original_pow = CreateOperatorNode(OP_POW, u_copy, v_copy);
    
    return CREATE_MUL(original_pow, inner_sum);
}

// синуса
TreeNode* DiffSin(TreeNode* node, char var)
{
    TreeNode* inner_deriv = Differentiate(node->left, var);
    TreeNode* cos_u = CreateFunctionNode("cos", CopySubtree(node->left));
    return CREATE_MUL(cos_u, inner_deriv);
}

// косинуса
TreeNode* DiffCos(TreeNode* node, char var)
{
    TreeNode* inner_deriv = Differentiate(node->left, var);
    TreeNode* sin_u = CreateFunctionNode("sin", CopySubtree(node->left));
    TreeNode* minus_one = CreateNumberNode(-1);
    TreeNode* minus_sin_u = CREATE_MUL(minus_one, sin_u);
    return CREATE_MUL(minus_sin_u, inner_deriv);
}

// натурального логарифма
TreeNode* DiffLn(TreeNode* node, char var)
{
    TreeNode* inner_deriv = Differentiate(node->left, var);
    return CreateOperatorNode(OP_DIV, inner_deriv, CopySubtree(node->left));
}

// экспоненты
TreeNode* DiffExp(TreeNode* node, char var)
{
    TreeNode* inner_deriv = Differentiate(node->left, var);
    TreeNode* exp_u = CreateFunctionNode("exp", CopySubtree(node->left));
    return CREATE_MUL(exp_u, inner_deriv);
}

// умножения на константу
TreeNode* DiffConstMul(TreeNode* node, char var)
{
    if (IsConstant(node->left, var))
    {
        TreeNode* constant = CopySubtree(node->left);
        TreeNode* inner_deriv = Differentiate(node->right, var);
        return CREATE_MUL(constant, inner_deriv);
    }
    return NULL;
}

// проверка на константу
int IsConstant(TreeNode* node, char var)
{
    if (!node) return 1;
    
    if (node->type == NODE_NUMBER)
    {
        return 1;
    }
    else if (node->type == NODE_VARIABLE)
    {
        return node->value.variable != var;
    }
    else if (node->type == NODE_OPERATOR || node->type == NODE_FUNCTION)
    {
        return IsConstant(node->left, var) && 
               (node->right ? IsConstant(node->right, var) : 1);
    }
    
    return 0;
}

// значение константы
double GetConstantValue(TreeNode* node)
{
    if (node && node->type == NODE_NUMBER)
    {
        return node->value.number;
    }
    return 0;
}


TreeNode* Differentiate(TreeNode* node, char var)
{
    if (!node) return NULL;
    
    switch (node->type)
    {
        case NODE_NUMBER:
            return DiffNumber();
            
        case NODE_VARIABLE:
            return DiffVariable();
            
        case NODE_OPERATOR:
            switch (node->value.op)
            {
                case OP_ADD:
                    return DiffAdd(node, var);
                case OP_SUB:
                    return DiffSub(node, var);
                case OP_MUL:
                    {
                        TreeNode* simple = DiffConstMul(node, var);
                        if (simple) return simple;
                    }
                    return DiffMul(node, var);
                case OP_DIV:
                    return DiffDiv(node, var);
                case OP_POW:
                    return DiffPow(node, var);
                case OP_SIN:
                    return DiffSin(node, var);
                case OP_COS:
                    return DiffCos(node, var);
                case OP_LN:
                    return DiffLn (node, var);
                case OP_EXP:
                    return DiffExp(node, var);
                default:
                    return NULL;
            }
            
        case NODE_FUNCTION:
            if (strcmp(node->value.func_name, "sin") == 0)
            {
                return DiffSin(node, var);
            }
            else if (strcmp(node->value.func_name, "cos") == 0)
            {
                return DiffCos(node, var);
            }
            else if (strcmp(node->value.func_name, "ln") == 0)
            {
                return DiffLn(node, var);
            }
            else if (strcmp(node->value.func_name, "exp") == 0)
            {
                return DiffExp(node, var);
            }
            return NULL;
    }
    
    return NULL;
}