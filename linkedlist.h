#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <QDebug>

template <typename T>
class LinkedList {
private:
    struct Node {
        T data;
        Node* next;
        Node(T d) : data(d), next(nullptr) {}
    };
    Node* head;
    int count;

public:
    LinkedList() : head(nullptr), count(0) {}

    ~LinkedList() {
        Node* current = head;
        while (current != nullptr) {
            Node* next = current->next;
            delete current;
            current = next;
        }
    }

    void append(T data) {
        Node* newNode = new Node(data);
        if (!head) {
            head = newNode;
        } else {
            Node* current = head;
            while (current->next) {
                current = current->next;
            }
            current->next = newNode;
        }
        count++;
    }

    T at(int index) {
        if (index < 0 || index >= count) {
            throw std::out_of_range("Index out of range");
        }
        Node* current = head;
        for (int i = 0; i < index; ++i) {
            current = current->next;
        }
        return current->data;
    }

    template <typename Func>
    T find(Func criteria) {
        Node* current = head;
        while (current) {
            if (criteria(current->data)) {
                return current->data;
            }
            current = current->next;
        }
        return nullptr;
    }

    int size() const {
        return count;
    }


    Node* begin() { return head; }
    Node* end() { return nullptr; }
    const Node* begin() const { return head; }
    const Node* end() const { return nullptr; }
};

#endif // LINKEDLIST_H
