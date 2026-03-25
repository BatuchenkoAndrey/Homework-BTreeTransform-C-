#include "btree.h"
#include <iostream>

int main() {
    std::vector<int> keys = {5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70};
    
    std::cout << "Keys: ";
    for (int k : keys) std::cout << k << " ";
    std::cout << "\n\n";
    

    int K1 = 3;
    std::cout << "K = " << K1 << "\n";
    auto tree1 = std::make_shared<BTree>(K1);
    for (int key : keys) {
        tree1->insert(key);
    }
    tree1->printTree();
    

    int K2 = 2;
    std::cout << "\nK = " << K2 << "\n";
    auto tree2 = tree1->transform(K2);
    tree2->printTree();
    

    int K3 = 5;
    std::cout << "\nK = " << K3 << "\n";
    auto tree3 = tree1->transform(K3);
    tree3->printTree();
    
    return 0;
}

