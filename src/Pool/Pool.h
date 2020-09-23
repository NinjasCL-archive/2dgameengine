#ifndef POOL_H
#define POOL_H

// Required to have a vector of pools containing different object types
class IPool {
    public:
        virtual ~IPool() {}
};

// A pool is just a vector (contiguous data) of objects of type T
template <typename T>
class Pool: public IPool {
    private:
        std::vector<T> data;
    
    public:
        Pool(int size = 100) {
            Resize(size);
        }

        virtual ~Pool() = default;

        bool IsEmpty() const {
            return data.empty();
        }

        int GetSize() const {
            return data.size();
        }

        void Resize(int n) {
            data.resize(n);
        }

        void Clear() {
            data.clear();
        }

        void Add(T object) {
            data.push_back(object);
        }

        void Set(int index, T object) {
            data[index] = object;
        }

        T& Get(int index) {
            return static_cast<T&>(data[index]);
        }

        T& operator [](unsigned int index) {
            return data[index];
        }
};

#endif