#include <iostream>
#include "treeB.h"

void print_bool(bool cond) {
    if (cond) {
        std::cout << "True\n";
    } else {
        std::cout << "False\n";
    }
}

int main() {
    BPlusTree<int> bpt(4);

    bpt.insert(10);
    bpt.insert(27);
    bpt.insert(29);
    bpt.insert(17);
    bpt.insert(25);
    bpt.insert(21);
    bpt.insert(15);
    bpt.insert(31);
    bpt.insert(13);
    bpt.insert(51);
    bpt.insert(20);
    bpt.insert(24);
    bpt.insert(48);
    bpt.insert(19);
    bpt.insert(60);
    bpt.insert(35);
    bpt.insert(66);


    bpt.generateGraphvizDot("grafos.dot");

    //bpt.insert(44);

    /* for(int i=1; i<12; i++){
        bpt.insert(i*13+87);
        std::cout<<i*13+87<<' ';
        bpt.insert(i*99-28);
        std::cout<<i*99-28<<' ';

    } */

    std::cout<<std::endl;
    bpt.bpt_print();

    return 0;
}