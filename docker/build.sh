#!/usr/bin/env bash
set -e

DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
echo "=== Construyendo PKHeX DSi (.nds) con Docker + BlocksDS ==="

if ! docker info >/dev/null 2>&1; then
    echo "ERROR: El demonio de Docker no está en ejecución."
    echo "Por favor abre Docker Desktop y vuelve a ejecutar este script."
    exit 1
fi

mkdir -p "$DIR/bin"

echo "-> Compilando imagen de compilación BlocksDS..."
docker build -t pkhex-dsi-builder "$DIR/docker"

echo "-> Compilando binario ROM .nds..."
docker run --rm \
    -v "$DIR:/build" \
    -v "$DIR/bin:/output" \
    pkhex-dsi-builder

echo "=== Compilación completada con éxito. Archivo generado en bin/pkhex_dsi.nds ==="
