#include <fstream>
#include <iomanip>
#include <iostream>

using namespace std;

typedef char str30[31];
typedef char str20[21];
typedef char str10[11];
typedef unsigned short ushort;

const ushort MAX_ARTICULOS = 10000;
const ushort MAX_COMPRAS = 100;
const ushort RUBROS = 15;
const ushort OFERTAS = 14;

struct sArticulo {
    int codArt;
    short codRub;
    str30 descArt;
    ushort stock;
    float preUni;
    str10 medida;
    short ofertas[OFERTAS];
};

struct sDescArticulo {
    str30 descArt;
    int posArt;
    bool estado;
};

struct sRubro {
    short codRub;
    str20 descRub;
};

struct sCompra {
    str30 descArt;
    short cantReq;
};

typedef sArticulo tvrArticulos[MAX_ARTICULOS];
typedef sDescArticulo tvrDescArticulos[MAX_ARTICULOS];
typedef sRubro tvrRubros[RUBROS];
typedef sCompra tvrCompras[MAX_COMPRAS];

// Agrupa los archivos de entrada/salida
struct sArchivos {
    fstream Articulos;
    ifstream DescArticulos;
    ifstream Rubros;
    ifstream Compras;
};

// Agrupa los registros principales
struct sRegistros {
    tvrArticulos vrArticulos;
    tvrDescArticulos vrDescArticulos;
    tvrRubros vrRubros;
    tvrCompras vrCompras;
    ushort cantArt = 0;
    ushort cantCompra = 0;
};

bool LeerArticulo(fstream &Articulos, sArticulo &rArticulo) {
    Articulos >> rArticulo.codArt;
    Articulos >> rArticulo.codRub;
    Articulos.ignore();
    Articulos.get(rArticulo.descArt, 31);
    Articulos >> rArticulo.stock;
    Articulos >> rArticulo.preUni;
    Articulos.ignore();
    Articulos.get(rArticulo.medida, 11);

    for (ushort i = 0; i < OFERTAS; i++) {
        Articulos >> rArticulo.ofertas[i];
    }

    Articulos.ignore();
    return Articulos.good();
}

bool LeerDescArticulo(ifstream &DescArticulos, sDescArticulo &rDescArticulo) {
    DescArticulos.get(rDescArticulo.descArt, 31);
    DescArticulos >> rDescArticulo.posArt;
    DescArticulos >> rDescArticulo.estado;
    DescArticulos.ignore();
    return DescArticulos.good();
}

bool LeerRubro(ifstream &Rubros, sRubro &rRubro) {
    Rubros >> rRubro.codRub;
    Rubros.ignore();
    Rubros.get(rRubro.descRub, 21);
    Rubros.ignore();
    return Rubros.good();
}

bool LeerCompra(ifstream &Compras, sCompra &rCompra) {
    Compras.get(rCompra.descArt, 31);
    Compras >> rCompra.cantReq;
    Compras.ignore();
    return Compras.good();
}

void Abrir(sArchivos &arch) {
    arch.Articulos.open("Articulos.txt");
    arch.DescArticulos.open("IndDescripArt.txt");
    arch.Rubros.open("Rubros.txt");
    arch.Compras.open("ListaCompras.txt");
}

void VolcarArchivos(sArchivos &arch, sRegistros &reg) {
    sArticulo rArticulo;
    sDescArticulo rDescArticulo;
    sRubro rRubro;
    sCompra rCompra;

    while (!arch.Articulos.eof() && reg.cantArt < MAX_ARTICULOS) {
        LeerArticulo(arch.Articulos, rArticulo);
        reg.vrArticulos[reg.cantArt++] = rArticulo;
    }

    for (ushort i = 0; i < reg.cantArt; i++) {
        LeerDescArticulo(arch.DescArticulos, rDescArticulo);
        reg.vrDescArticulos[i] = rDescArticulo;
    }

    for (ushort i = 0; i < RUBROS; i++) {
        LeerRubro(arch.Rubros, rRubro);
        reg.vrRubros[i] = rRubro;
    }

    while (!arch.Compras.eof() && reg.cantCompra < MAX_COMPRAS) {
        LeerCompra(arch.Compras, rCompra);
        reg.vrCompras[reg.cantCompra++] = rCompra;
    }
}

void Cerrar(sArchivos &arch) {
    arch.Articulos.close();
    arch.DescArticulos.close();
    arch.Rubros.close();
    arch.Compras.close();
}

int main() {
    sArchivos arch;
    sRegistros reg;

    Abrir(arch);
    VolcarArchivos(arch, reg);
    Cerrar(arch);

    return 0;
}