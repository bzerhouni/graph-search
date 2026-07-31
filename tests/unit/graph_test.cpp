#include <boost/test/unit_test.hpp>

#include <gs/graph.hpp>

#include <vector>

using namespace gs;

BOOST_AUTO_TEST_SUITE(graph)

BOOST_AUTO_TEST_CASE(empty_graph_has_no_nodes_or_edges) {
    gs_digraph<> g;
    BOOST_CHECK_EQUAL(g.num_nodes(), 0u);
    BOOST_CHECK_EQUAL(g.num_edges(), 0u);
}

BOOST_AUTO_TEST_CASE(add_node_assigns_sequential_ids) {
    gs_digraph<> g;
    const gs_node_id n0 = g.add_node();
    const gs_node_id n1 = g.add_node();
    const gs_node_id n2 = g.add_node();
    BOOST_CHECK_EQUAL(gs_idx(n0), 0u);
    BOOST_CHECK_EQUAL(gs_idx(n1), 1u);
    BOOST_CHECK_EQUAL(gs_idx(n2), 2u);
    BOOST_CHECK_EQUAL(g.num_nodes(), 3u);
}

BOOST_AUTO_TEST_CASE(reserve_does_not_change_size) {
    gs_digraph<> g;
    g.reserve_nodes(100);
    g.reserve_edges(100);
    BOOST_CHECK_EQUAL(g.num_nodes(), 0u);
    BOOST_CHECK_EQUAL(g.num_edges(), 0u);
    g.add_node();
    BOOST_CHECK_EQUAL(g.num_nodes(), 1u);
}

BOOST_AUTO_TEST_CASE(source_and_target_accessors_match_edge) {
    gs_digraph<> g;
    const gs_node_id a = g.add_node();
    const gs_node_id b = g.add_node();
    const gs_edge_id e = g.add_edge(a, b);
    BOOST_CHECK(g.source(e) == a);
    BOOST_CHECK(g.target(e) == b);
    BOOST_CHECK(g.neighbor(e, a) == b);
}

BOOST_AUTO_TEST_CASE(directed_add_edge_only_updates_source_incidence) {
    gs_digraph<> g;
    const gs_node_id a = g.add_node();
    const gs_node_id b = g.add_node();
    g.add_edge(a, b);

    BOOST_CHECK_EQUAL(g.degree(a), 1u);
    BOOST_CHECK_EQUAL(g.degree(b), 0u);
}

BOOST_AUTO_TEST_CASE(undirected_add_edge_updates_both_endpoints) {
    gs_ugraph<> g;
    const gs_node_id a = g.add_node();
    const gs_node_id b = g.add_node();
    const gs_edge_id e = g.add_edge(a, b);

    BOOST_CHECK_EQUAL(g.degree(a), 1u);
    BOOST_CHECK_EQUAL(g.degree(b), 1u);
    BOOST_CHECK(g.incident_edges(a)[0] == e);
    BOOST_CHECK(g.incident_edges(b)[0] == e);
}

BOOST_AUTO_TEST_CASE(undirected_self_loop_is_recorded_once) {
    gs_ugraph<> g;
    const gs_node_id a = g.add_node();
    g.add_edge(a, a);

    // The self-loop guard (u != v) means a self-loop contributes a single
    // incidence entry rather than two, even though it touches the node twice.
    BOOST_CHECK_EQUAL(g.degree(a), 1u);
}

BOOST_AUTO_TEST_CASE(nodes_range_yields_every_node_id_in_order) {
    gs_digraph<> g;
    g.add_node();
    g.add_node();
    g.add_node();

    std::vector< std::uint32_t > seen;
    for (const gs_node_id n : g.nodes())
        seen.push_back(gs_idx(n));

    BOOST_CHECK((seen == std::vector< std::uint32_t >{0, 1, 2}));
}

BOOST_AUTO_TEST_CASE(all_edges_range_yields_every_edge_id_in_order) {
    gs_digraph<> g;
    const gs_node_id a = g.add_node();
    const gs_node_id b = g.add_node();
    g.add_edge(a, b);
    g.add_edge(b, a);

    std::vector< std::uint32_t > seen;
    for (const gs_edge_id e : g.all_edges())
        seen.push_back(gs_idx(e));

    BOOST_CHECK((seen == std::vector< std::uint32_t >{0, 1}));
}

BOOST_AUTO_TEST_CASE(adjacent_yields_edge_and_neighbor_pairs_directed) {
    gs_digraph<> g;
    const gs_node_id a = g.add_node();
    const gs_node_id b = g.add_node();
    const gs_node_id c = g.add_node();
    const gs_edge_id ab = g.add_edge(a, b);
    const gs_edge_id ac = g.add_edge(a, c);

    std::vector< std::pair< std::uint32_t, std::uint32_t > > seen;
    for (auto [e, v] : g.adjacent(a))
        seen.emplace_back(gs_idx(e), gs_idx(v));

    const std::vector< std::pair< std::uint32_t, std::uint32_t > > expected{{gs_idx(ab), gs_idx(b)},
                                                                            {gs_idx(ac), gs_idx(c)}};
    BOOST_CHECK(seen == expected);
}

BOOST_AUTO_TEST_CASE(adjacent_resolves_correct_neighbor_from_either_endpoint_undirected) {
    gs_ugraph<> g;
    const gs_node_id a = g.add_node();
    const gs_node_id b = g.add_node();
    g.add_edge(a, b);

    auto a_adj = g.adjacent(a);
    auto a_it = a_adj.begin();
    BOOST_REQUIRE(a_it != a_adj.end());
    BOOST_CHECK((*a_it).node == b);

    auto b_adj = g.adjacent(b);
    auto b_it = b_adj.begin();
    BOOST_REQUIRE(b_it != b_adj.end());
    BOOST_CHECK((*b_it).node == a);
}

BOOST_AUTO_TEST_CASE(direction_is_exposed_as_static_constant) {
    BOOST_CHECK(gs_digraph<>::direction == gs_direction::Directed);
    BOOST_CHECK(gs_ugraph<>::direction == gs_direction::Undirected);
}

BOOST_AUTO_TEST_SUITE_END()
