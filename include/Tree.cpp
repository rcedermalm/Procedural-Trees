#include "Tree.h"

Tree::~Tree() {
    destroyNode(root);
}

void Tree::destroyNode(Node *node) {
    if (node) {
        destroyNode(node->leftBranch);
        destroyNode(node->centerBranch);
        destroyNode(node->rightBranch);
        delete node;
    }
}



