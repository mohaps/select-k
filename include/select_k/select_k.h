/**
 *  select-k
 *  a template class in C++20 to implement k-nearest / k-best type logic
 *  
 *  Sample Usage:
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
 * 
 */

#pragma once
#include <functional>
#include <cstdint>
#include <cstddef>
#include <vector>
#include <queue>
#include <stack>
namespace k {
template <
    typename T, 
    typename ScoreType,
    class CompareType
>
class Select {
public:
    using Candidate = T;
    using Score = ScoreType;
    using ScoredCandidate = std::pair<Candidate, Score>;
    using ScoringFunction = std::function<Score(const Candidate&)>;
    using Compare = CompareType;
    using Container = std::vector<ScoredCandidate>;
    
    struct ScoredCompare {
        Compare compare;
        bool operator()(const ScoredCandidate& c1, const ScoredCandidate& c2) const {
            return compare(c1.second, c2.second);
        }
    };

    using Heap = std::priority_queue<ScoredCandidate, Container, ScoredCompare>;
    
    Select(size_t k, ScoringFunction scorer) : k_(k), scorer_(scorer) {}
    
    ~Select() = default;

    bool offer(const Candidate& candidate) {
        if (k_ == 0) { return false; }
        ScoreType score = scorer_(candidate);
        ScoredCandidate scored { candidate, score};
        if (selected_.size() < k_) {
            selected_.push({candidate, score});
            return true;
        }
        const auto& worst = selected_.top();
        if (scoredCompare_(scored, worst)) {
            selected_.pop();
            selected_.push(scored);
            return true;
        }
        return false;
    }

    template <typename OutputIterator>
    size_t results(OutputIterator out, bool sorted, bool preserveSelection) {
        if (preserveSelection) {
            if (sorted) {
                return extractSorted(out, selected_);
            } else {
                return extract(out, selected_);
            }
        } else {
            Heap copied = selected_;
            if (sorted) {
                return extractSorted(out, copied);
            } else {
                return extract(out, copied);
            }
        }
    }

protected:
    
    template <typename OutputIterator>
    size_t extract(OutputIterator out, Heap& heap) {
        size_t count = 0;
        while (!heap.empty()) {
            *out++ = heap.top().first;
            heap.pop();
            ++count;
        }
        return count;
    }

    template <typename OutputIterator>
    size_t extractSorted(OutputIterator out, Heap& heap) {
        std::stack<Candidate> s;
        while (!heap.empty()) {
            s.push(heap.top().first);
            heap.pop();
        }
        size_t count = 0;
        while (!s.empty()) {
            *out++ = s.top();
            s.pop();
            ++count;
        }
        return count;
    }
private:
    size_t k_;
    ScoringFunction scorer_;
    ScoredCompare scoredCompare_;
    Heap selected_;
};


template <typename T, typename ScoreType>
class Top {
public:
    using ScoringFunction = std::function<ScoreType(const T&)>;
    using Selector = Select<T, ScoreType, std::greater<ScoreType>>;
    Top(size_t k, ScoringFunction scorer) : select_(k, scorer) {}
    ~Top() = default;
    bool offer(const T& t) {
        return select_.offer(t);
    }
    template <typename OutputIterator>
    size_t results(OutputIterator out, bool sorted, bool preserveSelection = false) {
        return select_.results(out, sorted, preserveSelection);
    }


    template <typename OutputIterator, typename InputIterator>
    inline static size_t compute(OutputIterator out, size_t k, InputIterator begin, InputIterator end, ScoringFunction scoringFunction) {
        Top<T, ScoreType> selector(k, scoringFunction);
        for (auto iter = begin; iter != end; iter++) {
            selector.offer(*iter);
        }
        return selector.results(out, true, false);
    }
private:
    Selector select_;

};


template <typename T, typename ScoreType>
class Bottom {
public:
    using ScoringFunction = std::function<ScoreType(const T&)>;
    using Selector = Select<T, ScoreType, std::less<ScoreType>>;
    Bottom(size_t k, ScoringFunction scorer) : select_(k, scorer) {}
    ~Bottom() = default;
    bool offer(const T& t) {
        return select_.offer(t);
    }
    template <typename OutputIterator>
    size_t results(OutputIterator out, bool sorted, bool preserveSelection = false) {
        return select_.results(out, sorted, preserveSelection);
    }


    template <typename OutputIterator, typename InputIterator>
    inline static size_t compute(OutputIterator out, size_t k, InputIterator begin, InputIterator end, ScoringFunction scoringFunction) {
        Bottom<T, ScoreType> selector(k, scoringFunction);
        for (auto iter = begin; iter != end; iter++) {
            selector.offer(*iter);
        }
        return selector.results(out, true, false);
    }
private:
    Selector select_;
};


}