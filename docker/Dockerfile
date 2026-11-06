FROM skylyrac/blocksds:slim-v1.18.0

RUN apt-get update && apt-get install -y --no-install-recommends \
    curl \
    git \
    make \
    g++ \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*

RUN curl -sL https://raw.githubusercontent.com/Universal-Team/bmp2ds/master/bmp2ds.cpp -o /tmp/bmp2ds.cpp \
    && g++ -O2 /tmp/bmp2ds.cpp -o /usr/local/bin/bmp2ds \
    && rm /tmp/bmp2ds.cpp \
    && chmod +x /usr/local/bin/bmp2ds

WORKDIR /build

CMD ["/bin/bash", "-c", "make -j$(nproc) && (cp pkhex_dsi.nds /output/pkhex_dsi.nds 2>/dev/null || cp pkmn-chest.nds /output/pkhex_dsi.nds 2>/dev/null || true)"]
