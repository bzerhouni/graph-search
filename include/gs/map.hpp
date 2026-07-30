#ifndef GRAPH_SEARCH_MAP_HPP
#define GRAPH_SEARCH_MAP_HPP
#include "gs/ids.hpp"
#include <vector>
#include <unordered_map>

/// @file map.hpp
/// @brief Per-node and per-edge scratch storage for search algorithms.
///
/// Three storage policies with the same interface, so an algorithm can be
/// written once and instantiated with whichever fits.
///
///   gs_<node|edge>_map          dense vector. Default. O(n) construction.
///   gs_stamped_<node|edge>_map  dense + generation counter. O(1) logical clear;
///                               only pays off if reused across queries.
///   gs_sparse_<node|edge>_map   hash map. Memory proportional to nodes/edges touched.
///
/// Common interface:
///   Map(const G& g, T init)
///   T&       operator[](gs_node_id)        // inserts init if absent
///   const T& operator[](gs_node_id) const  // returns init if absent
///   bool     contains(gs_node_id) const
///   std::size_t size() const

namespace gs {

// --- node maps ---------------------------------------------------------------------------------------------
/// Dense per-node storage. One indexed load per access; O(n) to construct.
template < class T > class gs_node_map {
  public:
    template < class G >
    explicit gs_node_map(const G &g, T init = T{}) : m_data(g.num_nodes(), std::move(init)) {}

    [[nodiscard]] T &operator[](const gs_node_id n) noexcept { return m_data[gs_idx(n)]; }
    [[nodiscard]] const T &operator[](const gs_node_id n) const noexcept { return m_data[gs_idx(n)]; }
    [[nodiscard]] bool contains(const gs_node_id) const noexcept { return true; }
    [[nodiscard]] std::size_t size() const noexcept { return m_data.size(); }

  private:
    std::vector< T > m_data;
};

/// Dense storage with a generation stamp: reset() is O(1) instead of O(n).
///
/// IMPORTANT: construct once, outside the query loop, and call reset() between
/// queries. A freshly constructed instance still costs O(n) to zero the stamps,
/// so building one per query gains nothing over gs_node_map.
template < class T > class gs_stamped_node_map {
  public:
    template < class G >
    explicit gs_stamped_node_map(const G &g, T init = T{})
        : m_default(std::move(init)), m_value(g.num_nodes()), m_stamp(g.num_nodes(), 0), m_gen(1) {}

    /// Logically clears every entry back to the default. O(1) amortized.
    void reset() noexcept {
        if (++m_gen == 0) { // 2^32 resets: wrap
            std::fill(m_stamp.begin(), m_stamp.end(), std::uint32_t{0});
            m_gen = 1;
        }
    }
    void reset(T init) {
        m_default = std::move(init);
        reset();
    }

    [[nodiscard]] bool contains(const gs_node_id n) const noexcept { return m_stamp[gs_idx(n)] == m_gen; }

    [[nodiscard]] T &operator[](const gs_node_id n) {
        const auto i = gs_idx(n);
        if (m_stamp[i] != m_gen) {
            m_value[i] = m_default;
            m_stamp[i] = m_gen;
        }
        return m_value[i];
    }
    [[nodiscard]] const T &operator[](const gs_node_id n) const noexcept {
        const auto i = gs_idx(n);
        return m_stamp[i] == m_gen ? m_value[i] : m_default;
    }

    [[nodiscard]] std::size_t size() const noexcept { return m_value.size(); }

  private:
    T m_default;
    std::vector< T > m_value;
    std::vector< std::uint32_t > m_stamp;
    std::uint32_t m_gen;
};

/// Hash-backed storage. Memory proportional to the nodes actually touched.
template < class T > class gs_sparse_node_map {
  public:
    /// The graph is accepted and ignored, so that all three policies share a
    /// constructor signature and algorithms stay uniform.
    template < class G > explicit gs_sparse_node_map(const G &, T init = T{}) : m_default(std::move(init)) {}

    [[nodiscard]] T &operator[](const gs_node_id n) {
        return m_data.try_emplace(gs_idx(n), m_default).first->second;
    }
    [[nodiscard]] const T &operator[](const gs_node_id n) const {
        const auto it = m_data.find(gs_idx(n));
        return it == m_data.end() ? m_default : it->second;
    }

    [[nodiscard]] bool contains(const gs_node_id n) const { return m_data.contains(gs_idx(n)); }
    /// Number of entries present, NOT the node count. Differs from the dense
    /// policies by design; algorithms should not depend on it.
    [[nodiscard]] std::size_t size() const noexcept { return m_data.size(); }

    void reserve(std::size_t n) { m_data.reserve(n); }
    void clear() noexcept { m_data.clear(); }

  private:
    T m_default;
    std::unordered_map< std::uint32_t, T > m_data;
};

// --- edge maps ---------------------------------------------------------------------------------------------
/// Dense per-edge storage. One indexed load per access; O(n) to construct.
template < class T > class gs_edge_map {
  public:
    template < class G >
    explicit gs_edge_map(const G &g, T init = T{}) : m_data(g.num_edges(), std::move(init)) {}

    [[nodiscard]] T &operator[](const gs_edge_id n) noexcept { return m_data[gs_idx(n)]; }
    [[nodiscard]] const T &operator[](const gs_edge_id n) const noexcept { return m_data[gs_idx(n)]; }
    [[nodiscard]] bool contains(const gs_edge_id) const noexcept { return true; }
    [[nodiscard]] std::size_t size() const noexcept { return m_data.size(); }

  private:
    std::vector< T > m_data;
};

/// Dense storage with a generation stamp: reset() is O(1) instead of O(n).
///
/// IMPORTANT: construct once, outside the query loop, and call reset() between
/// queries. A freshly constructed instance still costs O(n) to zero the stamps,
/// so building one per query gains nothing over gs_edge_map.
template < class T > class gs_stamped_edge_map {
  public:
    template < class G >
    explicit gs_stamped_edge_map(const G &g, T init = T{})
        : m_default(std::move(init)), m_value(g.num_edges()), m_stamp(g.num_edges(), 0), m_gen(1) {}

    void reset() noexcept {
        if (++m_gen == 0) { // 2^32 resets: wrap
            std::fill(m_stamp.begin(), m_stamp.end(), std::uint32_t{0});
            m_gen = 1;
        }
    }
    void reset(T init) {
        m_default = std::move(init);
        reset();
    }

    [[nodiscard]] bool contains(const gs_edge_id n) const noexcept { return m_stamp[gs_idx(n)] == m_gen; }

    [[nodiscard]] T &operator[](const gs_edge_id n) {
        const auto i = gs_idx(n);
        if (m_stamp[i] != m_gen) {
            m_value[i] = m_default;
            m_stamp[i] = m_gen;
        }
        return m_value[i];
    }
    [[nodiscard]] const T &operator[](const gs_edge_id n) const noexcept {
        const auto i = gs_idx(n);
        return m_stamp[i] == m_gen ? m_value[i] : m_default;
    }

    [[nodiscard]] std::size_t size() const noexcept { return m_value.size(); }

  private:
    T m_default;
    std::vector< T > m_value;
    std::vector< std::uint32_t > m_stamp;
    std::uint32_t m_gen;
};

/// Hash-backed storage. Memory proportional to the edges actually touched.
template < class T > class gs_sparse_edge_map {
  public:
    template < class G > explicit gs_sparse_edge_map(const G &, T init = T{}) : m_default(std::move(init)) {}

    [[nodiscard]] T &operator[](const gs_edge_id n) {
        return m_data.try_emplace(gs_idx(n), m_default).first->second;
    }
    [[nodiscard]] const T &operator[](const gs_edge_id n) const {
        const auto it = m_data.find(gs_idx(n));
        return it == m_data.end() ? m_default : it->second;
    }

    [[nodiscard]] bool contains(const gs_edge_id n) const { return m_data.contains(gs_idx(n)); }
    [[nodiscard]] std::size_t size() const noexcept { return m_data.size(); }

    void reserve(std::size_t n) { m_data.reserve(n); }
    void clear() noexcept { m_data.clear(); }

  private:
    T m_default;
    std::unordered_map< std::uint32_t, T > m_data;
};

} // namespace gs

#endif // GRAPH_SEARCH_MAP_HPP
