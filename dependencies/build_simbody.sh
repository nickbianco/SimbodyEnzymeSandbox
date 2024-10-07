CMAKE_C_COMPILER=/home/nbianco/repos/llvm-project/build/bin/clang
CMAKE_CXX_COMPILER=/home/nbianco/repos/llvm-project/build/bin/clang++
CMAKE_INSTALL_PREFIX=/home/nbianco/repos/SimbodyEnzymeSandbox/dependencies/simbody_install

git clone https://github.com/simbody/simbody.git
mkdir simbody_build
cd simbody_build
cmake ../simbody -DCMAKE_BUILD_TYPE=Release -DBUILD_DYNAMIC_LIBRARIES=OFF -DBUILD_STATIC_LIBRARIES=ON -DBUILD_TESTING=OFF -DBUILD_EXAMPLES=OFF \
        -DCMAKE_C_COMPILER:FILEPATH=$CMAKE_C_COMPILER -DCMAKE_CXX_COMPILER:FILEPATH=$CMAKE_CXX_COMPILER \
        -DCMAKE_C_FLAGS:STRING="-flto" -DCMAKE_CXX_FLAGS:STRING="-flto" \
        -DCMAKE_INSTALL_PREFIX:PATH=$CMAKE_INSTALL_PREFIX
make -j8
make install
