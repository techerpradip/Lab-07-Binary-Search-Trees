/*
  Copyright (c) 2021
  Swarthmore College Computer Science Department, Swarthmore PA
  J. Brody, S. Caplan, A. Danner, L. Meeden, Z. Palmer, A. Soni, M. Wehar
  Distributed as course material for Fall 2021
  CPSC 035: Data Structures and Algorithms
*/

#include <cmath>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

#include <UnitTest++/UnitTest++.h>

#include "adts/BST.h"
#include "adts/list.h"
#include "linkedBST.h"
#include "linkedBSTNode.h"

using namespace std;

/**
 * This helper function will build a small BST on which we can perform
 * experiments.  We can call the methods of the BST
 * in a specific order to make sure it puts the nodes where we want them
 * (assuming, of course, that the insert and get functions work correctly).
 * Our tree will look like this:
 * @verbatim
 *         6
 *        / \
 *       /   \
 *      2     7
 *     / \     \
 *    1   4     9
 *       /
 *      3
 * @endverbatim
 * @return An example BST.  The caller is responsible for deleting this BST.
 */
LinkedBST<int, string>* makeExampleBST() {
    LinkedBST<int, string>* bst = new LinkedBST<int, string>();
    bst->insert(6, "6");
    bst->insert(2, "2");
    bst->insert(7, "7");
    bst->insert(1, "1");
    bst->insert(4, "4");
    bst->insert(9, "9");
    bst->insert(3, "3");
    bst->checkInvariants();
    return bst;
}

/**
 * TEST case to test the LinkedBST::insert(K, V) method.
 */
TEST(insertBST) {
    LinkedBST<int, string>* bst = makeExampleBST();
    bst->insert(5, "5");
    bst->insert(8, "8");
    CHECK_EQUAL("5", bst->get(5));
    CHECK_EQUAL("8", bst->get(8));

    CHECK_THROW(bst->insert(1, "2"), runtime_error);
    delete bst;
}

/**
 * TEST case to test LinkedBST::getSize() and 
 * LinkedBST::isEmpty() methods in an empty BST.
 */
TEST(emptyBST) {
    LinkedBST<int, string>* bst = new LinkedBST<int, string>();
    CHECK_EQUAL(0, bst->getSize());
    CHECK(bst->isEmpty());
    bst->checkInvariants();
    delete bst;
}

/**
 * TEST case to test the height of an empty BST.
 */
TEST(negativeHeight) {
    LinkedBST<int,string>* tree = new LinkedBST<int,string>();
    CHECK_EQUAL(-1, tree->getHeight());
    delete tree;
}

/**
 * TEST case to test the LinkedBST::get(K) mehtod.
 */
TEST(getBST) {
    LinkedBST<int, string>* bst = makeExampleBST();
    for (int i = 1; i <= 9; i++) {
        if (i != 5 && i != 8) {
            CHECK_EQUAL(to_string(i), bst->get(i));
        }
    }
    CHECK_THROW(bst->get(5), runtime_error);
    CHECK_THROW(bst->get(8), runtime_error);
    delete bst;
}

/**
 * TEST case to test the LinkedBST::getSize() method.
 */
TEST(sizeBST) {
    LinkedBST<int, string>* bst = makeExampleBST();
    CHECK_EQUAL(7, bst->getSize());
    CHECK(!bst->isEmpty());
    delete bst;
}

/**
 * TEST case to test the LinkedBST::update(K, V) method.
 */
TEST(updateBST) {
    LinkedBST<int, string>* bst = makeExampleBST();
    bst->update(1, "new1");
    CHECK_EQUAL("new1", bst->get(1));
    bst->update(3, "solve");
    CHECK_EQUAL("solve", bst->get(3));
    bst->update(1, "new_new1");
    CHECK_EQUAL("new_new1", bst->get(1));
    
    CHECK_THROW(bst->update(5, "hello"), runtime_error);
    delete bst;
}

/**
 * TEST case to test the LinkedBST::contains(K) method.
 */
TEST(containsBST) {
    LinkedBST<int, string>* bst = makeExampleBST();
    for (int i = 1; i <= 9; i++) {
        if (i != 5 && i != 8) {
            CHECK_EQUAL(true, bst->contains(i));
        }
    }
    CHECK_EQUAL(false, bst->contains(8));
    CHECK_EQUAL(false, bst->contains(5));
    CHECK_EQUAL(false, bst->contains(987654));
    delete bst;
}

/**
 * TEST case to test the LinkedBST::remove(K) method.
 */
TEST(removeBST) {
    LinkedBST<int, string>* bst = makeExampleBST();
    LinkedBST<int, string>* bst2 = makeExampleBST();
    CHECK_THROW(bst->remove(5), runtime_error);
    
    for (int i = 1; i <= 9; i++) {
        if (i != 5 && i != 8) {
            bst->remove(i);
            CHECK_EQUAL(false, bst->contains(i));
        }
    }
    CHECK_EQUAL(0, bst->getSize());

    bst2->remove(2);
    CHECK_EQUAL(6, bst2->getSize());

    delete bst;
    delete bst2;
}

/**
 * TEST case to test the LinkedBST::getKeys() method.
 */
TEST(getKeysBST) {
    LinkedBST<int, string>* bst = makeExampleBST();
    vector<int> key_vector = bst->getKeys();
    CHECK_EQUAL(6, key_vector[0]);
    CHECK_EQUAL(2, key_vector[1]);
    CHECK_EQUAL(7, key_vector[2]);
    CHECK_EQUAL(1, key_vector[3]);
    CHECK_EQUAL(4, key_vector[4]);
    CHECK_EQUAL(9, key_vector[5]);
    CHECK_EQUAL(3, key_vector[6]);
    delete bst;
}

/**
 * TEST case to test the LinkedBST::getItems() method.
 */
TEST(getItemsBST) {
    LinkedBST<int, string>* bst = makeExampleBST();
    vector<pair<int, string>> item_vector = bst->getItems();
    vector<pair<int, string>> expected;
    expected.push_back(pair<int, string>(6, "6"));
    expected.push_back(pair<int, string>(2, "2"));
    expected.push_back(pair<int, string>(7, "7"));
    expected.push_back(pair<int, string>(1, "1"));
    expected.push_back(pair<int, string>(4, "4"));
    expected.push_back(pair<int, string>(9, "9"));
    expected.push_back(pair<int, string>(3, "3"));
    REQUIRE CHECK_EQUAL(expected.size(), item_vector.size());
    for (int i = 0; i < expected.size(); i++) {
        CHECK_EQUAL(expected[i].first, item_vector[i].first);
        CHECK_EQUAL(expected[i].second, item_vector[i].second);
    }
    delete bst;
}

/**
 * TEST case to test the LinkedBST::getHeight() method.
 */
TEST(heightBST) {
    LinkedBST<int, string>* bst = makeExampleBST();
    CHECK_EQUAL(3, bst->getHeight());
    bst->remove(7);
    CHECK_EQUAL(3, bst->getHeight());
    bst->remove(4);
    CHECK_EQUAL(2, bst->getHeight());
    delete bst;
}

/**
 * TEST case to test the LinkedBST::getMaxKey() method.
 */
TEST(getMaximumKeyBST) {
    LinkedBST<int, string>* bst = makeExampleBST();
    CHECK_EQUAL(9, bst->getMaxKey());
    delete bst;

    LinkedBST<int, string> emptyBST;
    CHECK_THROW(emptyBST.getMaxKey(), runtime_error);
}

/**
 * TEST case to test the LinkedBST::getMinKey() method.
 */
TEST(getMininumKeyBST) {
    LinkedBST<int, string>* bst = makeExampleBST();
    CHECK_EQUAL(1, bst->getMinKey());
    delete bst;

    LinkedBST<int, string> emptyBST;
    CHECK_THROW(emptyBST.getMinKey(), runtime_error);
}

/**
 * TEST case to test the LinkedBST::traversePreOrder() method.
 */
TEST(preOrderTraversalBST) {
    LinkedBST<int, string>* bst = makeExampleBST();
    vector<pair<int, string>> traversal = bst->traversePreOrder();

    // build vector containing what the ordering should be
    vector<pair<int, string>> expected;
    expected.push_back(pair<int, string>(6, "6"));
    expected.push_back(pair<int, string>(2, "2"));
    expected.push_back(pair<int, string>(1, "1"));
    expected.push_back(pair<int, string>(4, "4"));
    expected.push_back(pair<int, string>(3, "3"));
    expected.push_back(pair<int, string>(7, "7"));
    expected.push_back(pair<int, string>(9, "9"));
    REQUIRE CHECK_EQUAL(expected.size(), traversal.size());
    for (int i = 0; i < expected.size(); i++) {
        CHECK_EQUAL(expected[i].first, traversal[i].first);
        CHECK_EQUAL(expected[i].second, traversal[i].second);
    }
    delete bst;
}

/**
 * TEST case to test the LinkedBST::traverseInOrder() method.
 */
TEST(inOrderTraversalBST) {
    LinkedBST<int, string>* bst = makeExampleBST();
    vector<pair<int, string>> traversal = bst->traverseInOrder();

    // build vector containing what the ordering should be
    vector<pair<int, string>> expected;
    expected.push_back(pair<int, string>(1, "1"));
    expected.push_back(pair<int, string>(2, "2"));
    expected.push_back(pair<int, string>(3, "3"));
    expected.push_back(pair<int, string>(4, "4"));
    expected.push_back(pair<int, string>(6, "6"));
    expected.push_back(pair<int, string>(7, "7"));
    expected.push_back(pair<int, string>(9, "9"));
    REQUIRE CHECK_EQUAL(expected.size(), traversal.size());
    for (int i = 0; i < expected.size(); i++) {
        CHECK_EQUAL(expected[i].first, traversal[i].first);
        CHECK_EQUAL(expected[i].second, traversal[i].second);
    }
    delete bst;
}

/**
 * TEST case to test the LinkedBST::traversePostOrder() method.
 */
TEST(postOrderTraversalBST) {
    LinkedBST<int, string>* bst = makeExampleBST();
    vector<pair<int, string>> traversal = bst->traversePostOrder();

    // build vector containing what the ordering should be
    vector<pair<int, string>> expected;
    expected.push_back(pair<int, string>(1, "1"));
    expected.push_back(pair<int, string>(3, "3"));
    expected.push_back(pair<int, string>(4, "4"));
    expected.push_back(pair<int, string>(2, "2"));
    expected.push_back(pair<int, string>(9, "9"));
    expected.push_back(pair<int, string>(7, "7"));
    expected.push_back(pair<int, string>(6, "6"));
    REQUIRE CHECK_EQUAL(expected.size(), traversal.size());
    for (int i = 0; i < expected.size(); i++) {
        CHECK_EQUAL(expected[i].first, traversal[i].first);
        CHECK_EQUAL(expected[i].second, traversal[i].second);
    }
    delete bst;
}

/**
 * TEST case to test the LinkedBST::traverseLevelOrder() method.
 */
TEST(levelOrderTraversalBST) {
    LinkedBST<int, string>* bst = makeExampleBST();
    vector<pair<int, string>> traversal = bst->traverseLevelOrder();

    // build vector containing what the ordering should be
    vector<pair<int, string>> expected;
    expected.push_back(pair<int, string>(6, "6"));
    expected.push_back(pair<int, string>(2, "2"));
    expected.push_back(pair<int, string>(7, "7"));
    expected.push_back(pair<int, string>(1, "1"));
    expected.push_back(pair<int, string>(4, "4"));
    expected.push_back(pair<int, string>(9, "9"));
    expected.push_back(pair<int, string>(3, "3"));
    REQUIRE CHECK_EQUAL(expected.size(), traversal.size());
    for (int i = 0; i < expected.size(); i++) {
        CHECK_EQUAL(expected[i].first, traversal[i].first);
        CHECK_EQUAL(expected[i].second, traversal[i].second);
    }
    delete bst;
}

/**
 * TEST case to test the results of traversal methods in an empty BST.
 */
TEST(emptyBSTallTraversals) {
    LinkedBST<int, string> bst;
    CHECK_EQUAL(0, bst.traversePreOrder().size());
    CHECK_EQUAL(0, bst.traverseInOrder().size());
    CHECK_EQUAL(0, bst.traversePostOrder().size());
    CHECK_EQUAL(0, bst.traverseLevelOrder().size());
}

int main() {
    return UnitTest::RunAllTests();
}
