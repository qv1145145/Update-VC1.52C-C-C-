// deque.h - 双端队列实现 for VC1.52C
#ifndef DEQUE_H
#define DEQUE_H

#include <stddef.h>
#include <malloc.h>
#include "stdbool.h"

class deque {
private:
    void __far **_data;
    size_t _front;
    size_t _back;
    size_t _capacity;
    size_t _element_size;
    size_t _size;
    
    bool _grow() {
        size_t new_capacity = _capacity * 2;
        void __far **new_data = new void __far*[new_capacity];
        if (!new_data) return false;
        
        // 复制元素到新数组，重新排列为从0开始
        for (size_t i = 0; i < _size; i++) {
            size_t old_index = (_front + i) % _capacity;
            new_data[i] = _data[old_index];
        }
        
        delete [] _data;
        _data = new_data;
        _front = 0;
        _back = _size;
        _capacity = new_capacity;
        
        return true;
    }
    
    size_t _next_index(size_t index) const {
        return (index + 1) % _capacity;
    }
    
    size_t _prev_index(size_t index) const {
        return (index == 0) ? _capacity - 1 : index - 1;
    }
    
public:
    deque(size_t element_size) {
        _capacity = 16;
        _data = new void __far*[_capacity];
        _front = 0;
        _back = 0;
        _size = 0;
        _element_size = element_size;
    }
    
    ~deque() {
        clear();
        delete [] _data;
    }
    
    // 尾部操作
    bool push_back(const void *element) {
        if (_size >= _capacity) {
            if (!_grow()) return false;
        }
        
        void __far *new_element = new unsigned char[_element_size];
        if (!new_element) return false;
        
        const unsigned char *src = (const unsigned char *)element;
        unsigned char __far *dest = (unsigned char __far *)new_element;
        for (size_t i = 0; i < _element_size; i++) {
            dest[i] = src[i];
        }
        
        _data[_back] = new_element;
        _back = _next_index(_back);
        _size++;
        return true;
    }
    
    bool pop_back() {
        if (empty()) return false;
        
        _back = _prev_index(_back);
        delete [] (unsigned char __far *)(_data[_back]);
        _size--;
        return true;
    }
    
    bool pop_back(void *out_element) {
        if (empty()) return false;
        
        _back = _prev_index(_back);
        if (out_element) {
            unsigned char __far *src = (unsigned char __far *)(_data[_back]);
            unsigned char *dest = (unsigned char *)out_element;
            for (size_t i = 0; i < _element_size; i++) {
                dest[i] = src[i];
            }
        }
        delete [] (unsigned char __far *)(_data[_back]);
        _size--;
        return true;
    }
    
    bool back(void *out_element) const {
        if (empty() || !out_element) return false;
        
        size_t last_index = _prev_index(_back);
        unsigned char __far *src = (unsigned char __far *)(_data[last_index]);
        unsigned char *dest = (unsigned char *)out_element;
        for (size_t i = 0; i < _element_size; i++) {
            dest[i] = src[i];
        }
        return true;
    }
    
    // 头部操作
    bool push_front(const void *element) {
        if (_size >= _capacity) {
            if (!_grow()) return false;
        }
        
        void __far *new_element = new unsigned char[_element_size];
        if (!new_element) return false;
        
        const unsigned char *src = (const unsigned char *)element;
        unsigned char __far *dest = (unsigned char __far *)new_element;
        for (size_t i = 0; i < _element_size; i++) {
            dest[i] = src[i];
        }
        
        _front = _prev_index(_front);
        _data[_front] = new_element;
        _size++;
        return true;
    }
    
    bool pop_front() {
        if (empty()) return false;
        
        delete [] (unsigned char __far *)(_data[_front]);
        _front = _next_index(_front);
        _size--;
        return true;
    }
    
    bool pop_front(void *out_element) {
        if (empty()) return false;
        
        if (out_element) {
            unsigned char __far *src = (unsigned char __far *)(_data[_front]);
            unsigned char *dest = (unsigned char *)out_element;
            for (size_t i = 0; i < _element_size; i++) {
                dest[i] = src[i];
            }
        }
        delete [] (unsigned char __far *)(_data[_front]);
        _front = _next_index(_front);
        _size--;
        return true;
    }
    
    bool front(void *out_element) const {
        if (empty() || !out_element) return false;
        
        unsigned char __far *src = (unsigned char __far *)(_data[_front]);
        unsigned char *dest = (unsigned char *)out_element;
        for (size_t i = 0; i < _element_size; i++) {
            dest[i] = src[i];
        }
        return true;
    }
    
    // 随机访问
    bool at(size_t index, void *out_element) const {
        if (index >= _size || !out_element) return false;
        
        size_t actual_index = (_front + index) % _capacity;
        unsigned char __far *src = (unsigned char __far *)(_data[actual_index]);
        unsigned char *dest = (unsigned char *)out_element;
        for (size_t i = 0; i < _element_size; i++) {
            dest[i] = src[i];
        }
        return true;
    }
    
    // 容量查询
    bool empty() const { 
        return _size == 0; 
    }
    
    size_t size() const { 
        return _size; 
    }
    
    size_t capacity() const { 
        return _capacity; 
    }
    
    void clear() {
        while (!empty()) {
            pop_front();
        }
    }
    
    bool reserve(size_t new_capacity) {
        if (new_capacity <= _capacity) return false;
        
        void __far **new_data = new void __far*[new_capacity];
        if (!new_data) return false;
        
        // 复制元素到新数组
        for (size_t i = 0; i < _size; i++) {
            size_t old_index = (_front + i) % _capacity;
            new_data[i] = _data[old_index];
        }
        
        delete [] _data;
        _data = new_data;
        _front = 0;
        _back = _size;
        _capacity = new_capacity;
        
        return true;
    }
    
private:
    deque(const deque &);
    deque &operator=(const deque &);
};

#endif /* DEQUE_H */