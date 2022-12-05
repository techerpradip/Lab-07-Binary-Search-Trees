/*
  Copyright (c) 2021
  Swarthmore College Computer Science Department, Swarthmore PA
  J. Brody, S. Caplan, A. Danner, L. Meeden, Z. Palmer, A. Soni, M. Wehar
  Distributed as course material for Fall 2021
  CPSC 035: Data Structures and Algorithms
*/

template <typename K, typename V>
LinkedBSTNode<K, V>::LinkedBSTNode(K key, V value) {
    this->key = key;
    this->value = value;
    this->left = nullptr;
    this->right = nullptr;
}

template <typename K, typename V>
LinkedBSTNode<K, V>::LinkedBSTNode(K key, V value, LinkedBSTNode<K, V>* left,
                                   LinkedBSTNode<K, V>* right) {
    this->key = key;
    this->value = value;
    this->left = left;
    this->right = right;
}

template <typename K, typename V> K LinkedBSTNode<K, V>::getKey() {
    return this->key;
}

template <typename K, typename V> void LinkedBSTNode<K, V>::setKey(K k) {
    this->key = k;
}

template <typename K, typename V> V LinkedBSTNode<K, V>::getValue() {
    return this->value;
}

template <typename K, typename V> void LinkedBSTNode<K, V>::setValue(V v) {
    this->value = v;
}

template <typename K, typename V>
LinkedBSTNode<K, V>* LinkedBSTNode<K, V>::getLeft() {
    return this->left;
}

template <typename K, typename V>
void LinkedBSTNode<K, V>::setLeft(LinkedBSTNode<K, V>* left) {
    this->left = left;
}

template <typename K, typename V>
LinkedBSTNode<K, V>* LinkedBSTNode<K, V>::getRight() {
    return this->right;
}

template <typename K, typename V>
void LinkedBSTNode<K, V>::setRight(LinkedBSTNode<K, V>* right) {
    this->right = right;
}
