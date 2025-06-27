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

struct DescArticulo {
    str30 descripcion;
    int posArticulo;
    bool estado;
};  // IndDescripArt.Txt

struct Rubro {
    short codRubro;
    str20 descripcion;
};  // Rubros.Txt

struct ItemCompra {
    str30 descripcion;
    short cantidad;
};  // ListaCompras.Txt

typedef Articulo tvrArticulos[MAX_ARTICULOS];
typedef DescArticulo tvrIndDescripArt[MAX_ARTICULOS];
typedef ItemCompra tvrListaCompras[MAX_LISTA_COMPRAS];

bool leerIndDescrip(ifstream &IndDescripArt, DescArticulo &rDescArticulo);
bool leerListaCompras(ifstream &ListaCompras, ItemCompra &rItemCompra);

void procIndDescrip(ifstream &IndDescripArt, tvrIndDescripArt vrIndDescripArt);
void ProcListaCompras(ifstream &ListaCompras, tvrListaCompras &vrListaCompras);

int main() {
    tvrIndDescripArt vrIndDescripArt;
    tvrListaCompras vrListaCompras;
    tvrArticulos vrArticulos;

    ifstream IndDescripArt("IndDescripArt.Txt");
    ifstream ListaCompras("ListaCompras.Txt");
    ifstream Articulos("Articulos.Txt");

    procIndDescrip(IndDescripArt, vrIndDescripArt);
    ProcListaCompras(ListaCompras, vrListaCompras);

    IndDescripArt.close();
    ListaCompras.close();
    return 0;
}

bool leerIndDescrip(ifstream &IndDescripArt, DescArticulo &rDescArticulo) {
    IndDescripArt.get(rDescArticulo.descripcion, 31);
    IndDescripArt >> rDescArticulo.posArticulo;
    IndDescripArt >> rDescArticulo.estado;
    IndDescripArt.ignore();
    return IndDescripArt.good();
}

void procIndDescrip(ifstream &IndDescripArt, tvrIndDescripArt vrIndDescripArt) {
    int cant = 0;
    DescArticulo rDescArticulo;

    while (leerIndDescrip(IndDescripArt, rDescArticulo) &&
           cant < MAX_ARTICULOS) {
        vrIndDescripArt[cant] = rDescArticulo;
        cant++;
    }
}

bool leerListaCompras(ifstream &ListaCompras, ItemCompra &rItemCompra) {
    ListaCompras.get(rItemCompra.descripcion, 31);
    ListaCompras >> rItemCompra.cantidad;
    ListaCompras.ignore();
    return ListaCompras.good();
}

void ProcListaCompras(ifstream &ListaCompras, tvrListaCompras &vrListaCompras) {
    int cant = 0;
    ItemCompra rItemCompra;

    while (leerListaCompras(ListaCompras, rItemCompra) &&
           cant < MAX_LISTA_COMPRAS) {
        vrListaCompras[cant] = rItemCompra;
        cant++;
    }
}