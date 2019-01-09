#include "Tree.h"

Tree::~Tree() {
    destroyNode(root);
}

void Tree::destroyNode(Node *node) {
    if (node) {
        destroyNode(node->firstBranch);
        destroyNode(node->secondBranch);
        destroyNode(node->thirdBranch);
        delete node;
    }
}

void Tree::createTreeFromLindenmayerSystem() {
    // Test to see if render works
    root->firstBranch = new Node(root, 40.f, 2.5f, 0.7f );
    root->secondBranch = new Node(root, -40.f, 2.5f, 0.7f );
    root->firstBranch->firstBranch = new Node(root->firstBranch, 40.f, 2.5f, 0.7f);
}

void Tree::renderTree(MeshObject& branch, MeshObject& split, glm::mat4 model, GLint modelLoc) {
    updateModelMatrix(root->rotationAngle, root->translationAmount, root->scaleAmount, model, modelLoc);
    branch.render();

    if(!root->isLeaf()){
        updateModelMatrix(0, 2.5f, 1.f, model, modelLoc);
        split.render();
        renderNode(root->firstBranch, branch, split, model, modelLoc);
        renderNode(root->secondBranch, branch, split, model, modelLoc);
        renderNode(root->thirdBranch, branch, split, model, modelLoc);
    }
}

void Tree::renderNode(Node *current_node, MeshObject &branch, MeshObject &split, glm::mat4 model, GLint modelLoc) {
    if(current_node == nullptr)
        return; // TODO: ADD "leafs" here (a green sphere)

    updateModelMatrix(current_node->rotationAngle, current_node->translationAmount, current_node->scaleAmount, model, modelLoc);
    branch.render();

    if(!current_node->isLeaf()){
        updateModelMatrix(0, 2.5f, 1.f, model, modelLoc);
        split.render();

        renderNode(current_node->firstBranch, branch, split, model, modelLoc);
        renderNode(current_node->secondBranch, branch, split, model, modelLoc);
        renderNode(current_node->thirdBranch, branch, split, model, modelLoc);
    }
}

void Tree::updateModelMatrix(float angles, float translationLength, float scaleAmount, glm::mat4 &model, GLint modelLoc) {
    model = glm::rotate(model, glm::radians(angles), glm::vec3(0,0,1));
    model = glm::translate(model, glm::vec3(0.f, translationLength, 0.f));
    model = glm::scale(model, glm::vec3(scaleAmount));

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
}





