#ifndef STACK_HPP
#define STACK_HPP


#include <algorithm>
#include <cstdlib>
#include <new>
#include <utility>


template <typename T>
class Stack {
private:
    int _size = 0;
public:
    Stack() = default;

    Stack(const Stack& other) = default;

    Stack(Stack&& other) noexcept : _size(other._size) {
        other._size = 0;
    }

    Stack& operator=(const Stack& other) {
        if (this == &other) {
            return *this;
        }
        _size = other._size;
        return *this;
    }

    Stack& operator=(Stack&& other) noexcept {
        if (this == &other) {
            return *this;
        }
        _size = other._size;
        other._size = 0;
        return *this;
    }

    virtual ~Stack() = default;

    /**
     * Clone the stack
     * @return pointer to the cloned stack
     */
    [[nodiscard]] virtual Stack* clone() const = 0;

    /**
     * Add a new element to the top of the stack
     * @param value new element to add
     * @return true if the element is added successfully, false otherwise
     */
    virtual bool push(T value) = 0;

    /**
     * Remove the top element of the stack
     * @param value reference to store the top element
     * @return true if the stack is not empty, false otherwise
     */
    virtual bool pop(T& value) = 0;

    /**
     * Get the top element of the stack without removing it
     * @param value reference to store the top element
     * @return true if the stack is not empty, false otherwise
     */
    virtual bool peek(T& value) = 0;

    /**
     * Clear the stack
     */
    void clear() {
        T value;
        while (pop(value)) {}
    }

    /**
     * Get the size of the stack
     * @return size of the stack
     */
    [[nodiscard]] int size() const { return _size; }

    /**
     * Check if the stack is empty
     * @return true if the stack is empty, false otherwise
     */
    [[nodiscard]] bool isEmpty() const { return _size == 0; }
};

template <typename T>
class ListStack : public Stack<T> {
private:
    struct Node {
        T data;
        Node* next;
        Node() = default;
        Node(T value, Node* next) : data(std::move(value)), next(next) {}
    };
    Node* top = nullptr;
public:
    ListStack() = default;

    ListStack(const ListStack& other) : Stack<T>(other), top(other.top) {
        Node** current = &(top);
        while (*current != nullptr) {
            Node* new_node = new Node((*current)->data, (*current)->next);
            *current = new_node;
            current = &((*current)->next);
        }
    }

    ListStack(ListStack&& other) noexcept : top(other.top) {
        other.top = nullptr;
        this->_size = other._size;
        other._size = 0;
    }

    ListStack& operator=(const ListStack& other) {
        if (this == &other) {
            return *this;
        }
        Stack<T>::operator=(other);
        top = other.top;
        Node** current = &(top);
        while (*current != nullptr) {
            Node* new_node = new Node((*current)->data, (*current)->next);
            *current = new_node;
            current = &((*current)->next);
        }
        return *this;
    }

    ListStack& operator=(ListStack&& other) noexcept {
        if (this == &other) {
            return *this;
        }
        Stack<T>::operator=(std::move(other));
        top = other.top;
        other.top = nullptr;
        return *this;
    }

    ~ListStack() override { this->clear(); }

    [[nodiscard]] Stack<T>* clone() const override {
        return new ListStack(*this);
    }

    bool push(T value) override {
        Node* new_node = new (std::nothrow) Node(value, top);
        if (new_node == nullptr)
            return false;
        top = new_node;
        this->_size++;
        return true;
    }

    bool pop(T& value) override {
        Node* top_node = top;
        if (top_node == nullptr) {
            return false;
        }
        top = top_node->next;
        value = std::move(top_node->data);
        delete top_node;
        this->_size--;
        return true;
    }

    bool peek(T& value) override {
        if (top == nullptr) {
            return false;
        }
        value = top->data;
        return true;
    }
};

template <typename T>
class StaticStack : public Stack<T> {
private:
    int _capacity;
    T* data;
    int top = -1;
public:
    explicit StaticStack(const int capacity) : _capacity(capacity), data(new T[_capacity]) {}

    StaticStack(const StaticStack& other) : Stack<T>(other), _capacity(other._capacity), data(new T[_capacity]), top(other.top) {
        std::copy(other.data, other.data + _capacity, data);
    }

    StaticStack(StaticStack&& other) noexcept : Stack<T>(std::move(other)), _capacity(other._capacity), data(other.data), top(other.top) {
        other._capacity = 0;
        other.data = nullptr;
        other.top = -1;
    }

    StaticStack& operator=(const StaticStack& other) {
        if (this == &other) {
            return *this;
        }
        Stack<T>::operator =(other);
        _capacity = other._capacity;
        top = other.top;
        data = new T[_capacity];
        std::copy(other.data, other.data + _capacity, data);
        return *this;
    }

    StaticStack& operator=(StaticStack&& other) noexcept {
        if (this == &other) {
            return *this;
        }
        Stack<T>::operator =(std::move(other));
        _capacity = other._capacity;
        other._capacity = 0;
        top = other.top;
        other.top = -1;
        data = other.data;
        other.data = nullptr;
        return *this;
    }

    ~StaticStack() override { delete[] data; }

    [[nodiscard]] Stack<T>* clone() const override {
        return new StaticStack(*this);
    }

    /**
     * Get the capacity of the stack
     * @return capacity of the stack
     */
    [[nodiscard]] int capacity() const { return _capacity; }

    /**
     * Check if the stack is full
     * @return true if the stack is full, false otherwise
     */
    [[nodiscard]] bool isFull() const { return top + 1 >= _capacity; }

    bool push(T value) override {
        if (top + 1 < _capacity) {
            data[++top] = value;
            this->_size++;
            return true;
        }
        return false;
    }

    bool pop(T& value) override {
        if (top != -1) {
            value = data[top--];
            this->_size--;
            return true;
        }
        return false;
    }

    bool peek(T& value) override {
        if (top != -1)
            value = data[top];
        return top != -1;
    }
};

template <typename T>
class DynamicStack : public Stack<T> {
private:
    int _capacity;
    T* data;
    int top;
public:
    DynamicStack() : _capacity(4), data(new T[_capacity]), top(-1) {}

    explicit DynamicStack(const int capacity) : _capacity(capacity), top(-1) {
        if (_capacity < 4)
            this->_capacity = 4;
        this->data = new T[_capacity];
    }

    DynamicStack(const DynamicStack& other) : Stack<T>(other), _capacity(other._capacity), data(new T[_capacity]), top(other.top) {
        std::copy(other.data, other.data + _capacity, data);
    }

    DynamicStack(DynamicStack&& other) noexcept : Stack<T>(std::move(other)), _capacity(other._capacity), data(other.data), top(other.top) {
        other._capacity = 0;
        other.data = nullptr;
        other.top = -1;
    }

    DynamicStack& operator=(const DynamicStack& other) {
        if (this == &other)
            return *this;
        Stack<T>::operator =(other);
        _capacity = other._capacity;
        data = new T[_capacity];
        std::copy(other.data, other.data + _capacity, data);
        top = other.top;
        return *this;
    }

    DynamicStack& operator=(DynamicStack&& other) noexcept {
        if (this == &other)
            return *this;
        Stack<T>::operator =(std::move(other));
        _capacity = other._capacity;
        other._capacity = 0;
        data = other.data;
        other.data = nullptr;
        top = other.top;
        other.top = -1;
        return *this;
    }

    ~DynamicStack() override { delete[] data; }

    [[nodiscard]] Stack<T>* clone() const override {
        return new DynamicStack(*this);
    }

    [[nodiscard]] int capacity() const { return _capacity; }

    bool push(T value) override {
        if (top + 1 >= _capacity) {
            _capacity *= 2;
            _capacity = std::max(_capacity, 4);
            T* new_data = new (std::nothrow) T[_capacity];
            if (new_data == nullptr) {
                return false;
            }
            std::copy(data, data + this->size(), new_data);
            free(data);
            data = new_data;
        }

        data[++top] = value;
        this->_size++;
        return true;
    }

    bool pop(T& value) override {
        if (top != -1) {
            value = data[top--];
            this->_size--;
            return true;
        }
        return false;
    }

    bool peek(T& value) override {
        if (top != -1) {
            value = data[top];
        }
        return top != -1;
    }
};


#endif // STACK_HPP
