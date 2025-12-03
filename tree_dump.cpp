#include "tree_dump.h"
#include <time.h>
#include <limits.h>
#include <stdlib.h>

const int COMMAND_BUFFER_SIZE = 1024;
const int FILE_NAME_SIZE      = 256;

void GenerateDotFile(TreeNode* node, FILE* file, int* node_counter)
{
    if (!node || !file || !node_counter) return;
    
    int current_id = (*node_counter)++;
    
    fprintf(file, "    node%d [label=\"", current_id);
    
    switch (node->type)
    {
        case NODE_NUMBER:
            fprintf(file, "%.2f", node->value.number);
            break;
        case NODE_VARIABLE:
            fprintf(file, "%c", node->value.variable);
            break;
        case NODE_OPERATOR:
            switch (node->value.op)
            {
                case OP_ADD: fprintf(file, "+");   break;
                case OP_SUB: fprintf(file, "-");   break;
                case OP_MUL: fprintf(file, "×");   break;
                case OP_DIV: fprintf(file, "÷");   break;
                case OP_POW: fprintf(file, "^");   break;
                case OP_SIN: fprintf(file, "sin"); break;
                case OP_COS: fprintf(file, "cos"); break;
                case OP_LN:  fprintf(file, "ln");  break;
                case OP_EXP: fprintf(file, "exp"); break;
                default:     fprintf(file, "?");   break;
            }
            break;
        case NODE_FUNCTION:
            fprintf(file, "%s", node->value.func_name);
            break;
        default:
            fprintf(file, "?");
            break;
    }
    
    fprintf(file, "\"];\n");
    
    if (node->left)
    {
        int left_id = *node_counter;
        GenerateDotFile(node->left, file, node_counter);
        fprintf(file, "    node%d -> node%d;\n", current_id, left_id);
    }
    
    if (node->right)
    {
        int right_id = *node_counter;
        GenerateDotFile(node->right, file, node_counter);
        fprintf(file, "    node%d -> node%d;\n", current_id, right_id);
    }
}

void SaveTreeToImage(Tree* tree, const char* base_name, int step_number)
{
    if (!tree || !base_name) return;
    
    char dot_filename[FILE_NAME_SIZE];
    char png_filename[FILE_NAME_SIZE];
    
    snprintf(dot_filename, sizeof(dot_filename) - 1, "step_%02d.dot", step_number);
    snprintf(png_filename, sizeof(png_filename) - 1, "step_%02d.png", step_number);
    
    FILE* dot_file = fopen(dot_filename, "w");
    if (!dot_file) return;
    
    fprintf(dot_file, "digraph DifferentiationTree {\n");
    fprintf(dot_file, "    node [shape=circle, style=filled, fillcolor=lightblue];\n");
    
    int node_counter = 0;
    GenerateDotFile(tree->root, dot_file, &node_counter);
    
    fprintf(dot_file, "}\n");
    fclose(dot_file);
    
    char* safe_command = (char*)malloc(COMMAND_BUFFER_SIZE);
    if (safe_command)
    {
        snprintf(safe_command, COMMAND_BUFFER_SIZE, "dot -Tpng %s -o %s", dot_filename, png_filename);
        system(safe_command);
        free(safe_command);
    }
    
    remove(dot_filename);
}

void TreeDump(Tree* tree, const char* filename, int step_number)
{
    SaveTreeToImage(tree, filename, step_number);
}