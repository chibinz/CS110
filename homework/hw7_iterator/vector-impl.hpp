#define INIT_CAPACITY 1000000

template <typename _Tp> class __detail::__iterator
{
  public:
    _Tp *p;

    // Constructor;
    __iterator()
    {
        p = nullptr;
    }
    // Constructor;
    __iterator(const __iterator &other)
    {
        p = other.p;
    }
    // Constructor;
    __iterator(_Tp *ptr)
    {
        p = ptr;
    }
    ~__iterator()
    {
    }

    __iterator(__detail::__const_iterator<_Tp> &other)
    {
        p = other.p;
    }
    // Assignment
    // Dereference
    // Dereference
    // Equal
    // Unequal
    // Pre-increment
    // Post-decrement
    // Post-increment
    // Post-decrement
    // Add int
    // Minus int
    // Add assign
    // Add assign
    __iterator &operator=(const __iterator &other)
    {
        p = other.p;
        return *this;
    }
    _Tp &operator*()
    {
        return *p;
    }
    _Tp *operator->()
    {
        return p;
    }
    bool operator==(const __iterator &other) const
    {
        return p == other.p;
    }
    bool operator!=(const __iterator &other) const
    {
        return p != other.p;
    }
    __iterator &operator++()
    {
        p += 1;
        return *this;
    }
    __iterator &operator--()
    {
        p -= 1;
        return *this;
    }
    __iterator operator++(int)
    {
        auto temp = __iterator(p);
        p += 1;
        return temp;
    }
    __iterator operator--(int)
    {
        auto temp = __iterator(p);
        p -= 1;
        return temp;
    }
    __iterator operator+(int other) const
    {
        return __iterator(p + other);
    }
    __iterator operator-(int other) const
    {
        return __iterator(p - other);
    }
    __iterator &operator+=(int other)
    {
        p += other;
        return *this;
    }
    __iterator &operator-=(int other)
    {
        p -= other;
        return *this;
    }
};

template <typename _Tp> class __detail::__const_iterator
{
  public:
    _Tp *p;

    // Constructor;
    __const_iterator()
    {
        p = nullptr;
    }
    // Constructor;
    __const_iterator(const __const_iterator &other)
    {
        p = other.p;
    }
    // Constructor;
    __const_iterator(_Tp *ptr)
    {
        p = ptr;
    }
    ~__const_iterator()
    {
    }

    // Constructor;
    __const_iterator(const __detail::__iterator<_Tp> &other)
    {
        p = other.p;
    }

    // Assignment
    __const_iterator &operator=(const __const_iterator &other)
    {
        p = other.p;
        return *this;
    }
    // Dereference
    const _Tp &operator*()
    {
        return *p;
    }
    // Dereference
    const _Tp *operator->()
    {
        return p;
    }
    // Equal
    bool operator==(const __const_iterator &other) const
    {
        return p == other.p;
    }
    // Unequal
    bool operator!=(const __const_iterator &other) const
    {
        return p != other.p;
    }
    // Pre-increment
    __const_iterator &operator++()
    {
        p += 1;
        return *this;
    }
    // Post-decrement
    __const_iterator &operator--()
    {
        p -= 1;
        return *this;
    }
    // Post-increment
    __const_iterator operator++(int)
    {
        auto temp = __const_iterator(p);
        p += 1;
        return temp;
    }
    // Post-decrement
    __const_iterator operator--(int)
    {
        auto temp = __const_iterator(p);
        p -= 1;
        return temp;
    }
    // Add int
    __const_iterator operator+(int other) const
    {
        return __const_iterator(p + other);
    }
    // Minus int
    __const_iterator operator-(int other) const
    {
        return __const_iterator(p - other);
    }
    // Add assign
    __const_iterator &operator+=(int other)
    {
        p += other;
        return *this;
    }
    // Add assign
    __const_iterator &operator-=(int other)
    {
        p -= other;
        return *this;
    }
};

// Default constructor
template <typename _Tp> vector<_Tp>::vector()
{
    _M_size = 0;
    // Initial capacity
    _M_capacity = INIT_CAPACITY;
    _M_data = new _Tp[_M_capacity];
}

// Constructor with initialzer value
template <typename _Tp> vector<_Tp>::vector(size_type size, const _Tp &value)
{
    // Initial capacity
    _M_size = size;
    _M_capacity = INIT_CAPACITY;

    _M_data = new _Tp[_M_capacity];
    for (int i = 0; i < size; i++)
        _M_data[i] = value;
}

// Constructor with intializer list
template <typename _Tp> vector<_Tp>::vector(std::initializer_list<_Tp> l)
{
    _M_size = l.size();
    _M_capacity = INIT_CAPACITY;
    _M_data = new _Tp[_M_capacity];

    int i = 0;
    for (auto it = l.begin(); it != l.end(); it++, i++)
        _M_data[i] = *it;
}

// Destructors
template <typename _Tp> vector<_Tp>::~vector()
{
    _M_size = 0;
    _M_capacity = 0;
    delete[] _M_data;
}

// Subscript access to the data contains in the vector
template <typename _Tp> typename vector<_Tp>::reference vector<_Tp>::operator[](size_type n)
{
    return _M_data[n];
}

// The size of the vector
template <typename _Tp> typename vector<_Tp>::size_type vector<_Tp>::size() const
{
    return _M_size;
}

// Iterators
template <typename _Tp> typename vector<_Tp>::iterator vector<_Tp>::begin()
{
    return iterator(_M_data);
}
template <typename _Tp> typename vector<_Tp>::iterator vector<_Tp>::end()
{
    return iterator(_M_data + _M_size);
}

// Const iterators
template <typename _Tp> typename vector<_Tp>::const_iterator vector<_Tp>::cbegin() const
{
    return const_iterator(_M_data);
}
template <typename _Tp> typename vector<_Tp>::const_iterator vector<_Tp>::cend() const
{
    return const_iterator(_M_data + _M_size);
}

// Auxiliary function for grow
template <typename _Tp> void vector<_Tp>::_grow()
{
    while (_M_size >= _M_capacity - 1)
    {
        _M_capacity *= 2;
        auto temp = new _Tp[_M_capacity];

        for (int i = 0; i < (int)_M_size; i++)
            temp[i] = _M_data[i];

        delete[] _M_data;

        _M_data = temp;
    }
}

// Insert given value into vector before specified iterator
// Return an iterator that points to the inserted data
template <typename _Tp> typename vector<_Tp>::iterator vector<_Tp>::insert(iterator pos, const _Tp &value)
{
    _grow();

    int index = pos.p - _M_data;

    for (int i = _M_size; i > index; i--)
        _M_data[i] = _M_data[i - 1];

    _M_data[index] = value;
    _M_size++;

    return pos;
}

template <typename _Tp> typename vector<_Tp>::iterator vector<_Tp>::insert(const_iterator pos, const _Tp &value)
{
    auto it = iterator(pos);
    return insert(it, value);
}

// Add data to the end of the vector
template <typename _Tp> void vector<_Tp>::push_back(const _Tp &value)
{
    _grow();

    _M_data[_M_size] = value;
    _M_size++;
}

// Remove element at given position
// Return an iterator to the next element (or end())
template <typename _Tp> typename vector<_Tp>::iterator vector<_Tp>::erase(iterator pos)
{
    int index = pos.p - _M_data;

    for (int i = index; i < (int)_M_size - 1; i++)
        _M_data[i] = _M_data[i + 1];

    _M_size--;

    return pos;
}

template <typename _Tp> typename vector<_Tp>::iterator vector<_Tp>::erase(const_iterator pos)
{
    auto it = iterator(pos);
    return erase(it);
}

// Remove last element
template <typename _Tp> void vector<_Tp>::pop_back()
{
    _M_size--;
}