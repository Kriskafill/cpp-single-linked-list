#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <string>
#include <utility>

template <typename Type>
class SingleLinkedList {

    struct Node {
        Node() = default;
        Node(const Type& val, Node* next)
            : value(val)
            , next_node(next) {
        }
        Type value;
        Node* next_node = nullptr;
    };

    template <typename ValueType>
    class BasicIterator {

        friend class SingleLinkedList;

        explicit BasicIterator(Node* node) {
            node_ = node;
        }

    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = Type;
        using difference_type = std::ptrdiff_t;
        using pointer = ValueType*;
        using reference = ValueType&;

        BasicIterator() = default;

        BasicIterator(const BasicIterator<Type>& other) noexcept {
            node_ = other.node_;
        }

        BasicIterator& operator=(const BasicIterator& rhs) = default;

        [[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept {
            return node_ == rhs.node_;
        }

        [[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept {
            return node_ != rhs.node_;
        }

        [[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept {
            return node_ == rhs.node_;
        }

        [[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept {
            return node_ != rhs.node_;
        }

        BasicIterator& operator++() noexcept {
            node_ = node_->next_node;
            return *this;
        }

        BasicIterator operator++(int) noexcept {
            auto old_value(*this);
            ++(*this);
            return old_value;
        }

        [[nodiscard]] reference operator*() const noexcept {
            return node_->value;
        }

        [[nodiscard]] pointer operator->() const noexcept {
            return &(node_->value);
        }

    private:
        Node* node_ = nullptr;
    };

public:
    using value_type = Type;
    using reference = value_type&;
    using const_reference = const value_type&;
    using Iterator = BasicIterator<Type>;
    using ConstIterator = BasicIterator<const Type>;

    explicit SingleLinkedList() = default;
    SingleLinkedList(std::initializer_list<Type> values) {
        Node* new_node;
        Node* last_node = &head_;

        for (const auto& value : values) {
            new_node = new Node(value, nullptr);
            last_node->next_node = new_node;
            last_node = new_node;
        }

        size_ = values.size();
    }

    SingleLinkedList(const SingleLinkedList& other) {
        Node* new_node;
        Node* last_node = &head_;
        Node* other_node = other.head_.next_node;

        while (other_node) {
            new_node = new Node(other_node->value, nullptr);
            last_node->next_node = new_node;
            last_node = new_node;
            other_node = other_node->next_node;
        }

        size_ = other.size_;
    }

    SingleLinkedList& operator=(const SingleLinkedList& rhs) {
        auto rhs_copy(rhs);
        swap(rhs_copy);
        return *this;
    }

    void swap(SingleLinkedList& other) noexcept {
        Node* other_node = other.head_.next_node;
        other.head_.next_node = head_.next_node;
        head_.next_node = other_node;

        size_t other_size = other.size_;
        other.size_ = size_;
        size_ = other_size;
    }

    void PushFront(const Type& value) {
        head_.next_node = new Node(value, head_.next_node);
        ++size_;
    }

    void PopFront() noexcept {
        Node* first_node = head_.next_node;
        head_.next_node = first_node->next_node;
        delete(first_node);
        --size_;
    }

    Iterator InsertAfter(ConstIterator pos, const Type& value) {
        Node* new_node = new Node(value, pos.node_->next_node);
        pos.node_->next_node = new_node;
        ++size_;
        return BasicIterator<Type>(new_node);
    }

    Iterator EraseAfter(ConstIterator pos) noexcept {
        Node* delete_node = pos.node_->next_node;
        pos.node_->next_node = delete_node->next_node;
        delete(delete_node);
        --size_;
        return BasicIterator<Type>(pos.node_->next_node);
    }

    void Clear() noexcept {
        while (head_.next_node) {
            Node* next = head_.next_node;
            head_.next_node = next->next_node;
            delete(next);
        }
        size_ = 0;
    }

    [[nodiscard]] size_t GetSize() const noexcept {
        return size_;
    }

    [[nodiscard]] bool IsEmpty() const noexcept {
        return !size_;
    }

    [[nodiscard]] Iterator begin() noexcept {
        return BasicIterator<Type>(head_.next_node);
    }

    [[nodiscard]] Iterator end() noexcept {
        return BasicIterator<Type>(nullptr);
    }

    [[nodiscard]] ConstIterator begin() const noexcept {
        return BasicIterator<Type>(head_.next_node);
    }

    [[nodiscard]] ConstIterator end() const noexcept {
        return BasicIterator<Type>(nullptr);
    }

    [[nodiscard]] ConstIterator cbegin() const noexcept {
        return BasicIterator<Type>(head_.next_node);
    }

    [[nodiscard]] ConstIterator cend() const noexcept {
        return BasicIterator<Type>(nullptr);
    }

    [[nodiscard]] Iterator before_begin() noexcept {
        return BasicIterator<Type>(&head_);
    }

    [[nodiscard]] ConstIterator cbefore_begin() const noexcept {
        return BasicIterator<Type>(&head_);
    }

    [[nodiscard]] ConstIterator before_begin() const noexcept {
        return BasicIterator<Type>(&head_);
    }

    ~SingleLinkedList() {
        Clear();
    }

private:
    mutable Node head_ = Node();
    size_t size_ = 0;
};

template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept {
    lhs.swap(rhs);
}

template <typename Type>
bool operator==(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    if (lhs.GetSize() != rhs.GetSize()) return false;
    auto lhs_it = lhs.begin();
    auto rhs_it = rhs.begin();

    while (lhs_it != lhs.end() && rhs_it != rhs.end()) {
        if (*lhs_it != *rhs_it) return false;
        ++lhs_it;
        ++rhs_it;
    }

    return true;
}

template <typename Type>
bool operator!=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs == rhs);
}

template <typename Type>
bool operator<(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::lexicographical_compare(
        lhs.begin(), lhs.end(),
        rhs.begin(), rhs.end()
    );
}

template <typename Type>
bool operator<=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return rhs == lhs || rhs < lhs;
}

template <typename Type>
bool operator>(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return rhs < lhs;
}

template <typename Type>
bool operator>=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return rhs <= lhs;
}