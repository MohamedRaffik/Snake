template <class T>
Node<T>::Node(T new_data)
{
    data = new_data;
    next = NULL;
}


template <class T>
List<T>::List()
{
    head_node = NULL;
    size = 0;
}


template <class T>
int List<T>::insert(T data)
{
    Node<T> * current = head_node;
    Node<T> * temp = new Node<T>(data);
    if (temp == NULL) { return 0; }
    if (head_node == NULL)
    {
        temp->next = head_node;
        head_node = temp;
        return 1;
    }
    while (current->next != NULL) { current = current->next; }
    current->next = temp;
    size++;
    return 1;
}


template <class T>
int List<T>::remove(T data)
{
    int items = 0;
    Node<T> * current = head_node;
    while (current != NULL)
    {
        Node<T> * extra = current->next;
        delete current;
        current = NULL;
        current = extra;
        items++;
    }
    return items;
}


template <class T>
Node<T> * List<T>::get_head() { return head_node; }


template <class T>
int List<T>::length() { return size; }
