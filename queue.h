// queue.h - 队列实现 (基于deque)
#ifndef QUEUE_H
#define QUEUE_H

#include "deque.h"

class queue {
private:
    deque _deque;
    
public:
    queue(size_t element_size) : _deque(element_size) {}
    
    // 队列操作 - 只能使用队列相关功能
    bool push(const void *element) {
        return _deque.push_back(element);
    }
    
    bool pop() {
        return _deque.pop_front();
    }
    
    bool pop(void *out_element) {
        return _deque.pop_front(out_element);
    }
    
    bool front(void *out_element) const {
        return _deque.front(out_element);
    }
    
    bool back(void *out_element) const {
        return _deque.back(out_element);
    }
    
    // 容量查询
    bool empty() const { 
        return _deque.empty(); 
    }
    
    size_t size() const { 
        return _deque.size(); 
    }
    
    size_t capacity() const { 
        return _deque.capacity(); 
    }
    
    void clear() {
        _deque.clear();
    }
    
    bool reserve(size_t new_capacity) {
        return _deque.reserve(new_capacity);
    }
    
private:
    // 禁用deque特有的功能
    queue(const queue &);
    queue &operator=(const queue &);
};

#endif /* QUEUE_H */