#include <bits/stdc++.h>

using namespace std;

ifstream fin("RemoveLambda.in");
ofstream fout("RemoveLambda.out");

int m, m2, n, n2, q0, q02, l, k, k2;
unordered_map <char, vector <int>> M[302], M2[302];
char alf[302], alf2[302];
bool F[302], F2[302];

void close(int x, vector<int> &v)
{
    queue<int> Q;
    bool viz[302] = {false};

    Q.push(x);
    viz[x] = true;
    while(!Q.empty()) {
        int y = Q.front();
        Q.pop();
        v.push_back(y);
        for(int z : M[y]['$'])
            if(!viz[z]) {
                viz[z] = true;
                Q.push(z);
            }
    }
}

void maketrans(int x, vector<int> v[])
{
    for(int i = 0; i < m2; i++) {
        char a = alf2[i];
        queue<int> Q, Q2;
        int viz[302] = {0};
        for(int y : v[x])
            Q.push(y);

        while(!Q.empty()) {
            int y = Q.front();
            Q.pop();
            for(int z : M[y][a])
                if(viz[z] != 1) {
                    viz[z] = 1;
                    Q2.push(z);
                }
        }

        while(!Q2.empty()) {
            int y = Q2.front();
            Q2.pop();
            for(int z : v[y]) {
                if(viz[z] != 2) {
                    viz[z] = 2;
                    M2[x][a].push_back(z);
                }
            }
        }
    }
}

void del(int x, int y)
{
    n2--;
    if(F2[x])
        k2--;
    for(int i = x; i < n; i++) {
        M2[i] = M2[i + 1];
        F2[i] = F2[i + 1];
    }
    if(q02 > x)
        q02--;
    for(int i = 0; i < n; i++) {
        for(int z = 0; z < m2; z++) {
            char a = alf2[z];
            int len = M2[i][a].size();
            for(int j = 0; j < len; j++) {
                if(M2[i][a][j] == x)
                    M2[i][a][j] = y;
                if(M2[i][a][j] > x)
                    M2[i][a][j]--;
            }

            bool g = false;

            for(int j = 0; j < len; j++)
                if(M2[i][a][j] == y) {
                    if(g)
                        M2[i][a].erase(M2[i][a].begin() + j);
                    else
                        g = true;
                }
        }
    }
}

void LNFA_to_NFA()
{
    vector<int> closed[302];

    for(int i = 0; i < n; i++)
        close(i, closed[i]);

    m2 = m;
    n2 = n;
    strcpy(alf2, alf);
    alf2[m] = '\0';
    q02 = q0;
    for(int i = 0; i < n2; i++)
        maketrans(i, closed);
    for(int i = 0; i < n2; i++)
        for(int j : closed[i])
            if(F[j]) {
                F2[i] = true;
                k2++;
                break;
            }
    for(int i = 0; i < n2; i++)
        for(int j = 0; j < m2; j++) {
            char a = alf2[j];
            sort(M2[i][a].begin(), M2[i][a].end());
        }
    for(int i = 0; i < n2 - 1; i++)
        for(int j = i + 1; j < n2; j++)
            if(M2[i] == M2[j] && F2[i] == F2[j]) {
                if(j != q02) {
                    del(j, i);
                    j--;
                }
                else {
                    del(i, j);
                    j = i;
                }
            }
}


int main()
{
    int final_state;
    fin >> n;
    fin >> m;
    for(int i = 0; i < m; i++)
        fin >> alf[i];

    alf[m] = '$';
    alf[m+1] = '\0';

    fin >> q0;
    fin >> k;

    for(int i = 0; i < k; i++) {
        fin >> final_state;
        F[final_state] = true;
    }

    fin >> l;
    for(int i = 1; i <= l; i++) {
        int x, y;
        char a;
        fin >> x >> a >> y;
        M[x][a].push_back(y);
    }

    LNFA_to_NFA();

    fout << "Noul numar de stari si lungimea alfabetului: ";
    fout << n2 << ' ' << m2 << '\n';
    fout << "Noul alfabet al limbajului: \n";
    for(int i = 0; i < m2; i++)
        fout << alf2[i] << ' ';


    fout << "\nNoua stare initiala: " << q02 << "\nNoul numar de stari finale: " << k2 << "\nNoile stari finale:\n";

    for(int i = 0; i < n2; i++)
        if(F2[i])
            fout << i << ' ';

    fout << "\nNoile tranzitii ale automatului:\n";

    for(int i = 0; i < n2; i++) {
        for(int t = 0; t < m2; t++) {
            char a = alf2[t];
            for(int y : M2[i][a])
                fout << i << ' ' << a << ' ' << y << '\n';
        }
    }
    return 0;
}
