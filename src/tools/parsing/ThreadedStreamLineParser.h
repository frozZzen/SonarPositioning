#pragma once

#include "../thread_safe/MessageQueue.h"

#include <atomic>
#include <chrono>
#include <thread>

namespace sp::tools::parsing
{
  using namespace std::chrono_literals;

  template <typename StreamLineParser, size_t QUEUE_LIMIT>
  class ThreadedStreamLineParser;

  template <template <typename> typename StreamLineParser, typename OutputData, size_t QUEUE_LIMIT>
  class ThreadedStreamLineParser<StreamLineParser<OutputData>, QUEUE_LIMIT>
  {
  public:
    template <typename... BaseParserArgs>
    ThreadedStreamLineParser(BaseParserArgs&& ...args);
    ~ThreadedStreamLineParser();

    std::optional<OutputData> getNextData();
    bool hasMoreData() const;

  private:
    using OutputDataQueue = tools::thread_safe::MessageQueue<OutputData>;
    using Parser          = StreamLineParser<OutputData>;

    void run();

    Parser            _parser;
    OutputDataQueue   _queue;
    std::atomic_bool  _isRunning;
    std::thread       _worker;
  };

  template <template <typename> typename StreamLineParser, typename OutputData, size_t QUEUE_LIMIT>
  template <typename... BaseParserArgs>
  ThreadedStreamLineParser<StreamLineParser<OutputData>, QUEUE_LIMIT>::ThreadedStreamLineParser(BaseParserArgs&& ...args)
    : _parser(std::forward<BaseParserArgs>(args)...)
    , _isRunning(true)
    , _worker([this] { run(); })
  {
  }

  template <template <typename> typename StreamLineParser, typename OutputData, size_t QUEUE_LIMIT>
  ThreadedStreamLineParser<StreamLineParser<OutputData>, QUEUE_LIMIT>::~ThreadedStreamLineParser()
  {
    _isRunning.store(false);
    if (_worker.joinable()) { _worker.join(); }
  }

  template <template <typename> typename StreamLineParser, typename OutputData, size_t QUEUE_LIMIT>
  std::optional<OutputData> ThreadedStreamLineParser<StreamLineParser<OutputData>, QUEUE_LIMIT>::getNextData()
  {
    return _queue.pop();
  }

  template <template <typename> typename StreamLineParser, typename OutputData, size_t QUEUE_LIMIT>
  bool ThreadedStreamLineParser<StreamLineParser<OutputData>, QUEUE_LIMIT>::hasMoreData() const
  {
    return _queue.isOpen() || _queue.size() > 0;
  }

  template <template <typename> typename StreamLineParser, typename OutputData, size_t QUEUE_LIMIT>
  void ThreadedStreamLineParser<StreamLineParser<OutputData>, QUEUE_LIMIT>::run()
  {
    while (_isRunning.load())
    {
      const bool continueParsing = _queue.size() < QUEUE_LIMIT / 2;
      if (continueParsing)
      {
        while (_queue.size() < QUEUE_LIMIT)
        {
          auto record = _parser.parseNextLine();
          if (!record) [[unlikely]]
          {
            _queue.close();
            return;
          }
          _queue.push(record.value());
          std::this_thread::yield();
        }
      }
      std::this_thread::sleep_for(10us);
    }
  }
}
