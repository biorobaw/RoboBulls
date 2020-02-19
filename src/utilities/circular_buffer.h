#ifndef CIRCULARBUFFER_H
#define CIRCULARBUFFER_H

template <class T>
class CircularBuffer
{
public:
    CircularBuffer(int size);
    //! @brief creates an array of size `size`
    //! and sets all its elements equal to `value`
    CircularBuffer(int size, T value);
    ~CircularBuffer();

    void push(T element);
    int getSize();

    //! @brief id can be both positive and negative.
    //! Id 0 is the newest added element.
    //! Id -1 or (size-1) is the oldest
    T& operator[](int id);

private:
    int size;
    int next_id = 0;
    T* data;
};

#endif // CIRCULARBUFFER_H



template <class T>
CircularBuffer<T>::CircularBuffer(int size) :
    size(size), data(new T[size])
{

}

template <class T>
CircularBuffer<T>::CircularBuffer(int size, T value) :
    size(size), data(new T[size] {value})
{

}

template <class T>
CircularBuffer<T>::~CircularBuffer(){
    delete[] data;
}

template <class T>
void CircularBuffer<T>::push(T element){
    data[next_id] = element;
    next_id = (next_id+1) % size;
}


template <class T>
int CircularBuffer<T>::getSize(){
    return size;
}

template <class T>
T& CircularBuffer<T>::operator[](int id){
    //observation: in a circular buffer, next_id points to the oldest element
    int id2 = (next_id-1 - id) % size;
    if(id2 < 0) id2+=size;
    return data[id2];
}
