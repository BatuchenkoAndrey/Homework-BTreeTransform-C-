#include "btree.h"
#include <iostream>
#include <cassert>

void testEdgeCase(const std::string& name, std::vector<int> keys, int K1, int K2) {
    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << name << "\n";
    std::cout << std::string(60, '=') << "\n";
    
    // Создать дерево
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
    
    // Проверки
    bool keys_match = (original_keys == transformed_keys);
    std::cout << "Input:      " << original_keys.size() << " keys\n";
    std::cout << "Original:   " << tree1->getNodeCount() << " nodes\n";
    std::cout << "Transformed: " << tree2->getNodeCount() << " nodes\n";
    std::cout << "Keys match: " << (keys_match ? "✓ YES" : "✗ NO") << "\n";
    
    if (!keys_match) {
        std::cout << "ERROR! Keys mismatch!\n";
        std::cout << "Original: ";
        for (int k : original_keys) std::cout << k << " ";
        std::cout << "\nTransformed: ";
        for (int k : transformed_keys) std::cout << k << " ";
        std::cout << "\n";
    }
    
    // Проверить поиск
    bool search_ok = true;
    for (int key : original_keys) {
        if (!tree2->search(key)) {
            std::cout << "ERROR! Key " << key << " not found after transformation!\n";
            search_ok = false;
        }
    }
    std::cout << "Search: " << (search_ok ? "✓ OK" : "✗ FAILED") << "\n";
    
    assert(keys_match && search_ok);
}

int main() {
    std::cout << "\n╔══════════════════════════════════════════════╗\n";
    std::cout << "║  EDGE CASE TESTS FOR B-TREE TRANSFORMATION  ║\n";
    std::cout << "╚══════════════════════════════════════════════╝\n";
    
    // Test 1: Single key
    testEdgeCase("TEST 1: Single key (K=2 → K=3)", {42}, 2, 3);
    
    // Test 2: Two keys
    testEdgeCase("TEST 2: Two keys (K=2 → K=3)", {10, 20}, 2, 3);
    
    // Test 3: Exactly fills one node
    testEdgeCase("TEST 3: Fills one node (K=3, 5 keys)", {1, 2, 3, 4, 5}, 3, 2);
    
    // Test 4: Large K values
    testEdgeCase("TEST 4: Large K values (K=5 → K=10)", 
        {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}, 5, 10);
    
    // Test 5: Descending then ascending K
    testEdgeCase("TEST 5: K=10 → K=2 (many nodes expected)",
        {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}, 10, 2);
    
    // Test 6: Many duplicates avoided (all unique)
    testEdgeCase("TEST 6: Sparse numbers",
        {1, 10, 100, 1000, 10000}, 2, 5);
    
    // Test 7: Sequential numbers
    testEdgeCase("TEST 7: Sequential 1-20",
        {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20}, 3, 4);
    
    // Test 8: Same K
    testEdgeCase("TEST 8: K=5 → K=5 (no change)",
        {5, 10, 15, 20, 25, 30, 35, 40}, 5, 5);
    
    // Test 9: Maximum difference
    testEdgeCase("TEST 9: K=2 → K=20 (extreme widening)",
        {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, 2, 20);
    
    // Test 10: Reverse maximum difference
    testEdgeCase("TEST 10: K=20 → K=2 (extreme narrowing)",
        {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}, 20, 2);
    
    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << "✓ ALL EDGE CASE TESTS PASSED\n";
    std::cout << std::string(60, '=') << "\n\n";
    
    return 0;
}
