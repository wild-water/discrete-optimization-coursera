#include <bits/stdc++.h>

#include <utility>

using namespace std;

struct Item {
    double weight, value;
    int idx;
};

struct LpResult {
    double lp_value, int_value;
    vector<int> int_sol;

    LpResult(double lp_value_, double int_value_, vector<int> int_sol_) :
            lp_value(lp_value_), int_value(int_value_), int_sol(std::move(int_sol_)) {
    }
};

chrono::steady_clock::time_point clock_begin = chrono::steady_clock::now();
double best_value = 0, eps = 1e-8;
vector<int> best_solution, current_solution;
bool optimal = true;


LpResult get_lp_value(double k_weight, int cur_index, vector<Item> &items) {
    double lp_res = 0, int_res = 0;
    vector<int> int_sol = current_solution;
    for (int i = cur_index; i < items.size() && k_weight >= eps; ++i) {
        double c = min(k_weight / items[i].weight, 1.0);
        k_weight -= c * items[i].weight;
        lp_res += c * items[i].value;
        if (abs(c - 1.0) < eps) {
            int_res += items[i].value;
            int_sol[items[i].idx] = 1;
        }
    }

    LpResult res(lp_res, int_res, int_sol);
    return res;
}


void search(double cur_value, double cur_weight, int cur_index,
            double k_weight, vector<Item> &items) {

    chrono::steady_clock::time_point clock_end = chrono::steady_clock::now();

    if (chrono::duration_cast<std::chrono::seconds>(clock_end - clock_begin).count() > 600) {
        optimal = false;
        return;
    }

    auto lp_results = get_lp_value(k_weight - cur_weight,
                                   cur_index, items);

    if (best_value < cur_value + lp_results.int_value) {
        best_value = cur_value + lp_results.int_value;
        best_solution = lp_results.int_sol;
    }

    if (cur_value + lp_results.lp_value <= best_value || cur_index == items.size()) {
        return;
    }

    if (cur_weight + items[cur_index].weight <= k_weight) {
        current_solution[items[cur_index].idx] = 1;
        search(cur_value + items[cur_index].value, cur_weight + items[cur_index].weight,
               cur_index + 1, k_weight, items);
        current_solution[items[cur_index].idx] = 0;
    }
    search(cur_value, cur_weight, cur_index + 1, k_weight, items);
}


void solve(char *input_file) {
    double k_weight; // knapsack's weight
    int n_items;     // number of items

    ifstream inp;
    inp.open(input_file);
    inp >> n_items >> k_weight;
    vector<Item> items(n_items);
    for (int i = 0; i < n_items; ++i) {
        inp >> items[i].value >> items[i].weight;
        items[i].idx = i;
    }

    sort(items.begin(), items.end(), [](Item x, Item y) {
        return x.value * y.weight > y.value * x.weight;
    });

    best_solution.assign(n_items, 0);
    current_solution.assign(n_items, 0);

    search(0, 0, 0, k_weight, items);
    cout << (int) best_value << " ";

    if (optimal) {
        cout << 1 << "\n";
    } else {
        cout << 0 << "\n";
    }

    for (auto v: best_solution) {
        cout << v << " ";
    }

    cout << "\n";
}


int main(int argc, char *argv[]) {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    solve(argv[1]);
    return 0;
}
