/**
 * @class Graph
 * @brief A C++ program to print topological sorting of a graph using indegrees.
 * 
 * Created by bernardo on 2/2/17.
 */
#ifndef TRIANGLEMODEL_GRAPH_H
#define TRIANGLEMODEL_GRAPH_H

#include <list>
#include <vector>
#include <stdexcept>
#include "../Constants.h"

using namespace std;

class Graph {
protected:
    /**
     * @brief The number of water sources represented as vertices of the Graph.
     */
    int V;    
    /**
     * @brief The number of streams (edges) in the Graph.
     */
    int n_edges = 0;
    /**
     * @brief A pointer to an array containing adjacency lists.
     * 
     */
    list<int> *adj;
    /**
     * @brief A 2D vector containing the list of upstream sources for each vertex.
     */
    vector<vector<int>> upstream_sources;
    /**
     * @brief A 2D vector containing the list of downstream sources for each vertex.
     */
    vector<vector<int>> downstream_sources;
    /**
     * @brief A vector containing the IDs of the water sources (vertices) in topological order.
     */
    vector<int> topological_order;
    /**
     * @brief A 2D vector containing the continuity matrix of the Graph.
     */
    vector<vector<double>> continuity_matrix_transpose;

    /**
     * @brief Adds a directed edge (a stream) to the graph's adjacency list and updates edge-related data structures.
     * This function adds an edge from vertex `u` to vertex `v` in the adjacency list, updates the list of 
     * downstream sources for vertex `u`, and increments the total edge count.
     * 
     * @param u The source vertex (water source) of the edge.
     * @param v The destination (water source) vertex of the edge.
     * 
     * @return void
     */
    void addEdgeToEdgesList(int u, int v);

    /**
     * @brief Performs a topological sort on the directed graph to sout sources from upstream to downstream..
     * This function computes a topological ordering of the vertices in the graph using Kahn's algorithm.
     * It detects cycles and returns an empty ordering if the graph contains a cycle.
     * 
     * @return A vector containing the topological order of the vertices. If the graph contains a cycle,
     * the returned vector may be incomplete, and a message is printed to indicate the cycle detection.
     * 
     */
    vector<int> topologicalSort();

    /**
     * @brief Finds the upstream sources for a given vertex.
     * This function identifies all vertices in the graph that have a directed edge
     * leading to the specified vertex `id`.
     * 
     * @param id The vertex for which upstream sources are to be found.
     * 
     * @return A vector containing all upstream source vertices for the given vertex `id`.
     * 
     */
    vector<int> findUpstreamSources(int id)const;

public:
    /**
     * @brief Constructs a graph with a specified number of vertices.
     * This constructor initializes the graph's adjacency list and downstream sources structure.
     * 
     * @param V The number of vertices in the graph.
     */
    Graph(int V);

    /**
     * @brief Base constructor for a new Graph object
     * 
     */
    Graph();

    /**
     * @brief Retrieves the topological order of the graph.
     * This function returns the precomputed topological order of the water sources (vertices) in the graph.
     * 
     * @return A constant vector containing the topological order of the vertices.
     */
    const vector<int> getTopological_order() const;

    /**
     * @brief Adds a directed edge (stream/pipes) to the graph and updates related structures.
     * This function adds an connection from vertex `u` to vertex `v`, recalculates the topological order, 
     * updates upstream sources for all vertices, and adjusts the continuity matrix transpose.
     * 
     * @param u The source vertex of the edge.
     * @param v The destination vertex of the edge.
     * 
     * @return void
     *  
     * @see addEdgeToEdgesList, topologicalSort, findUpstreamSources
     * 
     */
    void addEdge(int u, int v);

    /**
     * @brief Retrieves the upstream sources of each water source (vertex) in the graph.
     * 
     * @return A constant 2D vector containing the IDs of each vertex mapped to their respective upstream sources.
     */
    const vector<vector<int>> &getUpstream_sources() const;

    /**
     * @brief Constructs and returns the continuity matrix of the graph.
     * This function returns a matrix in which rows correspond to vertexes. 
     * The first set of columns correspond to pipes/streams and the second set to flows in and out of vertexes.
     * 
     * @return A 2D vector representing the continuity matrix of the graph.
     * 
     * @throws std::invalid_argument If the number of edges and sources is inconsistent with the graph structure.
     * 
     */
    const vector<vector<double>> getContinuityMatrix() const;

    /**
     * @brief Destroy the Graph object.
     * 
     */
    virtual ~Graph();

    /**
     * @brief Retrieves the downstream sources of each water source (vertex) in the graph.
     * 
     * @return A constant 2D vector containing the IDs of each vertex mapped to their respective downstream sources.
     */
    const vector<vector<int>> getDownSources() const;
};


#endif //TRIANGLEMODEL_GRAPH_H
