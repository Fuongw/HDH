#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
using namespace std;

int disk_size = 200; // Mặc định miền đĩa: 0 đến 199

void FIFO(vector<int> requests, int head) {
    int distance = 0;
    cout << "Truy cap theo thu tu: ";
    for (int r : requests) {
        cout << r << " ";
        distance += abs(r - head);
        head = r;
    }
    cout << "\nTong quang duong: " << distance << endl;
}

void SSTF(vector<int> requests, int head) {
    int distance = 0;
    vector<bool> visited(requests.size(), false);
    cout << "Truy cap theo thu tu: ";
    for (int i = 0; i < requests.size(); ++i) {
        int min_dist = 1e9, idx = -1;
        for (int j = 0; j < requests.size(); ++j) {
            if (!visited[j] && abs(requests[j] - head) < min_dist) {
                min_dist = abs(requests[j] - head);
                idx = j;
            }
        }
        visited[idx] = true;
        distance += abs(requests[idx] - head);
        head = requests[idx];
        cout << head << " ";
    }
    cout << "\nTong quang duong: " << distance << endl;
}

void SCAN(vector<int> requests, int head, string direction, int include_boundaries) {
    int distance = 0;
    vector<int> left, right;
    if (include_boundaries) {
        if (direction == "trai") left.push_back(0);
        else right.push_back(disk_size - 1);
    }

    for (int r : requests) {
        if (r < head) left.push_back(r);
        else right.push_back(r);
    }
    sort(left.begin(), left.end());
    sort(right.begin(), right.end());

    cout << "Truy cap theo thu tu: ";
    if (direction == "trai") {
        for (int i = left.size() - 1; i >= 0; --i) {
            cout << left[i] << " ";
            distance += abs(head - left[i]);
            head = left[i];
        }
        for (int i = 0; i < right.size(); ++i) {
            cout << right[i] << " ";
            distance += abs(head - right[i]);
            head = right[i];
        }
    } else {
        for (int i = 0; i < right.size(); ++i) {
            cout << right[i] << " ";
            distance += abs(head - right[i]);
            head = right[i];
        }
        for (int i = left.size() - 1; i >= 0; --i) {
            cout << left[i] << " ";
            distance += abs(head - left[i]);
            head = left[i];
        }
    }
    cout << "\nTong quang duong: " << distance << endl;
}

void LOOK(vector<int> requests, int head, string direction) {
    int distance = 0;
    vector<int> left, right;

    for (int r : requests) {
        if (r < head) left.push_back(r);
        else right.push_back(r);
    }
    sort(left.begin(), left.end());
    sort(right.begin(), right.end());

    cout << "Truy cap theo thu tu: ";
    if (direction == "trai") {
        for (int i = left.size() - 1; i >= 0; --i) {
            cout << left[i] << " ";
            distance += abs(head - left[i]);
            head = left[i];
        }
        for (int i = 0; i < right.size(); ++i) {
            cout << right[i] << " ";
            distance += abs(head - right[i]);
            head = right[i];
        }
    } else {
        for (int i = 0; i < right.size(); ++i) {
            cout << right[i] << " ";
            distance += abs(head - right[i]);
            head = right[i];
        }
        for (int i = left.size() - 1; i >= 0; --i) {
            cout << left[i] << " ";
            distance += abs(head - left[i]);
            head = left[i];
        }
    }
    cout << "\nTong quang duong: " << distance << endl;
}

void CSCAN(vector<int> requests, int head, string direction, int include_boundaries) {
    int distance = 0;
    vector<int> left, right;
    if (include_boundaries) {
        left.push_back(0);
        right.push_back(disk_size - 1);
    }

    for (int r : requests) {
        if (r < head) left.push_back(r);
        else right.push_back(r);
    }
    sort(left.begin(), left.end());
    sort(right.begin(), right.end());

    cout << "Truy cap theo thu tu: ";
    if (direction == "trai") {
        for (int i = left.size() - 1; i >= 0; --i) {
            cout << left[i] << " ";
            distance += abs(head - left[i]);
            head = left[i];
        }
        head = disk_size - 1;
        distance += abs(0 - head);
        for (int i = right.size() - 1; i >= 0; --i) {
            cout << right[i] << " ";
            distance += abs(head - right[i]);
            head = right[i];
        }
    } else {
        for (int i = 0; i < right.size(); ++i) {
            cout << right[i] << " ";
            distance += abs(head - right[i]);
            head = right[i];
        }
        head = 0;
        distance += abs(disk_size - 1 - head);
        for (int i = 0; i < left.size(); ++i) {
            cout << left[i] << " ";
            distance += abs(head - left[i]);
            head = left[i];
        }
    }
    cout << "\nTong quang duong: " << distance << endl;
}

void CLOOK(vector<int> requests, int head, string direction) {
    int distance = 0;
    vector<int> left, right;

    for (int r : requests) {
        if (r < head) left.push_back(r);
        else right.push_back(r);
    }
    sort(left.begin(), left.end());
    sort(right.begin(), right.end());

    cout << "Truy cap theo thu tu: ";
    if (direction == "trai") {
        for (int i = left.size() - 1; i >= 0; --i) {
            cout << left[i] << " ";
            distance += abs(head - left[i]);
            head = left[i];
        }
        if (!right.empty()) {
            distance += abs(head - right.back());
            head = right.back();
            for (int i = right.size() - 1; i >= 0; --i) {
                cout << right[i] << " ";
                if (i != (int)right.size() - 1)
                    distance += abs(head - right[i]);
                head = right[i];
            }
        }
    } else {
        for (int i = 0; i < right.size(); ++i) {
            cout << right[i] << " ";
            distance += abs(head - right[i]);
            head = right[i];
        }
        if (!left.empty()) {
            distance += abs(head - left[0]);
            head = left[0];
            for (int i = 0; i < left.size(); ++i) {
                cout << left[i] << " ";
                if (i != 0)
                    distance += abs(head - left[i]);
                head = left[i];
            }
        }
    }
    cout << "\nTong quang duong: " << distance << endl;
}

int main() {
    int n, head;
    cout << "Nhap so luong yeu cau: ";
    cin >> n;
    vector<int> requests(n);
    cout << "Nhap cac vi tri yeu cau: ";
    for (int i = 0; i < n; ++i)
        cin >> requests[i];
    cout << "Nhap vi tri dau doc ban dau: ";
    cin >> head;

    // Hỏi người dùng có muốn kiểm tra miền (0...199) hay không
    int check_range;
    cout << "Ban co muon kiem tra cac vi tri co nam trong mien [0..." << disk_size - 1 << "] khong? (1 = Co, 0 = Khong): ";
    cin >> check_range;

    if (check_range) {
        if (head < 0 || head >= disk_size) {
            cout << "Loi: Vi tri dau doc nam ngoai mien hop le!\n";
            return 1;
        }
        for (int i = 0; i < n; ++i) {
            if (requests[i] < 0 || requests[i] >= disk_size) {
                cout << "Loi: Yeu cau thu " << i + 1 << " nam ngoai mien hop le!\n";
                return 1;
            }
        }
    }

    int choice;
    cout << "\nChon thuat toan:\n1. FIFO\n2. SSTF\n3. SCAN\n4. LOOK\n5. C-SCAN\n6. C-LOOK\nChon: ";
    cin >> choice;

    string direction;
    int include_boundaries = 0;
    if (choice >= 3 && choice <= 6) {
        cout << "Nhap huong (trai/phai): ";
        cin >> direction;
        if (choice == 3 || choice == 5) {
            cout << "Tinh ca bien dia 0 va 199? (1 = Co, 0 = Khong): ";
            cin >> include_boundaries;
        }
    }

    switch (choice) {
        case 1: FIFO(requests, head); break;
        case 2: SSTF(requests, head); break;
        case 3: SCAN(requests, head, direction, include_boundaries); break;
        case 4: LOOK(requests, head, direction); break;
        case 5: CSCAN(requests, head, direction, include_boundaries); break;
        case 6: CLOOK(requests, head, direction); break;
        default: cout << "Lua chon khong hop le!\n";
    }

    return 0;
}
