// Usage: open file main.exe and maximize
// The tasks are always sorted based on their priority, so I don't need to create a function for sorting anymore

#include "main.h"
#include "Task.cpp"

int statistics[5] = {0};                    // count the number of each status
vector<Task> a[5][5][32][13];               // a[priority][status][day][month][year]
vector<Task> currentTasks;                  // to display tasks on the screen

void printResult(vector<Task> &result);
void selectTask2(string title, int status, int priority, string deadline);
void filterWith3Conditions( int priority, int status, Date deadline);
void filterByPriority( int priority);
void filterByStatus(int status);
void filterByDeadline(Date deadline);
void filterByDeadline2( Date deadline);
void filterByStatus2( Status status);
void filterByPriority2( Priority priority);
void filterByTitle(string title);
void updateCurrentTasks();
void typeInTask();
void writeToTxt_Filter();

void addTask(int priority, string title, int status,Date scheduled_time, Date deadline, string note);
void selectTask();
void deleteTask(Task temp);
void editTask(Task &temp);
void trackStatus();
void writeToTxt();
void findTask();
void filter();
void perform(int k);

int main() {
    ifstream inp("input.txt");
                 
    int status, priority;
    string temp, title, note;
    Date scheduled_time, deadline;
    while (!inp.eof()) {
        getline(inp, temp, ';'); status = toInt(temp);
        if (temp == "") break;
        getline(inp, title, ';');
        getline(inp, temp, ';'); priority = toInt(temp);
        getline(inp, temp, ';'); scheduled_time.setDate(temp);
        getline(inp, temp, ';'); deadline.setDate(temp);
        getline(inp, note, ';');
        inp.ignore();

        addTask( priority, title, status, scheduled_time, deadline, note);

    }
    inp.close();

    int k = 10;
    updateCurrentTasks();
    while (k > 0) {
        system("cls");
        // screen
        cout << "______________________________________________________________________________________________________________________________________________" << endl;
        cout << setw(4) << "| No. " 
             << setw(9) << "| Status" 
             << setw(16) << "| Title" 
             << setw(37) << "| Priority" 
             << setw(23) << "| Scheduled_time" 
             << setw(12) << "| Deadline" 
             << setw(10) << "| Note" << setw(30) << "|\n";
        cout << "|------|----------------|---------------------------------|----------------|-----------------|-------------|---------------------------------|" << endl;
        for (int i = 0; i < currentTasks.size(); ++i) {
            int tempSize = currentTasks[i].getTitle().size();
            cout << "| "<< i 
             << setw(4 + (3 - to_string(i).size())) << "| " << setw(strStatus[currentTasks[i].getStatus()].size()) << strStatus[currentTasks[i].getStatus()]
             << setw(4 + (13 - strStatus[currentTasks[i].getStatus()].size())) << "| " ;
            
            if (tempSize > 30) {
                
                for (int chr = 0; chr < 27; ++chr) cout << currentTasks[i].getTitle()[chr];
                cout << "...";
                tempSize = 30;
            }
            else cout << setw(currentTasks[i].getTitle().size()) << currentTasks[i].getTitle();
             cout << setw(4 + (30 - tempSize)) << "| " << setw(strPriority[currentTasks[i].getPriority()].size()) << strPriority[currentTasks[i].getPriority()]
             << setw(4 + (13 - strPriority[currentTasks[i].getPriority()].size())) << "| " << setw(10) << currentTasks[i].getScheduledTime().getDate() 
             << setw(8) << "| " << setw(10) << currentTasks[i].getDeadline().getDate()
             << setw(4) << "| ";
            tempSize = currentTasks[i].getNote().size();
            if (tempSize > 30) {
                for (int chr = 0; chr < 28; ++chr) cout << currentTasks[i].getNote()[chr]; cout << "... |\n";
                
            }
            else
            cout << setw(currentTasks[i].getNote().size()) << currentTasks[i].getNote() << setw(4 + 30 - currentTasks[i].getNote().size()) << "|\n";
        }    
        cout << "|--------------------------------------------------------------------------------------------------------------------------------------------|\n";
        
        cout << "\n1. Add Task\n"
             << "2. Select Task (to view, edit, delete)\n"
             << "3. Track Status\n"
             << "4. Write all tasks to the output.txt file\n"       
             << "5. Find task\n"            
             << "6. Filter\n"             
             << "0. Exit\n";
        // The tasks are always sorted based on their priority, so I don't need to create a function for sorting anymore
        cout << "Selection: "; cin >> k;
        perform(k);
        updateCurrentTasks();
    }
}

void perform(int k) {
    switch (k)
    {
    case 1:
        cin.ignore();
        typeInTask();
        break;
    case 2:
        selectTask();
        break;
    case 3:
        trackStatus();
        break;
    case 4:
        writeToTxt();
        break;
    case 5:
        findTask();
        break;
    case 6:
        filter();
        break;
    default:
        break;
    }
}

void updateCurrentTasks() {
    currentTasks.clear();
    for (int priority = 0; priority < 5; ++priority)
        for (int status = 0; status < 5; ++status)
            for (int day = 1; day < 32; ++day)
                for (int month = 1; month < 13; ++month)
                    if (a[priority][status][day][month].size() > 0)
                        for (auto x : a[priority][status][day][month])
                            currentTasks.emplace_back(x);
}

// Statistics are updated in the delete, add, edit functions.
void trackStatus() {
    string temp;
    system("cls");
    for (int i = 0; i < 5; ++i) {
        cout << strStatus[i] << ": " << statistics[i] << "\n";        
    }
    cout << '\n';
    cout << "Press Enter to exit... "; 
    cin.ignore(); getline(cin, temp);
}

void clearOutputFile() {
    std::ofstream outputFile("output.txt", std::ios::trunc);
    outputFile.close();
}

void writeToTxt() {
    clearOutputFile();
    int count = 0;
    ofstream out("output.txt", std::ios::app);
    for (int priority = 0; priority < 5; ++priority)
        for (int status = 0; status < 5; ++status)
            for (int day = 1; day < 32; ++day)
                for (int month = 1; month < 13; ++month)
                    if (a[priority][status][day][month].size() > 0)
                        for (auto x : a[priority][status][day][month])
                        {
                            out << "Task " << ++count << ": \n";
                            out << "Title:    " << x.getTitle() << "\n";
                            out << "Status:   " << strStatus[x.getStatus()] << "\n";
                            out << "Priority: " << strPriority[x.getPriority()] << "\n";
                            out << "Deadline: " << x.getScheduledTime().getDate() << " -- " << x.getDeadline().getDate() << "\n";
                            out << "Note:     " << x.getNote() << "\n\n";
                        }
    out.close();
}

void writeToTxt_Filter() {
    clearOutputFile();
    int count = 0;
    ofstream out("output.txt", std::ios::app);
    for (auto x : currentTasks)
    {
        out << "Task " << ++count << ": \n";
        out << "Title:    " << x.getTitle() << "\n";
        out << "Status:   " << strStatus[x.getStatus()] << "\n";
        out << "Priority: " << strPriority[x.getPriority()] << "\n";
        out << "Deadline: " << x.getScheduledTime().getDate() << " -- " << x.getDeadline().getDate() << "\n";
        out << "Note:     " << x.getNote() << "\n\n";
    }
    out.close();
}

void typeInTask() {
    bool validDeadline = false;
    string title, temp, note;
    int status, priority;
    Date scheduled_time, deadline;
    cout << "Title: "; getline(cin, title);
    cout << "Status (0-not_completed  1-in_progress 2-completed 3-cancelled 4-overdue): "; cin >> temp; status = toInt(temp);
    cout << "Priority (0-in_a_day 1-in_three_days 2-in_a_week 3-in_two_weeks 4-in_a_month): "; cin >> temp; priority = toInt(temp);
    cout << "Scheduled_time (DD/MM/YYYY): "; cin >> temp; scheduled_time.setDate(temp);
    while (!validDeadline) {
        cout << "Deadline (must be equal to or after scheduling): "; cin >> temp; deadline.setDate(temp);
        validDeadline = deadline.isValid(scheduled_time);
    }
    cin.ignore();
    cout << "Note: "; getline(cin, note);
    addTask(priority, title, status, scheduled_time, deadline, note);
}

// Select a task in currentTasks (on screen)
void selectTask() {
    int x;
    cout << "Which task do you choose? (0 1 2 3 ...): "; cin >> x;
    cout << "\n";
    int k = 3;
    while (k > 0) {
        system("cls");
        currentTasks[x].print();
        cout << "1. Edit task\n"
             << "2. Delete task\n"
             << "0. Exit\n";
        cout << "Selection: "; cin >> k;
        switch (k)
        {
        case 1:
            editTask(currentTasks[x]);
            break;
        case 2:
            deleteTask(currentTasks[x]);
            currentTasks.erase(currentTasks.begin() + x);
            k = 0;
            break;
        default:
            break;
        }
    }
}

void deleteTask(Task temp) {
    int i = 0;          // the index of temp in a[temp.getPriority()][temp.getStatus()][temp.getDeadline().day][temp.getDeadline().month]
    for (i; i < a[temp.getPriority()][temp.getStatus()][temp.getDeadline().day][temp.getDeadline().month].size(); ++i)
        if (a[temp.getPriority()][temp.getStatus()][temp.getDeadline().day][temp.getDeadline().month][i].getTitle() == temp.getTitle()
            && a[temp.getPriority()][temp.getStatus()][temp.getDeadline().day][temp.getDeadline().month][i].getNote() == temp.getNote())
        {
            break;
        }
    --statistics[temp.getStatus()];
    a[temp.getPriority()][temp.getStatus()][temp.getDeadline().day][temp.getDeadline().month].erase(a[temp.getPriority()][temp.getStatus()][temp.getDeadline().day][temp.getDeadline().month].begin() + i);
} 

void editTask(Task &temp) {
    int i = 0;          
    int k = -1;     // selection
    string s;
    bool validDeadline = false;    
    Date deadlineTemp, scheduled_time;
    while (k != 0) {
        system("cls");
        temp.print();
        cout << "Chon thong tin muon sua: \n"
             << "1. Title\n"
             << "2. Status\n"
             << "3. Priority\n"
             << "4. Scheduled_time\n"
             << "5. Deadline\n"
             << "6. Note\n"
             << "0. Exit \n";
        cout << "Selection: "; cin >> k;
        for (i=0; i < a[temp.getPriority()][temp.getStatus()][temp.getDeadline().day][temp.getDeadline().month].size(); ++i)
            if (a[temp.getPriority()][temp.getStatus()][temp.getDeadline().day][temp.getDeadline().month][i].getTitle() == temp.getTitle()
                && a[temp.getPriority()][temp.getStatus()][temp.getDeadline().day][temp.getDeadline().month][i].getNote() == temp.getNote())
            {
                break;
            }
        switch (k)
        {
        case 1:
            cin.ignore(265, '\n');
            cout << "Title: "; getline(cin, s);
            a[temp.getPriority()][temp.getStatus()][temp.getDeadline().day][temp.getDeadline().month][i].setTitle(s);
            temp.setTitle(s);           // update to show the current task 
            break;
        case 2:
            cout << "Status (0-not_completed  1-in_progress 2-completed 3-cancelled 4-overdue): "; cin >> s;
            --statistics[temp.getStatus()];        // update
            addTask(temp.getPriority(), temp.getTitle(), toInt(s), temp.getScheduledTime(), temp.getDeadline(),temp.getNote());
            a[temp.getPriority()][temp.getStatus()][temp.getDeadline().day][temp.getDeadline().month].erase(a[temp.getPriority()][temp.getStatus()][temp.getDeadline().day][temp.getDeadline().month].begin() + i);
            temp.setStatus(Status(toInt(s)));
            break;
        case 3:     
            cout << "Priority (0-in_a_day 1-in_three_days 2-in_a_week 3-in_two_weeks 4-in_a_month): "; cin >> s;
            --statistics[temp.getStatus()];     // because in addTask() has add statistics
            addTask(toInt(s), temp.getTitle(), temp.getStatus(), temp.getScheduledTime(), temp.getDeadline(),temp.getNote());
            a[temp.getPriority()][temp.getStatus()][temp.getDeadline().day][temp.getDeadline().month].erase(a[temp.getPriority()][temp.getStatus()][temp.getDeadline().day][temp.getDeadline().month].begin() + i);
            temp.setPriority(Priority(toInt(s)));
            break;
        case 4:
            cout << "Scheduled_time (DD/MM/YYYY): "; cin >> s;
            a[temp.getPriority()][temp.getStatus()][temp.getDeadline().day][temp.getDeadline().month][i].setScheduledTime(Date(s));
            temp.setScheduledTime(Date(s));
            if (temp.getDeadline().isValid(temp.getScheduledTime()))
                break;  // if scheduled_time entered is not valid then edit the deadline
            else cout << "Because the scheduled_time has been changed, you need to adjust the deadline: \n";
        case 5: 
            scheduled_time = temp.getScheduledTime();
            while (!validDeadline) {
                cout << "Deadline (must be equal to or after scheduling): "; cin >> s; deadlineTemp.setDate(s);
                validDeadline = deadlineTemp.isValid(scheduled_time);
            }
            --statistics[temp.getStatus()];     // because in addTask() has add statistics
            addTask(temp.getPriority(), temp.getTitle(), temp.getStatus(), temp.getScheduledTime(), Date(s),temp.getNote());
            a[temp.getPriority()][temp.getStatus()][temp.getDeadline().day][temp.getDeadline().month].erase(a[temp.getPriority()][temp.getStatus()][temp.getDeadline().day][temp.getDeadline().month].begin() + i);
            temp.setDeadline(Date(s));
            break;
        case 6:
            cout << "Note: ";  
            cin.ignore(265, '\n');
            getline(cin, s);
            a[temp.getPriority()][temp.getStatus()][temp.getDeadline().day][temp.getDeadline().month][i].setNote(s);
            temp.setNote(s);
            break;
        default:
            break;
        }
    }
}

void addTask( int priority, string title, int status,Date scheduled_time, Date deadline, string note) {
        int mid, l = 0, r = a[priority][status][deadline.day][deadline.month].size() - 1;
        while (l <= r) {
            mid = (l+r) / 2;
            if (mid - 1 != -1)
            {
                if (deadline.year <= a[priority][status][deadline.day][deadline.month][mid].getDeadline().year && deadline.year >= a[priority][status][deadline.day][deadline.month][mid-1].getDeadline().year) break;
            }
            else 
                if (deadline.year <= a[priority][status][deadline.day][deadline.month][mid].getDeadline().year) break;
            if (deadline.year >= a[priority][status][deadline.day][deadline.month][mid].getDeadline().year) l = mid+1;
            else 
                r = mid-1;
        }
        if (l > r)
            a[priority][status][deadline.day][deadline.month].emplace_back(Status(status), title, Priority(priority), scheduled_time, deadline, note);
        else
            if (mid == 0) a[priority][status][deadline.day][deadline.month].insert(a[priority][status][deadline.day][deadline.month].begin(), Task(Status(status), title, Priority(priority), scheduled_time, deadline, note));
        else
            a[priority][status][deadline.day][deadline.month].insert(a[priority][status][deadline.day][deadline.month].begin() + mid , Task(Status(status), title, Priority(priority), scheduled_time, deadline, note));
        ++statistics[status];   
}

void printResult(vector<Task> &result) {
    for (auto x : result)
        x.print();
}

void findTask() {
    system("cls");
    string title, scheduled_time, deadline = "01/01/0001", note;
    int status, priority;
    cin.ignore();
    cout << "Title: "; getline(cin, title);
    cout << "Status (0-not_completed  1-in_progress 2-completed 3-cancelled 4-overdue): "; cin >> status;
    cout << "Priority (0-in_a_day 1-in_three_days 2-in_a_week 3-in_two_weeks 4-in_a_month): "; cin >> priority;
    cout << "Scheduled_time (DD/MM/YYYY): "; cin >> scheduled_time;
    while (!(Date(deadline)).isValid(scheduled_time)) {
        cout << "Deadline (must be equal to or after scheduling): "; cin >> deadline; 
    }
    cout << "Note: "; cin.ignore(); getline(cin, note);
    Task temp = Task(Status(status), title, Priority(priority), Date(scheduled_time), Date(deadline), note);
    bool existence = false;
    for (auto x : a[priority][status][temp.getDeadline().day][temp.getDeadline().month])
        if (x.compare(temp)) {
            existence = true;
            break;
        } 
    if (!existence)
    {
        cout << "\nDoes not exist!\n";
        cout << "Press Enter to exit...";
        getline(cin, title);
    }  
    else
    {
        int k = 3;
        while (k > 0) {
            system("cls");
            temp.print();
            cout << "1. Edit task\n"
                 << "2. Delete task\n"
                 << "0. Exit\n";
            cout << "Selection: "; cin >> k;
            switch (k)
            {
            case 1:
                editTask(temp);
                break;
            case 2:
                deleteTask(temp);
                k = 0;
                break;
            default:
                break;
            }
        }
    }    
}

void filter() {
    currentTasks.clear();
    int k = 10, status = 5, priority = 5;
    string deadline = "N/A", title = "N/A";
    bool first = true;

    while (k > 0) {
        system("cls");
        // Filters that are being applied
        cout << "___________________________________________________________________________________" << endl;
        cout
             << setw(8) << "| Status" 
             << setw(16) << "| Title" 
             << setw(37) << "| Priority" 
             << setw(23) << "| Deadline    |\n" ;
        cout << "|----------------|---------------------------------|----------------|-------------|" << endl;
        int tempSize = title.size();
        cout << "| " << setw(strStatus[status].size()) << strStatus[status]
             << setw(4 + (13 - strStatus[status].size())) << "| " ;
            
            if (tempSize > 30) {   
                for (int chr = 0; chr < 27; ++chr) cout << title[chr];
                cout << "...";
                tempSize = 30;
            }
            else cout << setw(title.size()) << title;
            cout << setw(4 + (30 - tempSize)) << "| " << setw(strPriority[priority].size()) << strPriority[priority]
             << setw(4 + (13 - strPriority[priority].size())) << "| " << setw(10) << deadline 
             << setw(5) << "| \n" ;
        cout << "|---------------------------------------------------------------------------------|\n" << endl;

        // Tasks after applying filters
        cout << "______________________________________________________________________________________________________________________________________________" << endl;
        cout << setw(4) << "| No. " 
             << setw(9) << "| Status" 
             << setw(16) << "| Title" 
             << setw(37) << "| Priority" 
             << setw(23) << "| Scheduled_time" 
             << setw(12) << "| Deadline" 
             << setw(10) << "| Note" << setw(30) << "|\n";
        cout << "|------|----------------|---------------------------------|----------------|-----------------|-------------|---------------------------------|" << endl;
        for (int i = 0; i < currentTasks.size(); ++i) {
            tempSize = currentTasks[i].getTitle().size();
            cout << "| "<< i 
             << setw(4 + (3 - to_string(i).size())) << "| " << setw(strStatus[currentTasks[i].getStatus()].size()) << strStatus[currentTasks[i].getStatus()]
             << setw(4 + (13 - strStatus[currentTasks[i].getStatus()].size())) << "| " ;
            
            if (tempSize > 30) {   
                for (int chr = 0; chr < 27; ++chr) cout << currentTasks[i].getTitle()[chr];
                cout << "...";
                tempSize = 30;
            }
            else cout << setw(currentTasks[i].getTitle().size()) << currentTasks[i].getTitle();
            cout << setw(4 + (30 - tempSize)) << "| " << setw(strPriority[currentTasks[i].getPriority()].size()) << strPriority[currentTasks[i].getPriority()]
             << setw(4 + (13 - strPriority[currentTasks[i].getPriority()].size())) << "| " << setw(10) << currentTasks[i].getScheduledTime().getDate() 
             << setw(8) << "| " << setw(10) << currentTasks[i].getDeadline().getDate()
             << setw(4) << "| ";
            tempSize = currentTasks[i].getNote().size();
            if (tempSize > 30) {
                for (int chr = 0; chr < 28; ++chr) cout << currentTasks[i].getNote()[chr]; cout << "... |\n";    
            }
            else
            cout << setw(currentTasks[i].getNote().size()) << currentTasks[i].getNote() << setw(4 + 30 - currentTasks[i].getNote().size()) << "|\n";
        }
        cout << "|--------------------------------------------------------------------------------------------------------------------------------------------|\n\n";
        
        if (first) {
            cout << "1. Filter by status\n"
                 << "2. Filter by priority\n"
                 << "3. Filter by deadline\n"
                 << "4. Select Task\n"
                 << "0. Exit\n";
            cout << "Selection: "; cin >> k;
            switch (k)
            {
            case 1:
                first = false;
                cout << "\nStatus (0-not_completed  1-in_progress 2-completed 3-cancelled 4-overdue): "; cin >> status;
                filterByStatus(status);
                break;
            case 2:
                first = false;
                cout << "Priority (0-in_a_day 1-in_three_days 2-in_a_week 3-in_two_weeks 4-in_a_month): "; cin >> priority;
                filterByPriority(priority);
                break;
            case 3:
                first = false;
                cout << "Deadline: "; cin >> deadline;
                filterByDeadline(Date(deadline));
                break;
            case 4: 
                selectTask2(title, status, priority, deadline);
                break;
            default:
                break;
            }
        }
        else {
            cout << "1. Add filter by status\n"
                 << "2. Add filter by priority\n"
                 << "3. Add filter by deadline\n"
                 << "4. Add filter by title\n"
                 << "5. Select task\n"
                 << "6. Write to .txt\n"
                 << "0. Exit (clear all filters and exit) \n";
            cout << "Selection: "; cin >> k;
            switch (k)
            {
            case 1:
                if (status == 5) {
                    cout << "\nStatus (0-not_completed  1-in_progress 2-completed 3-cancelled 4-overdue): "; cin >> status;
                    filterByStatus2(Status(status));
                }
                break;
            case 2:
                if (priority == 5) {
                    cout << "Priority (0-in_a_day 1-in_three_days 2-in_a_week 3-in_two_weeks 4-in_a_month): "; cin >> priority;
                    filterByPriority2(Priority(priority));
                }
                break;
            case 3:
                if (deadline == "N/A") {
                    cout << "Deadline (DD/MM/YYYY): "; cin >> deadline;
                    filterByDeadline2(Date(deadline));
                }
                break;
            case 4: 
                if (title == "N/A") {
                    cout << "Title: "; cin.ignore(); getline(cin, title);
                    filterByTitle(title);
                }
                break;
            case 5:
                selectTask2(title, status, priority, deadline);
                break;
            case 6:
                writeToTxt_Filter();
                break;
            case 0:
                first = true;
                currentTasks.clear();
                status = priority = 5;
                deadline = title = "N/A";
                k = 10;                             // to return the screen displaying the first filter;
                break;
            default:
                break;
            }
        }
        
    }
}

void filterWith3Conditions(int priority, int status, Date deadline) {
    int l = 0, r = a[priority][status][deadline.day][deadline.month].size()-1, mid, best = a[priority][status][deadline.day][deadline.month].size() - 1;
    while (l <= r) {
        mid = (l + r) / 2;
        if (deadline.year == a[priority][status][deadline.day][deadline.month][mid].getDeadline().year)
        {
            if (mid < best) best = mid;
            r = mid - 1;
        }
        else 
            if (deadline.year < a[priority][status][deadline.day][deadline.month][mid].getDeadline().year) 
                r = mid - 1;
        else l = mid + 1;
    }
    while (a[priority][status][deadline.day][deadline.month][best].getDeadline().year == deadline.year && best < a[priority][status][deadline.day][deadline.month].size()) {
        currentTasks.push_back(a[priority][status][deadline.day][deadline.month][best]);
        ++best;
    }
}

void filterByPriority( int priority) {
    currentTasks.clear();
    for (int status = 0; status < 5; ++status)
        for (int day = 1; day < 32; ++day)
            for (int month = 1; month < 13; ++month)
                if (a[priority][status][day][month].size() > 0) {
                    for (auto x : a[priority][status][day][month])
                        currentTasks.push_back(x);
                }
}

void filterByStatus( int status) {
    currentTasks.clear();
    for (int priority = 0; priority < 5; ++priority) 
        for (int day = 1; day < 32; ++day)
            for (int month = 1; month < 13; ++month)
                if (a[priority][status][day][month].size() > 0)
                    for (auto x : a[priority][status][day][month])
                        currentTasks.push_back(x);
}

void filterByDeadline( Date deadline) {
    currentTasks.clear();
    for (int priority = 0; priority < 5; ++priority)
        for (int status = 0; status < 5; ++status)
            if (a[priority][status][deadline.day][deadline.month].size() > 0)
                filterWith3Conditions( priority, status, deadline);
}

void filterByPriority2(Priority priority) {
    vector<Task> temp;
    for (auto x : currentTasks) {
        if (x.getPriority() == priority) temp.push_back(x);
    }
    currentTasks.clear();
    currentTasks = temp;
}

void filterByStatus2( Status status) {
    vector<Task> temp;
    for (auto x : currentTasks) {
        if (x.getStatus() == status) temp.push_back(x);
    }
    currentTasks.clear();
    currentTasks = temp;
}

void filterByDeadline2( Date deadline) {
    vector <Task> temp;
    for (auto x : currentTasks) {
        if (x.getDeadline().getDate() == deadline.getDate()) temp.push_back(x);
    }
    currentTasks.clear();
    currentTasks = temp;
}

void selectTask2(string title, int status, int priority, string deadline) {
    int x;
    cout << "Which task do you choose? (0 1 2 3 ...): "; cin >> x;
    cout << "\n";
    int k = 3;
    bool check = false;
    Task temp = currentTasks[x];
    while (k > 0) {
        system("cls");
        currentTasks[x].print();
        cout << "1. Edit task\n"
             << "2. Delete task\n"
             << "0. Exit\n";
        cout << "Selection: "; cin >> k;
        switch (k)
        {
        case 1:
            editTask(currentTasks[x]);
            check = true;
            break;
        case 2:
            deleteTask(currentTasks[x]);
            currentTasks.erase(currentTasks.begin() + x);
            k = 0;
            break;
        default:
            break;
        }
    }
    if (check)
    {
        if (title != "N/A")
            if (currentTasks[x].getTitle().find(title) == -1) {
                currentTasks.erase(currentTasks.begin() + x);
                return;
            }
        if (status != 5) 
            if (currentTasks[x].getStatus() != status) {
                currentTasks.erase(currentTasks.begin() + x);
                return;
            }
        if (priority != 5)
            if (currentTasks[x].getPriority() != priority) {
                currentTasks.erase(currentTasks.begin() + x);
                return;
            }
        if (deadline != "N/A")
            if (currentTasks[x].getDeadline().getDate() != deadline) {
                currentTasks.erase(currentTasks.begin() + x);
                return;
            }
    }
}

void filterByTitle(string title) {
    vector <Task> temp;
    for (auto x : currentTasks) {
        if (x.getTitle().find(title) != -1) temp.push_back(x);
    }
    currentTasks.clear();
    currentTasks = temp;
}