:: This script downloads v4.3.4 libzmq and builds it

set ROOT=%~dp0

call "C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvars64.bat"

cd "%ROOT%"
mkdir workdir && cd workdir

git clone --branch v4.3.4 --depth 1 git@github.com:zeromq/libzmq.git

cmake libzmq -G "Visual Studio 17 2022" ^
   -DCMAKE_INSTALL_PREFIX=.. ^
   -DCMAKE_MFC_FLAG=1 ^
   -DBUILD_SHARED=ON ^
   -DBUILD_STATIC=OFF ^
   -DWITH_LIBSODIUM=OFF ^
   -DWITH_LIBSODIUM_STATIC=OFF ^
   -DENABLE_CURVE=OFF ^
   -DZMQ_BUILD_TESTS=OFF

devenv.com ZeroMQ.sln /Rebuild "Debug|x64" /Project INSTALL
devenv.com ZeroMQ.sln /Rebuild "Release|x64" /Project INSTALL

:: Clean up
cd "%ROOT%"
rm -rf CMake share workdir

:: Get the c++ bindings
git clone --branch v4.8.1 --depth 1 git@github.com:zeromq/cppzmq.git
cp cppzmq/zmq.hpp include
rm -rf cppzmq
