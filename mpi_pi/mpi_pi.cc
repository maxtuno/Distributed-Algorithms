/*
BSD 2-Clause License

Copyright (c) 2019, Oscar Riveros - www.peqnp.science.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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