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
void cb(const std::string &_topic, const std::string &_data)
{
    assert(_topic != "");
    std::cout << "\nCallback [" << _topic << "][" << _data << "]" << std::endl;
}

//////////////////////////////////////////////////
int main(int argc, char *argv[])
{
    // Read the command line arguments
    std::string master, topic("foo"), data("bar");
    int numMessages(10), rc;
    bool verbose(true), selfSubscribe(false);

    // Transport node
    transport::Node node(master, verbose);

    // Advertise a topic
    rc = node.Advertise(topic);
    if (rc != 0)
        std::cout << "Advertise did not work" << std::endl;

    if (selfSubscribe)
    {
        // Self-subscribe to the topic
        rc = node.Subscribe(topic, cb);
        if (rc != 0)
            std::cout << "Subscribe did not work" << std::endl;
    }

    // Publish data
    for (int i = 0; i < numMessages; ++i)
    {
        node.Publish(topic, data);
        node.SpinOnce();
    }

    // Zzzzzz Zzzzzz
    std::cout << "\nPress any key to exit" << std::endl;
    getchar();

    return 0;
}
