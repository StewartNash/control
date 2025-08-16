#pragma once
#include <vector>
#include <memory>

#include <string>
#include <random>
#include <sstream>
#include <iomanip>

/*
	In graph theory, a graph G=(V,E) is a set V=V(G) called vertices
	(or nodes), together with a set E=E(G) called edges, such that an
	edge is an unordered pair of vertices. (An edge where both verti-
	ces are the same element is called a loop. Loops will not be con-
	sidered.)
	
	An oriented graph -- a graph with directed edges --- is called a
	directed graph or a digraph. Formally, a digraph D is a set V=V(D)
	of vertices together with a set E=E(D) called directed edges (arcs)
	such that a directed edge is an ordered pair of vertices. The dig-
	raph is a generalization of the graph. For any associated graph G=
	(V,E), there is a digraph with ordered pairs (u,v) and (v,u) for
	any unordered pair (u,v) in the in E.
*/

class IVertex {
	public:
		virtual ~IVertex() = default;		
		virtual bool equals(const IVertex& other) const = 0;
		virtual std::string toString() const = 0;
};

class Vertex : public IVertex {
	protected:
		std::string uuid;
		
		static std::string generateUUID() {
			static std::random_device rd;
			static std::mt19937 gen(rd());
			static std::uniform_int_distribution<uint32_t> dist(0, 0xFFFFFFFF);
			
			std::stringstream ss;
			ss << std::hex << std::setfill('0')
			   << std::setw(8) << dist(gen) << "-"
			   << std::setw(4) << (dist(gen) & 0xFFFF) << "-"
			   << std::setw(4) << (dist(gen) & 0xFFFF) << "-"
			   << std::setw(4) << (dist(gen) & 0xFFFF) << "-"
			   << std::setw(12) << dist(gen);
			return ss.str();
		}
	public:
		Vertex() : uuid(generateUUID()) {}
		explicit Vertex(const std::string& id) : uuid(id) {}
		
		std::string getUUID() const { return uuid; }
		
		bool equals(const IVertex& other) const override {
			auto otherVertex = dynamic_cast<const Vertex*>(&other);
			return otherVertex && (uuid == otherVertex->uuid);
		}
		
		std::string toString() const override {
			return "Vertex[" + uuid + "]";
		}			
};

struct Edge {
	std::shared_ptr<IVertex> vertices[2];
	
	Edge(std::shared_ptr<IVertex> v1, std::shared_ptr<IVertex> v2) {
		vertices[0] = std::move(v1);
		vertices[1] = std::move(v2);
	}
};

typedef struct DirectedEdge {
	std::shared_ptr<IVertex> tail;
	std::shared_ptr<IVertex> head;
} Arc;

class Graph {
	protected:
		std::vector<std::shared_ptr<IVertex>> vertices;
		std::vector<Edge> edges;
		
		std::shared_ptr<IVertex> findVertex(const IVertex& v) const {
			for (const auto& existing : vertices) {
				if (existing->equals(v)) {
					return existing;
				}
			}

			return nullptr;
		}
	public:
		virtual ~Graph() = default;
		
		std::shared_ptr<IVertex> addVertex(std::shared_ptr<IVertex> v) {
			auto existing = findVertex(*v);
			if (existing) {
				return existing;
			}
			vertices.push_back(v);

			return v;
		}
		void addEdge(std::shared_ptr<IVertex> v1, std::shared_ptr<IVertex> v2) {
			auto vertex1 = addVertex(v1);
			auto vertex2 = addVertex(v2);
			edges.emplace_back(vertex1, vertex2);
		}
		virtual void draw() const {
			for (const auto& edge : edges) {
				std::cout << edge.vertices[0]->toString()
				          << " -- "
				          << edge.vertices[1]->toString()
				          << "\n";
			}
		}
		
		virtual std::shared_ptr<IVertex> createVertex() = 0;
};

class Digraph {
	protected:
		std::vector<std::shared_ptr<IVertex>> vertices;
		std::vector<Arc> edges;
		
		std::shared_ptr<IVertex> findVertex(const IVertex& v) const {
			for (const auto& existing : vertices) {
				if (existing->equals(v)) {
					return existing;
				}
			}

			return nullptr;
		}
	public:
		virtual ~Digraph() = default;
		
		std::shared_ptr<IVertex> addVertex(std::shared_ptr<IVertex> v) {
			auto existing = findVertex(*v);
			if (existing) {
				return existing;
			}
			vertices.push_back(v);

			return v;
		}
		void addEdge(std::shared_ptr<IVertex> tail, std::shared_ptr<IVertex> head) {
			Arc edge;
			edge.tail = std::move(tail);
			edge.head = std::move(head);
			edges.push_back(std::move(edge));			
		}
		virtual void draw() const {
			for (const auto& edge : edges) {
				std::cout << edge.tail->toString()
				          << " -- "
				          << edge.head->toString()
				          << "\n";
			}
		}
		
		virtual std::shared_ptr<IVertex> createVertex() = 0;
};


// Signal Flow Graphs

class Branch : public IVertex {

};

class Node {
	public:
		std::vector<Branch> inputs;
		std::vector<Branch> outputs;

};

class SignalFlowGraph : public Digraph {

};

// Block Diagrams

class Block : public IVertex {

};

class Signal : public IVertex {

};

class SummingJunction : public IVertex {

};

class TakeoffPoint : public IVertex {

};

class BlockDiagram : public Digraph {

};

// DSP Signal Flow Graphs

class Delay : public IVertex {

};

class Adder : public IVertex {

};

class Multiplier : public IVertex {

};

class BranchPoint : public IVertex {

};

class FilterStructureDiagram : public Digraph {

};
