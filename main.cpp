#include <iostream>

template<typename T, size_t SIZE>
class Array
{
public:
    constexpr size_t Size() const { return SIZE; }

    T& operator[](size_t index) { return m_Data[index]; }
    const T& operator[](size_t index) const { return m_Data[index]; }

    T* Data() { return m_Data; }
    const T* Data() const { return m_Data; }
private:
    T m_Data[SIZE];
};

int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
