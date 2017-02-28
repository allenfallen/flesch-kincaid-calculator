#ifndef FUNCTORS_H
#define FUNCTORS_H

template<typename T>
struct ascend
{
    bool operator()(const T& lhs, const T& rhs)
    {
        return lhs > rhs;
    }
};

template<typename T>
struct descend
{
    bool operator()(const T& lhs, const T& rhs)
    {
        return lhs < rhs;
    }
};

#endif // FUNCTORS_H

