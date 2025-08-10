#include <vector>

// Signal Flow Graphs

class Node {
	public:
		std::vector<Branch> inputs;
		std::vector<Branch> outputs;

};

class Branch {

};

class SignalFlowGraph {

};

// Block Diagrams

class Block {

};

class Signal {

};

class SummingJunction {

};

class TakeoffPoint {

};

class BlockDiagram {

};

// DSP Signal FLow Graphs

class Delay {

};

class Adder {

};

class Multiplier {

};

class BranchPoint {

};

class FilterStructureDiagram {
	public:
		std::vector<Delay> delays;
		std::vector<Adder> adders;
		std::vector<Multiplier> multipliers;
		std::vector<BranchPoint> branches;
};
