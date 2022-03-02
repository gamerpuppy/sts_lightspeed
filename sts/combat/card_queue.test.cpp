//
// Created by keega on 2/24/2022.
//

#include <sts/combat/card_queue.hpp>

#include <catch2/catch_all.hpp>

using namespace sts;

TEST_CASE("CardQueue Simple Init") {
    const CardQueue queue;

    REQUIRE(queue.size == 0);
    CHECK(queue.isEmpty() == true);
    CHECK(queue.containsCardWithuUniqueId(0) == false);
}

TEST_CASE("Simple Init") {
    CardQueue queue;

    REQUIRE(queue.size == 0);
    REQUIRE(queue.containsCardWithuUniqueId(45) == false);

    CardQueueItem item;
    item.card.uniqueId = 0;
    CardQueueItem item2;
    item2.card.uniqueId = 1;


    queue.pushBack(item);
    CHECK(queue.size == 1);
    CHECK(queue.containsCardWithuUniqueId(0) == true);
    CHECK(queue.isEmpty() == false);

    queue.pushBack(item2);
    CHECK(queue.size == 2);

    auto poppedItem = queue.popFront();
    CHECK(poppedItem.card == item.card);

    CHECK(queue.size == 1);
    CHECK(queue.containsCardWithuUniqueId(1) == true);
    CHECK(queue.containsCardWithuUniqueId(0) == false);

    queue.popFront();
    CHECK(queue.size == 0);
    CHECK(queue.isEmpty() == true);
}