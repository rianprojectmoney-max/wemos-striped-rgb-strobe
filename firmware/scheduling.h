#ifndef SCHEDULING_H
#define SCHEDULING_H

#include <time.h>

#define MAX_SCHEDULES 10

struct Schedule {
  uint16_t mode;
  uint8_t brightness;
  uint8_t speed;
  uint8_t color;
  uint8_t startHour;
  uint8_t startMinute;
  uint8_t endHour;
  uint8_t endMinute;
  uint8_t daysOfWeek;  // Bit 0=Mon, 1=Tue, ..., 6=Sun
  bool enabled;
  char name[32];
};

class Scheduler {
public:
  Schedule schedules[MAX_SCHEDULES];
  
  Scheduler() {
    for (int i = 0; i < MAX_SCHEDULES; i++) {
      schedules[i].enabled = false;
      strcpy(schedules[i].name, "");
    }
  }
  
  void begin() {
    // Configure NTP - configTime is built-in ESP8266 function
    configTime(0, 0, "pool.ntp.org", "time.nist.gov");
  }
  
  bool isScheduleActive(int index, uint16_t& mode, uint8_t& brightness, uint8_t& speed, uint8_t& color) {
    if (index >= MAX_SCHEDULES || !schedules[index].enabled) {
      return false;
    }
    
    time_t now = time(nullptr);
    struct tm* timeinfo = localtime(&now);
    
    uint16_t currentMinutes = timeinfo->tm_hour * 60 + timeinfo->tm_min;
    uint16_t startMinutes = schedules[index].startHour * 60 + schedules[index].startMinute;
    uint16_t endMinutes = schedules[index].endHour * 60 + schedules[index].endMinute;
    
    // Check if time is within range
    bool timeMatch = false;
    if (startMinutes < endMinutes) {
      timeMatch = (currentMinutes >= startMinutes && currentMinutes < endMinutes);
    } else {
      timeMatch = (currentMinutes >= startMinutes || currentMinutes < endMinutes);
    }
    
    // Check if day matches (wday: 0=Sunday, 1=Monday, ...)
    // Convert to our format where 0=Monday
    int wday = (timeinfo->tm_wday + 6) % 7;
    bool dayMatch = (schedules[index].daysOfWeek & (1 << wday));
    
    if (timeMatch && dayMatch) {
      mode = schedules[index].mode;
      brightness = schedules[index].brightness;
      speed = schedules[index].speed;
      color = schedules[index].color;
      return true;
    }
    
    return false;
  }
  
  void addSchedule(int index, const char* name, uint16_t mode, uint8_t brightness,
                   uint8_t speed, uint8_t color, uint8_t startHour, uint8_t startMinute,
                   uint8_t endHour, uint8_t endMinute, uint8_t daysOfWeek) {
    if (index < MAX_SCHEDULES) {
      schedules[index].mode = mode;
      schedules[index].brightness = brightness;
      schedules[index].speed = speed;
      schedules[index].color = color;
      schedules[index].startHour = startHour;
      schedules[index].startMinute = startMinute;
      schedules[index].endHour = endHour;
      schedules[index].endMinute = endMinute;
      schedules[index].daysOfWeek = daysOfWeek;
      schedules[index].enabled = true;
      strncpy(schedules[index].name, name, 31);
    }
  }
  
  void removeSchedule(int index) {
    if (index < MAX_SCHEDULES) {
      schedules[index].enabled = false;
    }
  }
  
  void toggleSchedule(int index) {
    if (index < MAX_SCHEDULES) {
      schedules[index].enabled = !schedules[index].enabled;
    }
  }
  
  Schedule getSchedule(int index) {
    return schedules[index];
  }
};

#endif // SCHEDULING_H
