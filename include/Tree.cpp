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
    root->firstBranch = new Node(root, initialRotationAngle, glm::vec2(0.f, 1.f), initialScaleFactor);
    root->secondBranch = new Node(root, 0.f, glm::vec2(0.f, 1.f), 1.f);
    root->firstBranch->firstBranch = new Node(root->firstBranch, initialRotationAngle, glm::vec2(1.f, 0.f), initialScaleFactor);
}

void Tree::renderTree(MeshObject& branch, MeshObject& split, glm::mat4 model, GLint modelLoc) {
    glUniform3f(objColourLoc, branchColour.x, branchColour.y, branchColour.z);
    updateModelMatrix(root->rotationAngle, root->rotationAxis, root->scaleAmount, model, modelLoc);
    branch.render();

    if(!root->hasStraightBranch()){
        updateModelMatrix(0, glm::vec2(0.f, 1.f), 1.f, model, modelLoc);
        split.render();
    }

    if(root->isLeaf()) {
        glUniform3f(objColourLoc, leafColour.x, leafColour.y, leafColour.z);
        updateModelMatrix(0, glm::vec2(0.f, 1.f), LEAF_SIZE, model, modelLoc);
        split.render();
        glUniform3f(objColourLoc, branchColour.x, branchColour.y, branchColour.z);
        return;
    }

    renderNode(root->firstBranch, branch, split, model, modelLoc);
    renderNode(root->secondBranch, branch, split, model, modelLoc);
    renderNode(root->thirdBranch, branch, split, model, modelLoc);
}

void Tree::renderNode(Node *current_node, MeshObject &branch, MeshObject &split, glm::mat4 model, GLint modelLoc) {
    if(current_node == nullptr)
        return;

    updateModelMatrix(current_node->rotationAngle, current_node->rotationAxis, current_node->scaleAmount, model, modelLoc);
    branch.render();

    if(!current_node->hasStraightBranch()) {
        updateModelMatrix(0.f, glm::vec2(0.f, 1.f), 1.f, model, modelLoc);
        split.render();
    }

    if(current_node->isLeaf()) {
        glUniform3f(objColourLoc, leafColour.x, leafColour.y, leafColour.z);
        updateModelMatrix(0, glm::vec2(0.f, 1.f), LEAF_SIZE, model, modelLoc);
        split.render();
        glUniform3f(objColourLoc, branchColour.x, branchColour.y, branchColour.z);
        return;
    }

    renderNode(current_node->firstBranch, branch, split, model, modelLoc);
    renderNode(current_node->secondBranch, branch, split, model, modelLoc);
    renderNode(current_node->thirdBranch, branch, split, model, modelLoc);

}

void Tree::updateModelMatrix(float angles, glm::vec2 rotationAxis, float scaleAmount, glm::mat4 &model, GLint modelLoc) {
    model = glm::rotate(model, glm::radians(angles), glm::vec3(rotationAxis.x, 0, rotationAxis.y));
    model = glm::translate(model, glm::vec3(0.f, initialTranslationLength, 0.f));
    model = glm::scale(model, glm::vec3(scaleAmount));

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
}





