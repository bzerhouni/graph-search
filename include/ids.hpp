#ifndef GRAPH_SEARCH_IDS_HPP
#define GRAPH_SEARCH_IDS_HPP
#include <cstdint>

/// @file ids.hpp
/// @brief Node and edge identity. An id is the index into the graph's storage.

namespace gs {

// --- identity -------------------------------------------------------------
// The id IS the index into the graph's storage. Scoped enums give a distinct
// type (a node id cannot be passed where an edge id is expected) at zero cost.

enum class gs_node_id : std::uint32_t {};
enum class gs_edge_id : std::uint32_t {};

constexpr std::uint32_t gs_idx(gs_node_id n) noexcept { return static_cast< std::uint32_t >(n); }
constexpr std::uint32_t gs_idx(gs_edge_id e) noexcept { return static_cast< std::uint32_t >(e); }

inline constexpr gs_node_id gs_invalid_node{~std::uint32_t{0}};
inline constexpr gs_edge_id gs_invalid_edge{~std::uint32_t{0}};
} // namespace gs

#endif // GRAPH_SEARCH_IDS_HPP
