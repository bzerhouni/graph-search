#ifndef GRAPH_SEARCH_GRAPH_H
#define GRAPH_SEARCH_GRAPH_H
#include "gs/ids.hpp"
#include "gs/fwd.hpp"
#include <utility>
#include <span>
#include <ranges>
#include <vector>

namespace gs {

/**
 *
 * @tparam node_properties attached to every node
 * @tparam edge_properties attached to every edge
 * @tparam gs_dir Directed or Undirected, fixed at compile time
 */
template < class node_properties = gs_empty, class edge_properties = gs_empty,
           gs_direction gs_dir = gs_direction::Directed >
class gs_graph {
  public:
    using Node = gs_node< node_properties >;
    using Edge = gs_edge< edge_properties >;
    static constexpr gs_direction direction = gs_dir;

    /// Yielded by adjacent(); supports structured bindings.
    struct adjacency {
        gs_edge_id edge;
        gs_node_id node;
    };

    // --- construction
    gs_node_id add_node(node_properties p = {}) {
        const gs_node_id id{static_cast< std::uint32_t >(m_nodes.size())};
        m_nodes.push_back(Node{id, std::move(p)});
        return id;
    }

    gs_edge_id add_edge(gs_node_id u, gs_node_id v, edge_properties p = {}) {
        const gs_edge_id id{static_cast< std::uint32_t >(m_edges.size())};
        m_edges.push_back(Edge{id, u, v, std::move(p)});
        m_nodes[gs_idx(u)].m_incident.push_back(id);
        if constexpr (direction == gs_direction::Undirected) {
            if (u != v)
                m_nodes[gs_idx(v)].m_incident.push_back(id);
        }
        return id;
    }

    void reserve_nodes(std::size_t n) { m_nodes.reserve(n); }
    void reserve_edges(std::size_t m) { m_edges.reserve(m); }

    // --- size
    [[nodiscard]] std::size_t num_nodes() const noexcept { return m_nodes.size(); }
    [[nodiscard]] std::size_t num_edges() const noexcept { return m_edges.size(); }

    // --- data access
    [[nodiscard]] Node &operator[](const gs_node_id n) noexcept { return m_nodes[gs_idx(n)]; }
    [[nodiscard]] const Node &operator[](const gs_node_id n) const noexcept { return m_nodes[gs_idx(n)]; }
    [[nodiscard]] Edge &operator[](const gs_edge_id e) noexcept { return m_edges[gs_idx(e)]; }
    [[nodiscard]] const Edge &operator[](const gs_edge_id e) const noexcept { return m_edges[gs_idx(e)]; }

    // --- topology
    [[nodiscard]] gs_node_id source(const gs_edge_id e) const noexcept { return m_edges[gs_idx(e)].source(); }
    [[nodiscard]] gs_node_id target(const gs_edge_id e) const noexcept { return m_edges[gs_idx(e)].target(); }

    [[nodiscard]] gs_node_id neighbor(const gs_edge_id e, const gs_node_id u) const noexcept {
        return m_edges[gs_idx(e)].neighbor(u);
    }

    [[nodiscard]] std::span< const gs_edge_id > incident_edges(const gs_node_id u) const noexcept {
        return m_nodes[gs_idx(u)].incident_edges();
    }
    [[nodiscard]] std::size_t degree(const gs_node_id u) const noexcept {
        return m_nodes[gs_idx(u)].degree();
    }

    // --- ranges
    [[nodiscard]] auto nodes() const noexcept {
        return std::views::iota(std::uint32_t{0}, static_cast< std::uint32_t >(m_nodes.size())) |
               std::views::transform([](const std::uint32_t i) { return gs_node_id{i}; });
    }

    [[nodiscard]] auto all_edges() const noexcept {
        return std::views::iota(std::uint32_t{0}, static_cast< std::uint32_t >(m_edges.size())) |
               std::views::transform([](const std::uint32_t i) { return gs_edge_id{i}; });
    }

    /// The primary traversal primitive:  for (auto [e, v] : g.adjacent(u))
    [[nodiscard]] auto adjacent(gs_node_id u) const noexcept {
        return incident_edges(u) |
               std::views::transform([this, u](gs_edge_id e) { return adjacency{e, neighbor(e, u)}; });
    }

  private:
    std::vector< Node > m_nodes;
    std::vector< Edge > m_edges;
};

} // namespace gs

#endif // GRAPH_SEARCH_GRAPH_H
