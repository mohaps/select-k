/**
 *  
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Contributor(s): Saurav Mohapatra (mohaps@gmail.com) 
 * 
 * 
 * ----------------------------------------------------------------------------------------------------------------
 *  Select-K : a template class in C++20 to implement k-nearest / k-best type logic 
 * ----------------------------------------------------------------------------------------------------------------
 *  
 *  Github: https://github.com/mohaps/select-k
 *  
 * 
 * Sample Usage:
 *      k::Top selector(k, scoringFunction);
 *      
 *      for (auto c : candidates) {
 *          selector.offer(c);
 *      }
 *      
 *      // now selector has min(k, canddiates.size()) best candidates (top scores from scoring function)
 *  
 *  Doing the same with k::Bottom will get you the bottom K from the candidates
 * 
 *  The scoring function returns a ScoreType for a Candidate
 *  k::Top preserves the K top score candidates
 *  k::Bottom preserves the K lowest score candidates
 *      
 *  Runtime Complexity: O(N * Log(K))   - where N is the number of candidates and K is the best candidate count
 *  Space Complexity: O(K)
 * ---------------------------------------------------------------------------------------------------------------
 * 
 */
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