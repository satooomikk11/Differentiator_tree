#ifndef TREE_DUMP_H
#define TREE_DUMP_H

#include "tree_functions.h"

void TreeDump(Tree* tree, const char* filename, int step_number);
void GenerateDotFile(TreeNode* node, FILE* file, int* node_counter);
void SaveTreeToImage(Tree* tree, const char* base_name, int step_number);

#endif // TREE_DUMP_H