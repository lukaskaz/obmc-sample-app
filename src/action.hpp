#include <boost/asio/io_context.hpp>
#include <boost/asio/steady_timer.hpp>

#include <memory>

using namespace std::chrono_literals;
using Callback = std::function<void()>;
using Milliseconds = std::chrono::duration<uint64_t, std::milli>;

class Timer
{
  public:
    Timer() = delete;
    Timer(boost::asio::io_context& ioc, Milliseconds intervalArg,
          Callback callbackArg) :
        timerPtr(std::make_shared<boost::asio::steady_timer>(ioc)),
        interval(intervalArg), callback(std::move(callbackArg))
    {}

    bool stop()
    {
        return timerPtr->cancel() > 0;
    }

  protected:
    std::shared_ptr<boost::asio::steady_timer> timerPtr;
    Milliseconds interval;
    Callback callback;

    virtual void tick() const = 0;
};

class OneShotTimer : public Timer
{
  public:
    OneShotTimer() = delete;
    OneShotTimer(boost::asio::io_context& ioc, Milliseconds intervalArg,
                 Callback callbackArg) :
        Timer(ioc, intervalArg, std::move(callbackArg))
    {
        tick();
    }

    void restart() const
    {
        tick();
    }

  private:
    void tick() const;
};

class PeriodicTimer : public Timer
{
  public:
    PeriodicTimer() = delete;
    PeriodicTimer(boost::asio::io_context& ioc, Milliseconds intervalArg,
                  Callback callbackArg) :
        Timer(ioc, intervalArg, std::move(callbackArg))
    {
        tick();
    }

  private:
    void tick() const;
};

class DetachedOneShotTimer :
    public Timer,
    public std::enable_shared_from_this<DetachedOneShotTimer>
{
  public:
    DetachedOneShotTimer() = delete;
    DetachedOneShotTimer(boost::asio::io_context& ioc, Milliseconds intervalArg,
                         Callback callbackArg) :
        Timer(ioc, intervalArg, std::move(callbackArg))
    {}

    static void create(boost::asio::io_context& ioc, Milliseconds intervalArg,
                       Callback callbackArg)
    {
        auto ptr = std::make_shared<DetachedOneShotTimer>(
            ioc, intervalArg, std::move(callbackArg));
        ptr->tick();
    }

  private:
    void tick() const;
};

class DetachedPeriodicTimer :
    public Timer,
    public std::enable_shared_from_this<DetachedPeriodicTimer>
{
  public:
    DetachedPeriodicTimer() = delete;
    DetachedPeriodicTimer(boost::asio::io_context& ioc,
                          Milliseconds intervalArg, Callback callbackArg,
                          bool& stopArg) :
        Timer(ioc, intervalArg, std::move(callbackArg)),
        stop(stopArg)
    {}

    static void create(boost::asio::io_context& ioc, Milliseconds intervalArg,
                       Callback callbackArg, bool& stopArg)
    {
        auto ptr = std::make_shared<DetachedPeriodicTimer>(
            ioc, intervalArg, std::move(callbackArg), stopArg);
        ptr->tick();
    }

  private:
    bool& stop;

    bool isStopped() const
    {
        return stop;
    }

    void tick() const;
};

void singleAction(boost::asio::io_context& ioc, Milliseconds delayMs,
                  Callback&& action);

void repeatingAction(boost::asio::io_context& ioc, Milliseconds intervalMs,
                     Callback&& action, bool& stop);
