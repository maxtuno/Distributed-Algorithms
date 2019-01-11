/*
Copyright (c) 2019 Oscar Riveros - www.peqnp.science.
All rights reserved.

Redistribution and use in source and binary forms are permitted
provided that the above copyright notice and this paragraph are
duplicated in all such forms and that any documentation,
advertising materials, and other materials related to such
distribution and use acknowledge that the software was developed
by the <organization>. The name of the
<organization> may not be used to endorse or promote products derived
from this software without specific prior written permission.
THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#include <iostream>
#include <random>

#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>

namespace mpi = boost::mpi;

typedef unsigned long long int natural;
typedef long double real;

template<class T, typename F>
class cluster {
public:
    cluster(const mpi::communicator &world, F functor) : __world(world), __functor(functor), __n(world.rank()), __m(world.size()) {}

    T operator()() {
        __n += __m;
        return __functor(__n - __m);
    }

private:
    const mpi::communicator &__world;
    const F __functor;
    T __n;
    T __m;
};

template<typename T>
T scale(const mpi::communicator &world, const T length) {
    return length / world.size() + world.size();
}

template<typename T, typename F>
std::vector<T> parallelize(const mpi::communicator &world, std::vector<T> &vs, F functor) {
    std::generate(vs.begin(), vs.end(), cluster<T, F>(world, functor));
    return vs;
}

template<typename T>
T accumulate(const mpi::communicator &world, std::vector<T> &vs, const T size) {
    real pi(0);
    mpi::all_reduce(world, 4 * std::accumulate(vs.begin(), vs.end(), real(0), std::plus<>()) / size, pi, std::plus<>());
    return pi;
}

int main() {
    mpi::environment env;
    mpi::communicator world;

    const natural size = 1000000;

    std::vector<real> vs(scale<natural>(world, size));

    std::random_device device;
    std::uniform_real_distribution<> distribution(0, 1);

    parallelize<real>(world, vs, [&](auto) {
        auto x = 2 * distribution(device) - 1;
        auto y = 2 * distribution(device) - 1;
        return std::pow(x, 2) + std::pow(y, 2) < 1;
    });

    auto pi = accumulate<real>(world, vs, size);

    if (!world.rank()) {
        std::cout << pi << std::endl;
    }

    return EXIT_SUCCESS;
}