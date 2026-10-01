FROM ubuntu:24.04 AS build

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    python3 \
    libpq-dev \
    libpng-dev \
    libqrencode-dev \
    libssl-dev \
    libasio-dev \
    zlib1g-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY . .

RUN cmake -S . -B build \
    -DCMAKE_BUILD_TYPE=Release

RUN cmake --build build -j$(nproc)


FROM ubuntu:24.04

RUN apt-get update && apt-get install -y \
    libpq5 \
    libpng16-16 \
    libqrencode4 \
    && rm -rf /var/lib/apt/lists/*

COPY --from=build /app/build/_deps/libharu-build/src/libhpdf.so.2.4 /usr/local/lib/

RUN ldconfig

WORKDIR /app

COPY --from=build /app/build/pizzaMasVentas .

EXPOSE 8082

CMD ["./pizzaMasVentas"]