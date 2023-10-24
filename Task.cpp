#pragma once
#include "main.h"

string strStatus[] = {"not_completed", "in_progress", "completed", "cancelled", "overdue", "N/A"};
string strPriority[] = {"in_a_day", "in_three_days", "in_a_week",  "in_two_weeks", "in_a_month", "N/A"};
// strStatus is a string representation of the statuses for easy display.

enum Status {
    not_completed,
    in_progress,
    completed,
    cancelled,
    overdue
};

enum Priority {
    in_a_day,
    in_three_days,
    in_a_week,  
    in_two_weeks,
    in_a_month
};

int toInt(string s) {
    int result = 0;
    for (int i = 0; i < s.size(); ++i) {
        result *= 10;
        result += (s[i] - 48);
    }
    return result;
}

struct Date {
    int day, month, year;
    Date() {
        day = month = year = 1;
    }
    Date(string s) {
        day = toInt(s.substr(0,2));
        month = toInt(s.substr(3,2));
        year = toInt(s.substr(6,4));
    }
    void setDate(string s) {
        day = toInt(s.substr(0,2));
        month = toInt(s.substr(3,2));
        year = toInt(s.substr(6,4));
    }
    string getDate() {
        string s = "";
        if (day < 10) s += "0" + to_string(day);
        else s += to_string(day);
        s += "/";
        if (month < 10) s += "0" + to_string(month);
        else s += to_string(month);
        s += "/";
        if (year < 10) s += "000";
        else if (year < 100) s += "00";
        else if (year < 1000) s += "0";
        s += to_string(year);
        return s;
    }
    bool isValid(Date scheduled_time) {
        if (this->year < scheduled_time.year) return false;
        if (this->year > scheduled_time.year) return true;
        if (this->month < scheduled_time.month) return false;
        if (this->month > scheduled_time.month) return true;
        if (this->day < scheduled_time.day) return false;
        return true;
    }
};

class Task {
private:
    Status status;
    string title;
    Priority priority;
    Date scheduled_time;
    Date deadline;
    string note;

public:
    Task(Status status, string title, Priority priority, Date scheduled_time, Date deadline, string note)
        : status(status), title(title), priority(priority), scheduled_time(scheduled_time), deadline(deadline), note(note)
    {}

    void setStatus(Status status) {
        this->status = status;
    }
    Status getStatus() {
        return this->status;
    }

    void setTitle(string title) {
        this->title = title;
    }
    string getTitle() {
        return this->title;
    }

    void setPriority(Priority priority) {
        this->priority = priority;
    }
    Priority getPriority() {
        return this->priority;
    }

    void setScheduledTime(Date scheduled_time) {
        this->scheduled_time = scheduled_time;
    }
    Date getScheduledTime() {
        return this->scheduled_time;
    }

    void setDeadline(Date deadline) {
        this->deadline = deadline;
    }
    Date getDeadline() {
        return this->deadline;
    }

    void setNote(string note) {
        this->note = note;
    }
    string getNote() {
        return this->note;
    }

    bool compare(Task x) {
        if (this->title != x.getTitle()) return false;
        if (this->status != x.getStatus()) return false;
        if (this->priority != x.getPriority()) return false;
        if (this->scheduled_time.getDate() != x.getScheduledTime().getDate()) return false;
        if (this->deadline.getDate() != x.getDeadline().getDate()) return false;
        if (this->note != x.getNote()) return false;
        return true;
    }

    void print() {
        cout << "Title:    " << this->title << "\n";
        cout << "Status:   " << strStatus[this->status] << "\n";
        cout << "Priority: " << strPriority[this->priority] << "\n";
        cout << "Deadline: " << this->scheduled_time.getDate() << " -- " << this->deadline.getDate() << "\n";
        cout << "Note:     " << this->note << "\n\n";
    }

};
