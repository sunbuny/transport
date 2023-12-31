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
#include <uuid.h>
#include <string>
#include "packet.h"
#include <catch2/catch_test_macros.hpp>


//////////////////////////////////////////////////
TEST_CASE("BasicHeaderAPI", "[PacketTest]")
{
  std::string topic = "topic_test";
  uuid_t guid;
  uuid_generate(guid);
  transport::Header header(TRNSP_VERSION, guid, topic, ADV, 0);

  std::string guidStr = transport::GetGuidStr(guid);

  // Check Header getters
  REQUIRE(header.GetVersion()== TRNSP_VERSION);
  std::string otherGuidStr = transport::GetGuidStr(header.GetGuid());
  REQUIRE(guidStr== otherGuidStr);
  REQUIRE(header.GetTopicLength()== topic.size());
  REQUIRE(header.GetTopic()== topic);
  REQUIRE(header.GetType()== ADV);
  REQUIRE(header.GetFlags()== 0);
  int headerLength = sizeof(header.GetVersion()) + sizeof(header.GetGuid()) +
    sizeof(header.GetTopicLength()) + topic.size() + sizeof(header.GetType()) +
    sizeof(header.GetFlags());
  REQUIRE(header.GetHeaderLength()== headerLength);

  // Check Header setters
  header.SetVersion(TRNSP_VERSION + 1);
  REQUIRE(header.GetVersion()== TRNSP_VERSION + 1);
  uuid_generate(guid);
  header.SetGuid(guid);
  otherGuidStr = transport::GetGuidStr(header.GetGuid());
  REQUIRE(guidStr != otherGuidStr);
  topic = "a_new_topic_test";
  header.SetTopic(topic);
  REQUIRE(header.GetTopic()== topic);
  REQUIRE(header.GetTopicLength()== topic.size());
  header.SetType(SUB);
  REQUIRE(header.GetType()== SUB);
  header.SetFlags(1);
  REQUIRE(header.GetFlags()== 1);
  headerLength = sizeof(header.GetVersion()) + sizeof(header.GetGuid()) +
    sizeof(header.GetTopicLength()) + topic.size() + sizeof(header.GetType()) +
    sizeof(header.GetFlags());
  REQUIRE(header.GetHeaderLength()== headerLength);
}

//////////////////////////////////////////////////
TEST_CASE( "HeaderIO", "[PacketTest]")
{
  std::string guidStr;
  std::string otherGuidStr;
  std::string topic = "topic_test";

  uuid_t guid;
  uuid_generate(guid);

  // Pack a Header
  transport::Header header(TRNSP_VERSION, guid, topic, ADV_SVC, 2);
  char *buffer = new char[header.GetHeaderLength()];
  size_t bytes = header.Pack(buffer);
  REQUIRE(bytes== header.GetHeaderLength());

  // Unpack the Header
  transport::Header otherHeader;
  otherHeader.Unpack(buffer);
  delete[] buffer;

  // Check that after Pack() and Unpack() the Header remains the same
  REQUIRE(header.GetVersion()== otherHeader.GetVersion());
  guidStr = transport::GetGuidStr(guid);
  otherGuidStr = transport::GetGuidStr(otherHeader.GetGuid());
  REQUIRE(guidStr== otherGuidStr);
  REQUIRE(header.GetTopicLength()== otherHeader.GetTopicLength());
  REQUIRE(header.GetTopic()== otherHeader.GetTopic());
  REQUIRE(header.GetType()== otherHeader.GetType());
  REQUIRE(header.GetFlags()== otherHeader.GetFlags());
  REQUIRE(header.GetHeaderLength()== otherHeader.GetHeaderLength());
}

//////////////////////////////////////////////////
TEST_CASE( "BasicAdvMsgAPI", "[PacketTest]")
{
  std::string topic = "topic_test";
  uuid_t guid;
  uuid_generate(guid);
  transport::Header otherHeader(TRNSP_VERSION, guid, topic, ADV, 3);

  std::string otherGuidStr = transport::GetGuidStr(guid);

  std::string address = "tcp://10.0.0.1:6000";
  transport::AdvMsg advMsg(otherHeader, address);

  // Check AdvMsg getters
  transport::Header header = advMsg.GetHeader();
  REQUIRE(header.GetVersion()== otherHeader.GetVersion());
  std::string guidStr = transport::GetGuidStr(header.GetGuid());
  REQUIRE(guidStr== otherGuidStr);
  REQUIRE(header.GetTopicLength()== otherHeader.GetTopicLength());
  REQUIRE(header.GetTopic()== otherHeader.GetTopic());
  REQUIRE(header.GetType()== otherHeader.GetType());
  REQUIRE(header.GetFlags()== otherHeader.GetFlags());
  REQUIRE(header.GetHeaderLength()== otherHeader.GetHeaderLength());

  REQUIRE(advMsg.GetAddressLength()== address.size());
  REQUIRE(advMsg.GetAddress()== address);
  size_t msgLength = advMsg.GetHeader().GetHeaderLength() +
    sizeof(advMsg.GetAddressLength()) + advMsg.GetAddress().size();
  REQUIRE(advMsg.GetMsgLength()== msgLength);

  uuid_generate(guid);
  topic = "a_new_topic_test";

  // Check AdvMsg setters
  transport::Header anotherHeader(TRNSP_VERSION + 1, guid, topic, ADV_SVC, 3);
  guidStr = transport::GetGuidStr(guid);
  advMsg.SetHeader(anotherHeader);
  header = advMsg.GetHeader();
  REQUIRE(header.GetVersion()== TRNSP_VERSION + 1);
  otherGuidStr = transport::GetGuidStr(anotherHeader.GetGuid());
  REQUIRE(guidStr== otherGuidStr);
  REQUIRE(header.GetTopicLength()== topic.size());
  REQUIRE(header.GetTopic()== topic);
  REQUIRE(header.GetType()== ADV_SVC);
  REQUIRE(header.GetFlags()== 3);
  int headerLength = sizeof(header.GetVersion()) + sizeof(header.GetGuid()) +
    sizeof(header.GetTopicLength()) + topic.size() + sizeof(header.GetType()) +
    sizeof(header.GetFlags());
  REQUIRE(header.GetHeaderLength()== headerLength);

  address = "inproc://local";
  advMsg.SetAddress(address);
  REQUIRE(advMsg.GetAddress()== address);
}

//////////////////////////////////////////////////
TEST_CASE( "AdvMsgIO", "[PacketTest]")
{
  uuid_t guid;
  uuid_generate(guid);
  std::string topic = "topic_test";

  // Pack an AdvMsg
  transport::Header otherHeader(TRNSP_VERSION, guid, topic, ADV, 3);
  std::string address = "tcp://10.0.0.1:6000";
  transport::AdvMsg advMsg(otherHeader, address);
  char *buffer = new char[advMsg.GetMsgLength()];
  size_t bytes = advMsg.Pack(buffer);
  REQUIRE(bytes== advMsg.GetMsgLength());

  // Unpack an AdvMsg
  transport::Header header;
  transport::AdvMsg otherAdvMsg;
  size_t headerBytes = header.Unpack(buffer);
  REQUIRE(headerBytes== header.GetHeaderLength());
  otherAdvMsg.SetHeader(header);
  char *pBody = buffer + header.GetHeaderLength();
  size_t bodyBytes = otherAdvMsg.UnpackBody(pBody);
  delete[] buffer;

  // Check that after Pack() and Unpack() the data does not change
  REQUIRE(otherAdvMsg.GetAddressLength()== advMsg.GetAddressLength());
  REQUIRE(otherAdvMsg.GetAddress()== advMsg.GetAddress());
  REQUIRE(otherAdvMsg.GetMsgLength()== advMsg.GetMsgLength());
  REQUIRE(otherAdvMsg.GetMsgLength() -
            otherAdvMsg.GetHeader().GetHeaderLength()== advMsg.GetMsgLength() -
            advMsg.GetHeader().GetHeaderLength());
  REQUIRE(bodyBytes== otherAdvMsg.GetMsgLength() -
            otherAdvMsg.GetHeader().GetHeaderLength());
}


