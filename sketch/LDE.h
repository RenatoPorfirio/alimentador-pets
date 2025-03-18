#ifndef _LDE_H_
#define _LDE_H_

template <class T>
class LDENode {
  public:
  T value;
  LDENode *prev, *next;

  LDENode(T value) {
    this->value = value;
    this->prev = nullptr;
    this->next = nullptr;
  }
};

template <class T>
class LDE {
  size_t length;
  LDENode<T> *head, *tail;

  public:
  LDE() {
    this->length = 0;
    this->head = this->tail = nullptr;
  }

  ~LDE() {
    clear();
  }

  size_t size() {
    return this->length;
  }

  T first() {
    if(length == 0) {
      return (T)nullptr;
    }
    return head->value;
  }

  T last() {
    if(length == 0) {
      return (T)nullptr;
    }
    return tail->value;
  }

  LDE<T>& push(T value) {
    LDENode<T> *x = new LDENode<T>(value);
    if(length == 0) {
      tail = x;
    } else {
      x->next = head;
      head->prev = x;
    }
    head = x;
    length++;
    return *this;
  }

  LDE<T>& pushBack(T value) {
    LDENode<T> *x = new LDENode<T>(value);
    if(length == 0) {
      head = x;
    } else {
      x->prev = tail;
      tail->next = x;
    }
    tail = x;
    length++;
    return *this;
  }

  T pop() {
    if(length == 0) return (T)nullptr;
    if(length == 1) tail = nullptr;
    LDENode<T> *x = head;
    T result = x->value;
    head = head->next;
    if(head) head->prev = nullptr;
    x->next = nullptr;
    delete x;
    length--;
    return result;
  }

  T popBack() {
    if(length == 0) return (T)nullptr;
    if(length == 1) head = nullptr;
    LDENode<T> *x = tail;
    T result = x->value;
    tail = tail->prev;
    if(tail) tail->next = nullptr;
    x->prev = nullptr;
    delete x;
    length--;
    return result;
  }

  T remove(size_t index) {
    if(index >= length || index < 0) return (T)nullptr;
    if(index == 0) return pop();
    if(index == length - 1) return popBack();
    LDENode<T> *x, *prev, *next;
    for(x = head; index > 0; index--, x = x->next);
    T value = x->value;
    prev = x->prev;
    next = x->next;
    if(prev) prev->next = next;
    if(next) next->prev = prev;
    x->prev = x->next = nullptr;
    delete x;
    length--;
    return value;
  }

  int getIndex(T value, bool (*eval)(T a, T b)) {
    int index = 0;
    LDENode<T> *x;
    for(x = head; x != nullptr; x = x->next, index++) {
      if(eval(value, x->value)) break;
    }
    if (index == length) return -1;
    return index;
  }

  void clear() {
    LDENode<T>* x = head;
    for(size_t i = 0; i < length; i++) {
      LDENode<T>* u = x;
      x = x->next;
      delete u;
    }
    head = nullptr;
    tail = nullptr;
    length = 0;
  }

  String show() {
    String str = "[";
    LDENode<T>* x = head;
    if(x) str.concat(String(x->value));
    if(length > 0) {
      for(x = x->next; x != nullptr; x = x->next) {
        str.concat(", ");
        str.concat(String(x->value));
      }
    }
    str.concat("]");
    return str;
  }

  void forEach(void (*f)(T, void* params), void* params=nullptr) {
    for(LDENode<T>* x = head; x != nullptr; x = x->next) {
      f(x->value, params);
    }
  }
};

#endif