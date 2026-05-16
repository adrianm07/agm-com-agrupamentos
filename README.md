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
├── CMakeLists.txt       # Configuração de build
├── include/
│   ├── Aresta.h         # Declaração da classe Aresta
│   ├── Vertice.h        # Declaração da classe Vertice
│   └── Grafo.h          # Declaração da classe Grafo
└── src/
    ├── Aresta.cpp        # Implementação de Aresta
    ├── Vertice.cpp       # Implementação de Vertice
    ├── Grafo.cpp         # Implementação de Grafo
    └── main.cpp          # Ponto de entrada
```

---

## Leitura de grafo via arquivo

O método `lerArquivo` aceita arquivos de texto no seguinte formato:

```
# Grafo não ponderado — cada linha: origem destino
1 2
1 3
2 4

# Grafo ponderado — cada linha: origem destino peso
1 2 5.0
1 3 2.5
2 4 1.0

# Vértice isolado — apenas um número na linha
7
```

Exemplo de uso no código:

```cpp
Grafo g(false, false); // não orientado, não ponderado
g.lerArquivo("grafo.txt");
g.imprimirGrafo();
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
