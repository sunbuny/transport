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
/// \brief Function is called everytime a topic update is received.
void cb(const std::string &_topic, int _rc, const std::string &_rep)
{
  assert(_topic != "");
  if (_rc == 0)
    std::cout << "\nCallback [" << _topic << "][" << _rep << "]" << std::endl;
  else
    std::cout << "\nCallback [" << _topic << "] Error" << std::endl;
}


//////////////////////////////////////////////////
int main(int argc, char *argv[])
{
  // Read the command line arguments
  std::string master, topic("foo"), data("bar"), response;
  int numSrvs(10), rc;
  bool verbose(false), async(true);

  // Transport node
  transport::Node node(master, verbose);

  for (int i = 0; i < numSrvs; ++i)
  {
    if (async)
      rc = node.SrvRequestAsync(topic, data, cb);
    else
    {
      rc = node.SrvRequest(topic, data, response);
      if (rc == 0)
        std::cout << "Result: " << response << std::endl;
    }

    if (rc != 0)
      std::cout << "srv_request did not work" << std::endl;

    //node.SpinOnce();
  }

  node.Spin();

  // Zzzzzz Zzzzzz
  std::cout << "\nPress any key to exit" << std::endl;
  getchar();

  return 0;
}
