#ifndef _CYCLE_QUEUE_H
#define _CYCLE_QUEUE_H

#include <exception>

template <class T>
class CycleQueue {
   private:
    unsigned int m_size;
    int m_front;
    int m_rear;
    T* m_data;

   public:
    CycleQueue(unsigned int size) : m_size(size), m_front(0), m_rear(0) {
        m_data = new T[size];
    }

    ~CycleQueue() {
        delete[] m_data;
    }

    bool isEmpty() {
        return m_front == m_rear;
    }

    bool isFull() {
        return m_front == (m_rear + 1) % m_size;
    }

    void push(T element) throw(std::bad_exception) {
        if (isFull()) throw std::bad_exception();
        m_data[m_rear] = element;
        m_rear = (m_rear + 1) % m_size;
    }

    void pop() throw(std::bad_exception) {
        if (isEmpty()) throw std::bad_exception();
        T element = m_data[m_front];
        m_front = (m_front + 1) % m_size;
        return element;
    }
};

#endif  // _CYCLE_QUEUE_H