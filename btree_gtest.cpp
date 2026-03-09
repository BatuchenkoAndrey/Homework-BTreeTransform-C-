#include "btree.h"
#include <gtest/gtest.h>
#include <vector>

class BTreeTransformTest : public ::testing::Test {
protected:
    void testTransformation(std::vector<int> keys, int K1, int K2) {
        // Создать исходное дерево
        auto tree1 = std::make_shared<BTree>(K1);
        for (int key : keys) {
            tree1->insert(key);
        }
        
        // Собрать ключи из исходного дерева
        std::vector<int> original_keys;
        tree1->inOrderTraversal(original_keys);
        
        // Трансформировать
        auto tree2 = BTreeTransformer::transform(tree1, K2);
        
        // Собрать ключи из преобразованного дерева
        std::vector<int> transformed_keys;
        tree2->inOrderTraversal(transformed_keys);
        
        // Проверить, что ключи совпадают
        EXPECT_EQ(original_keys, transformed_keys) 
            << "Keys mismatch after transformation";
        
        // Проверить поиск для каждого ключа
        for (int key : original_keys) {
            EXPECT_TRUE(tree2->search(key)) 
                << "Key " << key << " not found after transformation";
        }
    }
};

// ===== ОСНОВНЫЕ ФУНКЦИОНАЛЬНЫЕ ТЕСТЫ =====

TEST_F(BTreeTransformTest, K2GreaterThanK1_Widening) {
    // K=2 → K=3 (узкое → широкое)
    testTransformation({1, 2, 3, 4, 5, 6, 7}, 2, 3);
}

TEST_F(BTreeTransformTest, K2LessThanK1_Narrowing) {
    // K=3 → K=2 (широкое → узкое)
    testTransformation({10, 20, 30, 40, 50, 60, 70, 80}, 3, 2);
}

TEST_F(BTreeTransformTest, K2MuchGreaterThanK1) {
    // K=2 → K=4 (узкое → много шире)
    testTransformation({5, 15, 25, 35, 45, 55, 65, 75, 85, 95}, 2, 4);
}

TEST_F(BTreeTransformTest, K2MuchLessThanK1) {
    // K=4 → K=2 (много шире → узкое)
    testTransformation({2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24}, 4, 2);
}

TEST_F(BTreeTransformTest, K2ExtremelyGreaterThanK1) {
    // K=2 → K=5 (очень узкое → очень широкое)
    testTransformation({1, 3, 5, 7, 9, 11, 13}, 2, 5);
}

TEST_F(BTreeTransformTest, K1EqualsK2_NoChange) {
    // K=3 → K=3 (без изменений)
    testTransformation({10, 15, 20, 25, 30, 35}, 3, 3);
}

// ===== ГРАНИЧНЫЕ СЛУЧАИ =====

TEST_F(BTreeTransformTest, EdgeCase_SingleKey) {
    // Один ключ
    testTransformation({42}, 2, 3);
}

TEST_F(BTreeTransformTest, EdgeCase_TwoKeys) {
    // Два ключа
    testTransformation({10, 20}, 2, 3);
}

TEST_F(BTreeTransformTest, EdgeCase_FillsOneNode) {
    // Точно заполняет один узел
    testTransformation({1, 2, 3, 4, 5}, 3, 2);
}

TEST_F(BTreeTransformTest, EdgeCase_LargeKValues) {
    // Большие K
    testTransformation({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}, 5, 10);
}

TEST_F(BTreeTransformTest, EdgeCase_DescendingThenAscending) {
    // K=10 → K=2 (много узлов)
    testTransformation({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}, 10, 2);
}

TEST_F(BTreeTransformTest, EdgeCase_SparseNumbers) {
    // Разреженные числа
    testTransformation({1, 10, 100, 1000, 10000}, 2, 5);
}

TEST_F(BTreeTransformTest, EdgeCase_SequentialNumbers) {
    // Последовательные числа
    testTransformation({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20}, 3, 4);
}

TEST_F(BTreeTransformTest, EdgeCase_ExtremeWidening) {
    // K=2 → K=20 (экстремальное расширение)
    testTransformation({1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, 2, 20);
}

TEST_F(BTreeTransformTest, EdgeCase_ExtremeNarrowing) {
    // K=20 → K=2 (экстремальное сужение)
    testTransformation({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}, 20, 2);
}

// ===== ТЕСТЫ КОРРЕКТНОСТИ СТРУКТУРЫ =====

TEST_F(BTreeTransformTest, StructureValidity_AllKeysPresent) {
    auto tree1 = std::make_shared<BTree>(2);
    std::vector<int> keys = {1, 2, 3, 4, 5, 6, 7};
    for (int key : keys) {
        tree1->insert(key);
    }
    
    auto tree2 = BTreeTransformer::transform(tree1, 3);
    
    // Проверить количество ключей
    std::vector<int> result;
    tree2->inOrderTraversal(result);
    EXPECT_EQ(result.size(), keys.size());
    EXPECT_EQ(result, keys);
}

TEST_F(BTreeTransformTest, SearchAccuracy_AllKeysSearchable) {
    auto tree1 = std::make_shared<BTree>(3);
    std::vector<int> keys = {10, 20, 30, 40, 50, 60, 70, 80};
    for (int key : keys) {
        tree1->insert(key);
    }
    
    auto tree2 = BTreeTransformer::transform(tree1, 2);
    
    // Проверить поиск каждого ключа
    for (int key : keys) {
        EXPECT_TRUE(tree2->search(key)) 
            << "Key " << key << " should be found";
    }
    
    // Проверить что несуществующие ключи не найдены
    EXPECT_FALSE(tree2->search(15));
    EXPECT_FALSE(tree2->search(45));
    EXPECT_FALSE(tree2->search(100));
}

TEST_F(BTreeTransformTest, InOrderTraversal_Correctness) {
    auto tree1 = std::make_shared<BTree>(2);
    std::vector<int> keys = {5, 15, 25, 35, 45, 55, 65, 75, 85, 95};
    for (int key : keys) {
        tree1->insert(key);
    }
    
    auto tree2 = BTreeTransformer::transform(tree1, 4);
    
    std::vector<int> result;
    tree2->inOrderTraversal(result);
    
    EXPECT_EQ(result, keys);
    
    // Проверить что результат отсортирован
    for (size_t i = 1; i < result.size(); i++) {
        EXPECT_LT(result[i-1], result[i]);
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
