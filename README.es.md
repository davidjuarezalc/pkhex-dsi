# PKHeX DSi (nds-pkhex)

[![Licencia: GPL v3](https://img.shields.io/badge/Licencia-GPLv3-blue.svg)](LICENSE)
[![Plataforma](https://img.shields.io/badge/Plataforma-Nintendo%20DS%20%7C%20DSi%20%7C%203DS-red.svg)](https://github.com/DS-Homebrew/TWiLightMenu)
[![Compilador](https://img.shields.io/badge/Compilado%20con-BlocksDS-green.svg)](https://github.com/blocksds/sdk)

**PKHeX DSi** es un editor visual portátil y gestor de cajas de Pokémon en formato homebrew ejecutable `.nds`, diseñado específicamente para **Nintendo DSi** (con TWiLight Menu++) y tarjetas Flashcard para Nintendo DS / DS Lite (R4, DSTWO, Acekard).

> 🌐 **[Read this guide in English](README.md)**

---

## 💡 Acerca de este Proyecto

*PKHeX DSi* traslada la potencia de la edición de partidas de Pokémon directamente a la consola, eliminando la necesidad de extraer la tarjeta SD y usar un ordenador cada vez que quieras realizar ajustes en tus Pokémon o cajas.

* **Inspirado por:** [PKHeX](https://github.com/kwsch/PKHeX) de Kaphotics.
* **Construido sobre:** La base de código de [pkmn-chest](https://github.com/Universal-Team/pkmn-chest) y [PKSM-Core](https://github.com/Flagbrew/PKSM-Core) de Universal-Team y BernardoGiordano.
* **Licencia:** GNU General Public License v3.0 (GPLv3).

---

## 🎮 Juegos Compatibles

| Generación | Plataforma | Juegos | Formato de Partida |
| :--- | :--- | :--- | :--- |
| **Generación 3** | GBA | Rubí, Zafiro, Esmeralda, Rojo Fuego, Verde Hoja | Partidas `.sav` de 128 KB (mediante GBARunner2, GBARunner3 o Slot-2) |
| **Generación 4** | NDS | Diamante, Perla, Platino, HeartGold, SoulSilver | Partidas `.sav` de 512 KB |
| **Generación 5** | NDS | Blanco, Negro, Blanco 2, Negro 2 | Partidas `.sav` de 512 KB |

---

## ✨ Mejoras y Novedades respecto a la versión original

### 🛡️ 1. Doble Copia de Seguridad Automática y Escritura Atómica Segura
* **Historial Cronológico Permanente**: Cada vez que guardas cambios, la aplicación genera automáticamente copias de seguridad fechadas en dos ubicaciones independientes:
  1. Carpeta global: `sd:/_nds/pkmn-chest/backups/<juego>/YYYYMMDD-HHMMSS.sav.bak`
  2. Carpeta local junto a la partida: `<directorio_partida>/backups/<juego>_YYYYMMDD_HHMMSS.sav.bak`
* **Escritura Segura en Streaming (16 KB)**: Los datos se escriben en bloques pequeños sobre un archivo temporal antes de sustituir la partida original, protegiendo tus datos ante apagados accidentales o fallos de buffer de la tarjeta SD.
* **Verificación de Integridad del Equipo**: Ejecuta `fixParty()` para mantener el recuento y checksums del equipo en orden.

### 🧬 2. Selector Visual Completo de Habilidades
* Sustituye el selector básico anterior por una lista ordenada y categorizada:
  * `[1]` Primera habilidad natural.
  * `[2]` Segunda habilidad natural.
  * `[H]` Habilidad Oculta (*Dream World* / Gen 5).
  * Búsqueda alfabética completa de cualquier habilidad válida de la generación.

### 🎒 3. Nombres de Movimientos Dinámicos en MTs/MOs
* Los objetos de la bolsa muestran dinámicamente el movimiento que enseña cada MT según la versión del juego y el idioma seleccionado (ej. `MT01 Puño Certero` en vez de simplemente `MT01`).

### 🔍 4. Búsqueda Insensible a Tildes y Mayúsculas
* Normalización alfabética completa (`normalizeForSort`) para objetos, habilidades, movimientos y especies. Permite buscar términos como `puño`, `puno`, `epee` o `flamme` sin preocuparse por acentos ortográficos.

### 🚀 5. Optimización de Memoria RAM para ARM9 (Límite 4 MB de NDS)
* La Nintendo DS original cuenta con únicamente 4 MB de RAM (16 MB en DSi).
* Se sustituyeron asignaciones dinámicas `std::vector` en menús por estructuras estáticas `constexpr`, evitando la fragmentación del montículo (*heap*).
* Se eliminaron tablas de localización de Generaciones 6, 7 y 8 en el núcleo integrado, liberando valiosos kilobytes en memoria RAM.

### 🐛 6. Corrección de Errores Críticos de la Base Original
* **Corrección del orden por Velocidad**: El código original comparaba la Defensa Especial con la Velocidad al ordenar la caja.
* **Corrección táctil en Filtros y Estadísticas**: Solucionado el desfase de coordenadas táctiles donde se leía la posición del elemento seleccionado en lugar de la fila pulsada.
* **Entrada numérica segura**: Protección con `std::stoul` y control de límites contra excepciones al teclear cifras elevadas.
* **Protección contra índice negativo en Poder Oculto (*HPType*)**.
* **Alineación de memoria DMA en lectura del Slot 1**.
* **Eliminación de fugas de memoria en favoritos (`getline` y descriptores de archivo)**.

---

## 📂 Instalación en Consola (Nintendo DSi / 3DS / DS Lite)

1. Descarga el archivo `pkhex_dsi.nds` más reciente desde la sección de **[Releases](https://github.com/david/pkhex-dsi/releases)**.
2. Conecta la tarjeta SD de tu consola al ordenador.
3. Copia `pkhex_dsi.nds` en cualquier ruta de tu tarjeta SD (por ejemplo en `sd:/roms/nds/` o en la raíz `sd:/`).
4. *(Opcional pero recomendado para inicio rápido)*: Asegúrate de que existe la carpeta:
   ```text
   sd:/_nds/pkmn-chest/
   ```
5. Inserta la SD en la consola y arranca **TWiLight Menu++** (o el menú de tu flashcard).
6. Ejecuta `pkhex_dsi.nds`.
7. Navega por el explorador de archivos para seleccionar la partida `.sav` de GBA o NDS que desees editar.

---

## 🛠️ Cómo Compilar desde el Código Fuente

### Opción A: Compilación Automática con Docker (Recomendada)
Requiere tener [Docker](https://www.docker.com/) instalado y en ejecución.

```bash
# Clonar el repositorio
git clone https://github.com/david/pkhex-dsi.git
cd pkhex-dsi

# Ejecutar el script de compilación
./docker/build.sh
```
El binario resultante se guardará en `bin/pkhex_dsi.nds`.

### Opción B: Entorno local con BlocksDS SDK
Si dispones de la cadena de herramientas de [BlocksDS](https://github.com/blocksds/sdk) instalada en tu sistema:

```bash
make -j$(nproc)
```

---

## 📜 Créditos y Agradecimientos

* **[Universal-Team](https://github.com/Universal-Team)**: Desarrolladores de [pkmn-chest](https://github.com/Universal-Team/pkmn-chest), por la interfaz de usuario y arquitectura base en Nintendo DS.
* **[BernardoGiordano](https://github.com/BernardoGiordano)** y el equipo de **Flagbrew**: Creadores de [PKSM](https://github.com/BernardoGiordano/PKSM) y [PKSM-Core](https://github.com/Flagbrew/PKSM-Core).
* **[Kaphotics](https://github.com/kwsch)** y el equipo de PKHeX: Por ser el referente indiscutible de edición de partidas en la comunidad Pokémon.
* **[AntonioND](https://github.com/AntonioND)** y colaboradores de BlocksDS: Por el moderno SDK de desarrollo en C/C++ para Nintendo DS.

---

## 📄 Licencia

Este proyecto está publicado bajo la licencia **GNU General Public License v3.0 (GPLv3)**. Consulta el archivo [LICENSE](LICENSE) para más detalles.
