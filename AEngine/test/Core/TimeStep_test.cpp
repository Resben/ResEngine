#include <Catch2/catch_test_macros.hpp>
#include <Catch2/matchers/catch_matchers_floating_point.hpp>
#include <AEngine/Core/TimeStep.h>

using namespace AEngine;
using namespace Catch::Matchers;
TEST_CASE( "Constructors initialise correct values", "[TimeStep]" ) {
    REQUIRE_THAT( TimeStep().Seconds(), WithinRel(0.0f) );
    REQUIRE_THAT( TimeStep(1.0f).Seconds(), WithinRel(1.0f) );
}

TEST_CASE( "TimeStep::Seconds() returns correct value", "[TimeStep]" ) {
    REQUIRE_THAT( TimeStep(1.0f).Seconds(), WithinRel(1.0f ) );
    REQUIRE_THAT( TimeStep(0.5f).Seconds(), WithinRel(0.5f ) );
    REQUIRE_THAT( TimeStep(0.0f).Seconds(), WithinRel(0.0f ) );
}

TEST_CASE( "TimeStep::Milliseconds() returns correct value", "[TimeStep]" ) {
    REQUIRE_THAT( TimeStep(1.0f).Milliseconds(), WithinRel(1000.0f) );
    REQUIRE_THAT( TimeStep(0.5f).Milliseconds(), WithinRel(500.0f) );
    REQUIRE_THAT( TimeStep(0.0f).Milliseconds(), WithinRel(0.0f) );
}

TEST_CASE( "TimeStep::Microseconds() returns correct value", "[TimeStep]" ) {
    REQUIRE_THAT( TimeStep(1.0f).Microseconds(), WithinRel(1000000.0f) );
    REQUIRE_THAT( TimeStep(0.5f).Microseconds(), WithinRel(500000.0f) );
    REQUIRE_THAT( TimeStep(0.0f).Microseconds(), WithinRel(0.0f) );
}

TEST_CASE( "Scale timesteps", "[TimeStep]" ) {
    TimeStep ts(10.0f);

    SECTION( "Seconds") {
        REQUIRE_THAT( ts.Seconds(2.0f), WithinRel(20.0f) );
        REQUIRE_THAT( ts.Seconds(0.5f), WithinRel(5.0f) );
        REQUIRE_THAT( ts.Seconds(0.0f), WithinRel(0.0f) );
        REQUIRE_THAT( ts.Seconds(-1.0f), WithinRel(-10.0f) );
    }

    SECTION( "Milliseconds") {
        REQUIRE_THAT( ts.Milliseconds(2.0f), WithinRel(20000.0f) );
        REQUIRE_THAT( ts.Milliseconds(0.5f), WithinRel(5000.0f) );
        REQUIRE_THAT( ts.Milliseconds(0.0f), WithinRel(0.0f) );
        REQUIRE_THAT( ts.Milliseconds(-1.0f), WithinRel(-10000.0f) );
    }

    SECTION( "Microseconds") {
        REQUIRE_THAT( ts.Microseconds(2.0f), WithinRel(20000000.0f) );
        REQUIRE_THAT( ts.Microseconds(0.5f), WithinRel(5000000.0f) );
        REQUIRE_THAT( ts.Microseconds(0.0f), WithinRel(0.0f) );
        REQUIRE_THAT( ts.Microseconds(-1.0f), WithinRel(-10000000.0f) );

    }

    SECTION( "Nanoseconds") {
        REQUIRE_THAT( ts.Nanoseconds(2.0f), WithinRel(20000000000.0f) );
        REQUIRE_THAT( ts.Nanoseconds(0.5f), WithinRel(5000000000.0f) );
        REQUIRE_THAT( ts.Nanoseconds(0.0f), WithinRel(0.0f) );
        REQUIRE_THAT( ts.Nanoseconds(-1.0f), WithinRel(-10000000000.0f) );
    }
}

TEST_CASE( "Implicit float conversion works" , "[TimeStep]") {
    TimeStep ts(1.0f);
    float f = ts;
    REQUIRE_THAT( f, WithinRel(1.0f) );
}

TEST_CASE( "TimeStep::operator+() works", "[TimeStep]" ) {
    TimeStep ts1(1.0f);
    TimeStep ts2(2.0f);
    TimeStep ts3 = ts1 + ts2;
    REQUIRE_THAT( ts3.Seconds(), WithinRel(3.0f) );
}

TEST_CASE( "Timestep::operator=() works", "[TimeStep]") {
    TimeStep ts1(1.0f);
    TimeStep ts2(2.0f);
    ts1 = ts2;
    REQUIRE_THAT( ts1.Seconds(), WithinRel(2.0f) );
}

TEST_CASE( "TimeStep::operator+=() works", "[TimeStep]" ) {
    TimeStep ts1(1.0f);
    TimeStep ts2(2.0f);
    ts1 += ts2;
    REQUIRE_THAT( ts1.Seconds(), WithinRel(3.0f) );
}

TEST_CASE( "TimeStep::operator-() works", "[TimeStep]" ) {
    TimeStep ts1(1.0f);
    TimeStep ts2(2.0f);
    TimeStep ts3 = ts1 - ts2;
    REQUIRE_THAT( ts3.Seconds(), WithinRel(-1.0f) );
}