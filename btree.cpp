#include "btree.h"

// YZEL ============================================================================

BNode::BNode(int k, bool leaf)
    : K(k), isLeaf(leaf) {}

int BNode::findKey(int k) const {
    int idx = 0;
    while (idx < static_cast<int>(keys.size()) && keys[idx] < k) {
        idx++;
    }
    return idx;
}

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
    } // Если не лист, копируем детей
    
    // Поднять средний ключ в родителя
    keys.insert(keys.begin() + i, left->keys.back());
    left->keys.pop_back();
    
    // Вставить новый потомок
    children.insert(children.begin() + i + 1, right);
}

void BNode::insertLeaf(int key) {
    keys.push_back(key);
    std::sort(keys.begin(), keys.end());
}

void BNode::print() const {
    for (int key : keys) {
        std::cout << key << " ";
    }
}

// DEREVO =======================================================

BTree::BTree(int k)
    : K(k), nodeCount(1) {
    root = std::make_shared<BNode>(K, true);
}

void BTree::insert(int key) {
    // Если корень полон, разделить его
    if (static_cast<int>(root->keys.size()) == 2 * K - 1) {
        auto newRoot = std::make_shared<BNode>(K, false);
        newRoot->children.push_back(root);
        newRoot->splitChild(0);
        root = newRoot;
        nodeCount++;
    }
    
    root->insertNonFull(key);
    
    nodeCount = 0;
    countNodes(root, nodeCount); // обновить количество узлов
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
        // Для внутреннего узла чередуем детей и ключи
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



// TRANSFORMER (STATIC) ======================================================

std::shared_ptr<BTree> BTreeTransformer::transform(const std::shared_ptr<BTree>& tree, int K2) {
    // Собрать все ключи в отсортированный массив - O(m)
    std::vector<int> keys;
    collectKeys(tree->root, keys);
    
    // Построить новое дерево из массива - O(m)
    auto newTree = std::make_shared<BTree>(K2);

    for (int key : keys) {
        newTree->insert(key);
    }
    
    return newTree;
}

void BTreeTransformer::collectKeys(const std::shared_ptr<BNode>& node, std::vector<int>& keys) {
    if (!node) return;
    
    if (node->isLeaf) {
        for (int key : node->keys) {
            keys.push_back(key);
        }
    } else {
        int i = 0;
        for (; i < static_cast<int>(node->keys.size()); i++) {
            collectKeys(node->children[i], keys);
            keys.push_back(node->keys[i]);
        }
        collectKeys(node->children[i], keys);
    }
}
