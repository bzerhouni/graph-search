#include <boost/test/unit_test.hpp>

#include <gs/graph.hpp>
#include <gs/map.hpp>

using namespace gs;

namespace {
gs_digraph<> make_graph(const std::size_t nodes, const std::size_t edges) {
    gs_digraph<> g;
    for (std::size_t i = 0; i < nodes; ++i)
        g.add_node();
    for (std::size_t i = 0; (i < edges) && (nodes > 1); ++i)
        g.add_edge(gs_node_id{static_cast< std::uint32_t >(i % nodes)},
                   gs_node_id{static_cast< std::uint32_t >((i + 1) % nodes)});
    return g;
}
} // namespace

BOOST_AUTO_TEST_SUITE(node_map)

BOOST_AUTO_TEST_CASE(dense_map_initializes_every_node_to_the_given_default) {
    const auto g = make_graph(3, 0);
    gs_node_map< int > m(g, 7);
    BOOST_CHECK_EQUAL(m.size(), 3u);
    BOOST_CHECK_EQUAL(m[gs_node_id{0}], 7);
    BOOST_CHECK_EQUAL(m[gs_node_id{2}], 7);
    BOOST_CHECK(m.contains(gs_node_id{0}));
}

BOOST_AUTO_TEST_CASE(dense_map_defaults_to_value_initialized_element) {
    const auto g = make_graph(2, 0);
    gs_node_map< int > m(g);
    BOOST_CHECK_EQUAL(m[gs_node_id{0}], 0);
}

BOOST_AUTO_TEST_CASE(dense_map_writes_are_independent_per_node) {
    const auto g = make_graph(3, 0);
    gs_node_map< int > m(g, 0);
    m[gs_node_id{1}] = 42;
    BOOST_CHECK_EQUAL(m[gs_node_id{0}], 0);
    BOOST_CHECK_EQUAL(m[gs_node_id{1}], 42);
    BOOST_CHECK_EQUAL(m[gs_node_id{2}], 0);
}

BOOST_AUTO_TEST_CASE(stamped_map_starts_with_nothing_touched) {
    const auto g = make_graph(3, 0);
    gs_stamped_node_map< int > m(g, -1);
    BOOST_CHECK(!m.contains(gs_node_id{0}));
    BOOST_CHECK_EQUAL(m[gs_node_id{0}], -1); // reads still return the default
}

BOOST_AUTO_TEST_CASE(stamped_map_write_marks_entry_as_touched) {
    const auto g = make_graph(3, 0);
    gs_stamped_node_map< int > m(g, -1);
    m[gs_node_id{1}] = 99;
    BOOST_CHECK(m.contains(gs_node_id{1}));
    BOOST_CHECK_EQUAL(m[gs_node_id{1}], 99);
    BOOST_CHECK(!m.contains(gs_node_id{0}));
}

BOOST_AUTO_TEST_CASE(stamped_map_reset_reverts_all_entries_to_default) {
    const auto g = make_graph(3, 0);
    gs_stamped_node_map< int > m(g, -1);
    m[gs_node_id{0}] = 1;
    m[gs_node_id{1}] = 2;

    m.reset();

    BOOST_CHECK(!m.contains(gs_node_id{0}));
    BOOST_CHECK(!m.contains(gs_node_id{1}));
    BOOST_CHECK_EQUAL(m[gs_node_id{0}], -1);
}

BOOST_AUTO_TEST_CASE(stamped_map_reset_can_change_the_default) {
    const auto g = make_graph(2, 0);
    gs_stamped_node_map< int > m(g, -1);
    m[gs_node_id{0}] = 5;

    m.reset(100);

    BOOST_CHECK_EQUAL(m[gs_node_id{0}], 100);
    BOOST_CHECK_EQUAL(m[gs_node_id{1}], 100);
}

BOOST_AUTO_TEST_CASE(sparse_map_returns_default_for_untouched_key) {
    const auto g = make_graph(5, 0);
    gs_sparse_node_map< int > m(g, -1);
    BOOST_CHECK(!m.contains(gs_node_id{3}));
    BOOST_CHECK_EQUAL(m[gs_node_id{3}], -1);
}

BOOST_AUTO_TEST_CASE(sparse_map_size_counts_touched_entries_not_node_count) {
    const auto g = make_graph(100, 0);
    gs_sparse_node_map< int > m(g, 0);
    BOOST_CHECK_EQUAL(m.size(), 0u);
    m[gs_node_id{5}] = 1;
    m[gs_node_id{10}] = 1;
    BOOST_CHECK_EQUAL(m.size(), 2u);
    BOOST_CHECK(m.contains(gs_node_id{5}));
    BOOST_CHECK(!m.contains(gs_node_id{0}));
}

BOOST_AUTO_TEST_CASE(sparse_map_clear_removes_all_touched_entries) {
    const auto g = make_graph(10, 0);
    gs_sparse_node_map< int > m(g, 0);
    m[gs_node_id{1}] = 1;
    m[gs_node_id{2}] = 1;
    m.clear();
    BOOST_CHECK_EQUAL(m.size(), 0u);
    BOOST_CHECK(!m.contains(gs_node_id{1}));
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(edge_map)

BOOST_AUTO_TEST_CASE(dense_map_initializes_every_edge_to_the_given_default) {
    const auto g = make_graph(3, 3);
    gs_edge_map< int > m(g, 5);
    BOOST_CHECK_EQUAL(m.size(), 3u);
    BOOST_CHECK_EQUAL(m[gs_edge_id{0}], 5);
    BOOST_CHECK(m.contains(gs_edge_id{0}));
}

BOOST_AUTO_TEST_CASE(stamped_map_reset_reverts_all_entries_to_default) {
    const auto g = make_graph(3, 3);
    gs_stamped_edge_map< double > m(g, 0.0);
    m[gs_edge_id{0}] = 1.5;
    BOOST_CHECK(m.contains(gs_edge_id{0}));

    m.reset();

    BOOST_CHECK(!m.contains(gs_edge_id{0}));
    BOOST_CHECK_EQUAL(m[gs_edge_id{0}], 0.0);
}

BOOST_AUTO_TEST_CASE(sparse_map_size_counts_touched_entries_not_edge_count) {
    const auto g = make_graph(10, 10);
    gs_sparse_edge_map< int > m(g, 0);
    m[gs_edge_id{0}] = 1;
    BOOST_CHECK_EQUAL(m.size(), 1u);
    BOOST_CHECK(!m.contains(gs_edge_id{1}));
}

BOOST_AUTO_TEST_SUITE_END()
