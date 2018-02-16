#ifndef __LIST_H__
#define __LIST_H__

template <class T>
class Node
{
public:

    Node(T new_data);

    T data;

    Node<T> * next;
};


template <class T>
class List
{
public:

    List();

    int insert(T data);

    int remove(T data);

    int length();

    int empty();

    Node<T> * get_head();

private:

    Node<T> * head_node;

    int size;
};

#include "list.cpp"
#endif // __LIST_H__
