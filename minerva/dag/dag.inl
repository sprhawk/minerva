#include <cstdint>
#include <functional>
#include <queue>
#include <sstream>

namespace minerva {

template<class D, class O>
Dag<D, O>::~Dag() {
  for (auto i: index_to_node_) {
    delete i.second;
  }
}

template<class D, class O>
typename Dag<D, O>::DNode* Dag<D, O>::NewDataNode(const D& data) {
  typedef Dag<D, O>::DNode DNode;
  DNode* ret = new DNode;
  ret->set_data(data);
  ret->set_node_id( NewIndex() );
  index_to_node_.insert(std::make_pair(ret->node_id(), ret));
  return ret;
}

template<class D, class O>
typename Dag<D, O>::ONode* Dag<D, O>::NewOpNode(
    std::initializer_list<DataNode<D, O>*> inputs,
    std::initializer_list<DataNode<D, O>*> outputs, const O& op) {
  typedef OpNode<D, O> ONode;
  ONode* ret = new ONode;
  ret->set_op(op);
  ret->set_node_id( NewIndex() );
  index_to_node_.insert(std::make_pair(ret->node_id(), ret));
  for(auto in : inputs) {
    ret->AddParent(in);
  }
  for(auto out : outputs) {
    out->AddParent(ret);
  }
  return ret;
}

template<class D, class O>
uint64_t NewIndex() {
  static uint64_t index_counter = 0;
  return index_counter++;
}

template<class D, class O>
std::string Dag<D, O>::PrintDag() const {
  std::ostringstream out;
  out << "digraph G {" << std::endl;
  for (auto i: index_to_node_) {
    out << "  " << i.first << " [shape=";
    if (i.second->Type() == DagNode::OP_NODE) {
      out << "box";
    } else {
      out << "ellipse";
    }
    out << "];" << std::endl;
    for (auto j: i.second->successors_) {
      out << "  " << i.first << " -> " << j->node_id() << ";" << std::endl;
    }
  }
  out << "}";
  return out.str();
}

} // end of namespace minerva
