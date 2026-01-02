#ifndef VECTOR_HPP
#define VECTOR_HPP

template <typename T>
class Vector {
    unsigned int _size = 50;
    int nElements = 0;
    T* data = new T[_size]{};
    ~Vector() { delete[] data; }
    Vector()=default;
    [[nodiscard]] unsigned int size() const { return nElements; }
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
    template <typename>
    friend class ScapeGoatTree;
public:
    Vector(const Vector&)=delete;
    Vector& operator=(const Vector&) = delete;
    Vector(Vector&&) = delete;
    Vector& operator=(Vector&&) = delete;
};
#endif
