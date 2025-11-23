// list.h - 双向链表实现 for VC1.52C
#ifndef LIST_H
#define LIST_H

#include <stddef.h>
#include <malloc.h>
#include "stdbool.h"

struct ListNode {
    void __far *data;
    ListNode __far *prev;
    ListNode __far *next;
};

class list {
private:
    ListNode __far *_head;
    ListNode __far *_tail;
    size_t _size;
    size_t _element_size;
    
public:
    list(size_t element_size) {
        _element_size = element_size;
        _size = 0;
        _head = 0;
        _tail = 0;
    }
    
    ~list() {
        clear();
    }
    
    // 元素访问
    bool front(void *out_element) const {
        if (!_head || !out_element) return false;
        
        unsigned char __far *src = (unsigned char __far *)_head->data;
        unsigned char *dest = (unsigned char *)out_element;
        for (size_t i = 0; i < _element_size; i++) {
            dest[i] = src[i];
        }
        return true;
    }
    
    bool back(void *out_element) const {
        if (!_tail || !out_element) return false;
        
        unsigned char __far *src = (unsigned char __far *)_tail->data;
        unsigned char *dest = (unsigned char *)out_element;
        for (size_t i = 0; i < _element_size; i++) {
            dest[i] = src[i];
        }
        return true;
    }
    
    // 容量操作
    bool empty() const {
        return _size == 0;
    }
    
    size_t size() const {
        return _size;
    }
    
    void clear() {
        while (!empty()) {
            pop_front();
        }
    }
    
    // 修改器
    bool push_front(const void *element) {
        ListNode __far *new_node = new ListNode;
        if (!new_node) return false;
        
        new_node->data = new unsigned char[_element_size];
        if (!new_node->data) {
            delete new_node;
            return false;
        }
        
        // 复制数据
        const unsigned char *src = (const unsigned char *)element;
        unsigned char __far *dest = (unsigned char __far *)new_node->data;
        for (size_t i = 0; i < _element_size; i++) {
            dest[i] = src[i];
        }
        
        new_node->prev = 0;
        new_node->next = _head;
        
        if (_head) {
            _head->prev = new_node;
        } else {
            _tail = new_node;
        }
        _head = new_node;
        _size++;
        return true;
    }
    
    bool push_back(const void *element) {
        ListNode __far *new_node = new ListNode;
        if (!new_node) return false;
        
        new_node->data = new unsigned char[_element_size];
        if (!new_node->data) {
            delete new_node;
            return false;
        }
        
        // 复制数据
        const unsigned char *src = (const unsigned char *)element;
        unsigned char __far *dest = (unsigned char __far *)new_node->data;
        for (size_t i = 0; i < _element_size; i++) {
            dest[i] = src[i];
        }
        
        new_node->prev = _tail;
        new_node->next = 0;
        
        if (_tail) {
            _tail->next = new_node;
        } else {
            _head = new_node;
        }
        _tail = new_node;
        _size++;
        return true;
    }
    
    bool pop_front() {
        if (!_head) return false;
        
        ListNode __far *old_head = _head;
        _head = _head->next;
        if (_head) {
            _head->prev = 0;
        } else {
            _tail = 0;
        }
        
        delete [] (unsigned char __far *)old_head->data;
        delete old_head;
        _size--;
        return true;
    }
    
    bool pop_front(void *out_element) {
        if (!_head) return false;
        
        if (out_element) {
            front(out_element);
        }
        return pop_front();
    }
    
    bool pop_back() {
        if (!_tail) return false;
        
        ListNode __far *old_tail = _tail;
        _tail = _tail->prev;
        if (_tail) {
            _tail->next = 0;
        } else {
            _head = 0;
        }
        
        delete [] (unsigned char __far *)old_tail->data;
        delete old_tail;
        _size--;
        return true;
    }
    
    bool pop_back(void *out_element) {
        if (!_tail) return false;
        
        if (out_element) {
            back(out_element);
        }
        return pop_back();
    }
    
    // 实用工具函数
    bool insert_after(ListNode __far *node, const void *element) {
        if (!node) return push_back(element);
        
        ListNode __far *new_node = new ListNode;
        if (!new_node) return false;
        
        new_node->data = new unsigned char[_element_size];
        if (!new_node->data) {
            delete new_node;
            return false;
        }
        
        // 复制数据
        const unsigned char *src = (const unsigned char *)element;
        unsigned char __far *dest = (unsigned char __far *)new_node->data;
        for (size_t i = 0; i < _element_size; i++) {
            dest[i] = src[i];
        }
        
        new_node->prev = node;
        new_node->next = node->next;
        
        if (node->next) {
            node->next->prev = new_node;
        } else {
            _tail = new_node;
        }
        node->next = new_node;
        _size++;
        return true;
    }
    
    bool remove(ListNode __far *node) {
        if (!node) return false;
        
        if (node->prev) {
            node->prev->next = node->next;
        } else {
            _head = node->next;
        }
        
        if (node->next) {
            node->next->prev = node->prev;
        } else {
            _tail = node->prev;
        }
        
        delete [] (unsigned char __far *)node->data;
        delete node;
        _size--;
        return true;
    }
    
    ListNode __far *begin() const {
        return _head;
    }
    
    ListNode __far *end() const {
        return 0;
    }
    
    ListNode __far *rbegin() const {
        return _tail;
    }
    
    ListNode __far *rend() const {
        return 0;
    }
    
private:
    list(const list &);
    list &operator=(const list &);
};

#endif /* LIST_H */