FROM ubuntu:18.04

RUN apt update && apt install -y \
    build-essential \
    cmake \
    libboost-dev \
    libboost-filesystem-dev \
    libboost-system-dev \
    libboost-thread-dev \
    libeigen3-dev \
    libgmp-dev \
    libmpfr-dev \
    libqt5svg5-dev \
    qt5-default \
    wget \
    zlib1g-dev

RUN wget --no-check-certificate https://github.com/CGAL/cgal/releases/download/v5.1/CGAL-5.1.tar.xz && \
    tar xf CGAL-5.1.tar.xz && \
    cd CGAL-5.1 && mkdir build && cd build && \
    cmake -DCGAL_HEADER_ONLY=OFF -DCMAKE_BUILD_TYPE=Release .. && \
    make && \
    make install
