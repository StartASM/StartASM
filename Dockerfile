FROM ubuntu:20.04
ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    clang \
    libomp-dev \
    llvm \
    zlib1g-dev \
    git \
    ninja-build \
    && apt-get clean

WORKDIR /StartASM
COPY . /StartASM

RUN cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
RUN cmake --build build

ENTRYPOINT ["./startasm"]