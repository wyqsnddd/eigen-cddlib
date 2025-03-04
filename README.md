# eigen-cddlib

Eigen wrapper for Komei Fukuda's [cdd](https://www.inf.ethz.ch/personal/fukudak/cdd_home/) library.
For a cmake install, you can use the forked [repo](https://github.com/vsamy/cddlib)

## Installing

First install ``cddlib`` whether by installing [this (prefer way)](https://github.com/vsamy/cddlib) or with:
```sh
sudo apt-get install libcdd-dev
```
Then, install ``eigen-cddlib`` following the standard CMake procedure:

```sh
git clone --recursive https://github.com/wyqsnddd/eigen-cddlib
cd eigen-cddlib
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFX=<your_path>
make -j4
make install
```

If cdd has not been installed in the default path, CMake may not find it. In this case, add the path as a HINTS in ``find_package``.

### Testing

```sh
cd eigen-cddlib/build
make test
```

## Examples

There is no basic examples yet. Please see test files for an overview, and doxygen files for API documentation.
