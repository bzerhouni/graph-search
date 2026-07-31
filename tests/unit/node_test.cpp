#include <boost/test/unit_test.hpp>

#include <gs/graph.hpp>

#include <string>

using namespace gs;

namespace {
struct label_t {
    std::string name;
};
} // namespace

BOOST_AUTO_TEST_SUITE(node)

BOOST_AUTO_TEST_CASE(default_constructed_node_has_no_incident_edges) {
    gs_digraph<> g;
    const gs_node_id n = g.add_node();
    BOOST_CHECK_EQUAL(g[n].degree(), 0u);
    BOOST_CHECK(g[n].incident_edges().empty());
}

BOOST_AUTO_TEST_CASE(node_reports_its_own_id) {
    gs_digraph<> g;
    g.add_node();
    const gs_node_id n1 = g.add_node();
    BOOST_CHECK(g[n1].id() == n1);
    BOOST_CHECK_EQUAL(gs_idx(g[n1].id()), 1u);
}

BOOST_AUTO_TEST_CASE(properties_are_stored_and_mutable) {
    gs_digraph< label_t > g;
    const gs_node_id n = g.add_node(label_t{"start"});
    BOOST_CHECK_EQUAL(g[n].properties().name, "start");

    g[n].properties().name = "renamed";
    BOOST_CHECK_EQUAL(g[n].properties().name, "renamed");

    const auto &cg = g;
    BOOST_CHECK_EQUAL(cg[n].properties().name, "renamed");
}

BOOST_AUTO_TEST_CASE(incident_edges_reflect_directed_out_edges) {
    gs_digraph<> g;
    const gs_node_id a = g.add_node();
    const gs_node_id b = g.add_node();
    const gs_node_id c = g.add_node();
    const gs_edge_id ab = g.add_edge(a, b);
    const gs_edge_id ac = g.add_edge(a, c);

    BOOST_CHECK_EQUAL(g[a].degree(), 2u);
    const auto incident = g[a].incident_edges();
    BOOST_REQUIRE_EQUAL(incident.size(), 2u);
    BOOST_CHECK(incident[0] == ab);
    BOOST_CHECK(incident[1] == ac);

    // b and c are only targets in a directed graph: no out-edges recorded.
    BOOST_CHECK_EQUAL(g[b].degree(), 0u);
    BOOST_CHECK_EQUAL(g[c].degree(), 0u);
}

BOOST_AUTO_TEST_SUITE_END()
