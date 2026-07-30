#ifndef GRAPH_SEARCH_NODE_H
#define GRAPH_SEARCH_NODE_H
#include "fwd.hpp"
#include "ids.hpp"
#include <span>
#include <vector>

namespace gs
{

/**
 * A node: an identity and the user's properties.
 *
 * @tparam node_properties user-supplied type; must be move-constructible
 */
template <class node_properties = gs_empty>
class gs_node {
public:
    [[nodiscard]] gs_node_id id() const noexcept { return m_id; }

    [[nodiscard]] node_properties &properties() noexcept { return m_properties; }
    [[nodiscard]] const node_properties &properties() const noexcept { return m_properties; }

    /// Directed: edges leaving this node. Undirected: all edges touching it.
    [[nodiscard]] std::span<const gs_edge_id> incident_edges() const noexcept {
        return m_incident;
    }
    /// Directed: out degree of the given node. Undirected: real degree of the given node.
    [[nodiscard]] std::size_t degree() const noexcept { return m_incident.size(); }

private:
    template <class, class, gs_direction> friend class gs_graph;

    gs_node(const gs_node_id id, node_properties p): m_id(id), m_properties(std::move(p)) {}

    const gs_node_id m_id;
    [[no_unique_address]] node_properties m_properties;
    std::vector<gs_edge_id> m_incident;
};

}

#endif //GRAPH_SEARCH_NODE_H
