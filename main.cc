#ifndef MAIN_CC
#define MAIN_CC

#include <iostream>
#include <initializer_list>
#include <cspear/cspear>

template <typename vec>
void p(const vec& v) {
  for (auto it = v.begin(); it < v.end(); ++it) {
    std::cout << *it << " ";
  }
  std::cout << std::endl;
}

int testaccum() {
  csp::array<double> a = csp::arange<double>(20).reshape_({4,5});
  std::cout << a << std::endl;
  std::cout << a.cumsum_() << std::endl;
  std::cout << a.cumsum(0) << std::endl;
  std::cout << a.cumsum(1) << std::endl;
  csp::savetxt(a, "test.txt");
  return 0;
}

int testslice() {
  csp::array<double> a = csp::arange<double>(24.0);
  a.reshape_({4,3,2});
  std::cout << a << std::endl;
  auto c = a.slice({csp::whole(), {0,3}, {0,3}});
  std::cout << c << std::endl;
  auto b = a.slice({{1,4,2},{0,3,2},1});
  std::cout << b << std::endl;
  std::cout << b[0] << std::endl;
  std::cout << b[1] << std::endl;
  std::cout << b[2] << std::endl;
  std::cout << b[3] << std::endl;
}

int test1() {
  // auto b = csp::array<double>({1,2,3});
  auto b = csp::arange(10000000.0);
  // auto b = csp::zeros({10000000});
  // a.reshape({2,5});
  // auto b = csp::random::randn<double>(a.shape());
  // a += 12;
  // a.rdiv_(2);
  // b.exp_();
  // // b = -a;
  //
  // p<std::vector<double> > ({1,2,3,4,5});
  auto c = -b;
  c.maximum_(-1);

  c /= (b + 1.0);
  c += b;
  // auto d = c / (b + 1);
  // d += b;

  // auto a = d;
  // a.filter(d > 1) += 10;

  for (int i = 0; i < 1; ++i) {
    std::cout << c[i] << " ";
  }
  std::cout << std::endl;

  // for (int i = 0; i < b.size(); ++i) {
  //   std::cout << b.data()[i] << " ";
  // }
  // std::cout << std::endl;
  //
  // std::cout << "max: " << a.max() << std::endl;
  // std::cout << "min: " << a.min() << std::endl;
  // std::cout << "sum: " << a.sum() << std::endl;

  return 0;
}

int test_bcast() {
  auto a = csp::arange(10.0);
  auto b = csp::arange(10.0);
  auto c = a.expand_dims_(0) + b.expand_dims_(1);

  std::cout << "Shapes:" << std::endl;
  std::cout << a.shape() << std::endl;
  std::cout << b.shape() << std::endl;
  std::cout << c.shape() << std::endl;
  std::cout << std::endl;

  std::cout << "Result:" << std::endl;
  std::cout << c << std::endl;

  auto d = c.sum(1);
  std::cout << d << std::endl;

  std::cout << "Interpolation:" << std::endl;
  a.ravel_();
  d.ravel_();
  auto e = csp::interp_sorted(a+0.5, a, d);
  std::cout << a+0.5 << std::endl;
  std::cout << a << std::endl;
  std::cout << d << std::endl;
  std::cout << "Res:" << std::endl;
  std::cout << e << std::endl;

  std::cout << "Histogram:" << std::endl;
  auto f = csp::hist(c, a * 2.1);
  std::cout << f << std::endl;
  return 0;
}

int main() {
  // return test1();
  return testaccum();
  // return testslice();
  // return test_bcast();
}

#endif
