#include <iostream>

struct Data {
    int value;

    Data(): value(42) {}

    ~Data() {
        increment_counter();
    }
};

class DataBuffer {
    size_t size_;
    Data* buffer;

public:
    DataBuffer(size_t size): size_(size), buffer(new Data[size]) {}

    ~DataBuffer() {
        delete[] buffer;
    }

    DataBuffer(const DataBuffer&) = delete;
    DataBuffer& operator=(const DataBuffer&) = delete;

    DataBuffer(DataBuffer&& other) noexcept 
        : size_(other.size_), buffer(other.buffer) {
        other.size_ = 0;
        other.buffer = nullptr;
    }

    DataBuffer& operator=(DataBuffer&& other) noexcept {
        if (this != &other) {
            delete[] buffer;
            size_ = other.size_;
            buffer = other.buffer;
            other.size_ = 0;
            other.buffer = nullptr;
        }
        return *this;
    }

    size_t size() const {
        return size_;
    }

    Data& operator[](size_t index) {
        return buffer[index];
    }

    const Data& operator[](size_t index) const {
        return buffer[index];
    }

    friend void swap(DataBuffer& first, DataBuffer& second) noexcept {
        using std::swap;
        swap(first.size_, second.size_);
        swap(first.buffer, second.buffer);
    }
};

namespace std {
    template<>
    void swap(DataBuffer& a, DataBuffer& b) noexcept {
        a.swap(b);
    }


  void print_buffer(const DataBuffer &buffer) {
  for (size_t i = 0; i < buffer.size(); i++) 
    std::cout << "buffer[" << i << "] == " << buffer[i] << std::endl;
}

int main()
{
  {
    DataBuffer buffer_1{5}, buffer_2{3};

    std::cout << "buffer_1.size() == " << buffer_1.size() << std::endl;
    std::cout << "buffer_2.size() == " << buffer_2.size() << std::endl;

    for (size_t i = 0; i < buffer_2.size(); i++) 
      buffer_2[i] = i + 1;

    std::cout << "buffer_1[0] == " << buffer_1[0].value << std::endl;
    std::cout << "buffer_2[0] == " << buffer_2[0].value << std::endl;

    std::swap(buffer_1, buffer_2);
    // DataBuffer buffer_copy = buffer_1; // forbidden!

    std::cout << "buffer_1.size() == " << buffer_1.size() << std::endl;
    std::cout << "buffer_2.size() == " << buffer_2.size() << std::endl;

    print_buffer(buffer_1);
  }
  std::cout << "~Data() called " << increment_counter() << " times" << std::endl;
  return 0;
}