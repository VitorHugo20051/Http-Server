#include "ThreadPool.h"
#include <stdlib.h>
#include <stdio.h>

void * generic_thread_function(void *arg);
void add_work(struct ThreadPool *thread_pool, struct ThreadJob thread_job);

struct ThreadJob thread_job_constructor(void * (*job)(void *arg), void *arg) {
    struct ThreadJob thread_job;
    thread_job.job = job;
    thread_job.arg = arg;
    return thread_job;
}

struct ThreadPool * thread_pool_constructor(int num_threads) {
    struct ThreadPool *thread_pool = malloc(sizeof(struct ThreadPool));
    if (!thread_pool) return NULL;

    thread_pool->num_threads = num_threads;
    thread_pool->active = 1;
    thread_pool->work = queue_constructor();
    
    pthread_mutex_init(&thread_pool->lock, NULL);
    pthread_cond_init(&thread_pool->signal, NULL);

    thread_pool->pool = malloc(sizeof(pthread_t) * num_threads);
    thread_pool->add_work = add_work;

    for (int i = 0; i < num_threads; i++) {
        pthread_create(&thread_pool->pool[i], NULL, generic_thread_function, thread_pool);
    }
    return thread_pool;
}

void * generic_thread_function(void *arg) {
    struct ThreadPool *thread_pool = (struct ThreadPool *)arg;
    while (1) {
        pthread_mutex_lock(&thread_pool->lock);

        while (thread_pool->work.list.length == 0 && thread_pool->active) {
            pthread_cond_wait(&thread_pool->signal, &thread_pool->lock);
        }

        if (!thread_pool->active && thread_pool->work.list.length == 0) {
            pthread_mutex_unlock(&thread_pool->lock);
            break;
        }

        struct ThreadJob *job_ptr = (struct ThreadJob *)thread_pool->work.peek(&thread_pool->work);
        struct ThreadJob thread_job = *job_ptr;
        thread_pool->work.pop(&thread_pool->work);
        
        pthread_mutex_unlock(&thread_pool->lock);

        if (thread_job.job) {
            thread_job.job(thread_job.arg);
        }
    }
    return NULL;
}

void add_work(struct ThreadPool *thread_pool, struct ThreadJob thread_job) {
    pthread_mutex_lock(&thread_pool->lock);
    thread_pool->work.push(&thread_pool->work, &thread_job, sizeof(thread_job));
    pthread_cond_signal(&thread_pool->signal);
    pthread_mutex_unlock(&thread_pool->lock);
}

void thread_pool_destructor(struct ThreadPool *thread_pool) {
    if (!thread_pool) return;

    pthread_mutex_lock(&thread_pool->lock);
    thread_pool->active = 0;
    pthread_cond_broadcast(&thread_pool->signal); // Acorda todos para morrerem
    pthread_mutex_unlock(&thread_pool->lock);

    for (int i = 0; i < thread_pool->num_threads; i++) {
        pthread_join(thread_pool->pool[i], NULL);
    }

    free(thread_pool->pool);
    queue_destructor(&thread_pool->work);
    pthread_mutex_destroy(&thread_pool->lock);
    pthread_cond_destroy(&thread_pool->signal);
    free(thread_pool);
}