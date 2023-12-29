#ifndef DIRECT_HPP
#define DIRECT_HPP

#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <ctime>
#include <random>
#include "CoorD.hpp"

struct ComparePairs
{
    bool operator()(const std::pair<long double, CoorD> &a, const std::pair<long double, CoorD> &b) const
    {
        return a.first < b.first;
    }
};

class Direct
{
private:
    vector<CoorD> points;
    int maxIteraciones = 10;
public:
    Direct() {}
    Direct(std::vector<CoorD> points) : points(points) {}
    ~Direct() = default;
    void insert(CoorD data) { points.push_back(data); }
    void print()
    {
        for (const auto &c : points)
            std::cout << "(" << c.getX() << ", " << c.getY() << ")" << std::endl;
    }
    bool search(CoorD data)
    {
        for (const auto &c : points)
            if (c == data)
                return true;
        return false;
    }
    void clear() { points.clear(); }

    std::vector<std::vector<CoorD>>KMeans(int k) {
        vector<CoorD> centroids = Centroids(k);
        return KMeans(centroids, 0);
    }

    std::vector<std::vector<CoorD>>KMeans(const std::vector<CoorD>& centroids, int count) {
    Direct centers = Direct(centroids);
    std::vector<std::vector<CoorD>> clusters(centroids.size());

    for (const auto& point : points) {
        int nearestCentroid = centers.KMeansNearestCentroid(point, centroids);
        clusters[nearestCentroid].push_back(point);
    }

    std::vector<CoorD> newCentroids = ApproximateCentroids(clusters);

    if (count == maxIteraciones || newCentroids == centroids) {
        return clusters;
    }

    return KMeans(newCentroids, count + 1);
}

    int KMeansNearestCentroid(const CoorD& point, vector<CoorD> centroids) const {
        int nearestCentroid = -1;
        double minDistance = std::numeric_limits<double>::infinity();

        for (int i = 0; i < centroids.size(); ++i) {
            double distance = point.distance(centroids[i]);
            if (distance < minDistance) {
                minDistance = distance;
                nearestCentroid = i;
            }
        }

        return nearestCentroid;
    }

    vector<CoorD> KNN(CoorD data, int cantPoints)
    {
        set<std::pair<double, CoorD>, ComparePairs> closestPoints;
        for (const auto &c : this->points)
        {
            double dist = c.distance(data); // Cálculo euclidiano
            if (closestPoints.size() < static_cast<size_t>(cantPoints))
                closestPoints.emplace(dist, c);
            else
            {
                if (dist < closestPoints.rbegin()->first)
                {
                    closestPoints.erase(std::prev(closestPoints.end()));
                    closestPoints.emplace(dist, c);
                }
            }
        }

        vector<CoorD> neighbours;
        neighbours.reserve(cantPoints);
        for (const auto &pair : closestPoints)
            neighbours.push_back(pair.second);
        return neighbours;
    }
    // Función para generar centroides aleatorios
    std::vector<CoorD> Centroids(int k) {
        vector<CoorD> centroids;  // Vector que almacenará los centroides
        centroids.reserve(k);  // Reserva espacio para k elementos en el vector
        mt19937 gen(static_cast<long unsigned int>(12));  // Generador de números aleatorios Mersenne Twister
        uniform_int_distribution<int> distribution(0, points.size() - 1);  // Distribución uniforme entre 0 y el tamaño de 'points'

        for (int i = 0; i < k; i++) {
            int ind = distribution(gen);  // Obtiene un índice aleatorio
            // Asegura que el punto correspondiente al índice no esté ya en la lista de centroides
            while (find(centroids.begin(), centroids.end(), points[ind]) != centroids.end())
                ind = distribution(gen);  // Si ya está, elige otro índice aleatorio

            centroids.push_back(points[ind]);  // Agrega el punto correspondiente al índice como centroide
        }

        return centroids;  // Devuelve el vector de centroides generados
    }

    vector<CoorD> ApproximateCentroids(vector<vector<CoorD>> clusters)
    {
        vector<CoorD> newCentroids;
        newCentroids.reserve(clusters.size());
        for (int i = 0; i < clusters.size(); i++)
        {
            CoorD centroid = CoorD();
            for (const auto &point : clusters[i])
                centroid = centroid + point;
            double size = clusters[i].size();
            if (size != 0)
                centroid = centroid / size;
            newCentroids.push_back(centroid);
        }
        return newCentroids;
    }

    vector<vector<CoorD>> KMeansBruteforce(int k)
    {
        vector<CoorD> centroids = Centroids(k);
        return KMeansBruteforce(centroids, 0);
    }

    vector<vector<CoorD>> KMeansBruteforce(vector<CoorD> centroides, int count)
    {
        Direct centers = Direct(centroides);
        vector<vector<CoorD>> clusters(centroides.size());
        for (int i = 0; i < points.size(); i++)
        {
            vector<CoorD> num = centers.KNN(points[i], 1);
            for (int j = 0; j < centroides.size(); j++)
                if (num[0] == centroides[j])
                {
                    clusters[j].push_back(points[i]);
                    break;
                }
        }
        vector<CoorD> nuevoCentroides = ApproximateCentroids(clusters);
        if (count == maxIteraciones || nuevoCentroides == centroides)
            return clusters;
        return KMeansBruteforce(nuevoCentroides, count + 1);
    }
};
#endif