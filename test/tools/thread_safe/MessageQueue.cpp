#include "../../pch.h"

#include <common/types.h>
#include <tools/thread_safe/MessageQueue.h>

#include <chrono>
#include <future>
#include <vector>

using namespace sp;
using namespace sp::tools::thread_safe;
using namespace sp::types;
using namespace std::chrono_literals;

TEST(MessageQueue, SendingAndReceiving)
{
  constexpr size_t MESSAGE_COUNT = 1000;
  constexpr size_t REPEAT_COUNT = 100;

  for (int _ = 0; _ < REPEAT_COUNT; ++_)
  {
    MessageQueue<int> queue;
    std::queue<int> messages;
    for (int i = 0; i < MESSAGE_COUNT; ++i)
    {
      messages.push(i);
    }

    std::promise<void> senderFinishFlag;
    const auto senderJob = std::async([&] {
      while (!messages.empty())
      {
        int next = messages.front();
        messages.pop();
        queue.push(next);
      }

      EXPECT_TRUE(queue.isOpen());
      queue.close();
      senderFinishFlag.set_value();
      });

    std::vector<int> receivedMessages;
    receivedMessages.reserve(MESSAGE_COUNT);

    const auto start = Clock::now();
    auto current = start;
    while (current - start < 5s && (queue.isOpen() || queue.size() > 0))
    {
      auto message = queue.pop();
      if (message)
      {
        receivedMessages.push_back(message.value());
      }
      current = Clock::now();
    }

    const auto senderResult = senderFinishFlag.get_future().wait_for(5s);
    EXPECT_EQ(senderResult, std::future_status::ready);

    EXPECT_EQ(receivedMessages.size(), MESSAGE_COUNT);
    EXPECT_LE(current - start, 1s);

    int expectedMessage = 0;
    for (int receivedMessage : receivedMessages)
    {
      EXPECT_EQ(expectedMessage, receivedMessage);
      ++expectedMessage;
    }
  }
}
