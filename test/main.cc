#ifndef CSPEAR_TEST_MAIN_CC
#define CSPEAR_TEST_MAIN_CC

#include "gtest/gtest.h"
#include "test_params.h"
#include "test-array/test-init.h"
#include "test-array/test-nil.h"
#include "test-array/test-reshape.h"
#include "test-array/test-reduce.h"
#include "test-array/test-accum.h"
#include "test-array/test-op.h"
#include "test-array/test-bcast.h"
#include "test-view/test-filter-view.h"
#include "test-view/test-slice-view.h"
#include "test-view/test-contiguous-view.h"
#include "test-view/test-index-view.h"
#include "test-lib/test-hist.h"
#include "test-lib/test-interp.h"
#include "test-lib/test-grad.h"
#include "test-lib/test-array.h"
#include "test-lib/test-math.h"
#include "test-simd/test-avx.h"
#include "test-random/test-shuffle.h"
#include "test-cases/test-copy.h"
#include "test-cases/test-refs.h"

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

#endif
