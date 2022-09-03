#include "HashTable.hpp"
#include "Entry.hpp"
#include <iostream>
HashTable::HashList::HashList():head_(nullptr){}

HashTable::HashList::HashList(HashList & b){
    Entry* tmp = b.head_;
    while (tmp != nullptr){
        insert(tmp->key, tmp->value);
        tmp = tmp->next;
    }  
}

HashTable::HashList::~HashList(){
    freeList();
}

void HashTable::HashList::insert(const Key& k, const Value& v){
    if (head_ == nullptr) {
        head_ = new Entry(const_cast<Key&>(k),const_cast<Value&>(v));
        return;
    }
    head_ = new Entry(const_cast<Key&>(k),const_cast<Value&>(v),head_);
    return;
}


void HashTable::HashList::freeList(){
    while(head_ != nullptr){
        Entry* next = head_->next;
        delete head_;
        head_ = next;
    }
}

void HashTable::HashList::printList() const{
    Entry* tmp = head_;
    while (tmp != nullptr){
        std::cout << tmp->key << " " << tmp->value.age << " " << tmp->value.name <<std::endl;
        tmp = tmp->next;
    }
}


bool HashTable::HashList::search(const Key& k) const{
    Entry* tmp = head_;
    while (tmp != nullptr){
        if (tmp->key == k) return true;
        tmp = tmp->next;
    }
    return false;
}

bool HashTable::HashList::remove(const Key& k){
    if (head_->key == k){
        Entry* t = head_;
        head_ = head_->next;
        delete t;
        return true;
    }
    Entry* before_tmp = head_;
    Entry* tmp = head_;
    while (tmp->next != nullptr && tmp->key != k) {
        before_tmp = tmp;
        tmp = tmp->next;
    }
    if (tmp->key == k){
        before_tmp->next = tmp->next;
        delete tmp;
        return true;
    }
    return false;

}

HashTable::HashList& HashTable::HashList::operator=(const HashList& other){
    if (*this != other){
        freeList();
        Entry* tmp = other.head_;
        while (tmp != nullptr){
            insert(tmp->key, tmp->value);
            tmp = tmp->next;
        }
    }
    return *this;
}

bool HashTable::HashList::operator==(const HashList& b){
    if (this == &b) return true;
    Entry* tmp_a = head_;
    Entry* tmp_b = b.head_;
    int count1 = 0;
    int count2 = 0;

    while (tmp_a != nullptr || tmp_b != nullptr){
        if (tmp_a != nullptr){
            count1++;
            tmp_a = tmp_a->next;
        }

        if (tmp_b != nullptr){
            count2++;
            tmp_b = tmp_b->next;
        }
    }
    if (count1 != count2) return false;

    tmp_a = head_;
    tmp_b = b.head_; 
    bool flag = false;   
    while (tmp_a != nullptr){
        while (tmp_b != nullptr){
            if (*tmp_a == *tmp_b){
                flag = true;
                break;
            }
            tmp_b = tmp_b->next;
        }
        if (!flag) return false;
        flag = false;
        tmp_a = tmp_a->next;
        tmp_b = b.head_;    
    }
    return true;
}

bool HashTable::HashList::operator!=(const HashList& b){
    return (!operator==(b));
}

Entry* HashTable::HashList::pop(){
    if (head_ == nullptr) return nullptr;
    Entry* tmp = head_;
    head_ = head_->next;
    return tmp;
}

Value* HashTable::HashList::search(const Key& k){
    Entry* tmp = head_;
    while (tmp != nullptr){
        if (tmp->key == k) return &(tmp->value);
        tmp = tmp->next;
    }
    return nullptr;
}

void HashTable::HashList::reverse(){
    Entry* current = head_;
    Entry *prev = nullptr, *next;

    while (current != nullptr){
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    head_ = prev;
}

Entry* HashTable::HashList::get_head(){
    return head_;
}

size_t HashTable::HashList::get_size(){
    size_t size = 0;
    Entry* tmp = head_;
    while (tmp != nullptr){
        size++;
        tmp = tmp->next;
    }
    return size;
}