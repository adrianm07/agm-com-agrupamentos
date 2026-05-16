#ifndef ARESTA_H
#define ARESTA_H

class Vertice; // forward declaration — evita dependência circular

class Aresta {
public:
    Aresta(Vertice *v, double peso);
    Vertice *getDestino();
    double getPeso();
    void setDestino(Vertice *v);
    void setPeso(double peso);
    ~Aresta();

private:
    Vertice *destino;
    double peso;
};

#endif