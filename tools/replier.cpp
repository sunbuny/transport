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

#include <iostream>
#include <string>

#include "transport.h"


//////////////////////////////////////////////////
/// \brief Function is called everytime a service call is requested.
int echo(const std::string &_topic, const std::string &_data, std::string &_rep)
{
  assert(_topic != "");
  std::cout << "\nCallback [" << _topic << "][" << _data << "]" << std::endl;
  _rep = _data;
  return 0;
}


//////////////////////////////////////////////////
int main(int argc, char *argv[])
{
  // Read the command line arguments
  std::string master, topic("foo"), data, response;
  bool verbose(false), selfCall(false);

  // Transport node
  transport::Node node(master, verbose);

  // Advertise a service call
  if (node.SrvAdvertise(topic, echo) != 0)
    std::cout << "SrvAdvertise did not work" << std::endl;

  if (selfCall)
  {
    // Request my own service call
    data = "";
    int rc = node.SrvRequest(topic, data, response);
    if (rc == 0)
      std::cout << "Response: " << response << std::endl;
    else
      std::cout << "SrvRequest did not work" << std::endl;
  }

  // Unadvertise a service call
  //if (node.SrvUnAdvertise(topic) != 0)
  //  std::cout << "SrvUnAdvertise did not work" << std::endl;

  // Zzzzzz Zzzzzz
  node.Spin();

  return 0;
}
