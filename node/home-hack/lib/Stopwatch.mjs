/** import */

export class Stopwatch {
  m_isStarted = false;
  m_startTime = -1;
  m_lapStartTime = -1;
  constructor(isStart = false) {
    if (isStart) {
      this.start();
      this.lap();
    }
  }

  reset() {
    this.m_isStarted = false;
    this.m_startTime = -1;
    this.m_lapStartTime = -1;
  }

  isStarted() { return this.m_startTime !== -1 };

  start() {
    if (!this.m_isStarted) {
      this.m_isStarted = true;
      this.m_startTime = performance.now();
    }
  }

  stop() {
    if (this.m_isStarted) {
      this.m_isStarted = false;
    }
  }

  elapsedMs() {
    if (this.m_startTime !== -1) {
      return performance.now() - this.m_startTime;
    }
    return 0;
  }

  lap() {
    this.m_lapStartTime = performance.now();
  }

  lapMs() {
    if (this.m_lapStartTime !== -1) {
      return performance.now() - this.m_lapStartTime;
    }
    return 0;
  }
}
