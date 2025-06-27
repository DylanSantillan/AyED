#include <fstream>
#include <iostream>
using namespace std;

const int MAX_ARTICULOS = 10000;
const int MAX_LISTA_COMPRAS = 100;

typedef char str30[31];
typedef char str20[21];
typedef char str10[11];
typedef unsigned short ushort;

struct Articulo {
    int codArt;
    short codRubro;
    str30 descripcion;
    ushort stockAct;
    float precioUnitario;
    str10 unidadMedida;
    short promociones[14];
};  // Articulos.Txt

struct descArticulo {
    str30 descripcion;
    int posArticulo;
    bool estado;
};  // IndDescripArt.Txt

struct Rubro {
    short codRubro;
    str20 descripcion[21];
};  // Rubros.Txt

struct ItemCompra {
    str30 descripcion[31];
    short cantidad;
};  // ListaCompras.Txt

typedef Articulo tvrArticulos[MAX_ARTICULOS];
typedef descArticulo tvrIndDescripArt[MAX_ARTICULOS];
typedef ItemCompra tvrListaCompras[MAX_LISTA_COMPRAS];

bool leerDescArticulo(ifstream &IndDescrip, descArticulo &rDescArticulo) {
    IndDescrip.get(rDescArticulo.descripcion, 31);
    IndDescrip >> rDescArticulo.posArticulo;
    IndDescrip >> rDescArticulo.estado;
    IndDescrip.ignore();
    return IndDescrip.good();
}

void procIndDescrip(ifstream &IndDescrip, tvrIndDescripArt vrIndDescripArt) {
    int cant = 0;
    descArticulo rDescArticulo;

    while (leerDescArticulo(IndDescrip, rDescArticulo) &&
           cant < MAX_ARTICULOS) {
        vrIndDescripArt[cant] = rDescArticulo;
        cout << vrIndDescripArt[cant].posArticulo << "\n";
        cant++;
    }
}

int main() {
    tvrArticulos vrArticulos;
    tvrIndDescripArt vrIndDescripArt;
    tvrListaCompras vrListaCompras;

    ifstream Articulos("Articulos.Txt");
    ifstream IndDescrip("IndDescripArt.Txt");
    ifstream Rubros("Rubros.Txt");
    ifstream Lista("ListaCompras.Txt");

    procIndDescrip(IndDescrip, vrIndDescripArt);
    IndDescrip.close();

    return 0;
}
