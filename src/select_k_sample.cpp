#include "select_k/select_k.h"
#include <iostream>

void testInts() {
    std::vector<int> inputs {
        1, 4, 2, 30, 5, 6, 11, 10, 9, 100,
    };
    std::cout << "Inputs : [";
    bool first = true;
    for (auto v : inputs) {
        if (first) { first = false; }
        else { std::cout << ", "; }
        std::cout << v;
    }
    std::cout << "]" << std::endl;
    k::Top<int, int> selectTop(3, [](const int& v){ return v; });
    k::Bottom<int, int> selectBottom(3, [](const int& v){ return v; });
    for (auto i : inputs) {
        selectTop.offer(i);
        selectBottom.offer(i);
    }

    std::vector<int> topk, bottomk;

    selectTop.results(std::back_inserter(topk), true, false);
    std::cout << "Top =>"<<std::endl;
    for (auto v : topk) {
        std::cout << "  => " << v << std::endl;
    }
    selectBottom.results(std::back_inserter(bottomk), true, false);
    std::cout << "Bottom =>"<<std::endl;
    for (auto v : bottomk) {
        std::cout << "  => " << v << std::endl;
    }

}
void testPoints() {
    using Point = std::pair<int, int>;
    std::vector<Point> inputs {
        {3, 1},
        {3, 2},
        {3, 3},
        {1, 1},
        {1, 2},
        {1, 3},
        {2, 1},
        {2, 2},
        {2, 3},
    };
    auto sqEuclidian = [](const Point& p){
        // custom scoring function (square of euclidean distance from origin)
        return p.first * p.first + p.second * p.second;
    };
    std::cout << "finding k-nearest points to origin k=4 ..." << std::endl;
    // streaming method
    k::Bottom<Point, int> nearest(4, sqEuclidian);
    std::cout << "Trying iterative/streaming compute ..." << std::endl;
    for (auto p : inputs) {
        nearest.offer(p);
    }

    std::vector<Point> results;
    nearest.results(std::back_inserter(results), true, false);
    for (auto p : results) {
        std::cout << " => " << p.first <<","<<p.second << std::endl;
    }
    
    std::cout << "Trying one-shot compute ..." << std::endl;
    // one shot static method
    results.clear();
    k::Bottom<Point, int>::compute(std::back_inserter(results), 4, inputs.begin(), inputs.end(), sqEuclidian);
    for (auto p : results) {
        std::cout << " => " << p.first <<","<<p.second << std::endl;
    }
}
int main(int argc, char** argv) {
    std::cout << "**** TESTING INTS ... " << std::endl;
    testInts();

    std::cout << "**** TESTING POINTS ..." << std::endl;
    testPoints();
    
    return 0;
}