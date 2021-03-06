#ifndef CSPEAR_VIEWS_SLICE_VIEW_H
#define CSPEAR_VIEWS_SLICE_VIEW_H

#include <vector>
#include <type_traits>
#include <cspear/views/slice.h>
#include <cspear/lib/idx.h>
#include <cspear/tools/assert.h>

namespace csp {
  template <typename I>
  class SliceView {
    std::vector< Slice<I> > slices_ = {};
    std::vector<I> data_shape_ = {}; // the original data shape
    std::vector<I> shape_ = {}; // the view's shape
    I sz_ = 0;

  public:
    SliceView() {}
    template <template<typename> typename Vec>
    SliceView(const Vec< Slice<I> >& slices,
              const std::vector<I>& datashape);

    void reshape(const std::vector<I>& shape);
    void reshape(std::initializer_list<I> shape);

    I size() const { return sz_; }
    const std::vector<I>& shape() const { return shape_; }
    I idx(I i) const;

    const std::vector< Slice<I> >& slices() const { return slices_; }
    const std::vector<I>& data_shape() const { return data_shape_; }
  };

  template <typename I>
  template <template<typename> typename Vec>
  SliceView<I>::SliceView(const Vec< Slice<I> >& slices,
                       const std::vector<I>& datashape) {
    slices_ = slices;
    data_shape_ = datashape;

    // get the view's shape and size
    shape_.resize(slices_.size());
    sz_ = 1;
    for (I i = 0; i < slices_.size(); ++i) {
      slices_[i].normalize(datashape[i]);
      I nelmt = slices_[i].nelmt;
      shape_[i] = nelmt;
      sz_ *= nelmt;
    }
  }

  template <typename I>
  void SliceView<I>::reshape(const std::vector<I>& shape) {
    _cspear_assert(false, "The slice view cannot be reshaped. "
      "Please use .copy() to make the contiguous view");
  }
  template <typename I>
  void SliceView<I>::reshape(std::initializer_list<I> shape) {
    _cspear_assert(false, "The slice view cannot be reshaped. "
      "Please use .copy() to make the contiguous view");
  }

  template <typename I>
  I SliceView<I>::idx(I i) const {
    std::vector<I> multi_idx = unravel_index(i, shape_);
    std::vector<I> multi_idx_data(multi_idx.size());
    std::vector<I> idx_begin(multi_idx.size());
    for (I i = 0; i < multi_idx.size(); ++i) {
      auto& s = slices_[i];
      multi_idx_data[i] = multi_idx[i] * s.step + s.begin;
      idx_begin[i] = s.begin;
    }
    return ravel_index(multi_idx_data, data_shape_) -
           ravel_index(idx_begin, data_shape_);
  }
}

#endif
