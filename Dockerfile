# Use Ubuntu 22.04 LTS (Jammy) for broad GLIBC backwards-compatibility (GLIBC 2.35)
FROM ubuntu:22.04

# Avoid tzdata interactive prompt during apt-get install
ENV DEBIAN_FRONTEND=noninteractive

# Update system and install essential build tools and library dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    wget \
    gnupg2 \
    software-properties-common \
    tar \
    pkg-config \
    ninja-build \
    libjpeg-dev \
    libpng-dev \
    libwebp-dev \
    libtiff-dev \
    libasound2-dev \
    libpulse-dev \
    libx11-dev \
    libxext-dev \
    libxrandr-dev \
    libxcursor-dev \
    libxfixes-dev \
    libxi-dev \
    libxss-dev \
    libxtst-dev \
    libxkbcommon-dev \
    libdrm-dev \
    libgbm-dev \
    libgl1-mesa-dev \
    libgles2-mesa-dev \
    libegl1-mesa-dev \
    libdbus-1-dev \
    libibus-1.0-dev \
    libudev-dev \
    libwayland-dev \
    libdecor-0-dev \
    libpipewire-0.3-dev \
    && rm -rf /var/lib/apt/lists/*

# Download and build SDL3 from source
RUN git clone --depth 1 https://github.com/libsdl-org/SDL.git /tmp/SDL && \
    cd /tmp/SDL && \
    mkdir build && cd build && \
    cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local .. && \
    make -j$(nproc) && make install && \
    rm -rf /tmp/SDL

# Download and build SDL3_image from source
RUN git clone --depth 1 https://github.com/libsdl-org/SDL_image.git /tmp/SDL_image && \
    cd /tmp/SDL_image && \
    mkdir build && cd build && \
    cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local .. && \
    make -j$(nproc) && make install && \
    rm -rf /tmp/SDL_image

# Download and build SDL3_shadercross from source
RUN git clone --depth 1 https://github.com/libsdl-org/SDL_shadercross.git /tmp/SDL_shadercross && \
    cd /tmp/SDL_shadercross && \
    mkdir build && cd build && \
    cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local -DSDLSHADERCROSS_STATIC=OFF -DSDLSHADERCROSS_VENDORED=ON .. && \
    make -j$(nproc) && make install && \
    rm -rf /tmp/SDL_shadercross

# Refresh dynamic linker
RUN ldconfig
