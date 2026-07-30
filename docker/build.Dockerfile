# syntax=docker/dockerfile:1
FROM ubuntu:24.04

RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    cmake \
    ninja-build \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /workspace

COPY . .

ENV BUILD_TESTS=ON \
    BUILD_EXAMPLES=ON

RUN cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release \
    && cmake --build build

CMD ["ctest", "--test-dir", "build", "--output-on-failure"]
