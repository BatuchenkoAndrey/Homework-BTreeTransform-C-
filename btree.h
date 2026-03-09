#ifndef BTREE_H
#define BTREE_H

#include <vector>
#include <memory>
#include <iostream>
#include <algorithm>

// Узел B-дерева
class BNode {
public:
    int K;  // Степень дерева
    std::vector<int> keys;  // Ключи в узле
    std::vector<std::shared_ptr<BNode>> children;  // Указатели на детей
    bool isLeaf;  // Листовой ли узел
    
    BNode(int k, bool leaf = true);
    

    int findKey(int k) const;  // Найти позицию ключа, куда нужно его вставить
    void insertNonFull(int key);  // Вставить ключ в незаполненный узел
    void splitChild(int i);  // Разделить потомка
    void insertLeaf(int key);  // Вставить ключ в листовой узел
    void removeKey(int key);  // Удалить ключ
    
    void print() const;  // Вывод узла
};

// B-дерево
class BTree {
public:
    std::shared_ptr<BNode> root;  // Корень дерева
    int K;  // Степень дерева
    int nodeCount;  // Количество узлов в дереве
    
    BTree(int k);
    ~BTree() = default;
    
    void insert(int key);  // Вставить ключ
    bool search(int key) const;  // Поиск ключа
    void inOrderTraversal(std::vector<int>& result, const std::shared_ptr<BNode>& node = nullptr) const;  // In-order обход
    
    int getK() const { return K; }
    int getNodeCount() const { return nodeCount; } 
    
    void printTree(const std::shared_ptr<BNode>& node = nullptr, int indent = 0) const;  // Вывод дерева
    
    // подсчёт узлов
    void countNodes(const std::shared_ptr<BNode>& node, int& count) const;
};

//  Преобразование
class BTreeTransformer {
public:
    static std::shared_ptr<BTree> transform(const std::shared_ptr<BTree>& tree, int K2);
    
private:
    static void collectKeys(const std::shared_ptr<BNode>& node, std::vector<int>& keys);
};

#endif
