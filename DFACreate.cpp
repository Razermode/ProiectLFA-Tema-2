#include <bits/stdc++.h>

using namespace std;
ifstream fin("DFACreate.in");
ofstream fout("DFACreate.out");

int m, m2, n, n2, q0, q02, l, k, k2;
unordered_map<char, vector<int>> M[302], Mvect[302];
unordered_map<char, int>M2[302];
char alf[302], alf2[302];
bool F[302], F2[302];
vector<vector<int>> state;



void NFA_to_DFA()
{
    m2 = m;
    q02 = 0;
    n2 = 1;
    strcpy(alf2, alf);
    state.push_back({q0});
    for(int i = 0; i < m2; i++)
    {
        char a = alf2[i];
        Mvect[0][a] = M[q0][a];
        if(!Mvect[0][a].empty())
        {
            sort(Mvect[0][a].begin(), Mvect[0][a].end());
            bool g = false;
            for(int j = 0; j < n2 && !g; j++)
                if(Mvect[0][a] == state[i])
                    g = true;
            if(!g)
            {
                n2++;
                state.push_back(Mvect[0][a]);
            }
        }
    }

    for(int i = 1; i < n2; i++)
    {
        for(int j = 0; j < m2; j++)
        {
            char a = alf2[j];
            bool viz[302] = {false};

            for(int z : state[i])
            {
                for(int y : M[z][a])
                    if(!viz[y])
                    {
                        Mvect[i][a].push_back(y);
                        viz[y] = true;
                    }
            }

            if(!Mvect[i][a].empty())
            {
                sort(Mvect[i][a].begin(), Mvect[i][a].end());
                bool g = false;
                for(int t = 0; t < n2 && !g; t++)
                    if(Mvect[i][a] == state[t])
                        g = true;
                if(!g)
                {
                    n2++;
                    state.push_back(Mvect[i][a]);
                }
            }
        }
    }

    for(int i = 0; i < n2; i++)
    {
        for(int j : state[i])
            if(F[j])
            {
                F2[i] = true;
                k2++;
                break;
            }
    }

    for(int i = 0; i < n2; i++)
    {
        for(int j = 0; j < m2; j++)
        {
            char a = alf2[j];
            if(!Mvect[i][a].empty())
            {
                for(int y = 0; y < n2; y++)
                    if(Mvect[i][a] == state[y])
                    {
                        M2[i][a] = y;
                        break;
                    }
            }
            else
                M2[i][a] = -1;
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

    alf[m] = '\0';

    fin >> q0;

    fin >> k;

    for(int i = 0; i < k; i++)
    {
        fin >> final_state;
        F[final_state] = true;
    }

    fin >> l;
    for(int i = 1; i <= l; i++)
    {
        int x, y;
        char a;
        fin >> x >> a >> y;
        M[x][a].push_back(y);
    }

    NFA_to_DFA();

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

    for(int i = 0; i < n2; i++)
    {
        for(int t = 0; t < m2; t++)
        {
            char a = alf2[t];
            if(M2[i][a] != -1)
                fout << i << ' ' << a << ' ' << M2[i][a] << '\n';
        }
    }

    return 0;
}
