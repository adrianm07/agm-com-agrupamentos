import sys
import os
import random
import networkx as nx

def gerar_grafo_planar_conectado(num_vertices, num_arestas):
    if num_arestas < num_vertices - 1:
        raise ValueError("Número de arestas (m) é insuficiente para conectar todos os vértices (n).")
    
    if num_vertices >= 3:
        max_arestas_planares = 3 * num_vertices - 6
        if num_arestas > max_arestas_planares:
            raise ValueError(f"Para um grafo ser planar com {num_vertices} vértices, o número máximo de arestas é {max_arestas_planares}.")
    
    G = nx.Graph()
    G.add_nodes_from(range(num_vertices))
    
    vertices_conectados = [0]
    vertices_restantes = list(range(1, num_vertices))
    random.shuffle(vertices_restantes)
    
    arestas_com_peso = []
    
    for u in vertices_restantes:
        v = random.choice(vertices_conectados)
        peso = random.randint(1, 50)
        G.add_edge(u, v)
        arestas_com_peso.append((u, v, peso))
        vertices_conectados.append(u)
        
    tentativas = 0
    max_tentativas = num_arestas * 50
    
    while G.number_of_edges() < num_arestas and tentativas < max_tentativas:
        tentativas += 1
        u = random.randint(0, num_vertices - 1)
        v = random.randint(0, num_vertices - 1)
        
        if u != v and not G.has_edge(u, v):
            G.add_edge(u, v)
            is_planar, _ = nx.check_planarity(G)
            if is_planar:
                peso = random.randint(1, 50)
                arestas_com_peso.append((u, v, peso))
            else:
                G.remove_edge(u, v)
                
    if G.number_of_edges() < num_arestas:
        raise RuntimeError("Não foi possível gerar um grafo planar com essa densidade de arestas. Tente diminuir 'm'.")
                
    return arestas_com_peso

def atribuir_grupos(num_vertices, num_grupos):
    nos_grupos = {i: set() for i in range(num_vertices)}
    
    for no in range(num_vertices):
        grupo_obrigatorio = random.randint(0, num_grupos - 1)
        nos_grupos[no].add(grupo_obrigatorio)
        
    grupos_atribuidos = set().union(*nos_grupos.values())
    grupos_faltantes = [g for g in range(num_grupos) if g not in grupos_atribuidos]
    
    if grupos_faltantes:
        nos_aleatorios = list(range(num_vertices))
        random.shuffle(nos_aleatorios)
        for idx, grupo in enumerate(grupos_faltantes):
            no = nos_aleatorios[idx % num_vertices]
            nos_grupos[no].add(grupo)
            
    for no in range(num_vertices):
        qtd_grupos_extras = random.randint(0, min(2, num_grupos - 1)) 
        for _ in range(qtd_grupos_extras):
            grupo_aleatorio = random.randint(0, num_grupos - 1)
            nos_grupos[no].add(grupo_aleatorio)
            
    return nos_grupos

def obter_proximo_nome_arquivo(diretorio):
    """
    Varre o diretório alvo e descobre qual é o próximo número sequencial disponível
    para evitar a sobrescrita (ex: instancia_1.txt, instancia_2.txt...)
    """
    # Garante que a pasta exista. Se não existir, o Python cria ela agora.
    if not os.path.exists(diretorio):
        os.makedirs(diretorio)
        
    contador = 1
    while True:
        nome_arquivo = f"instancia_{contador}.txt"
        caminho_completo = os.path.join(diretorio, nome_arquivo)
        # Se o arquivo não existir, achamos o nome ideal!
        if not os.path.exists(caminho_completo):
            return caminho_completo
        contador += 1

def main():
    if len(sys.argv) != 5:
        print("Uso: python gerador_de_instancia.py <n_vertices> <m_arestas> <k_grupos> <pasta_de_saida>")
        sys.exit(1)
        
    try:
        n = int(sys.argv[1]) 
        m = int(sys.argv[2]) 
        k = int(sys.argv[3]) 
        pasta_saida = sys.argv[4]
    except ValueError:
        print("Os parâmetros n, m e k devem ser inteiros.")
        sys.exit(1)

    try:
        arestas = gerar_grafo_planar_conectado(n, m)
        grupos_por_no = atribuir_grupos(n, k)
        
        # Define o caminho do arquivo de forma sequencial automática
        caminho_arquivo = obter_proximo_nome_arquivo(pasta_saida)
        
        with open(caminho_arquivo, 'w') as f:
            # Cabeçalho
            f.write(f"{n} {m} {k}\n")
            
            # Vértices e seus grupos
            for no in range(n):
                grupos_str = " ".join(map(str, sorted(list(grupos_por_no[no]))))
                f.write(f"{no} {grupos_str}\n")
                
            # Flag
            f.write("ARESTAS\n")
            
            # Arestas e pesos
            for u, v, peso in arestas:
                f.write(f"{u} {v} {peso}\n")
                
        # Pega apenas o nome do arquivo para exibir no print formatado
        nome_final = os.path.basename(caminho_arquivo)
        print(f"Sucesso! Gerado o arquivo '{nome_final}' dentro de '{pasta_saida}'.")
        
    except Exception as e:
        print(f"Erro ao gerar instância: {e}")

if __name__ == "__main__":
    main()