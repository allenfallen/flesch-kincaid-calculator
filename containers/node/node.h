#ifndef NODE_H
#define NODE_H
#include <string>
#include <vector>
#include <sstream>

#define uint unsigned int

template<typename T>
struct node
{
    T data;
    uint count;
    node* child[2];
    std::vector<uint> lineNum;
    std::vector<uint> paraNum;

    node(const T &d = T(), const uint& ln = 0, const uint& pn = 0, const uint &c = 0);
    node(const node<T> &other);
    node<T>& operator=(const node<T> &other);
    ~node();

    void swap(node<T>& other);
    void clear();

    template<typename U>
    friend std::ostream& operator<<(std::ostream& out, const node<U> &n);

    template<typename U>
    friend std::istream& operator>>(std::istream& in, node<U> &n);

    template<typename U>
    friend bool operator >(const node<U> &x, const node<U> &y);

    template<typename U>
    friend bool operator <(const node<U> &x, const node<U> &y);
};

template<typename T>
node<T>::node(const T &d, const unsigned int &ln, const unsigned int &pn, const uint &c)
{
    data = d;
    count = c;
    child[0] = child[1] = NULL;
    ln ? lineNum.push_back(ln) : lineNum.clear();
    pn ? paraNum.push_back(pn) : paraNum.clear();
}

template<typename T>
node<T>::node(const node<T> &n)
{
    data = n.data;
    count = n.count;
    lineNum = n.lineNum;
    paraNum = n.paraNum;
    child[0] = child[1] = NULL;
}

template<typename T>
node<T>& node<T>::operator=(const node<T> &other)
{
    if(this != &other)
    {
        data = other.data;
        count = other.count;
        lineNum = other.lineNum;
        paraNum = other.paraNum;
    }
    return *this;
}

template<typename T>
node<T>::~node()
{
    clear();
}

template<typename T>
void node<T>::swap(node<T> &x)
{
    T temp = x.data;
    x.data = data;
    data = temp;
}

template<typename T>
void node<T>::clear()
{
    data = T();
    child[0] = child[1] = NULL;
    count = 0;
    lineNum.clear();
    paraNum.clear();
}

template<typename U>
std::ostream& operator<<(std::ostream& out, const node<U> &n)
{
//    if(&out == &std::cout)
//        out<<n.data;
//    else
    {
        std::vector<uint>::const_iterator it;

        // Count
        out<<n.data<<" ["<<n.count<<"][";

        // Line numbers
        for(it = n.lineNum.cbegin(); it != n.lineNum.cend()-1; it++)
            out<<*it<<" ";
        out<<*it<<"][";

        // Paragraph numbers
        for(it = n.paraNum.cbegin(); it != n.paraNum.cend()-1; it++)
            out<<*it<<" ";
        out<<*it<<"]";
    }
    return out;
}

template<typename U>
std::istream& operator>>(std::istream& in, node<U> &n)
{
    in>>n.data;
    if(&in != &std::cin)
    {
        std::string line, nums;
        uint begin, end, num;
        std::stringstream ss;
        getline(in, line);

        // Count
        begin = line.find_first_of('[') + 1;
        end = line.find_first_of(']') - 2;
        ss<<line.substr(begin, end);
        ss>>n.count;
        ss.clear();

        // Line numbers
        begin = end + 4;
        end = line.find_last_of('[') - begin - 1;
        nums = line.substr(begin, end);
        while(ss<<nums)
        {
            ss>>num;
            n.lineNum.push_back(num);
            nums.clear();
        }
        ss.clear();


        // Paragraph numbers
        begin = end + begin + 2;
        end = line.length() - begin - 1;
        nums = line.substr(begin, end);
        while(ss<<nums)
        {
            ss>>num;
            n.paraNum.push_back(num);
            nums.clear();
        }
    }
    n.child[0] = n.child[1] = NULL;
    return in;
}

template<typename U>
bool operator >(const node<U> &x, const node<U> &y)
{
    return x.data > y.data;
}

template<typename U>
bool operator <(const node<U> &x, const node<U> &y)
{
    return x.data < y.data;
}

//bool operator >(node<std::string> &x, node<std::string> &y)
//{
//    return x.data.compare(y.data) > 0;
//}

//bool operator <(node<std::string> &x, node<std::string> &y)
//{
//    return x.data.compare(y.data) < 0;
//}

#endif // NODE_H
