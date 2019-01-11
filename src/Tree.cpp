#include <vector>
#include "Tree.h"

Tree::~Tree() {
    destroyNode(root);
    delete gen;
    delete dis;
}

void Tree::createTreeFromLindenmayerSystem(std::string axiom, std::vector<std::pair<char, std::string>> constructors, int noOfIterations) {
    std::string finalSystem = evaluateLindenmayerSystem(axiom, constructors,noOfIterations);

    Node* current = root;
    float rotAngle = 0.f;
    float scaleFactor = 1.f;
    glm::vec2 rotAxis = glm::vec2(0.f, 1.f);
    std::vector<Node*> fallbacks;

    myclock::duration d = myclock::now() - beginning;
    unsigned seed2 = d.count();
    (*gen).seed(seed2);

    for(int i = 0; i < finalSystem.length(); i++){
        rotAxis.x = (*dis)(*gen);
        rotAxis.y = (*dis)(*gen);
        int noOfBranches = current->getNumberOfBranches();
        switch(finalSystem[i]) {
            case 'F':
                if(fabs(rotAngle) > EPSILON) {
                    scaleFactor = initialScaleFactor;
                }
                switch(noOfBranches) {
                    case 0:
                        current->firstBranch = new Node(current, rotAngle, rotAxis, scaleFactor);
                        current = current->firstBranch;
                        break;
                    case 1:
                        current->secondBranch = new Node(current, rotAngle, rotAxis, scaleFactor);
                        current = current->secondBranch;
                        break;
                    case 2:
                        current->thirdBranch = new Node(current, rotAngle, rotAxis, scaleFactor);
                        current = current->thirdBranch;
                        break;
                    default:
                        break;
                }
                rotAngle = 0.f;
                scaleFactor = 1.f;
                break;
            case '+':
                rotAngle = initialRotationAngle;
                break;
            case '-':
                rotAngle = -1.f * initialRotationAngle;
                break;
            case '[':
                fallbacks.push_back(current);
                break;
            case ']':
                current = fallbacks.back();
                fallbacks.pop_back();
                break;
            default:
                break;
        }
    }
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

void Tree::destroyNode(Node *node) {
    if (node) {
        destroyNode(node->firstBranch);
        destroyNode(node->secondBranch);
        destroyNode(node->thirdBranch);
        delete node;
    }
}

std::string Tree::evaluateLindenmayerSystem(std::string axiom, std::vector<std::pair<char, std::string>> constructors, int noOfIterations) {
    int iter = 0;
    std::string system = axiom;
    std::string newSystem = "";

    while(iter < noOfIterations) {
        for(int i = 0; i < system.length(); i++){
            bool addedConstructor = false;
            for(int j = 0; j < constructors.size(); j++){
                if(system[i] == constructors[j].first) {
                    newSystem += constructors[j].second;
                    addedConstructor = true;
                }
            }
            if(!addedConstructor)
                newSystem += system[i];

        }
        system = newSystem;
        newSystem = "";
        iter++;
    }

    return system;
}

void Tree::updateModelMatrix(float angles, glm::vec2 rotationAxis, float scaleAmount, glm::mat4 &model, GLint modelLoc) {
    model = glm::rotate(model, glm::radians(angles), glm::vec3(rotationAxis.x, 0, rotationAxis.y));
    model = glm::translate(model, glm::vec3(0.f, initialTranslationLength, 0.f));
    model = glm::scale(model, glm::vec3(scaleAmount));

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
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








