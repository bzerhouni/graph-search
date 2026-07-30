#ifndef GRAPH_SEARCH_FWD_H
#define GRAPH_SEARCH_FWD_H

/// @file fwd.hpp
/// @brief Declarations shared by node.hpp, edge.hpp and graph.hpp.
///
/// gs_node and gs_edge must name gs_graph to befriend it, and gs_graph needs
/// both. Rather than let them include each other, the mutual declarations live
/// here. This header pulls in nothing.

namespace gs {

/// Placeholder for "no properties".
struct gs_empty {};

enum class gs_direction { Directed, Undirected };

template < class node_properties > class gs_node;

template < class edge_properties > class gs_edge;

template < class node_properties, class edge_properties, gs_direction dir > class gs_graph;

} // namespace gs

#endif // GRAPH_SEARCH_FWD_H
