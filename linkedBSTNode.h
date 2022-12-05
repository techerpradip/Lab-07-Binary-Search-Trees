#pragma once

/*
  Copyright (c) 2021
  Swarthmore College Computer Science Department, Swarthmore PA
  J. Brody, S. Caplan, A. Danner, L. Meeden, Z. Palmer, A. Soni, M. Wehar
  Distributed as course material for Fall 2021
  CPSC 035: Data Structures and Algorithms
*/

/**
 * A class to contain BST node data.  This is similar to a linked list node: it
 * is just a simple data container.
 */
template <typename K, typename V> class LinkedBSTNode {
  private:
    K key;
    V value;
    LinkedBSTNode<K, V>* left;
    LinkedBSTNode<K, V>* right;

  public:
    /**
     * Creates a LinkedBSTNode with a set key and value.  The pointer fields are
     * set to nullptr.
     * @param key The key for this node.
     * @param value The value for this node.
     */
    LinkedBSTNode(K key, V value);
    /**
     * Creates a LinkedBSTNode with a set key, value, and children.
     * @param key The key for this node.
     * @param value The value for this node.
     * @param left The left child of this node.
     * @param right The right child of this node.
     */
    LinkedBSTNode(K key, V value, LinkedBSTNode<K, V>* left,
                  LinkedBSTNode<K, V>* right);

    /** Accessor and Mutator Methods for LinkedBSTNode **/
    K getKey();
    void setKey(K key);

    V getValue();
    void setValue(V value);

    LinkedBSTNode<K, V>* getLeft();
    LinkedBSTNode<K, V>* getRight();

    void setLeft(LinkedBSTNode<K, V>* newleft);
    void setRight(LinkedBSTNode<K, V>* newright);

    // You can safely ignore the following code.  This marks some default C++
    // class routines as "private", preventing you from using them accidentally.
    // Specifically, we are disabling the use of the copy constructor and the
    // copy assignment operator.  You can read more here:
    //   http://www.cplusplus.com/articles/y8hv0pDG/
  private:
    LinkedBSTNode(const LinkedBSTNode& other) = delete;
    LinkedBSTNode& operator=(const LinkedBSTNode& other) = delete;
};

// All public methods are defined here
#include "linkedBSTNode-inl.h"
