#ifndef _MIN_HEAP_H_
#define _MIN_HEAP_H_

/*
  * MinHeap
  * Estrutura de dados de heap mínimo
  * Propriedades:
  * - data: array de valores do tipo genérico
  * - capacity: capacidade máxima do array
  * - length: quantidade de elementos no array
  * Métodos:
  * - size(): retorna o tamanho do array
  * - isEmpty(): verifica se o array está vazio
  * - push(T value): insere um valor no array
  * - top(): retorna o menor valor do array
  * - pop(): remove e retorna o menor valor do array
  * - clear(): remove todos os valores do array
  * - show(): retorna uma string com os valores do array
  * - forEach(void (*f)(T, void* params), void* params): executa uma função para cada valor
  * - remove(T value, bool (*cmp)(T a, T b)): remove um valor do array
  * - copy(): retorna uma cópia do array
  */
template <class T>
class MinHeap {
private:
  T* data;
  size_t capacity;
  size_t length;

  void swap(size_t i, size_t j) {
    T temp = data[i];
    data[i] = data[j];
    data[j] = temp;
  }

  void heapifyUp(size_t index) {
    if (index == 0) return;
    size_t parent = (index - 1) / 2;
    if (*data[index] < *data[parent]) {
      swap(index, parent);
      heapifyUp(parent);
    }
  }

  void heapifyDown(size_t index) {
    size_t left = 2 * index + 1;
    size_t right = 2 * index + 2;
    size_t smallest = index;

    if (left < length && *data[left] < *data[smallest])
      smallest = left;
    if (right < length && *data[right] < *data[smallest])
      smallest = right;

    if (smallest != index) {
      swap(index, smallest);
      heapifyDown(smallest);
    }
  }

public:
  MinHeap(size_t capacity = 20) {
    this->capacity = capacity;
    this->length = 0;
    this->data = new T[capacity];
  }

  ~MinHeap() {
    delete[] data;
  }

  size_t size() const {
    return length;
  }

  bool isEmpty() const {
    return length == 0;
  }

  void push(T value) {
    if (length >= capacity) {
      capacity *= 2;
      T* newData = new T[capacity];
      for (size_t i = 0; i < length; i++)
        newData[i] = data[i];
      delete[] data;
      data = newData;
    }

    data[length] = value;
    heapifyUp(length);
    length++;
  }

  T top() {
    if (isEmpty()) return nullptr;
    return data[0];
  }

  T pop() {
    if (isEmpty()) return nullptr;

    T root = data[0];
    data[0] = data[length - 1];
    length--;
    heapifyDown(0);

    return root;
  }

  void clear() {
    length = 0;
  }

  String show() {
    String str = "[";
    for (size_t i = 0; i < length; i++) {
      str.concat(String(*data[i]));
      if (i < length - 1)
        str.concat(", ");
    }
    str.concat("]");
    return str;
  }

  void forEach(void (*f)(T, void* params), void* params = nullptr) {
    for (size_t i = 0; i < length; i++) {
      f(data[i], params);
    }
  }

  T remove(T value, bool (*cmp)(T a, T b)) {
    int8_t index = -1;

    for (size_t i = 0; i < length; i++) {
      if (cmp(value, data[i])) {
        index = i;
        break;
      }
    }

    if (index == -1) {
      return nullptr;
    }

    T removedValue = data[index];

    if (index == length - 1) {
      length--;
      return removedValue;
    }

    data[index] = data[length - 1];
    length--;

    if (index > 0 && *data[index] < *data[(index - 1) / 2]) {
      heapifyUp(index);
    } else {
      heapifyDown(index);
    }
  
    return removedValue;
  }

  MinHeap<T> copy() {
    MinHeap<T> newHeap(capacity);
    for (size_t i = 0; i < length; i++) {
      newHeap.push(data[i]);
    }
    return newHeap;
  }
};

#endif
