#ifndef INDEX_H
#define INDEX_H

#include <cstdint>
#include <iostream>
#include <type_traits>
template <typename T>
class IndexBase {
    // 只支持 unsigned int 类型
    static_assert(std::is_integral<T>::value && std::is_unsigned<T>::value);

public:
    enum NodeSatuc : T { NODE_USED = static_cast<T>(-1), NODE_TAIL = static_cast<T>(-2) };

    static constexpr T INVALID_ID = static_cast<T>(-1);

public:
    IndexBase() : len_ {0}, node_(nullptr), head_(0), tail_(0) {}

    // 用于初始化 id 范围
    uint32_t Init(size_t size, T* node)
    {
        if (size == 0 || size >= INVALID_ID) {
            return INVALID_ID;
        }
        node_ = node;
        len_ = size;
        T i = 0;
        for (; i < len_ - 1; ++i) {
            node_[i] = i + 1;  // 初始时，每个结点后一个结点即是下标+1
        }
        node_[i] = NODE_TAIL;  // 标识最后一个结点
        head_ = 0;
        tail_ = len_ - 1;
        return 0;
    }
    // 申请分配一个 id
    T AllocId()
    {
        if (head_ == NODE_TAIL) {
            return INVALID_ID;
        }
        T id = head_;
        head_ = node_[head_];
        node_[id] = NODE_USED;

        if (head_ == NODE_TAIL) {
            tail_ = NODE_TAIL;
        }
        return id;
    }
    // 释放 id
    uint32_t FreeId(T id)
    {
        if (id >= len_) {
            return INVALID_ID;
        }
        if (node_[id] != NODE_USED) {
            return INVALID_ID;
        }
        if (head_ == NODE_TAIL) {
            node_[id] = NODE_TAIL;
            head_ = id;
            tail_ = id;
        } else {
            node_[id] = NODE_TAIL;
            node_[tail_] = id;
            tail_ = id;
        }
        return 0;
    }
    void Print()
    {
        for (T i = 0; i < len_; ++i) {
            if (node_[i] == NODE_USED) std::cout << "_,";
            else if (node_[i] == NODE_TAIL) std::cout << "T,";
            else std::cout << (uint32_t)node_[i] << ",";
        }
        std::cout << std::endl;
    }

private:
    T len_;
    T* node_;
    T head_;
    T tail_;
};

template <typename T>
class Index : public IndexBase<T> {
    static constexpr T INVALID_ID = static_cast<T>(-1);

public:
    ~Index() { delete[] buff_; }
    uint32_t Init(T size)
    {
        if (size == 0 || size >= INVALID_ID) {
            return INVALID_ID;
        }
        buff_ = new T[size];
        return IndexBase<T>::Init(size, buff_);
    }

private:
    T* buff_;
};

template <typename T, T N>
class ArrIndex : public IndexBase<T> {
    static_assert(N > 0);
    static_assert(N < std::numeric_limits<T>::max() - 1);

public:
    ArrIndex() { IndexBase<T>::Init(N, &node_[0]); }

private:
    T node_[N];
};

#endif
