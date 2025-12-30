#ifndef VECTOR_HPP
#define VECTOR_HPP

template <typename T>
class Vector {
    unsigned int _size = 50;
    int nElements = 0;
    T* data = new T[_size]{};

public:
    ~Vector() { delete[] data; }

    // Disable copying to prevent crashes
    Vector(const Vector&)=delete;
    [[nodiscard]] unsigned int size() const { return _size; }
    void push_back(const T& value) {
       if (nElements >= _size) {
           _size *= 2;
           T* newData = new T[_size]{};
           for (unsigned int i = 0; i < nElements; ++i)
               newData[i] = data[i];
           delete[] data;
           data = newData;
       }
       data[nElements++] = value;
    }
    T& operator[](unsigned int index) { return data[index]; }
    const T& operator[](unsigned int index) const { return data[index]; }
    T operator[](int i) const { return data[i]; }
};
#endif