#pragma once

/*
  Copyright (c) 2021
  Swarthmore College Computer Science Department, Swarthmore PA
  J. Brody, S. Caplan, A. Danner, L. Meeden, Z. Palmer, A. Soni, M. Wehar
  Distributed as course material for Fall 2021
  CPSC 035: Data Structures and Algorithms
*/

#include <utility>
#include <vector>

using std::pair;
using std::vector;

#include "adts/BST.h"
#include "adts/list.h"
#include "linkedBSTNode.h"

template <typename K, typename V> class LinkedBST : public BST<K, V> {
  public:
    /* see description in adts/BST.h
     * for documentation of these methods
     */
    LinkedBST();
    ~LinkedBST();

    int getSize();
    bool isEmpty();
    void insert(K key, V value);
    void update(K key, V value);
    V get(K key);
    bool contains(K key);
    void remove(K key);
    vector<K> getKeys();
    vector<pair<K, V>> getItems();

    /**
     * This verifies that the BST is the proper size and also checks
     * that the BST property holds throughout the entire tree.
     * Throws errors when inconsistencies are found.
     */
    void checkInvariants();

    int getHeight();
    K getMaxKey();
    K getMinKey();
    vector<pair<K, V>> traversePreOrder();
    vector<pair<K, V>> traverseInOrder();
    vector<pair<K, V>> traversePostOrder();
    vector<pair<K, V>> traverseLevelOrder();

  private:
    /** Contains the number of nodes in this tree. */
    int size;
    /** Contains a pointer to the root node of this tree (or nullptr if no root
        exists). */
    LinkedBSTNode<K, V>* root;

    /* private helper methods */
    /**
     * Given a BST node, returns the value associated with the given key in the
     * subtree
     * @param current The root of the subtree to search.
     * @param key The key to find.
     * @return The value associated with that key
     * @throws runtime_error If the key is not found in the subtree.
     */
    V findInSubtree(LinkedBSTNode<K, V>* current, K key);

    /**
     * Recursive function that checks if the subtree
     *  pointed by current contains the given key
     * @param current The root of the subtree to search
     * @param key The key to find
     * @return bool : true if the key was found
     */
    bool containsInSubtree(LinkedBSTNode<K, V>* current, K key);

    /**
     * Given a BST node, updates the value associated
                 * with the given key in the subtree
     * @param current The root of the subtree to search.
     * @param key The key to find.
     * @param value The new value to associate with the key
     * @throws runtime_error If the key is not found in the subtree.
     */
    void updateInSubtree(LinkedBSTNode<K, V>* current, K key, V value);

    /**
     * Given a BST node, determines the height of the subtree it represents.
     * @param current The root of the subtree.
     * @return The height of that subtree.
     */
    int getHeightInSubtree(LinkedBSTNode<K, V>* current);

    /**
     * Given a BST node, determines the minimum key which appears in its
     * subtree.
     * @param current The root of the subtree.
     * @return The key-value pair for the minimum key value that appears in that
     *         subtree.
     * @throws runtime_error If the subtree is empty.
     */
    pair<K, V> getMinInSubtree(LinkedBSTNode<K, V>* current);

    /**
     * Given a BST node, determines the maximum key which appears in its
     * subtree.
     * @param current The root of the subtree.
     * @return The key-value pair for the maximum key value that appears in that
     *         subtree.
     * @throws runtime_error If the subtree is empty.
     */
    pair<K, V> getMaxInSubtree(LinkedBSTNode<K, V>* current);

    /**
     * Given a BST node, inserts a new node with the provided key into the
     * subtree
     * at the appropriate location.  If this function does not throw an
     * exception,
     * it adds a new node to the subtree.  In particular, it will return a
     * non-null(ptr)
     * value when given a null(ptr) root; the caller should then use this new
     * value to
     * replace the old subtree.
     * @param current The root of the subtree.
     * @param key The key for which we are creating a mapping.
     * @param value The value for which we are creating a mapping.
     * @return The new root of the subtree.
     * @throws runtime_error If the provided key already exists in this tree.
     */
    LinkedBSTNode<K, V>* insertInSubtree(LinkedBSTNode<K, V>* current, K key,
                                         V value);

    /**
     * Given a BST node, removes the node with the specified key from the
     * subtree.
     * If this function does not throw an exception, it deletes a node from the
     * subtree, reducing the subtree's size by 1.  In that case, it will provide
     * a
     * pointer to the new root of this subtree (since the subtree's root may
     * have
     * been deleted or otherwise rearranged by this call).
     * @param current The root of the subtree.
     * @param key The key we are removing from the subtree.
     * @return The new root of the subtree.
     * @throws runtime_error If the provided key does not exist in this tree.
     */
    LinkedBSTNode<K, V>* removeFromSubtree(LinkedBSTNode<K, V>* current, K key);

    /**
     * Deletes every node that appears in a subtree.
     * @param node The subtree to delete.
     */
    void deleteSubtree(LinkedBSTNode<K, V>* current);

    /**
     * Constructs a pre-order traversal of the provided subtree, putting the
     * nodes
     * into the provided list.
     * @param node The root of the subtree to traverse.
     * @param list The list into which the nodes should be added.
     */
    void buildPreOrderTraversal(LinkedBSTNode<K, V>* current,
                                List<pair<K, V>>* list);

    /**
     * Constructs an in-order traversal of the provided subtree, putting the
     * nodes
     * into the provided list.
     * @param current The root of the subtree to traverse.
     * @param list The list into which the nodes should be added.
     */
    void buildInOrderTraversal(LinkedBSTNode<K, V>* current,
                               List<pair<K, V>>* list);

    /**
     * Constructs a post-order traversal of the provided subtree, putting the
     * nodes
     * into the provided list.
     * @param current The root of the subtree to traverse.
     * @param list The list into which the nodes should be added.
     */
    void buildPostOrderTraversal(LinkedBSTNode<K, V>* current,
                                 List<pair<K, V>>* list);

    /**
     * This private helper is used by checkInvariants to ensure that the
     * number of reachable nodes in the BST matches the data member size.
     * @param current A BST node
     * @return number of nodes in the BST
     */
    int countNodes(LinkedBSTNode<K, V>* current);

    /**
     * Checks that all keys in the given subtree are within a provided bound.
     * Throws a runtime error if a problem is found.
     * @param current A BST node.
     * @param minApplies True if the minimum bound applies; false if it does
     *                   not.
     * @param minBound The minimum bound for all keys in this subtree.
     * @param maxApplies True if the maximum bound applies; false if it does
     *                   not.
     * @param maxBound The maximum bound for all keys in this subtree.
     */
    void verifyKeysBoundedBy(LinkedBSTNode<K, V>* current, bool minApplies,
                             K minBound, bool maxApplies, K maxBound);
};

// All public methods are defined here
#include "linkedBST-inl.h"

// All private methods are defined here
#include "linkedBST-private-inl.h"
