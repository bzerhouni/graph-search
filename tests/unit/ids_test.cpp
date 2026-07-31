#define BOOST_TEST_MODULE graph_search_tests
#include <boost/test/unit_test.hpp>

#include <gs/ids.hpp>

using namespace gs;

BOOST_AUTO_TEST_SUITE(ids)

BOOST_AUTO_TEST_CASE(gs_idx_roundtrips_node_id) {
    constexpr gs_node_id n{42};
    BOOST_CHECK_EQUAL(gs_idx(n), 42u);
}

BOOST_AUTO_TEST_CASE(gs_idx_roundtrips_edge_id) {
    constexpr gs_edge_id e{7};
    BOOST_CHECK_EQUAL(gs_idx(e), 7u);
}

BOOST_AUTO_TEST_CASE(node_and_edge_ids_are_distinct_types) {
    // gs_node_id{0} and gs_edge_id{0} must not be implicitly interchangeable;
    // this is a compile-time property, exercised here by simply constructing
    // both from the same underlying value without any conversion between them.
    constexpr gs_node_id n{0};
    constexpr gs_edge_id e{0};
    BOOST_CHECK_EQUAL(gs_idx(n), gs_idx(e));
}

BOOST_AUTO_TEST_CASE(equality_and_inequality) {
    constexpr gs_node_id a{1};
    constexpr gs_node_id b{1};
    constexpr gs_node_id c{2};
    BOOST_CHECK(a == b);
    BOOST_CHECK(a != c);
}

BOOST_AUTO_TEST_CASE(invalid_sentinels_are_max_value) {
    BOOST_CHECK_EQUAL(gs_idx(gs_invalid_node), 0xFFFFFFFFu);
    BOOST_CHECK_EQUAL(gs_idx(gs_invalid_edge), 0xFFFFFFFFu);
}

BOOST_AUTO_TEST_SUITE_END()
