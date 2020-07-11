#include "vector.hpp"
#include <iostream>

using namespace __detail;

// Remove last element
// template<typename _Tp> void vector<_Tp>::print()
// {
//     std::cout << _M_size << "!\n";
//     std::cout << _M_capacity << "!\n";
//     for (int i = 0; i < _M_size; i++)
//         std::cout << _M_data[i] << "\n";

//     std::cout << "\n";
// }

int main()
{
    int a[] = {1, 2, 3, 4, 5};

    auto it = __iterator<int>(a);
    ++it;
    std::cout << *it << "\n";
    --it;
    std::cout << *it << "\n";
    it += 2;
    std::cout << *it << "\n";
    it -= 1;
    std::cout << *it << "\n";

    auto nit = it + 3;
    std::cout << *it << "\n";

    *nit = 9;
    std::cout << *nit << "\n";

    auto cit = __const_iterator<int>(nit);

    auto iit = __iterator<int>(cit);

    std::cout << *iit << "arstarst\n";

    auto ve = new vector<int>(4, 0);
    auto &vec = *ve;

    // // vec.print();
    // for(int i = 0; i < 5; i++)
    //     vec.push_back(i);

    // // vec.print();
    // vec.pop_back();
    // // vec.print();

    // for(auto it = vec.begin(); it != vec.end(); it++)
    //     std::cout << *it << "\n";

    // nit = it++;
    // std::cout << nit.p - it.p << "-\n";

    // it = vec.end();
    // --it;

    // it = vec.erase(it);
    // std::cout << (it == vec.end()) << "*\n";

    // // vec.print();
    // vec.insert(it - 2, 16);
    // // vec.print();

    auto begin = vec.begin();
    auto end = vec.end();

    for (auto it = vec.begin(); it != vec.end(); it++)
        std::cout << *it << " a\n";

    vec.insert(end, 101);
    vec.insert(begin, 909);

    for (auto it = vec.begin(); it != vec.end(); it++)
        std::cout << *it << " a\n";

    delete ve;

    return 0;
}