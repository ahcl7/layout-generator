#include <bits/stdc++.h>

//#define int long long

using namespace std;
const int inf = 1e6;
const int mod = 1000000007;
const int gap = 8;
const double splitCost = 0.2;

typedef vector<double> vi;
typedef vector<vi> vii;
typedef vector<vii> viii;

typedef pair<int,int> ii;
typedef pair<int,ii> iii;


iii call(int w, int h, int n) {
    double ret = 0;
    int besta = 0;
    int bestb = 0;
    for(int i = 1; i <= n; i++) {
        if (n % i == 0) {
            int a = i;
            int b = n / i;
            double unitW = (1. * w - gap * (a - 1)) / a;
            double unitH = (1. * h - gap * (b - 1)) / b;
            double k = min(1.0 * unitW / 16, 1.0 * unitH / 9);
            if (k * 9 >= 100 && unitW / unitH <= 1.8 && unitW / unitH >= 1.6) {
                double t = k * k * 9 * 16 * n;
                if (t > ret) {
                    ret = t;
                    besta = a;
                    bestb = b;
                }
            }
        }
    }
    return iii(ret, ii(besta, bestb));
}

double dp(int w, int h, int n, viii& f) {
    if (f[w][h][n] != -1.0) return f[w][h][n];
    if(w < 100 || h < 100) return 0;
    double ret = call(w, h, n).first;
    if (n > 1) {
//        for(int i = 100; i < h; i++) {
//            ret = (max(ret, dp(w, i, n, f)));
//        }
        for(int i = 100; i <= w / 2; i++) {
            for(int j = 1; j < n; j++) {
                double a = dp(i, h, j, f);
                if (a == 0) continue;
                double b = dp(w - i - gap, h, n - j, f);
                if (b == 0) continue;
                double t = max(0., a + b - w * h * splitCost);
                if (a > 0 && b > 0) ret = max(ret, t);
            }
        }
        for(int i = 100; i <= h / 2; i++) {
            for(int j = 1; j < n; j++) {
                double a = dp(w, i, j, f);
                if (a == 0) continue;
                double b = dp(w, h - i - gap, n - j, f);
                if (b == 0) continue;
                double t = max(0., a + b - w * h * splitCost);
                if (a > 0 && b > 0) ret = max(ret, t);
            }
        }
    }
    f[w][h][n] = ret;
    return ret;
}

void trace(int w, int h, int n, viii& f, int x, int y) {
//    cout << endl;
//    cout << w <<" " << h <<" " << n << endl;
    iii tmp = call(w, h, n);
    if (tmp.first == f[w][h][n]) {
        int a = tmp.second.first;
        int b = tmp.second.second;
        double unitW = (1. * w - gap * (a - 1)) / a;
        double unitH = (1. * h - gap * (b - 1)) / b;
        for(int i = 0 ; i < a; i++) {
            for(int j = 0 ; j < b; j++) {
                double xx = x + (unitW + gap) * i;
                double yy = y + (unitH + gap) * j;
                cout << "\t\t\t[ " <<  xx << ", "<< yy << ", " << unitW <<", " << unitH << " ], " << endl;
            }
        }
        return;
    }

    if (n > 1) {
//        for(int i = 0; i < w; i++) {
//            if (f[i][h][n] == f[w][h][n]) {
//                trace(i, h, n, f, x, y);
//                return;
//            }
//        }
//        for(int i = 0; i < h; i++) {
//            if (f[w][i][n] == f[w][h][n]) {
//                trace(w, i, n, f, x, y);
//                return;
//            }
//        }
        for(int i = 100; i <= w / 2; i++) {
            for(int j = 1; j < n; j++) {
                double a = dp(i, h, j, f);
                if (a == 0) continue;
                double b = dp(w - i - gap, h, n - j, f);
                if (b == 0) continue;
                double t = max(a + b - w * h * splitCost, 0.);
                if (t == f[w][h][n]) {
//                    cout << "vertical " << i <<" " << j << endl;
                    trace(i, h, j, f, x, y);
                    trace(w - i - gap, h, n - j, f, x + i + gap, y);
                    return;
                }
            }
        }
        for(int i = 100; i <= h / 2; i++) {
            for(int j = 1; j < n; j++) {
                double a = dp(w, i, j, f);
                if (a == 0) continue;
                double b = dp(w, h - i - gap, n - j, f);
                if (b == 0) continue;
                double t = max(a + b - w * h * splitCost, 0.);
                if (t == f[w][h][n]) {
//                    cout << "horizontal " << i <<" " << j << endl;
                    trace(w, i, j, f, x, y);
                    trace(w, h - i - gap, n - j, f, x, y + i + gap);
                    return;
                }
            }
        }
    }
}

void solve(int maxW, int maxH, int n) {
    cout << "{\n\twidth: " << maxW << ",\n\theight: " <<  maxH <<  ",\n\tn: " << n << ",\n\trectangles:\n\t\t[\n";
    maxW -= gap * 2;
    maxH -= gap * 2;
    int gap = 8;
    int g = __gcd(maxW, maxH);
//    maxW /= g;
//    maxH /= g;
//    int n = 4;
//    cout << maxW <<" " << maxH <<" " << n << endl;
    viii f(maxW + 1, vii(maxH + 1, vi(n + 1, -1.0)));
//    cout << dp(maxW, maxH, n, f) << endl;
    double t = dp(maxW, maxH, n, f);
    double best = t;
    int bestCutW = maxW;
    int bestCutH = maxH;
    for(int i = 1; i < maxW; i++) {
        if (best <= dp(i, maxH, n, f))
        {
            best = dp(i, maxH, n, f);
            bestCutW = i;
        }
    }
    for(int i = 1; i < maxH; i++) {
        if (best <= dp(maxW, i, n, f))
        {
            best = dp(maxW, i, n, f);
            bestCutH = i;
            bestCutW = maxW;
        }
    }
//    cout << best <<" " << bestCutW<<" " << bestCutH << endl;
    trace(bestCutW, bestCutH, n, f, gap - 1 + (maxW - bestCutW) / 2, gap - 1 + (maxH - bestCutH) / 2);
    cout << "\t\t]\n}, \n";
}

int32_t main() {
//    freopen("inp.txt", "r", stdin);
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    vector<ii> window_sizes({
                            ii(1383, 1037),
                            ii(742, 1058),
                            ii(1383, 468),
                            ii(742, 468)
                            });
    cout << "[" << endl;
    for(int i = 0 ; i < 4; i++) {
        for(int j = 1; j <= 5; j++) {
            solve(window_sizes[i].first, window_sizes[i].second, j);
        }
    }
    cout << "]" << endl;
    return 0;
}
