#ifndef ARRAY3D_H
#define ARRAY3D_H

#include <vector>
#include <assert.h>
#include "serialize.h"

template <class T> class Array2D : public SerializableObject{

    public:
        Array2D();
        Array2D(size_t sizeX, size_t sizeY, const T& value = 0);
        T& operator () (size_t i, size_t j);
        T operator () (size_t i, size_t j) const;
        const T* operator () (size_t i) const;

        size_t getSizeX() const;
        size_t getSizeY() const;

        void setConstant(const T& c);

        // SerializableObject interface
        void serialize(std::ofstream& binaryFile) const;
        void deserialize(std::ifstream& binaryFile);

    private:
        size_t getIndex(size_t i, size_t j) const;

        size_t sizeX, sizeY;
        std::vector<T> v;
};

template <class T> class Array3D : public SerializableObject{

    public:
        Array3D();
        Array3D(size_t sizeX, size_t sizeY, size_t sizeZ, const T& value = 0);
        T& operator () (size_t i, size_t j, size_t k);
        T operator () (size_t i, size_t j, size_t k) const;
        const T* operator () (size_t i, size_t j) const;

        size_t getSizeX() const;
        size_t getSizeY() const;
        size_t getSizeZ() const;

        void setConstant(const T& c);

        // SerializableObject interface
        void serialize(std::ofstream& binaryFile) const;
        void deserialize(std::ifstream& binaryFile);

    private:
        size_t getIndex(size_t i, size_t j, size_t k) const;

        size_t sizeX, sizeY, sizeZ;
        std::vector<T> v;
};

template <class T> class Array4D : public SerializableObject{

    public:
        Array4D();
        Array4D(size_t sizeX, size_t sizeY, size_t sizeZ, size_t sizeW, const T& value = 0);
        T& operator () (size_t i, size_t j, size_t k, size_t l);
        T operator () (size_t i, size_t j, size_t k, size_t l) const;
        const T* operator () (size_t i, size_t j, size_t k) const;

        size_t getSizeX() const;
        size_t getSizeY() const;
        size_t getSizeZ() const;
        size_t getSizeW() const;

        void setConstant(const T& c);

        // SerializableObject interface
        void serialize(std::ofstream& binaryFile) const;
        void deserialize(std::ifstream& binaryFile);

    private:
        size_t getIndex(size_t i, size_t j, size_t k, size_t l);

        size_t sizeX, sizeY, sizeZ, sizeW;
        std::vector<T> v;
};

///
/// Array2D
///

template <class T>
inline Array2D<T>::Array2D() : sizeX(0), sizeY(0) {
    v.resize(0);
}

template <class T>
inline Array2D<T>::Array2D(size_t sizeX, size_t sizeY, const T& value) : sizeX(sizeX), sizeY(sizeY) {
    v.resize(sizeX*sizeY, value);
}

template <class T>
inline T& Array2D<T>::operator ()(size_t i, size_t j) {
    return v[getIndex(i,j)];
}

template <class T>
inline T Array2D<T>::operator ()(size_t i, size_t j) const {
    return v[getIndex(i,j)];
}

template <class T>
inline const T* Array2D<T>::operator ()(size_t i) const{
    assert (i < sizeX);
    return &(v[sizeY*i]);
}

template <class T>
inline size_t Array2D<T>::getSizeX() const {
    return sizeX;
}

template <class T>
inline size_t Array2D<T>::getSizeY() const{
    return sizeY;
}

template <class T>
inline void Array2D<T>::setConstant(const T& c) {
    std::fill(v.begin(), v.end(), c);
}

template <class T>
inline void Array2D<T>::serialize(std::ofstream& binaryFile) const {
    Serializer::serialize(sizeX, binaryFile);
    Serializer::serialize(sizeY, binaryFile);
    for (unsigned int i = 0; i < v.size(); ++i)
        Serializer::serialize(v[i], binaryFile);
}

template <class T>
inline void Array2D<T>::deserialize(std::ifstream& binaryFile) {
    Serializer::deserialize(sizeX, binaryFile);
    Serializer::deserialize(sizeY, binaryFile);
    v.resize(sizeX*sizeY);
    for (unsigned int i = 0; i < v.size(); ++i)
        Serializer::deserialize(v[i], binaryFile);
}

template <class T>
inline size_t Array2D<T>::getIndex(size_t i, size_t j) const {
    assert (i < sizeX);
    assert (j < sizeY);
    return j + sizeY*i;
}

///
/// Array3D
///

template <class T>
inline Array3D<T>::Array3D() : sizeX(0), sizeY(0), sizeZ(0) {
    v.resize(0);
}

template <class T>
inline Array3D<T>::Array3D(size_t sizeX, size_t sizeY, size_t sizeZ, const T& value) : sizeX(sizeX), sizeY(sizeY), sizeZ(sizeZ) {
    v.resize(sizeX*sizeY*sizeZ, value);
}

template <class T>
inline T& Array3D<T>::operator ()(size_t i, size_t j, size_t k) {
    return v[getIndex(i,j,k)];
}

template <class T>
inline T Array3D<T>::operator ()(size_t i, size_t j, size_t k) const {
    return v[getIndex(i,j,k)];
}

template <class T>
inline const T* Array3D<T>::operator ()(size_t i, size_t j) const{
    assert (i < sizeX);
    assert (j < sizeY);
    return &(v[sizeZ*(j + sizeY*i)]);
}

template <class T>
inline size_t Array3D<T>::getSizeX() const {
    return sizeX;
}

template <class T>
inline size_t Array3D<T>::getSizeY() const{
    return sizeY;
}

template <class T>
inline size_t Array3D<T>::getSizeZ() const {
    return sizeZ;
}

template <class T>
inline void Array3D<T>::setConstant(const T& c) {
    std::fill(v.begin(), v.end(), c);
}

template <class T>
inline void Array3D<T>::serialize(std::ofstream& binaryFile) const {
    Serializer::serialize(sizeX, binaryFile);
    Serializer::serialize(sizeY, binaryFile);
    Serializer::serialize(sizeZ, binaryFile);
    for (unsigned int i = 0; i < v.size(); ++i)
        Serializer::serialize(v[i], binaryFile);
}

template <class T>
inline void Array3D<T>::deserialize(std::ifstream& binaryFile) {
    Serializer::deserialize(sizeX, binaryFile);
    Serializer::deserialize(sizeY, binaryFile);
    Serializer::deserialize(sizeZ, binaryFile);
    v.resize(sizeX*sizeY*sizeZ);
    for (unsigned int i = 0; i < v.size(); ++i)
        Serializer::deserialize(v[i], binaryFile);
}

template <class T>
inline size_t Array3D<T>::getIndex(size_t i, size_t j, size_t k) const {
    assert (i < sizeX);
    assert (j < sizeY);
    assert (k < sizeZ);
    return k+sizeZ*(j + sizeY*i);
}

///
/// Array4D
///

template <class T>
inline Array4D<T>::Array4D() : sizeX(0), sizeY(0), sizeZ(0), sizeW(0) {
    v.resize(0);
}

template <class T>
inline Array4D<T>::Array4D(size_t sizeX, size_t sizeY, size_t sizeZ, size_t sizeW, const T& value) : sizeX(sizeX), sizeY(sizeY), sizeZ(sizeZ), sizeW(sizeW) {
    v.resize(sizeX*sizeY*sizeZ*sizeW, value);
}

template <class T>
inline T& Array4D<T>::operator ()(size_t i, size_t j, size_t k, size_t l) {
    return v[getIndex(i,j,k,l)];
}

template <class T>
inline T Array4D<T>::operator ()(size_t i, size_t j, size_t k, size_t l) const {
    return v[getIndex(i,j,k,l)];
}

/*template <class T>
inline std::vector<T> Array4D<T>::operator ()(size_t i, size_t j, size_t k) const {
    assert (i < sizeX);
    assert (j < sizeY);
    assert (k < sizeZ);
    return std::vector<T>(v.begin()+ sizeW*(k + sizeZ*(j + sizeY*i)), v.begin()+ sizeW*((k+1) + sizeZ*(j + sizeY*i)));
}*/

template <class T>
inline const T* Array4D<T>::operator ()(size_t i, size_t j, size_t k) const {
    assert (i < sizeX);
    assert (j < sizeY);
    assert (k < sizeZ);
    return &(v[sizeW*(k + sizeZ*(j + sizeY*i))]);
    //return std::vector<T>(v.begin()+ sizeW*(k + sizeZ*(j + sizeY*i)), v.begin()+ sizeW*((k+1) + sizeZ*(j + sizeY*i)));
}

template <class T>
inline size_t Array4D<T>::getSizeX() const {
    return sizeX;
}

template <class T>
inline size_t Array4D<T>::getSizeY() const{
    return sizeY;
}

template <class T>
inline size_t Array4D<T>::getSizeZ() const {
    return sizeZ;
}

template <class T>
inline size_t Array4D<T>::getSizeW() const {
    return sizeW;
}

template <class T>
inline void Array4D<T>::setConstant(const T& c) {
    std::fill(v.begin(), v.end(), c);
}

template <class T>
inline void Array4D<T>::serialize(std::ofstream& binaryFile) const {
    Serializer::serialize(sizeX, binaryFile);
    Serializer::serialize(sizeY, binaryFile);
    Serializer::serialize(sizeZ, binaryFile);
    Serializer::serialize(sizeW, binaryFile);
    for (unsigned int i = 0; i < v.size(); ++i)
        Serializer::serialize(v[i], binaryFile);
}

template <class T>
inline void Array4D<T>::deserialize(std::ifstream& binaryFile) {
    Serializer::deserialize(sizeX, binaryFile);
    Serializer::deserialize(sizeY, binaryFile);
    Serializer::deserialize(sizeZ, binaryFile);
    Serializer::deserialize(sizeW, binaryFile);
    v.resize(sizeX*sizeY*sizeZ*sizeW);
    for (unsigned int i = 0; i < v.size(); ++i)
        Serializer::deserialize(v[i], binaryFile);
}

template <class T>
inline size_t Array4D<T>::getIndex(size_t i, size_t j, size_t k, size_t l) {
    assert (i < sizeX);
    assert (j < sizeY);
    assert (k < sizeZ);
    assert (l < sizeW);
    return l + sizeW*(k + sizeZ*(j + sizeY*i));
}

#endif // ARRAY3D_H
