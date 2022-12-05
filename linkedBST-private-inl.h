/*
  Copyright (c) 2021
  Swarthmore College Computer Science Department, Swarthmore PA
  J. Brody, S. Caplan, A. Danner, L. Meeden, Z. Palmer, A. Soni, M. Wehar
  Distributed as course material for Fall 2021
  CPSC 035: Data Structures and Algorithms
*/

#include <stdexcept>
#include <utility>

#include "adts/list.h"
#include "adts/stlList.h"

using std::runtime_error;

template <typename K, typename V>
V LinkedBST<K, V>::findInSubtree(LinkedBSTNode<K, V>* current, K key) {
    if (current == nullptr) {
        // edge case: empty subtree.
        throw runtime_error("No Key Found!");
    } else if (current->getKey() == key) {
        return current->getValue();
    } else if (key > current->getKey()){
        return findInSubtree(current->getRight(), key); // recursive.
    } else {
        return findInSubtree(current->getLeft(), key); // recursive.
    }
}

template <typename K, typename V>
bool LinkedBST<K, V>::containsInSubtree(LinkedBSTNode<K, V>* current, K key) {
    if (current == nullptr) {
        // edge case: empty subtree.
        return false;
    }
    if (key != current->getKey()){
        if(key > current->getKey()){
        return containsInSubtree(current->getRight(), key); // recursive.
        } else {
        return containsInSubtree(current->getLeft(), key); // recursive.
        }
    } else{
        return true;
    }
}

template <typename K, typename V>
void LinkedBST<K, V>::updateInSubtree(LinkedBSTNode<K, V>* current, K key,
                                      V value) {
    if (current == nullptr) {
        throw runtime_error("No Key Found!"); 
    }
    if (key != current->getKey()){
        if(key > current->getKey()){
        updateInSubtree(current->getRight(), key, value); // recursive.
        } else {
        updateInSubtree(current->getLeft(), key, value); // recursive.
        }
    } else{
        current->setValue(value);
    }
}

template <typename K, typename V>
int LinkedBST<K, V>::getHeightInSubtree(LinkedBSTNode<K, V>* current) {
    if (current == nullptr) {
        int height = -1;
        return height;
    } else {
        int left_height = getHeightInSubtree(current->getLeft()); // recurisve.
        int right_height = getHeightInSubtree(current->getRight()); // recursive.
        int sub_height = std::max(left_height, right_height) + 1;
        return sub_height;
    }
}

template <typename K, typename V>
pair<K, V> LinkedBST<K, V>::getMinInSubtree(LinkedBSTNode<K, V>* current) {
    if (current == nullptr) {
        // edge case: emptty subtreee.
        throw runtime_error("Subtree is empty!");
    } else if (current->getLeft() == nullptr){
        pair<K, V> key_value(current->getKey(), current->getValue());
        return key_value;
    } else {
        return getMinInSubtree(current->getLeft()); // recursive.
    }
}

template <typename K, typename V>
pair<K, V> LinkedBST<K, V>::getMaxInSubtree(LinkedBSTNode<K, V>* current) {
    if (current == nullptr) {
        // edge case: empty subtree.
        throw runtime_error("Subtree is empty!");
    } else if (current->getRight() == nullptr){
        pair<K, V> key_value(current->getKey(), current->getValue());
        return key_value;
    } else {
        return getMaxInSubtree(current->getRight()); // recursive.
    }
}

template <typename K, typename V>
LinkedBSTNode<K, V>*
LinkedBST<K, V>::insertInSubtree(LinkedBSTNode<K, V>* current, K key, V value) {
    if (current == nullptr) {
        LinkedBSTNode<K, V>* bst = new LinkedBSTNode<K, V>(key, value);
        this->size += 1;
        return bst;
    } else if (key == current->getKey()) {
        throw runtime_error("Provided key already exists in this tree!");
    } else if (key < current->getKey()) {
        // recursive step.
        current->setLeft(insertInSubtree(current->getLeft(), key, value));
        return current;
    } else {
        // recursive step.
        current->setRight(insertInSubtree(current->getRight(), key, value));
        return current;
    }
}

template <typename K, typename V>
LinkedBSTNode<K, V>*
LinkedBST<K, V>::removeFromSubtree(LinkedBSTNode<K, V>* current, K key) {
    if (current == nullptr){
        // edge case: empty subtree.
        throw runtime_error("Provided key doesn't exist!");
    } else if (key < current->getKey()) {
        current->setLeft(removeFromSubtree(current->getLeft(), key)); // recursive.
        return current;
    } else if (key > current->getKey()) {
        current->setRight(removeFromSubtree(current->getRight(), key)); // recursive.
        return current;
    } else {
        if (current->getLeft() == nullptr && current->getRight() == nullptr) {
            delete current;
            this->size -= 1;
            return nullptr;
        } else if (current->getLeft() == nullptr) {
            LinkedBSTNode<K, V>* temp = current->getRight();
            delete current;
            this->size -= 1;
            return temp;
        } else if (current->getRight() == nullptr) {
            LinkedBSTNode<K, V>* temp = current->getLeft();
            delete current;
            this->size -= 1;
            return temp;
        } else {
            pair<K, V> min_pair = getMinInSubtree(current->getRight());
            current->setKey(min_pair.first);
            current->setValue(min_pair.second);
            current->setRight(removeFromSubtree(current->getRight(), min_pair.first));
            return current;
        }
    }
}

template <typename K, typename V>
void LinkedBST<K, V>::deleteSubtree(LinkedBSTNode<K, V>* current) {
    if (current != nullptr) {
        // inductive step; edge case is excluded by the condition inside the IF.
        deleteSubtree(current->getLeft());
        deleteSubtree(current->getRight());
        delete current;
    }
}

template <typename K, typename V>
void LinkedBST<K, V>::buildPreOrderTraversal(LinkedBSTNode<K, V>* current,
                                             List<pair<K, V>>* list) {
    if (current != nullptr) {
        // inductive step; edge case is excluded by the condition inside the IF.
        pair<K, V> key_value(current->getKey(), current->getValue());
        list->insertLast(key_value);
        buildPreOrderTraversal(current->getLeft(), list);
        buildPreOrderTraversal(current->getRight(), list);
    }
}

template <typename K, typename V>
void LinkedBST<K, V>::buildInOrderTraversal(LinkedBSTNode<K, V>* current,
                                            List<pair<K, V>>* list) {
    if (current != nullptr) {
        // inductive step; edge case is excluded by the condition inside the IF.
        pair<K, V> key_value(current->getKey(), current->getValue());
        buildInOrderTraversal(current->getLeft(), list);
        list->insertLast(key_value);
        buildInOrderTraversal(current->getRight(), list);
    }
}

template <typename K, typename V>
void LinkedBST<K, V>::buildPostOrderTraversal(LinkedBSTNode<K, V>* current,
                                              List<pair<K, V>>* list) {
    if (current != nullptr) {
        // inductive step; edge case is excluded by the condition inside the IF.
        pair<K, V> key_value(current->getKey(), current->getValue());
        buildPostOrderTraversal(current->getLeft(), list);
        buildPostOrderTraversal(current->getRight(), list);
        list->insertLast(key_value);
    }
}

template <typename K, typename V>
int LinkedBST<K, V>::countNodes(LinkedBSTNode<K, V>* current) {
    if (current == nullptr) {
        return 0;
    } else {
        return this->countNodes(current->getLeft()) +
               this->countNodes(current->getRight()) + 1;
    }
}

template <typename K, typename V>
void LinkedBST<K, V>::verifyKeysBoundedBy(LinkedBSTNode<K, V>* current,
                                          bool minApplies, K minBound,
                                          bool maxApplies, K maxBound) {
    if (minApplies && current->getKey() < minBound) {
        throw runtime_error("LinkedBST::verifyKeysBoundedBy: a node has a "
                            "right descendent with lesser key");
    }
    if (maxApplies && current->getKey() > maxBound) {
        throw runtime_error("LinkedBST::verifyKeysBoundedBy: a node has a left "
                            "descendent with greater key");
    }
    if (current->getLeft() != nullptr) {
        verifyKeysBoundedBy(current->getLeft(), minApplies, minBound, true,
                            current->getKey());
    }
    if (current->getRight() != nullptr) {
        verifyKeysBoundedBy(current->getRight(), true, current->getKey(),
                            maxApplies, maxBound);
    }
}
