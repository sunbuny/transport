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

#include <string>
#include "topicsInfo.h"
#include "catch2/catch_test_macros.hpp"

bool callbackExecuted = false;

//////////////////////////////////////////////////
void myCb(const std::string &p1, const std::string &p2)
{
  callbackExecuted = true;
}

//////////////////////////////////////////////////
void myReqCb(const std::string &p1, int p2, const std::string &p3)
{
  callbackExecuted = true;
}

//////////////////////////////////////////////////
int myRepCb(const std::string &p1, const std::string &p2, std::string &p3)
{
  callbackExecuted = true;
  return 0;
}

//////////////////////////////////////////////////
TEST_CASE("BasicTopicsInfoAPI", "[PacketTest]")
{
  transport::TopicsInfo topics;
  std::string topic = "test_topic";
  std::string address = "tcp://10.0.0.1:6000";
  transport::TopicInfo::Topics_L v;
  transport::TopicInfo::Callback cb;
  transport::TopicInfo::ReqCallback reqCb;
  transport::TopicInfo::RepCallback repCb;

  // Check getters with an empty TopicsInfo object
  REQUIRE(!topics.HasTopic(topic));
  REQUIRE(!topics.GetAdvAddresses(topic, v));
  REQUIRE(!topics.HasAdvAddress(topic, address));
  REQUIRE(!topics.Connected(topic));
  REQUIRE(!topics.Subscribed(topic));
  REQUIRE(!topics.AdvertisedByMe(topic));
  REQUIRE(!topics.Requested(topic));
  REQUIRE(!topics.GetCallback(topic, cb));
  REQUIRE(!topics.GetReqCallback(topic, reqCb));
  REQUIRE(!topics.GetRepCallback(topic, repCb));
  REQUIRE(!topics.PendingReqs(topic));

  // Check getters after inserting a topic in a TopicsInfo object
  topics.AddAdvAddress(topic, address);
  REQUIRE(topics.HasTopic(topic));
  REQUIRE(topics.HasAdvAddress(topic, address));
  REQUIRE(topics.GetAdvAddresses(topic, v));
  REQUIRE(v.at(0)== address);
  REQUIRE(!topics.Connected(topic));
  REQUIRE(!topics.Subscribed(topic));
  REQUIRE(!topics.AdvertisedByMe(topic));
  REQUIRE(!topics.Requested(topic));
  REQUIRE(!topics.GetCallback(topic, cb));
  REQUIRE(!topics.GetReqCallback(topic, reqCb));
  REQUIRE(!topics.GetRepCallback(topic, repCb));
  REQUIRE(!topics.PendingReqs(topic));

  // Check that there's only one copy stored of the same address
  topics.AddAdvAddress(topic, address);
  REQUIRE(topics.GetAdvAddresses(topic, v));
  REQUIRE(v.size()== 1);

  // Check SetConnected
  topics.SetConnected(topic, true);
  REQUIRE(topics.Connected(topic));

  // Check SetSubscribed
  topics.SetSubscribed(topic, true);
  REQUIRE(topics.Subscribed(topic));

  // Check SetRequested
  topics.SetRequested(topic, true);
  REQUIRE(topics.Requested(topic));

  // Check SetAdvertisedByMe
  topics.SetAdvertisedByMe(topic, true);
  REQUIRE(topics.AdvertisedByMe(topic));

  // Check SetCallback
  topics.SetCallback(topic, myCb);
  REQUIRE(topics.GetCallback(topic, cb));
  callbackExecuted = false;
  cb("topic", "data");
  REQUIRE(callbackExecuted);

  // Check SetReqCallback
  topics.SetReqCallback(topic, myReqCb);
  REQUIRE(topics.GetReqCallback(topic, reqCb));
  callbackExecuted = false;
  reqCb("topic", 0, "answer");
  REQUIRE(callbackExecuted);

  // Check SetRepCallback
  topics.SetRepCallback(topic, myRepCb);
  REQUIRE(topics.GetRepCallback(topic, repCb));
  callbackExecuted = false;
  std::string result;
  REQUIRE(repCb("topic", "ReqParams", result)== 0);
  REQUIRE(callbackExecuted);

  // Check the address removal
  topics.RemoveAdvAddress(topic, address);
  REQUIRE(!topics.HasAdvAddress(topic, address));
  REQUIRE(!topics.GetAdvAddresses(topic, v));

  // Check the addition of asynchronous service call requests
  std::string req1 = "paramsReq1";
  std::string req2 = "paramsReq2";
  REQUIRE(!topics.DelReq(topic, req1));
  for (auto topicInfo : topics.GetTopicsInfo())
    REQUIRE(!topics.PendingReqs(topicInfo.first));

  topics.AddReq(topic, req1);
  REQUIRE(topics.PendingReqs(topic));

  topics.AddReq(topic, req2);
  REQUIRE(topics.PendingReqs(topic));

  REQUIRE(topics.DelReq(topic, req2));
  REQUIRE(topics.PendingReqs(topic));

  REQUIRE(topics.DelReq(topic, req1));
  REQUIRE(!topics.PendingReqs(topic));

  REQUIRE(!topics.DelReq(topic, req1));
  REQUIRE(!topics.PendingReqs(topic));
}

