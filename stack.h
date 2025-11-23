// stack.h - 完整内联实现
#ifndef STACK_H
#define STACK_H

#include <stddef.h>
#include <malloc.h>

class stack {
private:
    void __far **_data;
    void __far **_top;
    void __far **_end;
    size_t _element_size;
    
    bool _grow() {
        size_t current_size = size();
        size_t new_capacity = capacity() * 2;
        
        void __far **new_data = new void __far*[new_capacity];
        if (!new_data) return false;
        
        for (size_t i = 0; i < current_size; i++) {
            new_data[i] = _data[i];
        }
        
        delete [] _data;
        _data = new_data;
        _top = _data + current_size;
        _end = _data + new_capacity;
        
        return true;
    }
    
public:
    stack(size_t element_size) {
        _data = new void __far*[16];
        if (_data) {
            _top = _data;
            _end = _data + 16;
            _element_size = element_size;
        } else {
            _top = _end = 0;
            _element_size = 0;
        }
    }
    
    ~stack() {
        clear();
        delete [] _data;
    }
    
    bool push(const void *element) {
        if (!_data) return false;
        
        if (_top >= _end) {
            if (!_grow()) return false;
        }
        
        void __far *new_element = new unsigned char[_element_size];
        if (!new_element) return false;
        
        const unsigned char *src = (const unsigned char *)element;
        unsigned char __far *dest = (unsigned char __far *)new_element;
        for (size_t i = 0; i < _element_size; i++) {
            dest[i] = src[i];
        }
        
        *_top = new_element;
        _top++;
        return true;
    }
    
    bool pop() {
        if (empty()) return false;
        
        _top--;
        delete [] (unsigned char __far *)(*_top);
        return true;
    }
    
    bool pop(void *out_element) {
        if (empty()) return false;
        
        _top--;
        if (out_element) {
            unsigned char __far *src = (unsigned char __far *)(*_top);
            unsigned char *dest = (unsigned char *)out_element;
            for (size_t i = 0; i < _element_size; i++) {
                dest[i] = src[i];
            }
        }
        delete [] (unsigned char __far *)(*_top);
        return true;
    }
    
    bool top(void *out_element) const {
        if (empty() || !out_element) return false;
        
        unsigned char __far *src = (unsigned char __far *)(*(_top - 1));
        unsigned char *dest = (unsigned char *)out_element;
        for (size_t i = 0; i < _element_size; i++) {
            dest[i] = src[i];
        }
        return true;
    }
    
    bool empty() const { 
        return !_data || (_top == _data); 
    }
    
    size_t size() const { 
        return _data ? (_top - _data) : 0; 
    }
    
    size_t capacity() const { 
        return _data ? (_end - _data) : 0; 
    }
    
    void clear() {
        if (!_data) return;
        
        while (_top > _data) {
            _top--;
            delete [] (unsigned char __far *)(*_top);
        }
    }
    
    bool reserve(size_t new_capacity) {
        if (!_data || new_capacity <= capacity()) return false;
        
        size_t current_size = size();
        void __far **new_data = new void __far*[new_capacity];
        if (!new_data) return false;
        
        for (size_t i = 0; i < current_size; i++) {
            new_data[i] = _data[i];
        }
        
        delete [] _data;
        _data = new_data;
        _top = _data + current_size;
        _end = _data + new_capacity;
        
        return true;
    }
    
private:
    stack(const stack &);
    stack &operator=(const stack &);
};

#endif /* STACK_H */
