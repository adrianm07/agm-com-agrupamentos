# Grafo — Biblioteca de Grafos em C++

Implementação de uma estrutura de grafos orientados/não orientados e ponderados/não ponderados em C++17, com suporte a leitura de grafos via arquivo.

## Funcionalidades
- **Manipulação de Vértices e Arestas**: Adicionar e remover vértices e arestas.
- **Consultas**: Verificar adjacência, listar vizinhos, listar arestas e obter o grau do vértice.
- **Tipos de Grafo**: Suporta grafos orientados e não orientados.
- **Pesos**: Suporte a arestas com ou sem pesos (possibilidade de alterar os pesos).
- **Algoritmos**: Árvore Geradora Mínima (Algoritmo de Prim - *Prim AGM*).
- **Entrada de Arquivo**: Leitura de configurações do grafo a partir de arquivos `.txt`.
- **Interface e Testes**: Menu interativo integrado, e testes pré-configurados (`GrafoTests`).

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
├── data/
│   └── arquivoExemplo.txt # Exemplo de arquivo de entrada para leitura do grafo
├── include/
│   ├── Aresta.h          # Declaração da classe Aresta
│   ├── Grafo.h           # Declaração da classe Grafo
│   ├── GrafoTests.h      # Declaração da classe GrafoTests
│   ├── Menu.h            # Declaração da classe Menu
│   └── Vertice.h         # Declaração da classe Vertice
└── src/
    ├── Aresta.cpp        # Implementação de Aresta
    ├── Grafo.cpp         # Implementação de Grafo
    ├── GrafoTests.cpp    # Implementação de GrafoTests
    ├── main.cpp          # Ponto de entrada
    ├── Menu.cpp          # Implementação de Menu
    └── Vertice.cpp       # Implementação de Vertice
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
