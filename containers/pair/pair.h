#ifndef PAIR_H
#define PAIR_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <locale>
#include <string>
#include <vector>

// This structure will "overload" the input separator charactor for any input data or buffer
struct reader: std::ctype<char>
{
    reader(): ctype<char>(get_table()) //Constructor calls get_table which is the translation table
    {}
    static ctype_base::mask const* get_table()
    {
        static std::vector<ctype_base::mask> rc(table_size, ctype_base::mask());
        rc['\a'] = ctype_base::space; // Escape sequence '\a' is the input separator
        rc['\n'] = ctype_base::space; // Escape sequence '\n' is still kept as a separator
        return &rc[0];
    }
};

// Templated pair structure
template<typename K, typename V>
struct myPair
{
    K key;
    V value;

    myPair(K k = K(), V v = V());
    myPair(const myPair& p);
    myPair& operator =(myPair<K, V> p);
    ~myPair();

    // Comparison
    template<typename KK, typename VV>
    friend bool operator <(const myPair<KK, VV>& left, const myPair<KK, VV>& right);

    template<typename KK, typename VV>
    friend bool operator >(const myPair<KK, VV>& left, const myPair<KK, VV>& right);

    template<typename KK, typename VV>
    friend bool operator <(const myPair<KK, VV>& left, const VV& right);

    template<typename KK, typename VV>
    friend bool operator >(const myPair<KK, VV>& left, const VV& right);

    template<typename KK, typename VV>
    friend bool operator ==(const myPair<KK, VV>& left, const myPair<KK, VV>& right);

    template<typename KK, typename VV>
    friend bool operator ==(const myPair<KK, VV>& left, const KK& k);

    template<typename KK, typename VV>
    friend bool operator !=(const myPair<KK, VV>& left, const myPair<KK, VV>& right);

    // Input/Output
    template<typename KK, typename VV>
    friend std::ostream& operator <<(std::ostream& out, const myPair<KK, VV>& p);

    template<typename KK, typename VV>
    friend std::istream& operator <<(std::istream& in, const myPair<KK, VV>& p);

};

template<typename K, typename V>
myPair<K, V>::myPair(K k, V v)
{
    key = k;
    value = v;
}

template<typename K, typename V>
myPair<K, V>::myPair(const myPair& p)
{
    key = p.key;
    value = p.value;
}

template<typename K, typename V>
myPair<K, V>& myPair<K, V>::operator =(myPair<K, V> p)
{
    if(this != &p)
    {
        key = p.key;
        value = p.value;
    }
    return *this;
}
template<typename K, typename V>
myPair<K, V>::~myPair()
{
    key = K();
    value = V();
}

template<typename KK, typename VV>
bool operator <(const myPair<KK, VV>& left, const myPair<KK, VV>& right)
{
    return left.value < right.value;
}

template<typename KK, typename VV>
bool operator >(const myPair<KK, VV>& left, const myPair<KK, VV>& right)
{
    return left.value > right.value;
}

template<typename KK, typename VV>
bool operator <(const myPair<KK, VV>& left, const VV& right)
{
    return left.value < right;
}

template<typename KK, typename VV>
bool operator >(const myPair<KK, VV>& left, const VV& right)
{
    return left.value > right;
}

template<typename KK, typename VV>
bool operator ==(const myPair<KK, VV>& left, const myPair<KK, VV>& right)
{
    return left.value == right.value;
}

template<typename KK, typename VV>
bool operator ==(const myPair<KK, VV>& left, const VV& k)
{
    return left.value == k;
}

template<typename KK, typename VV>
bool operator !=(const myPair<KK, VV>& left, const myPair<KK, VV>& right)
{
    return !(left.value == right.value);
}

template<typename KK, typename VV>
std::ostream& operator <<(std::ostream& out, const myPair<KK, VV>& p)
{
    if(&out == &std::cout)
        out<<"("<<p.key<<" : "<<p.value<<")";
    else
        out<<p.key<<"\a"<<p.value;
    return out;
}

template<typename KK, typename VV>
std::istream& operator <<(std::istream& in, const myPair<KK, VV>& p)
{
    if(&in != &std::cin)
        in.imbue(std::locale(std::locale(), new reader()));
    in>>p.key>>p.value;
    return in;
}
#endif // PAIR_H
