#ifndef BTREE_H
#define BTREE_H

#include <vector>
#include <memory>
#include <iostream>

class BNode {
public:
    int K;  // Степень дерева
    std::vector<int> keys;  
    std::vector<std::shared_ptr<BNode>> children;
    bool isLeaf;
    
    BNode(int k, bool leaf = true);
    
    void insertNonFull(int key); 
    void splitChild(int i);
    
    void print() const;
};


class BTree {
public:
    std::shared_ptr<BNode> root;
    int K;  // Степень дерева
    int nodeCount;  
    
    BTree(int k);
    ~BTree() = default;
    
    void insert(int key);  
    bool search(int key) const;  
    void inOrderTraversal(std::vector<int>& result, const std::shared_ptr<BNode>& node = nullptr) const;
    
    std::shared_ptr<BTree> transform(int K2) const;  // Трансформировать в дерево с новой степенью
    
    int getK() const { return K; }
    int getNodeCount() const { return nodeCount; } 
    
    void printTree(const std::shared_ptr<BNode>& node = nullptr, int indent = 0) const;
    
  
    void countNodes(const std::shared_ptr<BNode>& node, int& count) const;
};

#endif
