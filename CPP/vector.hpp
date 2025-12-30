#ifndef VECTOR_HPP
#define VECTOR_HPP

template <typename T>
class Vector {
    T* data;
    size_t _size;
    size_t _capacity;

    void reserve(const size_t new_cap) {
        _capacity = (_capacity == 0) ? 1 : _capacity * 2;
        T* newData = new T[_capacity];
        for (int i = 0; i < _size; ++i) newData[i] = data[i];
        delete[] data;
        data = newData;
    }

public:
    Vector() : data(nullptr), _size(0), _capacity(0) {}
    ~Vector() { delete[] data; }
    void push_back(const T& value) {
        if (_size == _capacity) {
            reserve(_capacity == 0 ? 1 : _capacity * 2);
        }
        data[_size++] = value;
    }
    // Disable copying to prevent crashes
    Vector(const Vector&)=delete;
    [[nodiscard]] size_t size() const { return _size; }

    T& operator[](size_t index) { return data[index]; }
    const T& operator[](size_t index) const { return data[index]; }
    T operator[](int i) const { return data[i]; }
};
#endif