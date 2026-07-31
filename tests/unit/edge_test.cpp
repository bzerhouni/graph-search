#include <boost/test/unit_test.hpp>

#include <gs/graph.hpp>

using namespace gs;

namespace {
struct weight_t {
    double value = 0.0;
};
} // namespace

BOOST_AUTO_TEST_SUITE(edge)

BOOST_AUTO_TEST_CASE(edge_reports_id_and_endpoints) {
    gs_digraph<> g;
    const gs_node_id a = g.add_node();
    const gs_node_id b = g.add_node();
    const gs_edge_id e = g.add_edge(a, b);

    BOOST_CHECK(g[e].id() == e);
    BOOST_CHECK(g[e].source() == a);
    BOOST_CHECK(g[e].target() == b);
}

BOOST_AUTO_TEST_CASE(neighbor_resolves_the_other_endpoint) {
    gs_digraph<> g;
    const gs_node_id a = g.add_node();
    const gs_node_id b = g.add_node();
    const gs_edge_id e = g.add_edge(a, b);

    BOOST_CHECK(g[e].neighbor(a) == b);
    BOOST_CHECK(g[e].neighbor(b) == a);
}

BOOST_AUTO_TEST_CASE(neighbor_of_self_loop_is_itself) {
    gs_digraph<> g;
    const gs_node_id a = g.add_node();
    const gs_edge_id e = g.add_edge(a, a);

    BOOST_CHECK(g[e].source() == a);
    BOOST_CHECK(g[e].target() == a);
    BOOST_CHECK(g[e].neighbor(a) == a);
}

BOOST_AUTO_TEST_CASE(properties_are_stored_and_mutable) {
    gs_digraph< gs_empty, weight_t > g;
    const gs_node_id a = g.add_node();
    const gs_node_id b = g.add_node();
    const gs_edge_id e = g.add_edge(a, b, weight_t{2.5});

    BOOST_CHECK_EQUAL(g[e].properties().value, 2.5);
    g[e].properties().value = 4.0;
    BOOST_CHECK_EQUAL(g[e].properties().value, 4.0);

    const auto &cg = g;
    BOOST_CHECK_EQUAL(cg[e].properties().value, 4.0);
}

BOOST_AUTO_TEST_CASE(edge_ids_are_assigned_sequentially) {
    gs_digraph<> g;
    const gs_node_id a = g.add_node();
    const gs_node_id b = g.add_node();
    const gs_edge_id e0 = g.add_edge(a, b);
    const gs_edge_id e1 = g.add_edge(b, a);
    BOOST_CHECK_EQUAL(gs_idx(e0), 0u);
    BOOST_CHECK_EQUAL(gs_idx(e1), 1u);
}

BOOST_AUTO_TEST_SUITE_END()
