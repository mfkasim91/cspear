#ifndef CSPEAR_LIB_HIST_H
#define CSPEAR_LIB_HIST_H

#include <algorithm>
#include <cspear/array/array.h>
#include <cspear/tools/assert.h>
#include <cspear/tools/types.h>

namespace csp {
  template <typename R=tools::Int, typename InpType1, typename InpType2>
  csp::array<R> hist_sorted(const InpType1& data, const InpType2& xbins) {
    using T1 = typename InpType1::DataType;
    using I1 = typename InpType1::IndexType;
    using View1 = typename InpType1::ViewType;
    using T2 = typename InpType2::DataType;
    using I2 = typename InpType2::IndexType;
    using View2 = typename InpType2::ViewType;
    using ResType = typename csp::array<R>;
    using TR = typename ResType::DataType;
    using IR = typename ResType::IndexType;
    using ViewR = typename ResType::ViewType;

    // xbins must be a 1D array
    _cspear_assert(xbins.ndim() == 1,
        "The second argument of hist must be a 1D array.");

    ResType res = ResType::zeros({xbins.size()-1});

    // data is already sorted

    // pointer for xbins
    auto ibin = EWiseIterator<T2,I2,View2>((T2*)xbins.data(), xbins.view());
    auto ibinr = EWiseIterator<T2,I2,View2>((T2*)xbins.data(), xbins.view());
    ++ibinr;
    // result pointer
    auto ir = EWiseIterator<TR,IR,ViewR>((TR*)res.data(), res.view());
    // the data pointer
    auto ix = EWiseIterator<T1,I1,View1>((T1*)data.data(), data.view());
    for (; ix; ++ix) {
      auto& xi = *ix;
      // move the p pointer until xi < xbins[ibin+1]
      while (xi >= *ibinr) {
        ++ibin;
        ++ibinr;
        ++ir;
        if (!ibinr) break;
      }

      // break if it's already out of the bins
      if (!ibinr) break;

      // get the interpolated elements
      auto& xbinl = *ibin;
      auto& xbinr = *ibinr;

      // increment the histogram
      if ((xi < xbinr) && (xbinl <= xi)) {
        *ir += (TR)1;
      }
    }
    return res;
  }

  template <typename R=tools::Int, typename InpType1, typename InpType2>
  csp::array<R> hist(const InpType1& data, const InpType2& xbins) {
    using T1 = typename InpType1::DataType;
    using I1 = typename InpType1::IndexType;
    using View1 = typename InpType1::ViewType;

    // sort the data
    array<T1,I1> cdata = data;
    std::sort(cdata.data(), cdata.data()+cdata.size());

    return hist_sorted<R>(cdata, xbins);
  }
}

#endif
