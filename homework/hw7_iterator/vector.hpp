//  ShanghaiTech University
//
//  Computer Architecture I
//
//  HW 7 C++ Vector

#ifndef HW7_VECTOR_HPP
#define HW7_VECTOR_HPP

#include <cstddef>          // size_t ptrdiff_t
#include <cstring>          // memory
#include <initializer_list> // initializer_list

namespace __detail
{
// Iterator of vector (implement this)
template <typename _Tp> class __iterator;

// Const iterator of vector (implement this)
template <typename _Tp> class __const_iterator;
} // namespace __detail

template <typename _Tp> class vector
{
  public:
    // Type renaming
    typedef size_t size_type;
    typedef _Tp value_type;
    typedef _Tp *pointer;
    typedef _Tp &reference;
    typedef const _Tp &const_reference;
    typedef ptrdiff_t difference_type;
    typedef typename __detail::__iterator<_Tp> iterator;
    typedef typename __detail::__const_iterator<_Tp> const_iterator;

    // Default constructor
    vector();

    // Copy and remove constructor are deleted
    vector(const vector &other) = delete;
    vector(vector &&other) = delete;

    // Create a vector with $size copies of $value
    vector(size_type size, const _Tp &value);
    // Create a vector consists of the elements in the initialize_list
    vector(std::initializer_list<_Tp> l);

    // Destructor
    ~vector();

    // Copy and move assignment operator are deleted
    vector &operator=(const vector &other) = delete;
    vector &operator=(vector &&other) = delete;

    // Subscript access to the data contains in the vector
    reference operator[](size_type n);

    // The size of the vector
    size_type size() const;

    // Iterators
    iterator begin();
    iterator end();

    // Const iterators
    const_iterator cbegin() const;
    const_iterator cend() const;

    // Insert given value into vector before specified iterator
    // Return an iterator that points to the inserted data
    iterator insert(iterator pos, const _Tp &value);
    iterator insert(const_iterator pos, const _Tp &value);

    // Add data to the end of the vector
    void push_back(const _Tp &value);

    // Remove element at given position
    // Return an iterator to the next element (or end())
    iterator erase(iterator pos);
    iterator erase(const_iterator pos);

    // Remove last element
    void pop_back();

  private:
    size_type _M_size;
    size_type _M_capacity;
    pointer _M_data;

    // grow vector to size of capacity
    inline void _grow();
};

#include "vector-impl.hpp"

#endif