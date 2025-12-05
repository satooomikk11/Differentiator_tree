#include "tree_functions.h"
#include <ctype.h>

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

// сохраняет дерево в файл
TreeErr_t TreeSaveToFile(Tree* tree, const char* filename)
{
    assert(tree);
    assert(filename);

    FILE* file = fopen(filename, "w");
    if (file == NULL)
    {
        return TREE_ERR_FILE;
    }

    TreeErr_t result = TreeSaveNodeToFile(tree->root, file);
    fclose(file);
    
    return result;
}

// сохраняет узел в файл (рекурсивно)
TreeErr_t TreeSaveNodeToFile(TreeNode* node, FILE* filestream)
{
    assert(filestream);
    
    if (node == NULL)
    {
        fprintf(filestream, "()");
        return TREE_OK;
    }

    fprintf(filestream, "(");
    
    // сохраняем тип и значение узла
    switch (node->type)
    {
        case NODE_NUMBER:
            fprintf(filestream, "%lf", node->value.number);
            break;
        case NODE_VARIABLE:
            fprintf(filestream, "%c", node->value.variable);
            break;
        case NODE_OPERATOR:
            {
                const char* op_str = NULL;
                switch (node->value.op)
                {
                    case OP_ADD: op_str = "+";   break;
                    case OP_SUB: op_str = "-";   break;
                    case OP_MUL: op_str = "*";   break;
                    case OP_DIV: op_str = "/";   break;
                    case OP_POW: op_str = "**";  break;
                    case OP_SIN: op_str = "SIN"; break;
                    case OP_COS: op_str = "COS"; break;
                    case OP_LN:  op_str = "LN";  break;
                    case OP_EXP: op_str = "EXP"; break;
                    default:     op_str = "UNKNOWN"; break;
                }
                fprintf(filestream, "%s", op_str);
            }
            break;
        case NODE_FUNCTION:
            fprintf(filestream, "%s", node->value.func_name);
            break;
        default:
            fprintf(filestream, "UNKNOWN");
            break;
    }
    
    // сохраняем левое и правое поддерево
    fprintf(filestream, " ");
    TreeSaveNodeToFile(node->left, filestream);
    
    fprintf(filestream, " ");
    TreeSaveNodeToFile(node->right, filestream);
    
    fprintf(filestream, ")");
    
    return TREE_OK;
}

// загружает дерево из файла
TreeErr_t TreeLoadFromFile(Tree* tree, const char* filename)
{
    assert(tree);
    assert(filename);

    FILE* file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Не смогли открыть файл: %s\n", filename);
        return TREE_ERR_FILE;
    }
    
    // определяем размер файла
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    // выделяем буфер для всего файла
    char* buffer = (char*)calloc(file_size + 1, sizeof(char));
    if (buffer == NULL)
    {
        fclose(file);
        printf("Calloc не сработал :(\n");
        return TREE_ERR_MEMORY;
    }
    
    // читаем весь файл
    size_t bytes_read = fread(buffer, 1, file_size, file);
    buffer[bytes_read] = '\0';
    
    fclose(file);

    // создаем дерево из строки
    TreeErr_t result = CreateTreeFromStr(tree, buffer);
    
    free(buffer);
    
    return result;
}

// создает дерево из строкового представления
TreeErr_t CreateTreeFromStr(Tree* tree, const char* str)
{
    assert(tree);
    assert(str);

    const char* str_ptr = str;
    tree->root = CreateNodeFromStr(&str_ptr);
    
    if (tree->root == NULL)
    {
        printf("Ошибка чтения файла\n");
        return TREE_ERR_PARSE;
    }
    
    tree->size = CountNodes(tree->root);
    
    return TREE_OK;
}

// парсит строку и создает узел дерева
TreeNode* CreateNodeFromStr(const char** str_ptr)
{
    assert(str_ptr);
    
    SkipSpaces(str_ptr);
    
    // проверяем, начинается ли узел с '('
    if (**str_ptr != '(')
    {
        return NULL;
    }
    
    (*str_ptr)++; // пропускаем '('
    SkipSpaces(str_ptr);
    
    // если сразу закрывающая скобка - пустой узел
    if (**str_ptr == ')')
    {
        (*str_ptr)++;
        return NULL;
    }
    
    // читаем тип узла
    char type_str[32] = {0};
    int i = 0;
    while (**str_ptr != ' ' && **str_ptr != '\0' && **str_ptr != ')')
    {
        type_str[i++] = **str_ptr;
        (*str_ptr)++;
    }
    type_str[i] = '\0';
    
    SkipSpaces(str_ptr);
    
    TreeNode* node = (TreeNode*)calloc(1, sizeof(TreeNode));
    if (!node) return NULL;
    
    // обрабатываем разные типы узлов
    if (strcmp(type_str, "NUMBER") == 0)
    {
        node->type = NODE_NUMBER;
        sscanf(*str_ptr, "%lf", &node->value.number);
        // пропускаем число
        while (isdigit(**str_ptr) || **str_ptr == '.' || **str_ptr == '-' || **str_ptr == '+' || **str_ptr == 'e' || **str_ptr == 'E')
        {
            (*str_ptr)++;
        }
    }
    else if (strcmp(type_str, "VARIABLE") == 0)
    {
        node->type = NODE_VARIABLE;
        node->value.variable = **str_ptr;
        (*str_ptr)++;
    }
    else if (strcmp(type_str, "OPERATOR") == 0)
    {
        node->type = NODE_OPERATOR;
        
        char op_str[32] = {0};
        i = 0;
        while (**str_ptr != ' ' && **str_ptr != '\0' && **str_ptr != ')')
        {
            op_str[i++] = **str_ptr;
            (*str_ptr)++;
        }
        op_str[i] = '\0';
        
        if      (strcmp(op_str, "ADD") == 0) node->value.op = OP_ADD;
        else if (strcmp(op_str, "SUB") == 0) node->value.op = OP_SUB;
        else if (strcmp(op_str, "MUL") == 0) node->value.op = OP_MUL;
        else if (strcmp(op_str, "DIV") == 0) node->value.op = OP_DIV;
        else if (strcmp(op_str, "POW") == 0) node->value.op = OP_POW;
        else if (strcmp(op_str, "SIN") == 0) node->value.op = OP_SIN;
        else if (strcmp(op_str, "COS") == 0) node->value.op = OP_COS;
        else if (strcmp(op_str, "LN")  == 0) node->value.op = OP_LN;
        else if (strcmp(op_str, "EXP") == 0) node->value.op = OP_EXP;
    }
    else if (strcmp(type_str, "FUNCTION") == 0)
    {
        node->type = NODE_FUNCTION;
        
        char func_str[32] = {0};
        i = 0;
        while (**str_ptr != ' ' && **str_ptr != '\0' && **str_ptr != ')')
        {
            func_str[i++] = **str_ptr;
            (*str_ptr)++;
        }
        func_str[i] = '\0';
        
        node->value.func_name = strdup(func_str);
    }
    else
    {
        free(node);
        return NULL;
    }
    
    SkipSpaces(str_ptr);
    
    // рекурсивно читаем левое и правое поддерево
    node->left = CreateNodeFromStr(str_ptr);
    SkipSpaces(str_ptr);
    node->right = CreateNodeFromStr(str_ptr);
    
    SkipSpaces(str_ptr);
    
    // проверяем закрывающую скобку
    if (**str_ptr == ')')
    {
        (*str_ptr)++;
    }
    else
    {
        DestroyTreeNode(node);
        return NULL;
    }
    
    return node;
}

void SkipSpaces(const char** str_ptr)
{
    while (**str_ptr == ' ' || **str_ptr == '\t' || **str_ptr == '\n' || **str_ptr == '\r')
    {
        (*str_ptr)++;
    }
}

size_t CountNodes(TreeNode* node)
{
    if (node == NULL)
    {
        return 0;
    }
    
    return 1 + CountNodes(node->left) + CountNodes(node->right);
}