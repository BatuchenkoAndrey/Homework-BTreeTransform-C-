#include "btree.h"

BNode::BNode(int k, bool leaf)
    : K(k), isLeaf(leaf) {}

void BNode::insertNonFull(int key) {
    int i = keys.size() - 1;
    
    if (isLeaf) {
        keys.push_back(0);
        while (i >= 0 && keys[i] > key) {
            keys[i + 1] = keys[i];
            i--;
        }
        keys[i + 1] = key;
    } else {
        // Найти позицию для вставки
        while (i >= 0 && keys[i] > key) {
            i--;
        }
        i++;
        
        // Проверить, полон ли потомок
        if (static_cast<int>(children[i]->keys.size()) == 2 * K - 1) {
            splitChild(i);
            if (keys[i] < key) {
                i++;
            }
        }
        children[i]->insertNonFull(key);
    }
}

void BNode::splitChild(int i) {
    auto left = children[i];
    auto right = std::make_shared<BNode>(K, left->isLeaf);
    
    right->keys.assign(left->keys.begin() + K, left->keys.end());
    left->keys.erase(left->keys.begin() + K, left->keys.end());
    
    if (!left->isLeaf) {
        right->children.assign(left->children.begin() + K, left->children.end());
        left->children.erase(left->children.begin() + K, left->children.end());
    }
    
    // Поднять средний ключ в родителя
    keys.insert(keys.begin() + i, left->keys.back());
    left->keys.pop_back();
    
    // Вставить новый потомок
    children.insert(children.begin() + i + 1, right);
}

void BNode::print() const {
    for (int key : keys) {
        std::cout << key << " ";
    }
}

BTree::BTree(int k)
    : K(k), nodeCount(1) {
    root = std::make_shared<BNode>(K, true);
}

void BTree::insert(int key) {
    if (static_cast<int>(root->keys.size()) == 2 * K - 1) {
        auto newRoot = std::make_shared<BNode>(K, false);
        newRoot->children.push_back(root);
        newRoot->splitChild(0);
        root = newRoot;
        nodeCount++;
    }
    
    root->insertNonFull(key);
    nodeCount = 0;
    countNodes(root, nodeCount);
}

bool BTree::search(int key) const {
    auto current = root;
    
    while (current) {
        int i = 0;
        while (i < static_cast<int>(current->keys.size()) && key > current->keys[i]) {
            i++;
        }
        
        if (i < static_cast<int>(current->keys.size()) && key == current->keys[i]) {
            return true;
        }
        
        if (current->isLeaf) {
            return false;
        }
        
        current = current->children[i];
    }
    
    return false;
}

void BTree::inOrderTraversal(std::vector<int>& result, const std::shared_ptr<BNode>& node) const {
    const auto& current = (node == nullptr) ? root : node;
    
    if (!current) return;
    
    if (current->isLeaf) {
        for (int key : current->keys) {
            result.push_back(key);
        }
    } else {
        // Не лист, значит чередуем детей и значения
        for (int i = 0; i < static_cast<int>(current->keys.size()); i++) {
            inOrderTraversal(result, current->children[i]);
            result.push_back(current->keys[i]);
        }
        inOrderTraversal(result, current->children[current->children.size() - 1]);
    }
}

void BTree::printTree(const std::shared_ptr<BNode>& node, int indent) const {
    const auto& current = (node == nullptr) ? root : node;
    
    if (!current) return;
    
    for (int i = 0; i < indent; i++) std::cout << "  ";
    std::cout << "[ ";
    current->print();
    std::cout << "]" << (current->isLeaf ? " (LEAF)" : " (INTERNAL)") << "\n";
    
    if (!current->isLeaf) {
        for (const auto& child : current->children) {
            printTree(child, indent + 1);
        }
    }
}

void BTree::countNodes(const std::shared_ptr<BNode>& node, int& count) const {
    if (!node) return;
    count++;
    if (!node->isLeaf) {
        for (const auto& child : node->children) {
            countNodes(child, count);
        }
    }
}

std::shared_ptr<BTree> BTree::transform(int K2) const {
    std::vector<int> keys;
    inOrderTraversal(keys);
    
    auto newTree = std::make_shared<BTree>(K2);

    for (int key : keys) {
        newTree->insert(key);
    }
    
    return newTree;
}


/*
Трансформация T(n):

T(n) = inOrderTraversal(n)      + insertAll(n)
     = O(n)               + O(n)
     = O(n) ✓

Где:
  inOrderTraversal(n) = O(n) — каждый ключ копируется один раз
  insertAll(n)   = O(n) — амортизированно, благодаря отсортированности
*/

