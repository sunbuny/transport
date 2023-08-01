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
#include <vector>

#include "transport.h"


//////////////////////////////////////////////////
/// \brief Function is called everytime a topic update is received.
void cb(const std::string &_topic, const std::string &_data)
{
    assert(_topic != "");
    std::cout << "\nCallback [" << _topic << "][" << _data << "]" << std::endl;
}



//////////////////////////////////////////////////
int main(int argc, char *argv[])
{
    // Read the command line arguments
    std::string master;
    bool verbose(true);
    std::vector<std::string> topics{"foo"};

    // Transport node
    transport::Node node(master, verbose);

    // Subscribe to the list of topics
    for (size_t i = 0; i < topics.size(); ++i)
    {
        int rc = node.Subscribe(topics[i], cb);
        if (rc != 0)
            std::cout << "subscribe for topic [" << topics[i] << "] did not work\n";

        /*
        rc = node.UnSubscribe(topics[i]);
        if (rc != 0)
          std::cout << "unsubscribe for topic [" << topics[i] << "] did not work\n";
        */
    }

    node.Spin();

    return 0;
}
