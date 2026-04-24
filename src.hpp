// ACM EXPRESS Solution
#include <iostream>
#include <algorithm>
#include "base.hpp"

using namespace std;

struct date {
    int year, month, day;
    date() = default;
    date(int y, int m, int d) : year(y), month(m), day(d) {}
    
    friend istream& operator>>(istream& is, date& d) {
        is >> d.year >> d.month >> d.day;
        return is;
    }
    
    bool operator<(const date& other) const {
        // Assuming 360 days/year, 12 months/year, 30 days/month
        int days1 = year * 360 + month * 30 + day;
        int days2 = other.year * 360 + other.month * 30 + other.day;
        return days1 < days2;
    }
};

class mail : public object {
protected:
    string postmark;
    date send_date;
    date arrive_date;

public:
    mail() = default;

    mail(string _contain_, string _postmark_, date send_d, date arrive_d) : object(_contain_) {
        postmark = _postmark_;
        send_date = send_d;
        arrive_date = arrive_d;
    }

    virtual void copy(object *o) {
        contain = reinterpret_cast<mail *>(o)->contain;
        postmark = reinterpret_cast<mail *>(o)->postmark;
        send_date = reinterpret_cast<mail *>(o)->send_date;
        arrive_date = reinterpret_cast<mail *>(o)->arrive_date;
    }

    virtual string send_status(int y, int m, int d) {
        return "not send";
    }

    virtual string type() {
        return "no type";
    }

    virtual void print() {
        object::print();
        cout << "[mail] postmark: " << postmark << endl;
    }

    virtual ~mail() {}
};

class air_mail : public mail {
protected:
    string airlines;
    date take_off_date;
    date land_date;

public:
    air_mail() = default;

    air_mail(string _contain_, string _postmark_, date send_d, date arrive_d, date take_off, date land, string _airline) 
        : mail(_contain_, _postmark_, send_d, arrive_d) {
        take_off_date = take_off;
        land_date = land;
        airlines = _airline;
    }

    void copy(object *o) {
        mail::copy(o);
        airlines = reinterpret_cast<air_mail *>(o)->airlines;
        take_off_date = reinterpret_cast<air_mail *>(o)->take_off_date;
        land_date = reinterpret_cast<air_mail *>(o)->land_date;
    }

    string send_status(int y, int m, int d) {
        date ask_date(y, m, d);
        if (ask_date < send_date)
            return "mail not send";
        else if (ask_date < take_off_date)
            return "wait in airport";
        else if (ask_date < land_date)
            return "in flight";
        else if (ask_date < arrive_date)
            return "already land";
        else
            return "already arrive";
    }

    string type() {
        return "air";
    }

    void print() {
        mail::print();
        cout << "[air] airlines: " << airlines << endl;
    }

    ~air_mail() {}
};

class train_mail : public mail {
protected:
    string *station_name;
    date *station_time;
    int len;

public:
    train_mail() : station_name(nullptr), station_time(nullptr), len(0) {}

    train_mail(string _contain_, string _postmark_, date send_d, date arrive_d, string *sname, date *stime, int station_num)
        : mail(_contain_, _postmark_, send_d, arrive_d) {
        len = station_num;
        station_name = new string[len];
        station_time = new date[len];
        for (int i = 0; i < len; i++) {
            station_name[i] = sname[i];
            station_time[i] = stime[i];
        }
    }

    void copy(object *o) {
        mail::copy(o);
        train_mail *other = reinterpret_cast<train_mail *>(o);
        
        // Clean up existing arrays
        if (station_name != nullptr) delete[] station_name;
        if (station_time != nullptr) delete[] station_time;
        
        len = other->len;
        station_name = new string[len];
        station_time = new date[len];
        for (int i = 0; i < len; i++) {
            station_name[i] = other->station_name[i];
            station_time[i] = other->station_time[i];
        }
    }

    string send_status(int y, int m, int d) {
        date ask_date(y, m, d);
        if (ask_date < send_date)
            return "mail not send";
        
        for (int i = 0; i < len; i++) {
            if (ask_date < station_time[i]) {
                if (i == 0)
                    return "on the way to " + station_name[i];
                else
                    return "on the way to " + station_name[i];
            }
        }
        
        if (ask_date < arrive_date)
            return "on the way to destination";
        else
            return "already arrive";
    }

    string type() {
        return "train";
    }

    void print() {
        mail::print();
        cout << "[train] station_num: " << len << endl;
    }

    ~train_mail() {
        if (station_name != nullptr) delete[] station_name;
        if (station_time != nullptr) delete[] station_time;
    }
};

class car_mail : public mail {
protected:
    int total_mile;
    string driver;

public:
    car_mail() = default;

    car_mail(string _contain_, string _postmark_, date send_d, date arrive_d, int mile, string _driver)
        : mail(_contain_, _postmark_, send_d, arrive_d) {
        total_mile = mile;
        driver = _driver;
    }

    void copy(object *o) {
        mail::copy(o);
        total_mile = reinterpret_cast<car_mail *>(o)->total_mile;
        driver = reinterpret_cast<car_mail *>(o)->driver;
    }

    string send_status(int y, int m, int d) {
        date ask_date(y, m, d);
        if (ask_date < send_date)
            return "mail not send";
        else if (ask_date < arrive_date) {
            // Calculate current_mile
            int used_days = (ask_date.year * 360 + ask_date.month * 30 + ask_date.day) 
                          - (send_date.year * 360 + send_date.month * 30 + send_date.day);
            int total_days = (arrive_date.year * 360 + arrive_date.month * 30 + arrive_date.day) 
                           - (send_date.year * 360 + send_date.month * 30 + send_date.day);
            double current_mile = (double)used_days / (double)total_days * (double)total_mile;
            return to_string(current_mile);
        }
        else
            return "already arrive";
    }

    string type() {
        return "car";
    }

    void print() {
        mail::print();
        cout << "[car] driver_name: " << driver << endl;
    }

    ~car_mail() {}
};

void obj_swap(object *&lhs, object *&rhs) {
    object *temp = lhs;
    lhs = rhs;
    rhs = temp;
}
