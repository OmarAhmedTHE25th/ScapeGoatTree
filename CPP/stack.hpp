//
// Created by DELL on 03/01/2026.
//

#ifndef SCAPEGOATPROJECT_STACK_HPP
#define SCAPEGOATPROJECT_STACK_HPP
#include <stdexcept>
#include "vector.hpp"

template<typename T>
class Stack {
    Vector<T> data;
public:
    /**
     * Pushes a new element onto the stack.
     */
    void push(const T& value) {
        data.push_back(value);
    }

    /**
     * Removes and returns the top element from the stack.
     * Throws std::out_of_range if the stack is empty.
     */
    T pop() {
        if (data.size() == 0) {
            throw std::out_of_range("pop on empty Stack");
        }
        return data.pop_back();
    }
    T top() {
        return data.data[0];
    }

    /**
     * Returns the number of elements currently in the stack.
     */
    [[nodiscard]] unsigned int size() const {
        return data.size();
    }

    /**
     * Checks if the stack is empty.
     */
    [[nodiscard]] bool isEmpty() const {
        return data.size() == 0;
    }
};


#endif //SCAPEGOATPROJECT_STACK_HPP