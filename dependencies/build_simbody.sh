git clone https://github.com/simbody/simbody.git
mkdir simbody_build
cd simbody_build
cmake ../simbody -DCMAKE_BUILD_TYPE=Release -DBUILD_DYNAMIC_LIBRARIES=OFF -DBUILD_STATIC_LIBRARIES=ON -DBUILD_TESTING=OFF -DBUILD_EXAMPLES=OFF \
        -DCMAKE_C_COMPILER:FILEPATH=/Users/nbianco/repos/llvm-project/build/bin/clang \
        -DCMAKE_CXX_COMPILER:FILEPATH=/Users/nbianco/repos/llvm-project/build/bin/clang++ \
        -DCMAKE_C_FLAGS:STRING="-flto" -DCMAKE_CXX_FLAGS:STRING="-flto" \
        -DCMAKE_INSTALL_PREFIX:PATH=/Users/nbianco/Repos/SimbodyEnzymeSandbox/dependencies/simbody_install
make -j8
make install
