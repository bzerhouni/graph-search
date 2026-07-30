#ifndef GRAPH_SEARCH_EDGE_H
#define GRAPH_SEARCH_EDGE_H
#include "ids.hpp"
#include "fwd.hpp"
#include <utility>

namespace gs {

/**
 * An edge: an identity, two endpoint *ids*, and the user's properties.
 *
 * @tparam edge_properties user-supplied type; must be move-constructible
 */
template <class edge_properties = gs_empty>
class gs_edge {
public:
    [[nodiscard]] gs_edge_id id() const noexcept { return m_id; }
    [[nodiscard]] gs_node_id source() const noexcept { return m_source; }
    [[nodiscard]] gs_node_id target() const noexcept { return m_target; }

    /// The endpoint that is not `u`. Resolves orientation for undirected graphs.
    [[nodiscard]] gs_node_id neighbor(const gs_node_id u) const noexcept {
        return u == m_source ? m_target : m_source;
    }

    [[nodiscard]] edge_properties &properties() noexcept { return m_properties; }
    [[nodiscard]] const edge_properties &properties() const noexcept { return m_properties; }

private:
    template <class, class, gs_direction> friend class gs_graph;

    gs_edge(const gs_edge_id id, const gs_node_id s, const gs_node_id t, edge_properties p)
        : m_id(id), m_source(s), m_target(t), m_properties(std::move(p)) {}

    const gs_edge_id m_id;
    const gs_node_id m_source;
    const gs_node_id m_target;
    edge_properties m_properties;
};

// aliases for convenient directed / undirected graphs creation
template <class node_properties = gs_empty, class edge_properties = gs_empty>
using gs_digraph = gs_graph<node_properties, edge_properties, gs_direction::Directed>;

template <class node_properties = gs_empty, class edge_properties = gs_empty>
using gs_ugraph = gs_graph<node_properties, edge_properties, gs_direction::Undirected>;

}

#endif //GRAPH_SEARCH_EDGE_H
