#include <queue>

static std::queue<int> g_work_queue;
static int g_timeslice = 0;
static int g_timer_ticks = 0;

void PushToWorkQueue(int thread_id) {
  g_work_queue.push(thread_id);
}

void PopFromWorkQueue() {
  if (!g_work_queue.empty()) {
    g_work_queue.pop();
    g_timer_ticks = 0;
  }
}

/**
 * Функция будет вызвана перед каждым тестом, если вы
 * используете глобальные и/или статические переменные
 * не полагайтесь на то, что они заполнены 0 - в них
 * могут храниться значения оставшиеся от предыдущих
 * тестов.
 *
 * timeslice - квант времени, который нужно использовать.
 * Поток смещается с CPU, если пока он занимал CPU функция
 * timer_tick была вызвана timeslice раз.
 **/
void scheduler_setup(int timeslice) {
  while (!g_work_queue.empty()) {
    g_work_queue.pop();
  }

  g_timeslice = timeslice;
  g_timer_ticks = 0;
}

/**
 * Функция вызывается, когда создается новый поток управления.
 * thread_id - идентификатор этого потока и гарантируется, что
 * никакие два потока не могут иметь одинаковый идентификатор.
 **/
void new_thread(int thread_id) {
  PushToWorkQueue(thread_id);
}

/**
 * Функция вызывается, когда поток, исполняющийся на CPU,
 * завершается. Завершится может только поток, который сейчас
 * исполняется, поэтому thread_id не передается. CPU должен
 * быть отдан другому потоку, если есть активный
 * (незаблокированный и незавершившийся) поток.
 **/
void exit_thread() {
  PopFromWorkQueue();
}

/**
 * Функция вызывается, когда поток, исполняющийся на CPU,
 * блокируется. Заблокироваться может только поток, который
 * сейчас исполняется, поэтому thread_id не передается. CPU
 * должен быть отдан другому активному потоку, если таковой
 * имеется.
 **/
void block_thread() {
  PopFromWorkQueue();
}

/**
 * Функция вызывается, когда один из заблокированных потоков
 * разблокируется. Гарантируется, что thread_id - идентификатор
 * ранее заблокированного потока.
 **/
void wake_thread(int thread_id) {
  PushToWorkQueue(thread_id);
}

/**
 * Ваш таймер. Вызывается каждый раз, когда проходит единица
 * времени.
 **/
void timer_tick() {
  g_timer_ticks++;
  
  if (g_timer_ticks == g_timeslice) {
    if (!g_work_queue.empty()) {
      int thread_id = g_work_queue.front();
      g_work_queue.pop();
      g_work_queue.push(thread_id);
    }

    g_timer_ticks = 0;
  }
}

/**
 * Функция должна возвращать идентификатор потока, который в
 * данный момент занимает CPU, или -1 если такого потока нет.
 * Единственная ситуация, когда функция может вернуть -1, это
 * когда нет ни одного активного потока (все созданные потоки
 * либо уже завершены, либо заблокированы).
 **/
int current_thread() {
  if (g_work_queue.empty()) {
    return -1;
  }
  else {
    return g_work_queue.front();
  }
}

int main() {
  return 0;
}
