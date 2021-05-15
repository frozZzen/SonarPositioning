#pragma once

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <optional>
#include <queue>
#include <thread>

namespace sp::tools::thread_safe
{
  template <typename Message>
  class MessageQueue
  {
  public:
    MessageQueue();

    template <typename MessageT>
    void push(MessageT&& message_);

    std::optional<Message> pop();

    bool isOpen() const;
    void close();
    size_t size() const;

  private:
    std::queue<Message>     _queue;
    std::mutex              _mutex;
    std::condition_variable _queueFill;
    std::atomic_bool        _isOpen;
  };

  template <typename Message>
  MessageQueue<Message>::MessageQueue()
    : _isOpen(true)
  {
  }

  template <typename Message>
  template <typename MessageT>
  void MessageQueue<Message>::push(MessageT&& message_)
  {
    std::unique_lock<std::mutex> guard{_mutex};
    _queue.push(std::forward<MessageT>(message_));
    _queueFill.notify_one();
  }

  template <typename Message>
  std::optional<Message> MessageQueue<Message>::pop()
  {
    std::unique_lock<std::mutex> guard{_mutex};
    while (_isOpen.load() || !_queue.empty())
    {
      while (_queue.empty() && _isOpen.load())
      {
        _queueFill.wait(guard);
      }
      if (!_queue.empty())
      {
        Message message = std::move(_queue.front());
        _queue.pop();
        return message;
      }
    }
    return std::nullopt;
  }

  template <typename Message>
  inline bool MessageQueue<Message>::isOpen() const
  {
    return _isOpen.load();
  }

  template <typename Message>
  inline void MessageQueue<Message>::close()
  {
    std::unique_lock<std::mutex> guard{_mutex};
    _isOpen.store(false);
    _queueFill.notify_one();
  }

  template <typename Message>
  inline size_t MessageQueue<Message>::size() const
  {
    return _queue.size();
  }
}
