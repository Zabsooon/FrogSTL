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

template<typename T>
class Vector
{
public:
    Vector()
    {
        ReAlloc(2);
    }

    ~Vector()
    {
        Clear();
        ::operator delete(m_Data, m_Capacity * sizeof(T));
    }

    void PushBack(const T& value)
    {
        if(m_Size >= m_Capacity)
            ReAlloc(m_Capacity + m_Capacity / 2);

        m_Data[m_Size] = value;
        m_Size++;
    }

    void PushBack(T&& value)
    {
        if(m_Size >= m_Capacity)
            ReAlloc(m_Capacity + m_Capacity / 2);

        m_Data[m_Size] = std::move(value);
        m_Size++;
    }

    template<typename... Args>
    T& EmplaceBack(Args&&... args)
    {
        if(m_Size >= m_Capacity)
            ReAlloc(m_Capacity + m_Capacity / 2);

        new(&m_Data[m_Size]) T(std::forward<Args>(args)...);
//        m_Data[m_Size] = T(std::forward<Args>(args)...);
        return m_Data[m_Size++];
    }

    void PopBack()
    {
        if(m_Size > 0)
        {
            m_Size--;
            m_Data[m_Size].~T();
        }
    }

    void Clear()
    {
        for(size_t i = 0; i < m_Size; i++)
            m_Data[i].~T();

        m_Size = 0;
    }

    T& operator[](size_t index)
    {
        if (index >= m_Size)
        {
            // assert
        }
        return m_Data[index];
    }
    const T& operator[](size_t index) const
    {
        if (index >= m_Size)
        {
            // assert
        }
        return m_Data[index];
    }

    size_t Size() const { return m_Size; }
private:
    void ReAlloc(size_t newCapacity)
    {
        // 1. allocate a new block of memory
        // 2. copy/move old elements into new block
        // 3. delete

        T* newBlock = (T*)::operator new(newCapacity *sizeof(T));

        if(newCapacity < m_Size)
            m_Size = newCapacity;

        for(size_t i = 0; i < m_Size; i++)
            new (&newBlock[i]) T(std::move(m_Data[i]));

        Clear();

        ::operator delete(m_Data, m_Capacity * sizeof(T));
        m_Data = newBlock;
        m_Capacity = newCapacity;
    }

private:
    T* m_Data = nullptr;

    size_t m_Size = 0;
    size_t m_Capacity = 0;

};

struct Vector3  // testing structure (temporary)
{
    float x = 1.0f, y = 1.0f, z = 1.0f;

    Vector3() = default;

    Vector3(float scalar)
        : x(scalar), y(scalar), z(scalar)
    { }

    Vector3(float x, float y, float z)
        : x(x), y(y), z(z)
    { }

    Vector3(const Vector3& other)
        : x(other.x), y(other.y), z(other.z)
    {
        printf("Copy\n");
    }

    Vector3(Vector3&& other)
        : x(other.x), y(other.y), z(other.z)
    {
        printf("Move\n");
    }

    ~Vector3()
    {
        printf("Destroyed!\n");
    }

    Vector3& operator=(const Vector3& other)
    {
        printf("Copy\n");
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }

    Vector3& operator=(Vector3&& other) noexcept
    {
        printf("Move\n");
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }
};

template<typename T>
void PrintVector(const Vector<T>& vector)
{
    for(size_t i = 0; i < vector.Size(); i++)
        std::cout << vector[i] << "\n";

    std::cout << "-------------------------------------\n";
}

template<>
void PrintVector(const Vector<Vector3>& vector)
{
    for(size_t i = 0; i < vector.Size(); i++)
        std::cout << vector[i].x << ", " << vector[i].y << ", " << vector[i].z << "\n";

    std::cout << "-------------------------------------\n";
}

int main() {
    Array<std::string, 2> data;
    data[0] = "Value of index 0";
    data[1] = "Value of index 1";

    Vector<Vector3> vector;
    vector.EmplaceBack(1.0f);
    vector.EmplaceBack(2, 3, 4);
    vector.EmplaceBack(1, 3, 4);
    vector.EmplaceBack();
    PrintVector(vector);
    vector.PopBack();
    vector.PopBack();
    vector.EmplaceBack(5, 2, 0);
    vector.EmplaceBack(1, 7, 9);
    PrintVector(vector);
    vector.Clear();
    PrintVector(vector);
    vector.EmplaceBack(5, 2, 0);
    vector.EmplaceBack(1, 7, 9);

    Vector<int> intVector;
    intVector.PushBack(5);
    intVector.EmplaceBack(2);
    intVector.EmplaceBack(2);
    intVector.EmplaceBack(2);
    intVector.EmplaceBack(2);
    intVector.EmplaceBack(2);
    intVector.EmplaceBack(2);
    PrintVector(intVector);
    intVector.PopBack();
    PrintVector(intVector);
    intVector.Clear();

    return 0;
}
