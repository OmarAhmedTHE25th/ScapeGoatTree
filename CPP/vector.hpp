#ifndef VECTOR_HPP
#define VECTOR_HPP

template <typename T>
class Vector {
public:
    unsigned int _size = 50;
    int nElements = 0;
    T* data = new T[_size]{};

    /**
     * Destroys the vector and releases the dynamically allocated memory.
     */
    ~Vector() { delete[] data; }

    /**
     * Default constructor for the Vector class.
     */
    Vector()=default;

    /**
     * Returns the number of elements currently stored in the vector.
     */
    [[nodiscard]] unsigned int size() const { return nElements; }

    /**
     * Appends a new element to the end of the vector, resizing if necessary.
     */
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

    /**
     * Provides access to the element at the specified index.
     */
    T& operator[](unsigned int index) { return data[index]; }

    /**
     * Provides read-only access to the element at the specified index.
     */
    const T& operator[](unsigned int index) const { return data[index]; }
    template <typename>
    friend class ScapeGoatTree;

    Vector(const Vector&)=delete;
    Vector& operator=(const Vector&) = delete;
    Vector(Vector&&) = delete;
    Vector& operator=(Vector&&) = delete;
};
#endif
