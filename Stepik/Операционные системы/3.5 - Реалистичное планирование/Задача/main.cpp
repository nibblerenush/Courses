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
 * ������� ����� ������� ����� ������ ������, ���� ��
 * ����������� ���������� �/��� ����������� ����������
 * �� ����������� �� ��, ��� ��� ��������� 0 - � ���
 * ����� ��������� �������� ���������� �� ����������
 * ������.
 *
 * timeslice - ����� �������, ������� ����� ������������.
 * ����� ��������� � CPU, ���� ���� �� ������� CPU �������
 * timer_tick ���� ������� timeslice ���.
 **/
void scheduler_setup(int timeslice) {
  while (!g_work_queue.empty()) {
    g_work_queue.pop();
  }

  g_timeslice = timeslice;
  g_timer_ticks = 0;
}

/**
 * ������� ����������, ����� ��������� ����� ����� ����������.
 * thread_id - ������������� ����� ������ � �������������, ���
 * ������� ��� ������ �� ����� ����� ���������� �������������.
 **/
void new_thread(int thread_id) {
  PushToWorkQueue(thread_id);
}

/**
 * ������� ����������, ����� �����, ������������� �� CPU,
 * �����������. ���������� ����� ������ �����, ������� ������
 * �����������, ������� thread_id �� ����������. CPU ������
 * ���� ����� ������� ������, ���� ���� ��������
 * (����������������� � ���������������) �����.
 **/
void exit_thread() {
  PopFromWorkQueue();
}

/**
 * ������� ����������, ����� �����, ������������� �� CPU,
 * �����������. ��������������� ����� ������ �����, �������
 * ������ �����������, ������� thread_id �� ����������. CPU
 * ������ ���� ����� ������� ��������� ������, ���� �������
 * �������.
 **/
void block_thread() {
  PopFromWorkQueue();
}

/**
 * ������� ����������, ����� ���� �� ��������������� �������
 * ��������������. �������������, ��� thread_id - �������������
 * ����� ���������������� ������.
 **/
void wake_thread(int thread_id) {
  PushToWorkQueue(thread_id);
}

/**
 * ��� ������. ���������� ������ ���, ����� �������� �������
 * �������.
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
 * ������� ������ ���������� ������������� ������, ������� �
 * ������ ������ �������� CPU, ��� -1 ���� ������ ������ ���.
 * ������������ ��������, ����� ������� ����� ������� -1, ���
 * ����� ��� �� ������ ��������� ������ (��� ��������� ������
 * ���� ��� ���������, ���� �������������).
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
