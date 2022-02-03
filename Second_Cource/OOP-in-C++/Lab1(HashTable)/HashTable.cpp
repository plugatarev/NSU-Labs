#include "HashTable.hpp"
#include <cmath>
#include <memory>
#include <limits>
#include<cassert>
typedef std::string Key;

HashTable::HashTable(size_t capacity):list_(new HashList*[capacity]()),capacity_(capacity), size_(0){}

HashTable::~HashTable(){
    clear();
    delete[] list_;
}
HashTable::HashTable(const HashTable& b):list_(new HashList*[b.capacity_]()){
    capacity_ = b.capacity_;
    size_ = b.size_;
    for (size_t i = 0; i < capacity_; i++){
        if (b.list_[i] != nullptr) {
            list_[i] = new HashList(*b.list_[i]);
        }
    }
}

size_t HashTable::size() const{
    return size_;
}

size_t HashTable::capacity() const{
    return capacity_;
}

bool HashTable::is_not_equal_table(const HashTable& b) const{
    size_t processed_elements = 0;
    for (size_t i = 0; i < capacity_ && processed_elements < size_; i++){
        if (list_[i] != nullptr){
            Entry* t = list_[i]->get_head();
            processed_elements+=list_[i]->get_size();
            while (t != nullptr){
                if (b.contains(t->key) && !(b.get_value_by_key(t->key) == t->value)) return true;
                t = t->next;
            }
        }
    }
    return false;
}

bool operator!=(const HashTable& a, const HashTable& b){
    if (&a == &b) return false;
    if (a.size_ != b.size_) return true;
    return a.is_not_equal_table(b);
}

bool operator==(const HashTable& a, const HashTable& b){
    return (!(a != b));
}

HashTable& HashTable::operator=(const HashTable& b){
    if (b != *this){
        clear();
        delete[] list_;
        capacity_ = b.capacity_;
        size_ = b.size_;
        list_ = new HashList*[capacity_]();
        for (size_t i = 0; i < capacity_; i++){
            if (b.list_[i] != nullptr) {
                list_[i] = new HashList();
                *list_[i] = *b.list_[i];
            }
        }
    }
    return *this;
}

bool HashTable::insert(const Key& k, const Value& v){
    if (size() > size_t(RESIZE_ON * capacity_)) resize();
    size_t hash = hashF(k);
    if (list_[hash] == nullptr) list_[hash] = new HashList();
    Value* val = list_[hash]->search(k);
    if (val != nullptr){
        val->age = v.age;
        val->name = v.name;
        return false;
    }
    size_++;
    list_[hash]->insert(const_cast<Key&>(k),const_cast<Value&>(v));
    return true;
}

bool HashTable::erase(const Key& k){
    if (size() == 0) return false;
    size_t hash = hashF(k);
    if (list_[hash] == nullptr) return false;
    if (list_[hash]->remove(k)){
        if (list_[hash]->get_head() == nullptr) {
            delete list_[hash];
            list_[hash] = nullptr;
        }
        size_--;
        return true;
    }
    return false;
}


bool HashTable::empty() const{
    return size() == 0;
}

size_t HashTable::hashF(const Key& k) const{
    size_t hash = 0;
	for (size_t i = 0; i < k.length(); i++) {
		hash += (k[i] % 5) * pow(3,i);
	}
	return hash % capacity_;
}

bool HashTable::resize(){
    if ( !(capacity_ * 2 < std::numeric_limits<uint64_t>::max() && capacity_ * 2 > capacity_)) throw std::runtime_error("capacity is more than UINT_MAX");
    size_t c = capacity();
    capacity_ = capacity_ * 2;

    HashList** tmp = new HashList*[capacity_]();
    for (size_t i = 0; i < c; i++){
        if (list_[i] != nullptr){
            Entry* l = (list_[i]->pop());
            while (l != nullptr){
                size_t hash = hashF(l->key);
                if (tmp[hash] == nullptr) tmp[hash] = new HashList();
                tmp[hash]->insert(l->key,l->value);
                delete l;
                l = list_[i]->pop();
            }
            delete list_[i];
        }
    }
    delete[] list_;
    list_ = tmp;
    return true;
}

void HashTable::clear(){
    for (size_t i = 0; i < capacity_ && size_ > 0; i++){
        if (list_[i] != nullptr){
            size_t size_list = list_[i]->get_size();
            delete list_[i];
            size_-=size_list;
        } 
    }
    assert(size_ == 0);
}

Value& HashTable::operator[](const Key& k){
    size_t hash = hashF(k);
    if (list_[hash] == nullptr) list_[hash] = new HashList();
    Value* tmp = list_[hash]->search(k);
    if (tmp == nullptr){
        size_++;
        static Value val = Value();
        list_[hash]->insert(k, val);
        return val;
    }

    return *tmp;
}

Value& HashTable::get_value_by_key(const Key& k) const{
    if (size() == 0) throw std::out_of_range("no such element exists");
    size_t hash = hashF(k);
    //exception if no such element exists
    if (list_[hash] == nullptr) throw std::out_of_range("no such element exists");
    Value * value = list_[hash]->search(k);
    if (value == nullptr) throw std::out_of_range("no such element exists");
    return *value;
}

Value& HashTable::at(const Key& k){
    return get_value_by_key(k);
}

const Value& HashTable::at(const Key& k) const{
    return (const_cast<Value&>(get_value_by_key(k)));
}

bool HashTable::contains(const Key& k) const{
    if (size() == 0) return false;
    size_t hash = hashF(k);
    if (list_[hash] == nullptr) return false;
    return list_[hash]->search(k);
}

void HashTable::swap(HashTable& b){
    std::swap(list_, b.list_);
    std::swap(capacity_, b.capacity_);
    std::swap(size_, b.size_);
}

void HashTable::operator<<(const HashTable& a) const{
      for (size_t i = 0; i < capacity_; i++){
            if (list_[i] != nullptr) list_[i]->printList();
        }
}