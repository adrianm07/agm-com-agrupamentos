# Grafo — Biblioteca de Grafos em C++

Implementação de uma estrutura de grafos orientados/não orientados e ponderados/não ponderados em C++17, com suporte a leitura de grafos via arquivo.

## Pré-requisitos

### Windows

| Ferramenta | Download |
|---|---|
| CMake ≥ 3.10 | https://cmake.org/download/ |
| MinGW-w64 (g++) | https://www.mingw-w64.org/downloads/ |

> Certifique-se de que `cmake` e `g++` estão no `PATH` do sistema.  
> Você pode verificar abrindo o PowerShell e digitando `cmake --version` e `g++ --version`.

### Linux (Debian/Ubuntu)

```bash
sudo apt update
sudo apt install cmake g++ build-essential
```

---

## Como compilar e executar

### Windows (PowerShell)

```powershell
# 1. Clone o repositório (ou entre na pasta do projeto)
cd caminho\para\grafos

# 2. Configure o projeto (somente na primeira vez ou ao mudar CMakeLists.txt)
cmake -S . -B build -G "MinGW Makefiles"

# 3. Compile
cmake --build build

# 4. Execute
.\build\bin\grafo.exe
```

### Linux (Terminal)

```bash
# 1. Clone o repositório (ou entre na pasta do projeto)
cd caminho/para/grafos

# 2. Configure o projeto (somente na primeira vez ou ao mudar CMakeLists.txt)
cmake -S . -B build

# 3. Compile
cmake --build build

# 4. Execute
./build/bin/grafo
```

---

## Estrutura do projeto

```
grafos/
├── CMakeLists.txt        # Configuração de build
├── include/
│   ├── Aresta.h          # Declaração da classe Aresta
│   ├── Vertice.h         # Declaração da classe Vertice
│   ├── Grafo.h           # Declaração da classe Grafo
│   └── GrafoTests.h      # Declaração da classe GrafoTests
└── src/
    ├── Aresta.cpp        # Implementação de Aresta
    ├── Vertice.cpp       # Implementação de Vertice
    ├── Grafo.cpp         # Implementação de Grafo
    ├── GrafoTests.cpp    # Implementação de GrafoTests
    └── main.cpp          # Ponto de entrada
```
---

## Recompilar após mudanças no código

Não é necessário rodar o `cmake -S . -B build` novamente — apenas recompile:

```bash
# Windows
cmake --build build

# Linux
cmake --build build
```

Para limpar a build e recomeçar do zero:

```bash
# Windows
Remove-Item -Recurse -Force build
cmake -S . -B build -G "MinGW Makefiles"
cmake --build build

# Linux
rm -rf build
cmake -S . -B build
cmake --build build
```
