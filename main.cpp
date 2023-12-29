#include <iostream>
#include <fstream>
#include <sstream>
#include "Cabeceras/kdtree.hpp"
#include "Cabeceras/direct.hpp"
#include <chrono>

using namespace std;

KDTree readAndConvert_(string fileN)
{
    string filename = fileN;
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "No se pudo abrir el archivo " << filename << endl;
        throw runtime_error("No se pudo abrir el archivo " + filename);
    }
    string firstLine;
    getline(file, firstLine);
    KDTree kdtree(2);
    string line;
    while (getline(file, line))
    {
        istringstream ss(line);
        string token;
        long double x, y;
        if (getline(ss, token, ','))
            x = stold(token);
        if (getline(ss, token, '\n'))
            y = stold(token);
        kdtree.insert(CoorD(x, y));
    }
    file.close();
    return kdtree;
}

Direct readAndConvertDirect_(string fileN)
{
    string filename = fileN;
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "No se pudo abrir el archivo " << filename << endl;
        throw runtime_error("No se pudo abrir el archivo " + filename);
    }
    string firstLine;
    getline(file, firstLine);
    Direct direct;
    string line;
    while (getline(file, line))
    {
        istringstream ss(line);
        string token;
        long double x, y;
        if (getline(ss, token, ','))
            x = stold(token);
        if (getline(ss, token, '\n'))
            y = stold(token);
        direct.insert(CoorD(x, y));
    }
    file.close();
    return direct;
}

int main()
{
        KDTree kdtree(2);
        kdtree = readAndConvert_("data2k.csv");
        Direct direct;
        direct = readAndConvertDirect_("data2k.csv");
        int n;
        vector<CoorD> vecinos;
        int opcion;
        auto start = chrono::high_resolution_clock::now();
        auto end = chrono::high_resolution_clock::now();
        std::vector<std::vector<CoorD>> clusters;
        chrono::duration<double, milli> elapsed;

    // Abrir el archivo
         std::ofstream archivo2("pruebas.txt");
         int k;
        do
        {
            cout << "\nSeleccione una operacion:" << endl;
            cout << "1. Imprimir KDTree" << endl;
            cout << "2. Buscar punto por KDTree" << endl;
            cout << "3. Buscar punto por Fuerza Bruta" << endl;
            cout << "4. Buscar N puntos vecinos en KDTree" << endl;
            cout << "5. Buscar N puntos vecinos en Fuerza Bruta" << endl;
            cout << "6. Buscar K-Means en Fuerza Bruta" << endl;
            cout << "7. Hacer K-Means en KNN" << endl;
            cout << "8. Salir" << endl;
            cout << "Opcion: ";
            cin >> opcion;

            switch (opcion)
            {
            case 1:
                start = chrono::high_resolution_clock::now();
                kdtree.print();
                end = chrono::high_resolution_clock::now();
                elapsed = end - start;
                cout << "Tiempo transcurrido: " << elapsed.count() << " ms" << endl;
                break;
            case 2:
                long double x, y;
                cout << "Ingrese las coordenadas (x, y) del punto a buscar: ";
                cin >> x >> y;
                start = chrono::high_resolution_clock::now();
                kdtree.search(CoorD(x, y)) ? cout << "El punto existe en el KDTree." << endl : cout << "El punto no existe en el KDTree." << endl;
                end = chrono::high_resolution_clock::now();
                elapsed = end - start;
                cout << "Tiempo transcurrido: " << elapsed.count() << " ms" << endl;
                break;
            case 3:
                long double x1, y1;
                cout << "Ingrese las coordenadas (x, y) del punto a buscar: ";
                cin >> x1 >> y1;
                start = chrono::high_resolution_clock::now();
                direct.search(CoorD(x1, y1)) ? cout << "El punto existe en Fuerza Bruta." << endl : cout << "El punto no existe en Fuerza Bruta." << endl;
                end = chrono::high_resolution_clock::now();
                elapsed = end - start;
                cout << "Tiempo transcurrido: " << elapsed.count() << " ms" << endl;
                break;
            case 4:
                cout << "Ingrese el numero de puntos a buscar: ";
                cin >> n;
                long double x2, y2;
                cout << "Ingrese las coordenadas (x, y) del punto a buscar: ";
                cin >> x2 >> y2;
                start = chrono::high_resolution_clock::now();
                vecinos = kdtree.KNN(CoorD(x2, y2), n);
                end = chrono::high_resolution_clock::now();
                cout << "Los " << n << " puntos vecinos son:" << endl;
                for (const auto &c : vecinos)
                {
                    cout << "(" << c.getX() << ", " << c.getY() << ") | Distancia: " << c.distance(CoorD(x2, y2)) << endl;
                }

                elapsed = end - start;
                cout << "Tiempo transcurrido: " << elapsed.count() << " ms" << endl;
                break;
            case 5:
                cout << "Ingrese el numero de puntos a buscar: ";
                cin >> n;
                long double x3, y3;
                cout << "Ingrese las coordenadas (x, y) del punto a buscar: ";
                cin >> x3 >> y3;
                start = chrono::high_resolution_clock::now();
                vecinos = direct.KNN(CoorD(x3, y3), n);
                end = chrono::high_resolution_clock::now();
                cout << "Los " << n << " puntos vecinos son:" << endl;
                for (const auto &c : vecinos)
                {
                    cout << "(" << c.getX() << ", " << c.getY() << ") | Distancia: " << c.distance(CoorD(x3, y3)) << endl;
                }
                elapsed = end - start;
                cout << "Tiempo transcurrido: " << elapsed.count() << " ms" << endl;
                break;
            case 6:
                start = chrono::high_resolution_clock::now();
                clusters = kdtree.KMeans(18);
                    end = chrono::high_resolution_clock::now();
                    cout << "Los clusters son:" << std::endl;
                    for (int i = 0; i < clusters.size(); i++)
                    {
                        cout << "Cluster " << i << ":" << clusters[i].size() << endl;
                        /*
                        for (int j=0; j<clusters[i].size(); j++){
                            std::cout << "(" << clusters[i][j].getX() << ", " << clusters[i][j].getY() << ")" << std::endl;
                        }*/

                    }
                    elapsed = end - start;
                    cout << "Tiempo transcurrido: " << elapsed.count() << " ms" << endl;

                    break;
            case 7:
                start = chrono::high_resolution_clock::now();
                clusters = direct.KMeans(18);
                end = chrono::high_resolution_clock::now();
                cout << "Los clusters son:" << std::endl;
                for (int i = 0; i < clusters.size(); i++)
                {
                    cout << "Cluster " << i << ":" << clusters[i].size() << endl;
                    archivo2<<"clusters_"<<to_string(i)<<std::endl;
                    /*
                    for (int j=0; j<clusters[i].size(); j++){
                        std::cout << "(" << clusters[i][j].getX() << ", " << clusters[i][j].getY() << ")" << std::endl;
                        archivo2<<clusters[i][j].getX() <<" , "<< clusters[i][j].getY() <<std::endl;
                    }*/

                }
                    elapsed = end - start;
                    cout << "Tiempo transcurrido: " << elapsed.count() << " ms" << endl;

                    break;
            default:
                cout << "Opcion invalida." << endl;break;
            }
        } while (opcion != 8);

    return 0;
}
