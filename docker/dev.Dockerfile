# syntax=docker/dockerfile:1
# Interactive dev container: toolchain + editor/debug tooling, no source baked in
# (the workspace is bind-mounted at /workspace by the Dev Containers extension).
FROM ubuntu:24.04

RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    cmake \
    ninja-build \
    gdb \
    clang-format \
    clang-tidy \
    git \
    pre-commit \
    sudo \
    && rm -rf /var/lib/apt/lists/*

ARG DEV_USER=dev
RUN useradd --uid 1001 --create-home --shell /bin/bash ${DEV_USER} \
    && echo "${DEV_USER} ALL=(ALL) NOPASSWD:ALL" > /etc/sudoers.d/${DEV_USER} \
    && chmod 0440 /etc/sudoers.d/${DEV_USER}

USER 1001
WORKDIR /workspace
