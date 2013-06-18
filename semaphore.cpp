#include <semaphore.h>

#include <iostream>
#include <vector>

#include <boost/chrono.hpp>
#include <boost/thread.hpp>

namespace
{

#ifdef USE_BOOST

class semaphore
{
public:
    semaphore( const size_t n )
        : total( n ), avail( n ) {}

    void acquire()
    {
        boost::unique_lock< boost::mutex > lock( mutex );
        while ( avail == 0 )
            cond.wait( lock );
        --avail;
    }

    void release()
    {
        boost::unique_lock< boost::mutex > lock( mutex );
        if ( avail < total )
        {
            ++avail;
            cond.notify_one();
        }
    }

private:
    const size_t total;
    size_t avail;
    boost::mutex mutex;
    boost::condition_variable cond;
};

#endif // USE_BOOST

#ifdef USE_MUTEX

class semaphore
{
public:
    semaphore( const size_t /* n */ ) {}
    ~semaphore()                      {}
    void acquire()                    { mutex.lock();   }
    void release()                    { mutex.unlock(); }
private:
    boost::mutex mutex;
};

#endif // USE_MUTEX

#ifdef USE_PTHREAD

class semaphore
{
public:
    semaphore( const size_t n ) { sem_init    ( &sem, 0, n ); }
    ~semaphore()                { sem_destroy ( &sem ); }
    void acquire()              { sem_wait    ( &sem ); }
    void release()              { sem_post    ( &sem ); }
private:
    sem_t sem;
};

#endif // USE_PTHREAD

class semaphore_guard
{
public:
    semaphore_guard( semaphore & sem ) : sem( sem ) { sem.acquire(); }
    ~semaphore_guard() { sem.release(); }
private:
    semaphore & sem;
};

size_t count = 0;

void inc_count( semaphore & sem, const size_t n_reps )
{
    for ( size_t j = 0; j < n_reps; ++j )
    {
        semaphore_guard guard( sem );
        ++count;
    }
}

}

int main( int argc, char * argv [] )
{
    const size_t n_threads = 10;
    const size_t n_reps = 100000;

    semaphore sem( 1 );

    namespace bc = boost::chrono;
    typedef bc::high_resolution_clock clock;
    const clock::time_point t0( clock::now() );

    typedef boost::shared_ptr< boost::thread > thread_ptr;
    std::vector< thread_ptr > thread_ptrs;
    for ( size_t i = 0; i < n_threads; ++i )
    {
        thread_ptr tp( new boost::thread( &inc_count, boost::ref( sem ), n_reps ) );
        thread_ptrs.push_back( tp );
    }

    for ( size_t i = 0; i < n_threads; ++i )
        thread_ptrs[i]->join();

    const clock::time_point t1( clock::now() );

    const size_t n_total( n_threads * n_reps );

    std::cout << argv[0] << ": "
              << n_threads << " threads * " << n_reps << " reps = "
              << n_total << " total; "
              << "elapsed = " << bc::duration_cast< bc::milliseconds >( t1 - t0 ) << std::endl;

    if ( count != n_total )
        std::cerr << argv[0] << ": count mismatch "
          "(expected " << n_total << ", got " << count << ")" << std::endl;
}
