#ifndef HEAP_H
#define HEAP_H
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <bitset>
#include <cmath>
#include <vector>
#include "functors.h"

template<typename T, typename HEAP_TYPE = ascend<T>>
class heap
{
    public:
        heap();
        heap(const T &d);
        heap<T, HEAP_TYPE>& operator=(const heap<T, HEAP_TYPE> &other);
        heap(const heap<T, HEAP_TYPE> &other);
        ~heap();

        bool empty();
        uint size();
        void clear();

        T& peek() const;
        heap<T, HEAP_TYPE>& operator<<(const T &d);
        heap<T, HEAP_TYPE>& operator>>(T &d);

        template<typename U, typename I>
        friend std::ostream& operator<<(std::ostream &out, const heap<U, I> &heap);

        template<typename U, typename I>
        friend std::istream& operator>>(std::istream &in, heap<U, I> &heap);

        template<typename U>
        friend bool getInput(std::istream& in, U &data);

    private:
        typedef uint (heap<T, HEAP_TYPE>::*fptrPos)(uint);

        std::vector<T*> list;
        HEAP_TYPE cmp;

        void copy(const heap<T, HEAP_TYPE> &other);
        void nukem();

        uint leftChild(uint who);
        uint rightChild(uint who);
        uint parent(uint who);

        void reheapifyUp();
        void reheapifyDown();
};

template<typename T, typename HEAP_TYPE>
heap<T, HEAP_TYPE>::heap()
{
}

template<typename T, typename HEAP_TYPE>
heap<T, HEAP_TYPE>::heap(const T &d)
{
    T* ptr = new T(d);
    list.push_back(ptr);
}

template<typename T, typename HEAP_TYPE>
heap<T, HEAP_TYPE>& heap<T, HEAP_TYPE>::operator=(const heap<T, HEAP_TYPE> &other)
{
    if(this != &other)
    {
        nukem();
        copy(other);
    }
    return *this;
}

template<typename T, typename HEAP_TYPE>
heap<T, HEAP_TYPE>::heap(const heap<T, HEAP_TYPE> &other)
{
    copy(other);
}

template<typename T, typename HEAP_TYPE>
heap<T, HEAP_TYPE>::~heap()
{
    nukem();
}

template<typename T, typename HEAP_TYPE>
bool heap<T, HEAP_TYPE>::empty()
{
    return list.empty();
}

template<typename T, typename HEAP_TYPE>
uint heap<T, HEAP_TYPE>::size()
{
    return list.size();
}

template<typename T, typename HEAP_TYPE>
void heap<T, HEAP_TYPE>::clear()
{
    return list.clear();
}

template<typename T, typename HEAP_TYPE>
T& heap<T, HEAP_TYPE>::peek() const
{
    return *list[0];
}

template<typename T, typename HEAP_TYPE>
heap<T, HEAP_TYPE>& heap<T, HEAP_TYPE>::operator<<(const T &d)
{
    // Insert at the end and reheapify
    T* ptr = new T(d);
    list.push_back(ptr);
    reheapifyUp();
    return *this;
}

template<typename T, typename HEAP_TYPE>
heap<T, HEAP_TYPE>& heap<T, HEAP_TYPE>::operator>>(T &d)
{
    // Swap the "root" with the end, extract the end, and reheapify
    swap(list[0], list[list.size()-1]);
    d = *list.back();
    list.pop_back();
    reheapifyDown();
    return *this;
}

template<typename T, typename HEAP_TYPE>
void heap<T, HEAP_TYPE>::copy(const heap<T, HEAP_TYPE> &other)
{
    if(other.list.size())
    {
        list.clear();
        list = other.list;
    }
}

template<typename T, typename HEAP_TYPE>
void heap<T, HEAP_TYPE>::nukem()
{
    list.clear();
}

template<typename T, typename HEAP_TYPE>
uint heap<T, HEAP_TYPE>::leftChild(uint who)
{
    return who*2 + 1;
}

template<typename T, typename HEAP_TYPE>
uint heap<T, HEAP_TYPE>::rightChild(uint who)
{
    return 2*who + 2;
}

template<typename T, typename HEAP_TYPE>
uint heap<T, HEAP_TYPE>::parent(uint who)
{
    return who ? (who-1)/2 : who;
}

template<typename T, typename HEAP_TYPE>
void heap<T, HEAP_TYPE>::reheapifyUp()
{
    // currChild is the farthest available node
    // Begin with the parent of currChild and currChild
    bool cont = true;
    uint currChild = list.size()-1,
         currParent = parent(currChild);

    // Max heap: parent must be greater than its children
    while(cont && list[currChild])
    {
        // Swap the parent and child, meaning the child is now the parent
        if(cmp(*list[currParent], *list[currChild]))
        {
            swap(list[currParent], list[currChild]);
            currChild = currParent;
            currParent = parent(currParent);
        }
        else
            cont = false;
    }
}

template<typename T, typename HEAP_TYPE>
void heap<T, HEAP_TYPE>::reheapifyDown()
{
    // Compare the parent with its children; if they are in the correct order, stop.
    // Replace the parent with the greater of its children, if any.
    // Continue with the parent being the child that was just replaced.
    uint parent = 0,
         height = (uint)((log10((double)list.size())/log10(2.))),
         dir;

    fptrPos whatToDo[2];
    whatToDo[0] = &heap<T, HEAP_TYPE>::leftChild;
    whatToDo[1] = &heap<T, HEAP_TYPE>::rightChild;

    // Reheapify down as long as we are in the heap and either children violate the heap property
    while(rightChild(parent) < list.size() &&
         (cmp(*list[parent], *list[leftChild(parent)]) ||
          cmp(*list[parent], *list[rightChild(parent)])))

    {
        // Two children: the direction of the greater between the two && heap property violated
        if((list[rightChild(parent)] && list[leftChild(parent)]))
        {
            dir = (*list[rightChild(parent)]) < (*list[leftChild(parent)]);
            if(!cmp(*list[parent], *list[(this->*whatToDo[dir])(parent)]))
                return;
        }

        // One child: its direction && heap property violated
        else if(list[leftChild(parent)] && cmp(*list[parent], *list[leftChild(parent)]))
            dir = 0;
        else if(list[rightChild(parent)] && cmp(*list[parent], *list[leftChild(parent)]))
            dir = 1;

        // No child: exit
        else
            return;

        // Swap the position of the parent and the child that violates the heap property
        // Continue with the new parent being the previous child
        swap(list[parent], list[(this->*whatToDo[dir])(parent)]);
        parent = (this->*whatToDo[dir])(parent);
    }
}

template<typename T, typename HEAP_TYPE>
void swap(T& you, T& me)
{
    T temp = you;
    you = me;
    me = temp;
}

template<typename U>
bool getInput(std::istream& in, U &data)
{
    std::string line;
    std::stringstream ss;
    std::cout<<"Please enter your data: ";
    getline(in,line);

    // Empty line: exit input
    if(line.empty())
       return false;

    // Insert input into the data
    ss<<line;
    ss>>data;
    return true;
}

template<typename U, typename I>
std::ostream& operator<<(std::ostream &out, const heap<U, I> &who)
{
    for(typename std::vector<U*>::const_iterator it = who.list.cbegin(); it != who.list.cend(); it++)
        out<<(**it)<<" ";
    return out;
}

template<typename U, typename I>
std::istream& operator>>(std::istream &in, heap<U, I> &who)
{
    std::stringstream ss;
    bool input = true;
    U data, *ptr;
    if(&in == &std::cin)
    {
        while(getInput(in, data))
        {
            ptr = new U(data);
            who.list.push_back(ptr);
        }
    }
    else
    {
        while(in>>data)
        {
            ptr = new U(data);
            who.heap.push_back(ptr);
        }
    }
    return in;
}
#endif // HEAP_H
