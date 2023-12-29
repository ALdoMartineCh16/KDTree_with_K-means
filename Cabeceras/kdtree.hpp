#ifndef KDTREE_HPP
#define KDTREE_HPP

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <utility>
#include <set>
#include "random"
#include "CoorD.hpp"

struct Neighbor
{
    CoorD point;
    double distance;
    Neighbor(CoorD p, double d) : point(p), distance(d) {}
    bool operator<(const Neighbor &other) const {
        return distance < other.distance;
    }
};

struct Node
{
    CoorD data;
    int depth;
    Node *left;
    Node *right;
    Node(CoorD data, int depth) : data(data), depth(depth), left(NULL), right(NULL) {}
};

class KDTree
{
private:
    Node *root;
    int k, size;

public:
    KDTree() : root(NULL), k(0) {}
    KDTree(int k) : root(NULL), k(k) {}
    ~KDTree() = default;
    void insert(CoorD data)
    {
        Insert(root, data, 0);
    }
    void print()
    {
        Print(root);
    }
    bool search(CoorD data)
    {
        return Search(root, data, 0);
    }
    void clear()
    {
        delete root;
        root = NULL;
    }
    vector<CoorD> KNN(CoorD data, int cantPoints)
    {
        vector<Neighbor> queue;
        KNNRecursive(root, data, cantPoints, queue);
        vector<CoorD> neighbours;
        for (auto &n : queue)
            neighbours.push_back(n.point);
        return neighbours;
    }
    vector<CoorD> GetAllPoints()
    {
        std::vector<CoorD> allPoints;
        allPoints.reserve(size); // Reserva espacio para evitar reallocaciones frecuentes

        GetAllPointsRecursive(root, allPoints);

        return allPoints;
    }
    std::vector<CoorD> Centroids(int k, std::vector<CoorD> points)
    {
        std::vector<CoorD> centroids;
        centroids.reserve(k);
        std::mt19937 gen(static_cast<long unsigned int>(12));
        std::uniform_int_distribution<int> distribution(0, points.size() - 1);
        for (int i = 0; i < k; i++)
        {
            int ind = distribution(gen);
            while (std::find(centroids.begin(), centroids.end(), points[ind]) != centroids.end())
                ind = distribution(gen);
            centroids.push_back(points[ind]);
        }
        return centroids;
    }
    std::vector<CoorD> ApproximateCentroids(std::vector<std::vector<CoorD>> clusters)
    {
        std::vector<CoorD> newCentroids;
        newCentroids.reserve(clusters.size());

        for (int i = 0; i < clusters.size(); i++)
        {
            CoorD centroid = CoorD();
            for (const auto &point : clusters[i])
            {
                centroid = centroid + point;
                // std::cout << "el point" << point.getX() << "," << point.getY() << std::endl;
            }
            double size = clusters[i].size();
            // std::cout << "el centroid 1" << centroid.getX() << "," << centroid.getY() << std::endl;

            if (size != 0)
                centroid = centroid / size;
            // std::cout << "el centroid" << centroid.getX() << "," << centroid.getY() << std::endl;
            newCentroids.push_back(centroid);
        }
        return newCentroids;
    }
    std::vector<std::vector<CoorD>> KMeans(int k)
    {
        std::vector<CoorD> all_points = GetAllPoints();
        std::vector<CoorD> centroids = Centroids(k, all_points);
        // for (int i = 0; i < all_points.size(); i++)
        //     std::cout << "Cluster " << i << ":" << all_points[i].getX() << "" << all_points[i].getY() << std::endl;
        return KMeans(centroids, 0, all_points);
    }

    std::vector<std::vector<CoorD>> KMeans(std::vector<CoorD> centroids, int count, std::vector<CoorD> points)
    {

        KDTree centers(2);
        // std::cout << "Cluster " << std::endl;
        for (const auto &centroid : centroids)
        {
            centers.insert(centroid);
        }
        // std::cout << "Cluster 2" << std::endl;
        std::vector<std::vector<CoorD>> clusters(centroids.size());
        for (int i = 0; i < size; i++)
        {
            // std::cout << "Cluster 3" << std::endl;

            std::vector<CoorD> num = centers.KNN(points[i], 1);
            for (int j = 0; j < centroids.size(); j++)
            {
                if (num[0] == centroids[j])
                {
                    clusters[j].push_back(points[i]);
                    break;
                }
            }
        }
        // for (int i = 0; i < clusters.size(); i++)
        // std::cout << "Cluster " << i << ":" << clusters[i].size() << std::endl;
        std::vector<CoorD> newCentroids = ApproximateCentroids(clusters);
        // for (int i = 0; i < newCentroids.size(); i++)
        //     std::cout << "nuevo centro " << i << ":" << newCentroids[i].getX() << "" << newCentroids[i].getY() << std::endl;
        if (count == 10 || newCentroids == centroids)
            return clusters;
        return KMeans(newCentroids, count + 1, points);
    }
private:
    //Funcion insertar de forma recursiva
    void Insert(Node *&root, CoorD data, int depth)
    {
        if (root == NULL) // Raiz este vacia
        {
            root = new Node(data, depth);
            return;
        }
        int cd = depth % k;
        if (cd == 0)
        {
            if (data.getX() < root->data.getX())
                Insert(root->left, data, depth++);
            else
                Insert(root->right, data, depth++);
        }
        else
        {
            if (data.getY() < root->data.getY())
                Insert(root->left, data, depth++);
            else
                Insert(root->right, data, depth++);
        }
    }
    void Print(Node *root)
    {
        if (root == NULL)
            return;
        Print(root->left);
        cout << "(" << root->data.getX() << ", " << root->data.getY();
        cout << ") Level:" << root->depth << std::endl;
        Print(root->right);
    }
    bool Search(Node *root, CoorD data, int depth)
    {
        if (root == NULL)
            return false;
        if (root->data == data)
            return true;
        int cd = depth % k;
        if (cd == 0)
        {
            if (data.getX() < root->data.getX())
                return Search(root->left, data, ++depth);
            else
                return Search(root->right, data, ++depth);
        }
        else
        {
            if (data.getY() < root->data.getY())
                return Search(root->left, data, ++depth);
            else
                return Search(root->right, data, ++depth);
        }
    }
    void KNNRecursive(Node *node, CoorD query, int cantPoints, std::vector<Neighbor> &neighbors)
    {
        if (node == nullptr)
            return;

        double dist = distance(node->data, query);
        if (neighbors.size() < cantPoints)
        {
            neighbors.emplace_back(node->data, dist);
            push_heap(neighbors.begin(), neighbors.end());
        }
        else if (dist < neighbors.front().distance)
        {
            pop_heap(neighbors.begin(), neighbors.end());
            neighbors.back() = Neighbor(node->data, dist);
            push_heap(neighbors.begin(), neighbors.end());
        }

        int axis = node->depth % k;
        Node *firstNode = nullptr;
        Node *secondNode = nullptr;

        if (query.getCord(axis) < node->data.getCord(axis))
        {
            firstNode = node->left;
            secondNode = node->right;
        }
        else
        {
            firstNode = node->right;
            secondNode = node->left;
        }

        KNNRecursive(firstNode, query, cantPoints, neighbors);
        if (neighbors.size() < cantPoints || fabs(query.getCord(axis) - node->data.getCord(axis)) < neighbors.front().distance)
        {
            KNNRecursive(secondNode, query, cantPoints, neighbors);
        }
    }
    double distance(CoorD a, CoorD b)
    {
        return sqrt(std::pow(a.getX() - b.getX(), 2) + std::pow(a.getY() - b.getY(), 2));
    }
    void GetAllPointsRecursive(Node *node, std::vector<CoorD> &allPoints)
    {
        if (node == nullptr)
        {
            return;
        }

        GetAllPointsRecursive(node->left, allPoints);
        allPoints.push_back(node->data);
        GetAllPointsRecursive(node->right, allPoints);
    }
};

#endif