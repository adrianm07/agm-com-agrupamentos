#ifndef GRAFO_TESTS_H
#define GRAFO_TESTS_H

class GrafoTests {
public:
  static void testAll();


private:
  static void testAdicionarVertice();
  static void testRemoverVertice();

  static void testAdicionarAresta();
  static void testAdicionarArestaOrientada();
  static void testRemoverAresta();
  static void testRemoverArestaOrientada();

  static void testVerificarExistenciaAresta();

  static void testAlterarPesoAresta();
  static void testAlterarPesoArestaOrientada();

  static void testImprimirGrafo();

  static void testGetGrau();

  static void testListarVizinhos();

  static void testVerificarAdjacencia();
  static void testVerificarAdjacenciaOrientada();

  static void testPrimAGM();


};

#endif