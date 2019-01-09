
#ifndef PROCEDURAL_TREES_TREESTRUCTURE_H
#define PROCEDURAL_TREES_TREESTRUCTURE_H

/********************************************************/
/********************** Struct Node *********************/
/********************************************************/

struct Node {
    Node(Node* parentIn)
            : parent(parentIn), leftBranch(nullptr), centerBranch(nullptr), rightBranch(nullptr) {}

    bool isLeaf(){
        return leftBranch == nullptr && centerBranch == nullptr && rightBranch == nullptr;
    }

    Node* parent;
    Node* leftBranch;
    Node* centerBranch;
    Node* rightBranch;
};

/********************************************************/
/********************** Class Tree **********************/
/********************************************************/

class Tree {

public:
    /** Constructor **/
    Tree(): root(new Node(nullptr)) {}

    /** Destructor **/
    ~Tree();

private:
    /** Recursive function that deletes the memory of the tree starting from the chosen node **/
    void destroyNode(Node* node);

    Node* root;
};

#endif //PROCEDURAL_TREES_TREESTRUCTURE_H
