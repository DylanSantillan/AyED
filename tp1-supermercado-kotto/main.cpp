#include <cstring>
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
const ushort LINEA_ART = 104;
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

struct sArchivos {
    fstream Articulos;
    ifstream DescArticulos;
    ifstream Rubros;
    ifstream Compras;
};

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
    arch.Articulos.open("ArticulosM.txt");
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

int BusBinVec(tvrDescArticulos &vrDescArticulo, str30 clave, ushort card) {
    int prim = 0;
    int ult = card - 1;
    int med;
    int cmp;

    while (prim <= ult) {
        med = (prim + ult) / 2;
        cmp = strcmp(vrDescArticulo[med].descArt, clave);
        if (cmp == 0)
            return med;
        else if (cmp < 0)
            prim = med + 1;
        else
            ult = med - 1;
    }
    return -1;
}

void ActLinea(fstream &Articulos, sArticulo &rArticulo, ushort posArt) {
    Articulos.clear();
    Articulos.seekp(LINEA_ART * posArt);

    Articulos << setw(8) << rArticulo.codArt << " ";
    Articulos << setw(2) << rArticulo.codRub << " ";
    Articulos << setw(30) << rArticulo.descArt << " ";
    Articulos << setw(4) << rArticulo.stock << " ";
    Articulos << fixed << setprecision(2) << setw(9) << rArticulo.preUni << " ";
    Articulos << setw(10) << rArticulo.medida;

    for (ushort i = 0; i < (OFERTAS / 2); i++) {
        Articulos << ' ' << rArticulo.ofertas[2 * i];
        Articulos << ' ' << setw(2) << rArticulo.ofertas[2 * i + 1];
    }

    Articulos << "\n";
}

void ProcCompras(sArchivos &arch, sRegistros &reg) {
    sCompra compra;
    short pos;
    ushort posArt;

    for (ushort i = 0; i < reg.cantCompra; i++) {
        compra = reg.vrCompras[i];
        pos = BusBinVec(reg.vrDescArticulos, compra.descArt, reg.cantArt);
        posArt = reg.vrDescArticulos[pos].posArt;

        if (pos == -1 || !reg.vrDescArticulos[pos].estado) {
            reg.vrCompras[i].cantReq = 0;
            continue;
        }

        if (reg.vrArticulos[posArt].stock < reg.vrCompras[i].cantReq) {
            reg.vrCompras[i].cantReq = reg.vrArticulos[posArt].stock;
            reg.vrArticulos[posArt].stock = 0;
        } else {
            reg.vrArticulos[posArt].stock -= reg.vrCompras[i].cantReq;
        }

        ActLinea(arch.Articulos, reg.vrArticulos[posArt], posArt);
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
    ProcCompras(arch, reg);
    Cerrar(arch);

    return 0;
}