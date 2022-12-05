/*
  Copyright (c) 2021
  Swarthmore College Computer Science Department, Swarthmore PA
  J. Brody, S. Caplan, A. Danner, L. Meeden, Z. Palmer, A. Soni, M. Wehar
  Distributed as course material for Fall 2021
  CPSC 035: Data Structures and Algorithms
*/

#include <stdexcept>
#include <utility>
#include <vector>

#include "adts/list.h"
#include "adts/queue.h"
#include "adts/stlList.h"
#include "adts/stlQueue.h"

#include "linkedBSTNode.h"
#include "helpers.h"

using std::pair;
using std::runtime_error;
using std::vector;

template <typename K, typename V> LinkedBST<K, V>::LinkedBST() {
  this->size = 0;
  this->root = nullptr;
}

template <typename K, typename V> LinkedBST<K, V>::~LinkedBST() {
  this->deleteSubtree(this->root);
}

template <typename K, typename V> int LinkedBST<K, V>::getSize() {
  return this->size;
}

template <typename K, typename V> bool LinkedBST<K, V>::isEmpty() {
  if (this->size == 0) {
    return true;
  } else {
    return false;
  }
}

template <typename K, typename V> void LinkedBST<K, V>::insert(K key, V value) {
  this->root = insertInSubtree(this->root, key, value);
}

template <typename K, typename V> void LinkedBST<K, V>::update(K key, V value) {
  this->updateInSubtree(this->root, key, value);
}

template <typename K, typename V> V LinkedBST<K, V>::get(K key) {
  return this->findInSubtree(this->root, key);
}

template <typename K, typename V> bool LinkedBST<K, V>::contains(K key) {
  return this->containsInSubtree(this->root, key);
}

template <typename K, typename V> void LinkedBST<K, V>::remove(K key) {
  this->root = this->removeFromSubtree(this->root, key);
}

template <typename K, typename V> vector<K> LinkedBST<K, V>::getKeys() {
  // using LevelOrder traversal.
  vector<K> key_vector;
  vector<pair<K, V>> items_vector = this->traverseLevelOrder();
  for (int i=0; i<items_vector.size(); i++) {
    key_vector.push_back(items_vector[i].first);
  }
  return key_vector;
}

template <typename K, typename V>
vector<pair<K, V>> LinkedBST<K, V>::getItems() {
  // using LevelOrder traversal.
  vector<pair<K, V>> items_vector = this->traverseLevelOrder();
  return items_vector;
}

template <typename K, typename V> int LinkedBST<K, V>::getHeight() {
  return this->getHeightInSubtree(this->root);
}

template <typename K, typename V> K LinkedBST<K, V>::getMaxKey() {
  pair<K, V> max_pair = this->getMaxInSubtree(this->root);
  return max_pair.first;
}

template <typename K, typename V> K LinkedBST<K, V>::getMinKey() {
  pair<K, V> min_pair = this->getMinInSubtree(this->root);
  return min_pair.first;
}

template <typename K, typename V>
vector<pair<K, V>> LinkedBST<K, V>::traversePreOrder() {
  List<pair<K, V>>* preOrderList = new STLList<pair<K, V>>();
  this->buildPreOrderTraversal(this->root, preOrderList);
  vector<pair<K, V>> preOrderVector = dynamicListToStaticVector(preOrderList);
  delete preOrderList;
  return preOrderVector;
}

template <typename K, typename V>
vector<pair<K, V>> LinkedBST<K, V>::traverseInOrder() {
  List<pair<K, V>>* inOrderList = new STLList<pair<K, V>>();
  this->buildInOrderTraversal(this->root, inOrderList);
  vector<pair<K, V>> inOrderVector = dynamicListToStaticVector(inOrderList);
  delete inOrderList;
  return inOrderVector;
}

template <typename K, typename V>
vector<pair<K, V>> LinkedBST<K, V>::traversePostOrder() {
  List<pair<K, V>>* postOrderList = new STLList<pair<K, V>>();
  this->buildPostOrderTraversal(this->root, postOrderList);
  vector<pair<K, V>> postOrderVector = dynamicListToStaticVector(postOrderList);
  delete postOrderList;
  return postOrderVector;
}

template <typename K, typename V>
vector<pair<K, V>> LinkedBST<K, V>::traverseLevelOrder() {
  LinkedBSTNode<K, V>* current = this->root;
  STLQueue<LinkedBSTNode<K, V>*> traversal_queue; // using Queue as it's FIFO.
  STLList<pair<K, V>> traversal_list;
  if (current == nullptr) {
    return traversal_list.toVector();
  } else {
    traversal_queue.enqueue(current);
    while (!traversal_queue.isEmpty()) {
      LinkedBSTNode<K, V>* current = traversal_queue.dequeue();
      pair<K, V> key_value(current->getKey(), current->getValue());
      traversal_list.insertLast(key_value);
      if (current->getLeft() != nullptr) {
        traversal_queue.enqueue(current->getLeft());
      }
      if (current->getRight() != nullptr) {
        traversal_queue.enqueue(current->getRight());
      }
    }
    return traversal_list.toVector();
  }
}

template <typename K, typename V> void LinkedBST<K, V>::checkInvariants() {
    if (this->countNodes(this->root) != this->size) {
        throw runtime_error(
            "Problem in BST: Node count doesn't match tree size");
    }
    if (this->root != nullptr) {
        // The bounds provided here are arbitrary because the false
        // arguments indicate that they do not apply.  But we need a
        // value of type K to fill this parameter since we don't have
        // globally min/max K values generically.
        this->verifyKeysBoundedBy(this->root, false, this->root->getKey(),
                                  false, this->root->getKey());
    }
}
