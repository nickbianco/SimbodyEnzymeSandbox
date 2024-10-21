FROM mcr.microsoft.com/devcontainers/cpp:1-ubuntu-22.04

RUN apt-get update && \
    apt-get install -y \
    build-essential libtool autoconf \
    pkg-config \
    wget \
    gcc \
    g++ \
    cmake \
    ninja-build \
    python3 \
    git \
    curl \
    gfortran \
    libopenblas-dev \
    liblapack-dev 

WORKDIR /opt
RUN git clone https://github.com/llvm/llvm-project.git

WORKDIR /opt/llvm-project
RUN git fetch --tags && git checkout tags/llvmorg-18.1.8 -b llvmorg-18.1.8

WORKDIR /opt/llvm-project/build
RUN cmake -G Ninja ../llvm \
    -DLLVM_TARGETS_TO_BUILD="host" \
    -DLLVM_ENABLE_PROJECTS="clang;lld" \
    -DLLVM_ENABLE_PLUGINS=ON \
    -DCMAKE_BUILD_TYPE=Release \
    -DLLVM_ENABLE_ASSERTIONS=ON \
    && ninja -j8

# Symlink ld to LLVM's ll.lld
RUN cp /usr/bin/ld /usr/bin/ld.bak
RUN ln -sf /opt/llvm-project/build/bin/ld.lld /usr/bin/ld

# Mark dependencies folders as safe
RUN git config --global --add safe.directory SimbodyEnzymeSandbox/dependencies/Enzyme
RUN git config --global --add safe.directory SimbodyEnzymeSandbox/dependencies/simbody

