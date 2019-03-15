#ifndef CSPEAR_CORE_ARRAY_H
#define CSPEAR_CORE_ARRAY_H

#include <cstring>
#include <vector>
#include <initializer_list>
#include <cspear/core/utils.h>
#include <cspear/core/assert.h>
#include <cspear/tools/misc.h>
#include <cspear/tools/types.h>

namespace csp {
  template <typename T, typename I=tools::Int>
  class array {
    T* data_;
    I sz_; // total number of elements
    std::vector<I> shape_;
    bool allocated_; // flag to indicate if the data is allocated by us

    public:
    // constructors and destructor
    array();
    array(std::initializer_list<T> elmts);
    array(const T* a, I sz);
    array(const T* a, std::initializer_list<I> shape);
    array(const array<T,I>& a);
    ~array();

    // indexing
    T& operator[](I idx) { return data_[idx]; }

    // assignment operator and copy
    array<T,I>& operator=(const array<T,I>& a);
    array<T,I> copy() const;

    // parameters
    T* data() { return data_; }
    const T* data() const { return data_; }
    I size() const { return sz_; }
    I ndim() const { return shape_.size(); }
    const std::vector<I>& shape() const { return shape_; }

    // casting and device movement
    template <typename T2> array<T2,I> astype() const;

    // reshape and resize
    array<T,I>& resize(I sz);
    array<T,I>& reshape(std::initializer_list<I> shape);

    private:
    void _realloc();
  };

  template <typename T, typename I>
  array<T,I>::array() {
    sz_ = 0;
    allocated_ = false;
  }

  template <typename T, typename I>
  array<T,I>::array(std::initializer_list<T> elmts) {
    // copy the data
    sz_ = elmts.size();
    _realloc();
    std::copy(elmts.begin(), elmts.end(), data_);

    // set the shape
    shape_ = {sz_};
  }

  template <typename T, typename I>
  array<T,I>::array(const T* a, I sz) : array(sz) {
    // copy the data
    std::copy(a, a+sz, data_);
  }

  template <typename T, typename I>
  array<T,I>::array(const T* a, std::initializer_list<I> shape) {
    // get the size
    sz_ = tools::_prod_init_list(shape);

    // copy the data
    _realloc();
    std::copy(a, a+sz_, data_);

    // set the shape
    shape_ = shape;
  }

  template <typename T, typename I>
  array<T,I>::array(const array<T,I>& a) {
    // copy the data
    sz_ = a.size();
    _realloc();
    std::copy(a.data(), a.data()+sz_, data_);

    // copy the shape as well
    shape_ = a.shape();
  }

  template <typename T, typename I>
  array<T,I>::~array() {
    if (allocated_) {
      std::free(data_);
    }
  }

  template <typename T, typename I>
  void array<T,I>::_realloc() {
    if (allocated_) std::free(data_);
    data_ = (T*) std::malloc(sz_*sizeof(*data_));
    assert_cpu(data_, "CPU memory allocation failed.");
    allocated_ = true;
  }

  template <typename T, typename I>
  array<T,I>& array<T,I>::resize(I sz) {
    if (sz_ == sz) {
      return *this;
    }
    else {
      sz_ = sz;
      _realloc();
      shape_ = {sz_}; // make it a one-dimensional array
      return *this;
    }
  }

  template <typename T, typename I>
  array<T,I>& array<T,I>::reshape(std::initializer_list<I> shape) {
    I sz = _calculate_size(shape);
    assert(sz == sz_);
    shape_ = shape;
    return *this;
  }
}

#endif
