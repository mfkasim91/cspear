#ifndef CSPEAR_CORE_ARRAY_H
#define CSPEAR_CORE_ARRAY_H

#include <cstring>
#include <vector>
#include <stdexcept>
#include <initializer_list>
#include <cspear/core/utils.h>
#include <cspear/tools/assert.h>
#include <cspear/tools/misc.h>
#include <cspear/tools/types.h>

namespace csp {
  template <typename T, typename I=tools::Int>
  class array {
    T* data_;
    I sz_ = 0; // total number of elements
    I prev_allocated_size_ = 0;
    std::vector<I> shape_;
    bool allocated_ = false; // flag to indicate if the data is allocated by us

    public:
    // constructors and destructor
    array();
    array(std::initializer_list<T> elmts);
    array(const T* a, I sz);
    array(const T* a, std::initializer_list<I> shape);
    array(const array<T,I>& a);
    ~array();

    // indexing
    inline T& operator[](I idx) { return data_[idx]; }

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
    template <typename T2> array<T2,I> astype() const; // ???

    // reshape and resize
    array<T,I>& resize(I sz);
    array<T,I>& reshape(std::initializer_list<I> shape);
    array<T,I>& reshape(const std::vector<I>& shape);
    array<T,I>& squeeze();
    array<T,I>& expand_dims(I idx);
    array<T,I>& ravel();

    // unary (arithmetic) operations
    array<T,I> operator-();

    // binary (arithmetic) operations
    /**begin repeatop
    opname = ["operator+", "operator-", "operator*", "operator/"];
    */
    /**begin repeattype
    tpename = ["const array<T,I>&", "T"];
    */
    array<T,I> @opname@(@tpename@ a);
    /**end repeattype**/
    /**end repeatop**/
    array<T,I> rdiv(T a);

    // binary (arithmetic) operations (inplace)
    /**begin repeatop
    opname = ["operator+=", "operator-=", "operator*=", "operator/="];
    */
    /**begin repeattype
    tpename = ["const array<T,I>&", "T"];
    */
    array<T,I>& @opname@(@tpename@ a);
    /**end repeattype**/
    /**end repeatop**/
    array<T,I>& rdiv_(T a);

    // comparison operations
    /**begin repeatop
    cmpopname = ["operator>", "operator<", "operator>=", "operator<=",
                  "operator==", "operator!="];
    */
    /**begin repeattype
    tpename = ["const array<T,I>&", "T"];
    */
    array<bool,I> @cmpopname@(@tpename@);
    /**end repeattype**/
    /**end repeatop**/

    // logical operations (only for array<bool,I>)

    // binary logical operations
    /**begin repeatop
    boolopname = ["operator&&", "operator||"];
    */
    /**begin repeattype
    tpename = ["const array<T,I>&", "T"];
    */
    array<bool,I> @boolopname@(@tpename@);
    /**end repeattype**/
    /**end repeatop**/

    // unary logical operations
    /**begin repeatop
    boolopname = ["operator!"];
    */
    array<bool,I> @boolopname@();
    /**end repeatop**/

    // reduction operator
    /**begin repeat
    reducefn = ["sum", "max", "min", "mean", "std"];
    */
    T @reducefn@();
    array<T,I> @reducefn@(I axis);
    array<T,I> @reducefn@(std::initializer_list<I> axes);
    /**end repeat**/

    // math functions
    /**begin repeat
    mathfn = ["sin", "cos", "tan", "asin", "acos", "atan", "atan2",
              "sinh", "cosh", "tanh", "asinh", "acosh", "atanh",
              "exp", "log", "log10", "exp2", "expm1", "log1p", "log2",
              "sqrt", "cbrt", "sqr", "cube",
              "erf", "erfc", "tgamma", "lgamma",
              "ceil", "floor", "round", "fabs"];
    */
    array<T,I> @mathfn@() const;
    array<T,I>& @mathfn@_(); // in-place
    /**end repeat**/

    // misc functions
    array<T,I> clip_lb(T lb) const;
    array<T,I> clip_ub(T ub) const;
    array<T,I> clip(T lb, T ub) const;
    array<T,I>& clip_lb_(T lb);
    array<T,I>& clip_ub_(T ub);
    array<T,I>& clip_(T lb, T ub);

    private:
    void _realloc();
    void _copy(const array<T,I>& a);
  };

  // constructors and destructor
  template <typename T, typename I>
  array<T,I>::array() {}

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
  array<T,I>::array(const T* a, I sz) {
    // copy the data
    sz_ = sz;
    _realloc();
    std::copy(a, a+sz, data_);

    // set the shape
    shape_ = {sz_};
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
    // copy the array to self
    _copy(a);
  }

  template <typename T, typename I>
  array<T,I>::~array() {
    if (allocated_) {
      std::free(data_);
    }
  }

  // assignment operator and copy
  template <typename T, typename I>
  array<T,I>& array<T,I>::operator=(const array<T,I>& a) {
    _copy(a);
    return *this;
  }

  template <typename T, typename I>
  array<T,I> array<T,I>::copy() const {
    array<T,I> res(*this);
    return res;
  }

  // resize and reshaped
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
    I sz = tools::_prod_init_list(shape);
    _cspear_assert(sz == sz_, "The size must stay the same");
    shape_ = shape;
    return *this;
  }

  template <typename T, typename I>
  array<T,I>& array<T,I>::reshape(const std::vector<I>& shape) {
    I sz = tools::_prod_init_list(shape);
    _cspear_assert(sz == sz_, "The size must stay the same");
    shape_ = shape;
    return *this;
  }

  template <typename T, typename I>
  array<T,I>& array<T,I>::squeeze() {
    I nd = ndim();
    if (nd <= 1) {
      return *this;
    }

    // get the new shape
    std::vector<I> newshape;
    newshape.reserve(nd);
    for (I i = 0; i < nd; ++i) {
      if (shape_[i] == 1) continue;
      newshape.push_back(shape_[i]);
    }
    shape_ = newshape;

    return *this;
  }

  template <typename T, typename I>
  array<T,I>& array<T,I>::expand_dims(I idx) {
    _cspear_assert((idx >= 0) && (idx <= ndim()),
                   "Expanded index must be between [0,ndim()]");
    shape_.insert(idx, 1);
    return *this;
  }

  template <typename T, typename I>
  array<T,I>& array<T,I>::ravel() {
    shape_ = {sz_};
    return *this;
  }

  // private functions
  template <typename T, typename I>
  void array<T,I>::_realloc() {
    if (sz_ == prev_allocated_size_) {
      return;
    }

    if (allocated_) std::free(data_);
    data_ = (T*) std::malloc(sz_*sizeof(*data_));
    tools::_assert_cpu(data_, "CPU memory allocation failed.");
    allocated_ = true;
    prev_allocated_size_ = sz_;
  }

  template <typename T, typename I>
  void array<T,I>::_copy(const array<T,I>& a) {
    // copy the data
    sz_ = a.size();
    _realloc();
    std::copy(a.data(), a.data()+sz_, data_);

    // copy the shape as well
    shape_ = a.shape();
  }
}

#endif
