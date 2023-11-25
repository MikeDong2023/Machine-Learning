#include "BinarySearchTree.hpp"
#include "unit_test_framework.hpp"


TEST(test_empty) {
    BinarySearchTree<int> testTree;
    ASSERT_TRUE(testTree.empty());
}

TEST(test_size) {
    BinarySearchTree<int> testTree;
    ASSERT_TRUE(testTree.size() == 0);
}

TEST(test_height) {
    BinarySearchTree<int> testTree;
    testTree.insert(10);
    testTree.insert(20);
    testTree.insert(30);
    testTree.insert(15);
    ASSERT_TRUE(testTree.height() == 3);
}

TEST(test_insert) {
    BinarySearchTree<int> testTree;
    testTree.insert(10);
    ASSERT_TRUE(testTree.size() == 1);
    testTree.insert(20);
    testTree.insert(5);
    ASSERT_TRUE(testTree.size() == 3);
    testTree.insert(30);
    testTree.insert(15);
    testTree.insert(7);
    testTree.insert(0);
    ASSERT_TRUE(testTree.height() == 3);
    ASSERT_TRUE(testTree.size() == 7);
}

TEST(test_copy_deep_and_shallow) {
    BinarySearchTree<int> testTree;
    testTree.insert(10);
    testTree.insert(20);
    testTree.insert(5);
    testTree.insert(30);
    testTree.insert(15);
    testTree.insert(7);
    testTree.insert(0);
    BinarySearchTree<int> copyTree(testTree);
    ASSERT_TRUE(*testTree.begin() == *copyTree.begin());
    testTree.insert(6);
    ASSERT_TRUE(testTree.size()!= copyTree.size());

}

TEST(test_find) {
    BinarySearchTree<int> testTree;
    testTree.insert(10);
    testTree.insert(20);
    testTree.insert(5);
    testTree.insert(30);
    testTree.insert(15);
    testTree.insert(7);
    testTree.insert(0);
    ASSERT_EQUAL(*testTree.find(7), 7);
}

TEST(test_min) {
    BinarySearchTree<int> testTree;
    testTree.insert(10);
    testTree.insert(20);
    testTree.insert(5);
    testTree.insert(30);
    testTree.insert(15);
    testTree.insert(7);
    testTree.insert(0);
    ASSERT_TRUE(*testTree.begin() == 0);
    ASSERT_TRUE(*++testTree.begin() == 5);
    ASSERT_TRUE(*++++testTree.begin() == 7);
}

TEST(test_check_sorting) {
    BinarySearchTree<int> testTree;
    testTree.insert(10);
    testTree.insert(20);
    testTree.insert(5);
    testTree.insert(30);
    testTree.insert(15);
    testTree.insert(7);
    testTree.insert(0);
    ASSERT_TRUE(testTree.check_sorting_invariant());
    *testTree.begin() = 100;
    ASSERT_FALSE(testTree.check_sorting_invariant());
}

TEST(test_min_greater_than) {
    BinarySearchTree<int> testTree;
    testTree.insert(10);
    testTree.insert(20);
    testTree.insert(5);
    testTree.insert(30);
    testTree.insert(15);
    testTree.insert(7);
    testTree.insert(0);
    ASSERT_TRUE(*testTree.min_greater_than(5) == 7);
    ASSERT_TRUE(*testTree.min_greater_than(15) == 20);
    ASSERT_TRUE(testTree.min_greater_than(30) == testTree.end());
}

TEST(test_traverse_inorder) {
    BinarySearchTree<int> testTree;
    testTree.insert(10);
    testTree.insert(20);
    testTree.insert(5);
    testTree.insert(30);
    testTree.insert(15);
    testTree.insert(7);
    testTree.insert(0);
    std::ostringstream oss;
    testTree.traverse_inorder(oss);
    std::string testOutput = oss.str();
    ASSERT_EQUAL(testOutput, "0 5 7 10 15 20 30 ");
}

TEST(test_traverse_preorder) {
    BinarySearchTree<int> testTree;
    testTree.insert(10);
    testTree.insert(20);
    testTree.insert(5);
    testTree.insert(30);
    testTree.insert(15);
    testTree.insert(7);
    testTree.insert(0);
    std::ostringstream oss;
    testTree.traverse_preorder(oss);
    std::string testOutput = oss.str();
    ASSERT_EQUAL(testOutput, "10 5 0 7 20 15 30 ");
}
TEST_MAIN()
