// vector.h - 动态数组实现 for VC1.52C
#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>
#include <malloc.h>
#include "stdbool.h"

class vector {
private:
    void __far *_data;
    size_t _size;
    size_t _capacity;
    size_t _element_size;
    
    bool _grow() {
		size_t new_capacity = _capacity * 2;
		void __far *new_data = new unsigned char[new_capacity * _element_size];
		if (!new_data) return false;
    
		// 复制现有数据
		size_t i;  // 将变量声明移到循环外
		for (i = 0; i < _size * _element_size; i++) {
			((unsigned char __far *)new_data)[i] = ((unsigned char __far *)_data)[i];
		}
    
		delete [] (unsigned char __far *)_data;
		_data = new_data;
		_capacity = new_capacity;
		return true;
	}
    
public:
    vector(size_t element_size) {
        _element_size = element_size;
        _capacity = 16;
        _data = new unsigned char[_capacity * _element_size];
        _size = 0;
    }
    
    ~vector() {
        delete [] (unsigned char __far *)_data;
    }
    
    // 元素访问
    bool at(size_t index, void *out_element) const {
        if (index >= _size || !out_element) return false;
        
        unsigned char __far *src = (unsigned char __far *)_data + index * _element_size;
        unsigned char *dest = (unsigned char *)out_element;
        for (size_t i = 0; i < _element_size; i++) {
            dest[i] = src[i];
        }
        return true;
    }
    
    bool front(void *out_element) const {
        return at(0, out_element);
    }
    
    bool back(void *out_element) const {
        return at(_size - 1, out_element);
    }
    
    void __far *data() const {
        return _data;
    }
    
    // 容量操作
    bool empty() const {
        return _size == 0;
    }
    
    size_t size() const {
        return _size;
    }
    
    size_t capacity() const {
        return _capacity;
    }
    
    bool reserve(size_t new_capacity) {
        if (new_capacity <= _capacity) return false;
        
        void __far *new_data = new unsigned char[new_capacity * _element_size];
        if (!new_data) return false;
        
        // 复制现有数据
        for (size_t i = 0; i < _size * _element_size; i++) {
            ((unsigned char __far *)new_data)[i] = ((unsigned char __far *)_data)[i];
        }
        
        delete [] (unsigned char __far *)_data;
        _data = new_data;
        _capacity = new_capacity;
        return true;
    }
    
    void shrink_to_fit() {
        if (_size == _capacity) return;
        
        void __far *new_data = new unsigned char[_size * _element_size];
        if (!new_data) return;
        
        for (size_t i = 0; i < _size * _element_size; i++) {
            ((unsigned char __far *)new_data)[i] = ((unsigned char __far *)_data)[i];
        }
        
        delete [] (unsigned char __far *)_data;
        _data = new_data;
        _capacity = _size;
    }
    
    // 修改器
    void clear() {
        _size = 0;
    }
    
    bool push_back(const void *element) {
		if (_size >= _capacity) {
			if (!_grow()) return false;
		}
    
		unsigned char __far *dest = (unsigned char __far *)_data + _size * _element_size;
		const unsigned char *src = (const unsigned char *)element;
		size_t i;  // 修复这里
		for (i = 0; i < _element_size; i++) {
			dest[i] = src[i];
		}
		_size++;
		return true;
	}
    
    bool pop_back() {
        if (empty()) return false;
        _size--;
        return true;
    }
    
    bool pop_back(void *out_element) {
        if (empty()) return false;
        
        if (out_element) {
            back(out_element);
        }
        _size--;
        return true;
    }
    
    bool insert(size_t index, const void *element) {
        if (index > _size) return false;
        
        if (_size >= _capacity) {
            if (!_grow()) return false;
        }
        
        // 移动元素
        for (size_t i = _size; i > index; i--) {
            unsigned char __far *dest = (unsigned char __far *)_data + i * _element_size;
            unsigned char __far *src = (unsigned char __far *)_data + (i - 1) * _element_size;
            for (size_t j = 0; j < _element_size; j++) {
                dest[j] = src[j];
            }
        }
        
        // 插入新元素
        unsigned char __far *dest = (unsigned char __far *)_data + index * _element_size;
        const unsigned char *src = (const unsigned char *)element;
        for (size_t i = 0; i < _element_size; i++) {
            dest[i] = src[i];
        }
        _size++;
        return true;
    }
    
    bool erase(size_t index) {
        if (index >= _size) return false;
        
        // 移动元素
        for (size_t i = index; i < _size - 1; i++) {
            unsigned char __far *dest = (unsigned char __far *)_data + i * _element_size;
            unsigned char __far *src = (unsigned char __far *)_data + (i + 1) * _element_size;
            for (size_t j = 0; j < _element_size; j++) {
                dest[j] = src[j];
            }
        }
        _size--;
        return true;
    }
    
    bool resize(size_t new_size, const void *value = 0) {
        if (new_size > _capacity) {
            if (!reserve(new_size)) return false;
        }
        
        if (new_size > _size && value) {
            // 用value填充新元素
            for (size_t i = _size; i < new_size; i++) {
                unsigned char __far *dest = (unsigned char __far *)_data + i * _element_size;
                const unsigned char *src = (const unsigned char *)value;
                for (size_t j = 0; j < _element_size; j++) {
                    dest[j] = src[j];
                }
            }
        }
        _size = new_size;
        return true;
    }
    
private:
    vector(const vector &);
    vector &operator=(const vector &);
};

#endif /* VECTOR_H */