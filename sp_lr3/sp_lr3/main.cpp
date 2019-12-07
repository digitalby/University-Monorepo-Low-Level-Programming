//
//  main.cpp
//  sp_lr3
//
//  Created by Digitalby on 05/11/2019.
//  Copyright © 2019 Yury. All rights reserved.
//

#include <iostream>

template <typename T>
struct Node {
    int data;
    Node* left;
    Node* right;
};

Node<int>* theRoot = NULL;

template <typename T>
Node<T>* Create(T data) {
    Node<T>* newNode = new Node<T>();
    newNode->data = data;
    newNode->left = newNode->right = NULL;
    return newNode;
}

template <typename T>
Node<T>* Insert(Node<T>* root, T data) {
    if(root == NULL) {
        root = Create(data);
    }
    else if(data <= root->data) {
        root->left = Insert(root->left,data);
    } else {
        root->right = Insert(root->right,data);
    }
    return root;
}

template <typename T>
Node<T>* Search(Node<T>* root, T data) {
    if(root == NULL) return NULL;
    if(root->data == data) return root;
    if(data < root->data) return Search(root->left,data);
    else return Search(root->right,data);
}

template <typename T>
Node<T>* FindMin(Node<T>* root) {
    while(root->left != NULL)
        root = root->left;
    return root;
}

template <typename T>
Node<T>* FindMax(Node<T>* root) {
    while(root->right != NULL)
        root = root->right;
    return root;
}

template <typename T>
struct Node<T>* Delete(Node<T>* root, T data) {
    if(root == NULL) return root;
    else if(data < root->data) root->left = Delete(root->left,data);
    else if(data > root->data) root->right = Delete(root->right, data);
    else {
        if(root->left == NULL && root->right == NULL) {
            delete root;
            root = NULL;
        } else if(root->left == NULL) {
            struct Node<T> *temp = root;
            root = root->right;
            delete temp;
        } else if(root->right == NULL) {
            struct Node<T> *temp = root;
            root = root->left;
            delete temp;
        } else {
            struct Node<T> *temp = FindMin(root->right);
            root->data = temp->data;
            root->right = Delete(root->right,temp->data);
        }
    }
    return root;
}

//template <typename T>
//void DeleteAll(Node<T>* root) {
//    Node<T> *minNode = FindMin(root);
//    while(minNode != NULL) {
//        Delete(root, minNode->data);
//        minNode = FindMin(root);
//    }
//    Node<T> *maxNode = FindMax(root);
//    while(maxNode != NULL) {
//        Delete(root, maxNode->data);
//        maxNode = FindMax(root);
//    }
//    Delete(root, root->data);
//}

template <typename T>
void Traverse(Node<T>* root) {
    if(root == NULL) {
        std::cout << "NULL";
        return;
    }

    std::cout << root->data;
    std::cout << "(";
    Traverse(root->left);
    std::cout << ",";
    Traverse(root->right);
    std::cout << ")";
}

template <typename T>
void DeleteAll(Node<T>* root) {
    if(root->left != NULL) {
        DeleteAll(root->left);
        root->left = NULL;
    }
    if(root->right != NULL) {
        DeleteAll(root->right);
        root->right = NULL;
    }
    delete root;
    root = NULL;
}

void testCase() {
    theRoot = Insert(theRoot, 15);
    theRoot = Insert(theRoot, 10);
    theRoot = Insert(theRoot, 12);
    theRoot = Insert(theRoot, 20);
    theRoot = Insert(theRoot, 25);
    theRoot = Insert(theRoot, 19);
    theRoot = Insert(theRoot, 26);

    Traverse(theRoot);
    std::cout << std::endl;
    theRoot = Delete(theRoot, 25);
    Traverse(theRoot);
    std::cout << std::endl;
    theRoot = Delete(theRoot, 10);
    Traverse(theRoot);
    std::cout << std::endl;
    theRoot = Delete(theRoot, 20);
    Traverse(theRoot);
    std::cout << std::endl;

    DeleteAll(theRoot);
    theRoot = NULL;
    Traverse(theRoot);
    std::cout << std::endl;
    getchar();
}

void doTask(int choice) {
    switch(choice) {
        case 1:
            int newData;
            std::cout << "> ";
            std::cin >> newData;
            theRoot = Insert(theRoot, newData);
            std::cout << std::endl << "Operation completed OK." << std::endl;
            break;
        case 2:
            int data;
            std::cout << "> ";
            std::cin >> data;
            theRoot = Delete(theRoot, data);
            std::cout << std::endl << "Operation completed OK." << std::endl;
            break;
        case 3:
            DeleteAll(theRoot);
            theRoot = NULL;
            std::cout << "Operation completed OK." << std::endl;
            break;
        case 4:
            Traverse(theRoot);
            std::cout << std::endl;
            break;
    }
}

int main(int argc, const char * argv[]) {
    int choice = 1;
    while(choice != 0) {
        std::cout << std::endl;
        std::cout << "1. Add" << std::endl
              << "2. Delete" << std::endl
              << "3. DeleteAll" << std::endl
              << "4. Browse" << std::endl
              << "0. Exit" << std::endl
              << "> ";
        std::cin >> choice;
        std::cout << std::endl;
        doTask(choice);
    }
    return 0;
}
