
#ifndef PROCEDURAL_TREES_TREESTRUCTURE_H
#define PROCEDURAL_TREES_TREESTRUCTURE_H
#include <iostream>
#include "MeshObject.h"

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLM
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

/********************************************************/
/********************** Struct Node *********************/
/********************************************************/

struct Node {
    Node(Node* parentIn)
            : parent(parentIn), firstBranch(nullptr), secondBranch(nullptr), thirdBranch(nullptr),
              rotationAngle(0.f), translationAmount(0.f), scaleAmount(1.f) {}

    Node(Node* parentIn, float rotationAngleIn, float translationAmountIn, float scaleAmountIn )
            : parent(parentIn), firstBranch(nullptr), secondBranch(nullptr), thirdBranch(nullptr),
              rotationAngle(rotationAngleIn), translationAmount(translationAmountIn), scaleAmount(scaleAmountIn) {}

    bool isLeaf(){
        return firstBranch == nullptr && secondBranch == nullptr && thirdBranch == nullptr;
    }

    Node* parent;
    Node* firstBranch;
    Node* secondBranch;
    Node* thirdBranch;
    float rotationAngle;
    float translationAmount;
    float scaleAmount;
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

    void createTreeFromLindenmayerSystem();

    void renderTree(MeshObject& branch, MeshObject& split, glm::mat4 model, GLint modelLoc);

private:
    /** Recursive function that deletes the memory of the tree starting from the chosen node **/
    void destroyNode(Node* node);

    void renderNode(Node* current_node, MeshObject& branch, MeshObject& split, glm::mat4 model, GLint modelLoc);

    void updateModelMatrix(float angles, float translationLength, float scaleAmount,
                           glm::mat4& model, GLint modelLoc);

    Node* root;
};

#endif //PROCEDURAL_TREES_TREESTRUCTURE_H
