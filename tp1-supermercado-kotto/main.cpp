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
const ushort MAX_RUBROS = 15;
const ushort TAM_LINEA = 104;
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
typedef sRubro tvrRubros[MAX_RUBROS];
typedef sCompra tvrCompras[MAX_COMPRAS];

void Abrir(fstream &Articulos, ifstream &DescArticulos, ifstream &Rubros,
           ifstream &Compras) {
    Articulos.open("ArticulosM.txt");
    DescArticulos.open("IndDescripArt.txt");
    Rubros.open("Rubros.txt");
    Compras.open("ListaCompras.txt");
}  // Abrir()

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
}  // LeerSuf()

bool LeerDescArticulo(ifstream &DescArticulos, sDescArticulo &rDescArticulo) {
    DescArticulos.get(rDescArticulo.descArt, 31);
    DescArticulos >> rDescArticulo.posArt;
    DescArticulos >> rDescArticulo.estado;
    DescArticulos.ignore();
    return DescArticulos.good();
}  // LeerSuf()

bool LeerRubro(ifstream &Rubros, sRubro &rRubro) {
    Rubros >> rRubro.codRub;
    Rubros.ignore();
    Rubros.get(rRubro.descRub, 21);
    Rubros.ignore();
    return Rubros.good();
}  // LeerSuf()

bool LeerCompra(ifstream &Compras, sCompra &rCompra) {
    Compras.get(rCompra.descArt, 31);
    Compras >> rCompra.cantReq;
    Compras.ignore();
    return Compras.good();
}  // LeerSuf()

void VolcarArchivos(fstream &Articulos, ifstream &DescArticulos,
                    ifstream &Rubros, ifstream &Compras,
                    tvrArticulos &vrArticulos,
                    tvrDescArticulos &vrDescArticulos, tvrRubros &vrRubros,
                    tvrCompras &vrCompras, ushort &cantArt,
                    ushort &cantCompra) {
    sArticulo rArticulo;
    sDescArticulo rDescArticulo;
    sRubro rRubro;
    sCompra rCompra;

    while (LeerArticulo(Articulos, rArticulo) && cantArt < MAX_ARTICULOS)
        vrArticulos[cantArt++] = rArticulo;

    for (ushort i = 0; i < cantArt; i++) {
        LeerDescArticulo(DescArticulos, rDescArticulo);
        vrDescArticulos[i] = rDescArticulo;
    }

    for (ushort i = 0; i < MAX_RUBROS; i++) {
        LeerRubro(Rubros, rRubro);
        vrRubros[i] = rRubro;
    }

    while (LeerCompra(Compras, rCompra) && cantCompra < MAX_COMPRAS)
        vrCompras[cantCompra++] = rCompra;
}  // VolcarArchivos()

int BusBinArt(tvrDescArticulos &vrDescArticulos, str30 descArt,
              ushort cantDesc) {
    int inicio = 0;
    int fin = cantDesc - 1;
    int medio;
    int comparacion;

    while (inicio <= fin) {
        medio = (inicio + fin) / 2;
        comparacion = strcmp(vrDescArticulos[medio].descArt, descArt);
        if (comparacion == 0)
            return medio;
        else if (comparacion < 0)
            inicio = medio + 1;
        else
            fin = medio - 1;
    }
    return -1;
}  // BusBinVec()

int BusBinRub(tvrRubros &vrRubros, short codRub, ushort cantRubros) {
    int inicio = 0;
    int fin = cantRubros - 1;
    int medio;
    int comparacion;

    while (inicio <= fin) {
        medio = (inicio + fin) / 2;
        comparacion = vrRubros[medio].codRub - codRub;
        if (comparacion == 0)
            return medio;
        else if (comparacion < 0)
            inicio = medio + 1;
        else
            fin = medio - 1;
    }
    return -1;
}  // BusBinVec()

void ActLinea(fstream &Articulos, sArticulo &rArticulo, ushort posArt) {
    Articulos.clear();
    Articulos.seekp(TAM_LINEA * posArt);

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
}  // ActLinea()

void ProcCompras(fstream &Articulos, tvrArticulos &vrArticulos,
                 tvrDescArticulos &vrDescArticulos, tvrCompras &vrCompras,
                 ushort cantArt, ushort cantCompra) {
    sCompra compra;
    ushort posArt;
    short pos;

    for (ushort i = 0; i < cantCompra; i++) {
        compra = vrCompras[i];
        pos = BusBinArt(vrDescArticulos, compra.descArt, cantArt);
        posArt = vrDescArticulos[pos].posArt;

        if (pos == -1 || !vrDescArticulos[pos].estado) {
            vrCompras[i].cantReq = 0;
            continue;
        }

        if (vrArticulos[posArt].stock < vrCompras[i].cantReq) {
            vrCompras[i].cantReq = vrArticulos[posArt].stock;
            vrArticulos[posArt].stock = 0;
        } else {
            vrArticulos[posArt].stock -= vrCompras[i].cantReq;
        }

        ActLinea(Articulos, vrArticulos[posArt], posArt);
    }
}  // ProcCompras()

void IntCmb(sArticulo &rArticulo1, sArticulo &rArticulo2) {
    sArticulo auxiliar = rArticulo1;
    rArticulo1 = rArticulo2;
    rArticulo2 = auxiliar;
}  // IntCmb()

void OrdxBurArt(tvrArticulos &vrArticulos, ushort cantArt) {
    ushort pasada = 0;
    bool estaOrdenado;

    do {
        estaOrdenado = true;
        pasada++;

        for (ushort i = 0; i < cantArt - pasada; i++) {
            if (vrArticulos[i].codRub > vrArticulos[i + 1].codRub) {
                estaOrdenado = false;
                IntCmb(vrArticulos[i], vrArticulos[i + 1]);
            }
        }
    } while (!estaOrdenado);
}  // OrdxBur()

string Replicate(char caracter, ushort cantidad) {
    string resultado = "";
    for (ushort i = 1; i <= cantidad; i++) resultado += caracter;
    return resultado;
}  // Replicate()

long GetDate(int &anio, int &mes, int &dia, int &diaSemana) {
    time_t tiempoActual;
    struct tm *infoTiempo;

    time(&tiempoActual);
    infoTiempo = localtime(&tiempoActual);
    anio = 1900 + infoTiempo->tm_year;
    mes = 1 + infoTiempo->tm_mon;
    dia = infoTiempo->tm_mday;
    diaSemana = 1 + infoTiempo->tm_wday;
    return anio * 10000 + mes * 100 + dia;
}  // GetDate()

long GetTime(int &hora, int &minuto, int &segundo) {
    time_t tiempoActual;
    struct tm *infoTiempo;

    time(&tiempoActual);
    infoTiempo = localtime(&tiempoActual);
    hora = infoTiempo->tm_hour;
    minuto = infoTiempo->tm_min;
    segundo = infoTiempo->tm_sec;
    return hora * 10000 + minuto * 100 + segundo;
}  // GetTime

void CabeceraTicket(int &diaSemana) {
    int anio, mes, dia, hora, minuto, segundo;
    GetDate(anio, mes, dia, diaSemana);
    GetTime(hora, minuto, segundo);
    char nomdia[13];

    switch (diaSemana) {
        case 1:
            strcpy(nomdia, "Domingo");
            break;
        case 2:
            strcpy(nomdia, "Lunes");
            break;
        case 3:
            strcpy(nomdia, "Martes");
            break;
        case 4:
            strcpy(nomdia, "Miercoles");
            break;
        case 5:
            strcpy(nomdia, "Jueves");
            break;
        case 6:
            strcpy(nomdia, "Viernes");
            break;
        case 7:
            strcpy(nomdia, "Sábado");
            break;
        default:
            break;
    }

    cout << "K O T T O\n";
    cout << "Yo te reconozco\n";
    cout << "SUC 170\n";
    cout << "XXXXXX…X 9999\n";
    cout << "XX…X\n";
    cout << "C.U.I.T.99-99999999-9\n";

    cout << "Fecha: " << nomdia << ' ';
    cout << dia << "/" << mes << "/" << anio << "\n";

    cout << "Hora: " << setw(2) << setfill('0') << hora;
    cout << ":" << setw(2) << setfill('0') << minuto;
    cout << ":" << setw(2) << setfill('0') << segundo << "\n";

    cout << "Nro. Ticket: 9999-99999999\n";
    cout << "Nro. Caja: 9999\n";

    cout << Replicate('-', 45) << "\n";
    cout << "F A C T U R A - B\n";
    cout << "ORIGINAL\n";
    cout << Replicate('-', 45) << "\n";
}

void PieTicket(float impTot, float impTotDesto, float impTotConDesto) {
    float pagoUsuario = impTotConDesto;

    float vuelto = pagoUsuario - impTotConDesto;

    cout << fixed << setprecision(2);
    cout << Replicate('-', 40) << endl;
    cout << left << setw(28) << "Total bruto:" << "$ " << setw(9) << impTot
         << endl;
    cout << left << setw(28) << "Descuento aplicado:" << "$ " << setw(9)
         << impTotDesto << endl;
    cout << left << setw(28) << "Total a pagar:" << "$ " << setw(9)
         << impTotConDesto << endl;
    cout << left << setw(28) << "Su pago con Tipo Pago:" << "$ " << setw(9)
         << pagoUsuario << endl;
    cout << left << setw(28) << "Su vuelto:" << "$ " << setw(9) << vuelto
         << endl;
    cout << endl;
    cout << "         G R A C I A S  P O R  S U  C O M P R A" << endl;
    cout << "Para consultas, sugerencias o reclamos" << endl;
    cout << "comunicarse al correo infoKotto.com.ar" << endl;
    cout << Replicate('-', 45) << endl;
}  // PieTicket

void EmitirTicket(tvrArticulos &vrArticulos, tvrDescArticulos &vrDescArticulos,
                  tvrCompras &vrCompras, ushort cantArt, ushort cantCompra) {
    int diaSemana;
    float impTot = 0.0f;
    float impDesto = 0.0f;
    short pos;
    sArticulo art;
    ushort cant;
    float precio;
    float subtotal;
    ushort tipo;
    ushort porc;
    float descuento;
    float total;
    float impTotDesto;

    freopen("Ticket.txt", "a", stdout);
    CabeceraTicket(diaSemana);

    for (ushort i = 0; i < cantCompra; i++) {
        if (vrCompras[i].cantReq == 0) continue;

        pos = BusBinArt(vrDescArticulos, vrCompras[i].descArt, cantArt);
        if (pos == -1 || !vrDescArticulos[pos].estado) continue;

        art = vrArticulos[vrDescArticulos[pos].posArt];
        cant = vrCompras[i].cantReq;
        precio = art.preUni;
        subtotal = cant * precio;

        tipo = art.ofertas[(diaSemana - 1) * 2];
        porc = art.ofertas[(diaSemana - 1) * 2 + 1];
        descuento = 0.0f;

        if (tipo >= 1 && tipo <= 6) descuento = subtotal * porc / 100.0f;

        total = subtotal - descuento;

        cout << right << setw(2) << setfill(' ') << cant << " x $ ";
        cout << right << setw(9) << fixed << setprecision(2) << setfill(' ')
             << precio << '\n';

        cout << setw(30) << setfill(' ') << art.descArt;
        cout << setw(10) << art.medida << '\n';

        cout << right << setw(8) << art.codArt;
        cout << right << setw(23) << "$ " << setw(9) << fixed << setprecision(2)
             << subtotal << '\n';

        if (descuento > 0.0f) {
            string etiqueta;
            switch (tipo) {
                case 1:
                    etiqueta = "Jub.";
                    break;
                case 2:
                    etiqueta = "Marca.";
                    break;
                case 3:
                    etiqueta = "MercPago";
                    break;
                case 4:
                    etiqueta = "Comunid.";
                    break;
                case 5:
                    etiqueta = "ANSES";
                    break;
                case 6:
                    etiqueta = "Promo";
                    break;
                default:
                    etiqueta = "Desconocido";
                    break;
            }

            cout << left << setw(12) << etiqueta << ' ' << tipo;
            cout << right << setw(17) << "$ -" << setw(9) << fixed
                 << setprecision(2) << descuento << '\n';
        }

        cout << "\n";

        impTot += subtotal;
        impDesto += descuento;
    }

    impTotDesto = impTot - impDesto;

    cout << '\n' << left << setw(28) << "SubTot. sin descuentos....:" << "$ ";
    cout << right << setw(10) << fixed << setprecision(2) << impTot << '\n';

    cout << left << setw(28) << "Descuentos por promociones:" << "$ -";
    cout << right << setw(9) << fixed << setprecision(2) << impDesto << '\n';

    cout << Replicate('=', 45) << '\n';
    cout << left << setw(28) << "T O T A L" << "$ " << right << setw(10)
         << impTotDesto << '\n';
    cout << Replicate('=', 45) << '\n';

    PieTicket(impTot, impDesto, impTotDesto);
    fclose(stdout);
}

void EmitirArt_x_Rubro(fstream &Articulos, ifstream &DescArticulos,
                       ifstream &Rubros, ifstream &Compras,
                       tvrArticulos &vrArticulos,
                       tvrDescArticulos &vrDescArticulos, tvrRubros &vrRubros,
                       tvrCompras &vrCompras, ushort cantArt,
                       ushort cantCompra) {
    OrdxBurArt(vrArticulos, cantArt);

    int ancho = 105;
    string titulo = "Listado de Articulos ordenados por Código de Rubro";
    ushort codRub = 100;

    std::ofstream out("Ticket.txt", std::ios::app);

    out << Replicate('-', ancho) << "\n";
    out << Replicate(' ', (ancho - titulo.length()) / 2) << titulo << "\n";
    out << Replicate('=', ancho) << "\n";

    for (ushort i = 0; i < cantArt; i++) {
        if (vrArticulos[i].codRub != codRub) {
            codRub = vrArticulos[i].codRub;

            if (BusBinRub(vrRubros, codRub, MAX_RUBROS) >= 1) {
                out << "\n";
            }
            out << "Cod. Rubro: " << codRub << " "
                << vrRubros[BusBinRub(vrRubros, codRub, MAX_RUBROS)].descRub
                << "\n";
            out << "Cod.Art. Descripcion                    Stk. Pre.Uni.  "
                   "U.Med.     TD % TD % TD % TD % TD % TD % TD %"
                << "\n";
            out << Replicate('-', ancho) << "\n";
        }

        out << setw(8) << vrArticulos[i].codArt << " ";
        out << setw(30) << vrArticulos[i].descArt << " ";
        out << setw(4) << vrArticulos[i].stock << " ";
        out << fixed << setprecision(2) << setw(9) << vrArticulos[i].preUni
            << " ";
        out << setw(10) << vrArticulos[i].medida;

        for (ushort j = 0; j < (OFERTAS / 2); j++) {
            out << ' ' << vrArticulos[i].ofertas[2 * j];
            out << ' ' << setw(2) << vrArticulos[i].ofertas[2 * j + 1];
        }

        out << "\n";
    }

    out.close();
}

void Cerrar(fstream &Articulos, ifstream &DescArticulos, ifstream &Rubros,
            ifstream &Compras) {
    Articulos.close();
    DescArticulos.close();
    Rubros.close();
    Compras.close();
}

int main() {
    fstream Articulos;
    ifstream DescArticulos;
    ifstream Rubros;
    ifstream Compras;

    tvrArticulos vrArticulos;
    tvrDescArticulos vrDescArticulos;
    tvrRubros vrRubros;
    tvrCompras vrCompras;

    ushort cantArt = 0;
    ushort cantCompra = 0;

    Abrir(Articulos, DescArticulos, Rubros, Compras);
    VolcarArchivos(Articulos, DescArticulos, Rubros, Compras, vrArticulos,
                   vrDescArticulos, vrRubros, vrCompras, cantArt, cantCompra);
    ProcCompras(Articulos, vrArticulos, vrDescArticulos, vrCompras, cantArt,
                cantCompra);
    EmitirTicket(vrArticulos, vrDescArticulos, vrCompras, cantArt, cantCompra);
    EmitirArt_x_Rubro(Articulos, DescArticulos, Rubros, Compras, vrArticulos,
                      vrDescArticulos, vrRubros, vrCompras, cantArt,
                      cantCompra);
    Cerrar(Articulos, DescArticulos, Rubros, Compras);

    return 0;
}