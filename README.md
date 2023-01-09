# Select-K
Single file header-only C++20 library to implement k-nearest / k-best type logic

## Sample Usage:
Just include the 
```
    auto scoringFunction = [](const Candidate& c) { /* return an int score */ }
    k::Top<Candidate, int> selector(k, scoringFunction);
    
    for (auto c : candidates) {
        selector.offer(c);
    }
    
    // now selector has min(k, canddiates.size()) best candidates (top scores from scoring function)
```

Doing the same with ```k::Bottom``` will get you the bottom K from the candidates

### One Shot Helpers
Helper functions ```kTop::compute()``` and ```kBottom::compute()``` are provided for one shot calls where the entire list of candidates is available e.g. in a ```std::vector```

### Build and Run
Sample ```Makefile``` and code (```src/select_k_usage```) is included.

```
$ make run
rm -rf bin/select_k_sample
g++ -std=c++20 -Iinclude -O2 -o bin/select_k_sample src/select_k_sample.cpp
bin/select_k_sample
**** TESTING INTS ... 
Inputs : [1, 4, 2, 30, 5, 6, 11, 10, 9, 100]
Top =>
  => 100
  => 30
  => 11
Bottom =>
  => 1
  => 2
  => 4
**** TESTING POINTS ...
finding k-nearest points to origin k=4 ...
Trying iterative/streaming compute ...
 => 1,1
 => 2,1
 => 1,2
 => 2,2
Trying one-shot compute ...
 => 1,1
 => 2,1
 => 1,2
 => 2,2
```

### Scoring Function and ``ScoreType```

The scoring function returns a ScoreType for a Candidate
* ```k::Top``` preserves the K top score candidates
* ```k::Bottom``` preserves the K lowest score candidates

Custom scoring functions can be passed in the constructor
```
auto sqEuclidian = [](const Point& p){
    // custom scoring function (square of euclidean distance from origin)
    return p.first * p.first + p.second * p.second;
};
k::Bottom<Point, int> nearest(4, sqEuclidian);
````

See ```src/select_k_sample.cpp``` for usage details


## Complexity 

For N candidates and selection of K samples:
* Runtime Complexity: ```O(N * Log(K))```
* Space Complexity: ```O(K)```



## Credits
* Author: Saurav Mohapatra (mohaps AT gmail DOT com)
* License: APACHE 2.0 - http://www.apache.org/licenses/LICENSE-2.0
 