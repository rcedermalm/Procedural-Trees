
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

const float EPSILON = 1e-4;
const float LEAF_SIZE = 2.f;

/********************************************************/
/********************** Struct Node *********************/
/********************************************************/

struct Node {
    Node(Node* parentIn)
            : parent(parentIn), firstBranch(nullptr), secondBranch(nullptr), thirdBranch(nullptr),
              rotationAngle(0.f), rotationAxis(glm::vec2(0.f, 1.f)), scaleAmount(1.f) {}

    Node(Node* parentIn, float rotationAngleIn, glm::vec2 rotationAxisIn, float scaleAmountIn )
            : parent(parentIn), firstBranch(nullptr), secondBranch(nullptr), thirdBranch(nullptr),
              rotationAngle(rotationAngleIn), rotationAxis(rotationAxisIn), scaleAmount(scaleAmountIn) {}

    bool isLeaf() {
        return firstBranch == nullptr && secondBranch == nullptr && thirdBranch == nullptr;
    }

    bool hasStraightBranch() {
        if(isLeaf())
            return true;

        if(firstBranch != nullptr && fabs(firstBranch->rotationAngle) < EPSILON
                || secondBranch != nullptr && fabs(secondBranch->rotationAngle) < EPSILON
                || thirdBranch != nullptr && fabs(thirdBranch->rotationAngle) < EPSILON)
            return true;

        return false;
    }

    int getNumberOfBranches() {
        int noOfBranches = 0;
        if(firstBranch != nullptr) noOfBranches++;
        if(secondBranch != nullptr) noOfBranches++;
        if(thirdBranch != nullptr) noOfBranches++;
        return noOfBranches;
    }

    Node* parent;
    Node* firstBranch;
    Node* secondBranch;
    Node* thirdBranch;
    float rotationAngle;
    glm::vec2 rotationAxis;
    float scaleAmount;
};

/********************************************************/
/********************** Class Tree **********************/
/********************************************************/

class Tree {

public:
    /** Constructor **/
    Tree(float translationLengthIn,  float rotationAngleIn, float scaleFactorIn, GLint objColourLocIn)
            : root(new Node(nullptr)), initialTranslationLength(translationLengthIn),
              initialRotationAngle(rotationAngleIn), initialScaleFactor(scaleFactorIn), objColourLoc(objColourLocIn) {}

    /** Destructor **/
    ~Tree();

    /** Creates the tree from a Lindenmayer system (L-system) **/
    void createTreeFromLindenmayerSystem(std::string axiom, int noOfIterations);

    /** Renders the entire tree **/
    void renderTree(MeshObject& branch, MeshObject& split, glm::mat4 model, GLint modelLoc);

private:
    /** Recursive function that deletes the memory of the tree starting from the chosen node **/
    void destroyNode(Node* node);

    /** Renders the given node and its children recursively **/
    void renderNode(Node* current_node, MeshObject& branch, MeshObject& split, glm::mat4 model, GLint modelLoc);

    /** Updates the given model matrix with the rotation of the given angles, a translation of the given amount and a
     * scaling with the given amount. */
    void updateModelMatrix(float angles, glm::vec2 rotationAxis, float scaleAmount,
                           glm::mat4& model, GLint modelLoc);

    Node* root;
    float initialTranslationLength;
    float initialRotationAngle;
    float initialScaleFactor;
    GLint objColourLoc;

    // Colour variables
    glm::vec3 branchColour = glm::vec3(102.f/255.f, 51.f/255.f, 0.f);
    glm::vec3 leafColour = glm::vec3(0.f, 102.f/255.f, 0.f);
};

#endif //PROCEDURAL_TREES_TREESTRUCTURE_H
