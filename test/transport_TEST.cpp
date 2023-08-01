/*
 * Copyright (C) 2014 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/

#include <limits.h>
#include "transport.h"
#include "catch2/catch_test_macros.hpp"

bool callbackExecuted;

//////////////////////////////////////////////////
/// \brief Function is called everytime a topic update is received.
void cb(const std::string &_topic, const std::string &_data)
{
  assert(_topic != "");
  REQUIRE(_data == "someData");
  callbackExecuted = true;
}

TEST_CASE("uuid", "[uuid]"){
    uuid_t guid;
    uuid_generate(guid);
    std::string guidStr = transport::GetGuidStr(guid);

    uuid_t guid1;
    uuid_generate(guid1);
    std::string guidStr1 = transport::GetGuidStr(guid1);
    REQUIRE(guidStr1 != guidStr);
}

//////////////////////////////////////////////////
TEST_CASE("PubWithoutAdvertise", "[transportTest]")
{
	std::string master = "";
	bool verbose = false;
	std::string topic1 = "foo";
	std::string data = "someData";

	// Subscribe to topic1
	transport::Node node(master, verbose);

	// Publish some data on topic1 without advertising it first
	REQUIRE(node.Publish(topic1, data) != 0);
}

//////////////////////////////////////////////////
TEST_CASE("PubSub", "[transportTest]")
{
	callbackExecuted = false;
	std::string master = "";
	bool verbose = true;
	std::string topic1 = "foo";
	std::string data = "someData";

	// Subscribe to topic1
	transport::Node node(master, verbose);
	REQUIRE(node.Subscribe(topic1, cb)== 0);
	node.SpinOnce();

	// Advertise and publish some data on topic1
	REQUIRE(node.Advertise(topic1) == 0);
	REQUIRE(node.Publish(topic1, data)== 0);
	std::this_thread::sleep_for(std::chrono::milliseconds (100));
	node.SpinOnce();

	// Check that the data was received
	REQUIRE(callbackExecuted);
	callbackExecuted = false;

	// Publish a second message on topic1
	REQUIRE(node.Publish(topic1, data)==0);
    std::this_thread::sleep_for(std::chrono::milliseconds (100));
	node.SpinOnce();

	// Check that the data was received
	REQUIRE(callbackExecuted);
	callbackExecuted = false;

	// Unadvertise topic1 and publish a third message
	node.UnAdvertise(topic1);
	REQUIRE(node.Publish(topic1, data)!=0);
    std::this_thread::sleep_for(std::chrono::milliseconds (100));
	node.SpinOnce();
	REQUIRE(!callbackExecuted);
}


