#include "btree.h"
#include <gtest/gtest.h>
#include <vector>

class BTreeTest : public ::testing::Test {
protected:
    // Вспомогательная функция для проверки трансформации
    void verifyTransformation(std::vector<int> keys, int K1, int K2) {
        auto tree1 = std::make_shared<BTree>(K1);
        for (int key : keys) {
            tree1->insert(key);
        }
        
        std::vector<int> original_keys;
        tree1->inOrderTraversal(original_keys);
        
        auto tree2 = tree1->transform(K2);
        
        std::vector<int> transformed_keys;
        tree2->inOrderTraversal(transformed_keys);
        
        EXPECT_EQ(original_keys, transformed_keys) 
            << "Keys mismatch after transformation";
        
        for (int key : original_keys) {
            EXPECT_TRUE(tree2->search(key)) 
                << "Key " << key << " not found";
        }
    }
};

// ============ ТЕСТЫ insertNonFull() ============

TEST_F(BTreeTest, InsertNonFull_SingleKeyInLeaf) {
    auto tree = std::make_shared<BTree>(2);
    tree->insert(10);
    
    std::vector<int> result;
    tree->inOrderTraversal(result);
    EXPECT_EQ(result, std::vector<int>{10});
}

TEST_F(BTreeTest, InsertNonFull_MultipleKeysInLeaf) {
    auto tree = std::make_shared<BTree>(3);
    std::vector<int> keys = {5, 3, 7, 1, 9};
    for (int k : keys) tree->insert(k);
    
    std::vector<int> result;
    tree->inOrderTraversal(result);
    std::vector<int> expected = {1, 3, 5, 7, 9};
    EXPECT_EQ(result, expected);
}

TEST_F(BTreeTest, InsertNonFull_CausingSplit) {
    auto tree = std::make_shared<BTree>(2);
    for (int i = 1; i <= 5; i++) {
        tree->insert(i);
    }
    
    std::vector<int> result;
    tree->inOrderTraversal(result);
    std::vector<int> expected = {1, 2, 3, 4, 5};
    EXPECT_EQ(result, expected);
    EXPECT_GT(tree->getNodeCount(), 1);  // Должно быть несколько узлов
}

// ============ ТЕСТЫ splitChild() ============

TEST_F(BTreeTest, SplitChild_FullNodeDivision) {
    auto tree = std::make_shared<BTree>(2);
    // Заполняем дерево так, чтобы произошло разделение
    for (int i = 1; i <= 7; i++) {
        tree->insert(i);
    }
    
    std::vector<int> result;
    tree->inOrderTraversal(result);
    std::vector<int> expected = {1, 2, 3, 4, 5, 6, 7};
    EXPECT_EQ(result, expected);
}

// ============ ТЕСТЫ search() ============

TEST_F(BTreeTest, Search_ExistingKeys) {
    auto tree = std::make_shared<BTree>(3);
    std::vector<int> keys = {10, 20, 30, 40, 50};
    for (int k : keys) tree->insert(k);
    
    for (int k : keys) {
        EXPECT_TRUE(tree->search(k)) << "Key " << k << " should be found";
    }
}

TEST_F(BTreeTest, Search_NonExistingKeys) {
    auto tree = std::make_shared<BTree>(3);
    std::vector<int> keys = {10, 20, 30};
    for (int k : keys) tree->insert(k);
    
    std::vector<int> not_present = {5, 15, 25, 35};
    for (int k : not_present) {
        EXPECT_FALSE(tree->search(k)) << "Key " << k << " should not be found";
    }
}

TEST_F(BTreeTest, Search_Multi_Level_Tree) {
    auto tree = std::make_shared<BTree>(2);
    for (int i = 1; i <= 10; i++) {
        tree->insert(i);
    }
    
    // Проверяем все ключи в многоуровневом дереве
    for (int i = 1; i <= 10; i++) {
        EXPECT_TRUE(tree->search(i)) << "Key " << i << " not found in multi-level tree";
    }
    
    EXPECT_FALSE(tree->search(0));
    EXPECT_FALSE(tree->search(11));
}

// ============ ТЕСТЫ inOrderTraversal() ============

TEST_F(BTreeTest, InOrderTraversal_Sorted) {
    auto tree = std::make_shared<BTree>(3);
    std::vector<int> input = {15, 5, 25, 10, 20, 30, 3};
    for (int k : input) tree->insert(k);
    
    std::vector<int> result;
    tree->inOrderTraversal(result);
    
    // Проверяем что результат отсортирован
    EXPECT_TRUE(std::is_sorted(result.begin(), result.end()));
    
    // Проверяем что все элементы присутствуют
    std::sort(input.begin(), input.end());
    EXPECT_EQ(result, input);
}

TEST_F(BTreeTest, InOrderTraversal_Empty) {
    auto tree = std::make_shared<BTree>(2);
    std::vector<int> result;
    tree->inOrderTraversal(result);
    EXPECT_EQ(result.size(), 0);
}

TEST_F(BTreeTest, InOrderTraversal_Single) {
    auto tree = std::make_shared<BTree>(2);
    tree->insert(42);
    
    std::vector<int> result;
    tree->inOrderTraversal(result);
    EXPECT_EQ(result, std::vector<int>{42});
}

// ============ ТЕСТЫ printTree() ============

TEST_F(BTreeTest, PrintTree_NoOutput) {
    // Просто проверяем что функция вызывается без ошибок
    auto tree = std::make_shared<BTree>(2);
    tree->insert(10);
    
    // Перенаправляем stdout чтобы не загромождать вывод
    std::ostringstream oss;
    std::streambuf* old_cout = std::cout.rdbuf(oss.rdbuf());
    
    tree->printTree();
    
    std::cout.rdbuf(old_cout);
    EXPECT_FALSE(oss.str().empty());
}

// ============ ТЕСТЫ TRANSFORM() ============

TEST_F(BTreeTest, Transform_K2GreaterThanK1) {
    verifyTransformation({1, 2, 3, 4, 5, 6, 7}, 2, 3);
}

TEST_F(BTreeTest, Transform_K2LessThanK1) {
    verifyTransformation({10, 20, 30, 40, 50, 60, 70, 80}, 3, 2);
}

TEST_F(BTreeTest, Transform_K2EqualsK1) {
    verifyTransformation({5, 10, 15, 20}, 3, 3);
}

TEST_F(BTreeTest, Transform_ExtremeWidening) {
    verifyTransformation({1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, 2, 10);
}

TEST_F(BTreeTest, Transform_ExtremeNarrowing) {
    verifyTransformation({1, 2, 3, 4, 5}, 5, 2);
}

TEST_F(BTreeTest, Transform_SingleKey) {
    verifyTransformation({42}, 2, 5);
}

TEST_F(BTreeTest, Transform_ManyKeys) {
    std::vector<int> keys;
    for (int i = 1; i <= 50; i++) {
        keys.push_back(i);
    }
    verifyTransformation(keys, 2, 4);
}

TEST_F(BTreeTest, Transform_RandomOrder) {
    verifyTransformation({15, 10, 20, 5, 25, 3, 30}, 2, 4);
}

// ============ ИНТЕГРАЦИОННЫЕ ТЕСТЫ ============

TEST_F(BTreeTest, Integration_LargeDataset) {
    auto tree = std::make_shared<BTree>(4);
    std::vector<int> keys;
    
    // Вставляем 100 ключей
    for (int i = 1; i <= 100; i++) {
        keys.push_back(i * 2);
        tree->insert(i * 2);
    }
    
    // Проверяем поиск
    for (int k : keys) {
        EXPECT_TRUE(tree->search(k));
    }
    
    // Проверяем in-order
    std::vector<int> result;
    tree->inOrderTraversal(result);
    std::sort(keys.begin(), keys.end());
    EXPECT_EQ(result, keys);
}

TEST_F(BTreeTest, Integration_TransformWithLargeDataset) {
    std::vector<int> keys;
    for (int i = 1; i <= 50; i++) {
        keys.push_back(i);
    }
    
    verifyTransformation(keys, 3, 2);
    verifyTransformation(keys, 3, 4);
    verifyTransformation(keys, 3, 5);
}

// ============ HEAVY LOAD TESTS ============

TEST_F(BTreeTest, HeavyLoad_Insert10000Keys) {
    auto tree = std::make_shared<BTree>(4);
    
    for (int i = 1; i <= 10000; i++) {
        tree->insert(i);
    }
    
    std::vector<int> result;
    tree->inOrderTraversal(result);
    
    EXPECT_EQ(result.size(), 10000);
    EXPECT_EQ(result.front(), 1);
    EXPECT_EQ(result.back(), 10000);
    
    // Проверка что все элементы на месте
    for (int i = 1; i <= 10000; i++) {
        EXPECT_TRUE(tree->search(i));
    }
}

TEST_F(BTreeTest, HeavyLoad_Transform10000Keys) {
    auto tree1 = std::make_shared<BTree>(3);
    
    // Вставляем 10000 ключей
    for (int i = 1; i <= 10000; i++) {
        tree1->insert(i);
    }
    
    // Трансформируем несколько раз
    auto tree2 = tree1->transform(2);
    auto tree3 = tree2->transform(5);
    auto tree4 = tree3->transform(3);
    
    std::vector<int> keys1, keys2, keys3, keys4;
    tree1->inOrderTraversal(keys1);
    tree2->inOrderTraversal(keys2);
    tree3->inOrderTraversal(keys3);
    tree4->inOrderTraversal(keys4);
    
    EXPECT_EQ(keys1, keys2);
    EXPECT_EQ(keys2, keys3);
    EXPECT_EQ(keys3, keys4);
}

TEST_F(BTreeTest, HeavyLoad_SearchMany) {
    auto tree = std::make_shared<BTree>(5);
    
    for (int i = 1; i <= 5000; i++) {
        tree->insert(i);
    }
    
    // Ищем много элементов
    for (int i = 1; i <= 5000; i++) {
        EXPECT_TRUE(tree->search(i));
    }
    
    // Ищем несуществующие элементы
    for (int i = 5001; i <= 10000; i++) {
        EXPECT_FALSE(tree->search(i));
    }
}

TEST_F(BTreeTest, HeavyLoad_ReverseInsert) {
    auto tree = std::make_shared<BTree>(4);
    
    // Вставляем в обратном порядке
    for (int i = 10000; i >= 1; i--) {
        tree->insert(i);
    }
    
    std::vector<int> result;
    tree->inOrderTraversal(result);
    
    EXPECT_EQ(result.size(), 10000);
    EXPECT_EQ(result.front(), 1);
    EXPECT_EQ(result.back(), 10000);
}

// ============ MAIN С ФИНАЛЬНЫМ ВЫВОДОМ ============

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    
    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << "  B-TREE COMPREHENSIVE TEST SUITE\n";
    std::cout << std::string(60, '=') << "\n\n";
    
    int result = RUN_ALL_TESTS();
    
    std::cout << "\n" << std::string(60, '=') << "\n";
    if (result == 0) {
        std::cout << "✓ ALL TESTS PASSED\n";
    } else {
        std::cout << "✗ SOME TESTS FAILED\n";
    }
    std::cout << std::string(60, '=') << "\n";
    std::cout << "\nTEST COVERAGE:\n";
    std::cout << "  ✓ insertNonFull() - листовые узлы и многоуровневые деревья\n";
    std::cout << "  ✓ splitChild() - разделение переполненных узлов\n";
    std::cout << "  ✓ search() - поиск существующих и несуществующих ключей\n";
    std::cout << "  ✓ inOrderTraversal() - корректность сортировки\n";
    std::cout << "  ✓ printTree() - вывод структуры\n";
    std::cout << "  ✓ transform() - трансформация между степенями K\n";
    std::cout << "  ✓ Integration tests - большие наборы данных\n";
    std::cout << std::string(60, '=') << "\n\n";
    
    return result;
}
