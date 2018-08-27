//
//  ThreadSafeQueue.hpp
//  CircuitApp
//
//  Created by Yue Wang on 8/27/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#ifndef ThreadSafeQueue_hpp
#define ThreadSafeQueue_hpp

#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>

template <typename T>
class ThreadSafeQueue {
public:
  ThreadSafeQueue();
  ~ThreadSafeQueue();
  T& front();
  void pop_front();
  void push_back(const T& item);
  void push_back(T&& item);
  size_t size();
  bool empty();
private:
  std::deque<T> queue_;
  std::mutex mutex_;
  std::condition_variable cond_;
};

template <typename T>
ThreadSafeQueue<T>::ThreadSafeQueue(){
}

template <typename T>
ThreadSafeQueue<T>::~ThreadSafeQueue(){
}

template <typename T>
T& ThreadSafeQueue<T>::front() {
  std::unique_lock<std::mutex> mlock(mutex_);
  while (queue_.empty()) {
    cond_.wait(mlock);
  }
  return queue_.front();
}

template <typename T>
void ThreadSafeQueue<T>::pop_front() {
  std::unique_lock<std::mutex> mlock(mutex_);
  while (queue_.empty()) {
    cond_.wait(mlock);
  }
  queue_.pop_front();
}

template <typename T>
void ThreadSafeQueue<T>::push_back(const T& item) {
  std::unique_lock<std::mutex> mlock(mutex_);
  queue_.push_back(item);
  mlock.unlock();     // unlock before notificiation to minimize mutex con
  cond_.notify_one(); // notify one waiting thread
}

template <typename T>
void ThreadSafeQueue<T>::push_back(T&& item) {
  std::unique_lock<std::mutex> mlock(mutex_);
  queue_.push_back(std::move(item));
  mlock.unlock();     // unlock before notificiation to minimize mutex con
  cond_.notify_one(); // notify one waiting thread
  
}

template <typename T>
size_t ThreadSafeQueue<T>::size() {
  std::unique_lock<std::mutex> mlock(mutex_);
  size_t size = queue_.size();
  mlock.unlock();
  return size;
}

template <typename T>
bool ThreadSafeQueue<T>::empty() {
  return size() == 0;
}

#endif /* ThreadSafeQueue_hpp */
