#include <fstream>
#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <cstring>

using namespace std;
ifstream fin("MinimalDFA.in");
ofstream fout("MinimalDFA.out");

int m, m2, n, n2, q0, q02, l, k, k2;
unordered_map<char, int> M[302], M2[302];
vector<int> inapoi[302], vizitari;
char alf[302], alf2[302];
bool F[302], F2[302];
bool accesibil[302];
bool nonechivalent[302][302];
bool viz[302];

void del(int x, int y)
{
    n2--;
    if(F2[x])
        k2--;
    for(int i = x; i < n2; i++)
    {
        M2[i] = M2[i + 1];
        F2[i] = F2[i + 1];
        accesibil[i] = accesibil[i + 1];
        viz[i] = viz[i + 1];
    }
    if(q02 > x)
        q02--;
    for(int i = 0; i < n2; i++)
    {
        for(int z = 0; z < m2; z++)
        {
            char a = alf2[z];
            if(M2[i][a] == x)
                M2[i][a] = y;
            if(M2[i][a] > x)
                M2[i][a]--;
        }
    }
    for(int j = 0; j < n2; j++)
        for(int i = x; i < n2; i++)
            nonechivalent[i][j] = nonechivalent[i + 1][j];

    for(int j = x; j < n2; j++)
        for(int i = 0; i < n2; i++)
            nonechivalent[i][j] = nonechivalent[i][j + 1];

}

void DFA_to_DFAmin()
{
    q02 = q0;
    n2 = n;
    m2 = m;
    strcpy(alf2, alf);
    for(int i = 0; i < n2; i++)
        M2[i] = M[i];
    for(int i = 0; i < n2 - 1; i++)
        for(int j = i + 1; j < n2; j++)
            if(F[i] != F[j])
                nonechivalent[i][j] = true;
    bool modificare = true;
    while(modificare)
    {
        modificare = false;
        for(int i = 0; i < n2 - 1; i++)
            for(int j = i + 1; j < n2; j++)
                if(!nonechivalent[i][j])
                    for(int k = 0; k < m2; k++)
                    {
                        char a = alf2[k];
                        int x = M2[i][a];
                        int y = M2[j][a];
                        if(nonechivalent[x][y])
                        {
                            modificare = true;
                            nonechivalent[i][j] = true;
                        }
                    }
    }

    for(int i = 0; i < n2; i++)
        F2[i] = F[i];
    k2 = k;

    for(int i = 0; i < n2 - 1; i++)
        for(int j = i + 1; j < n2; j++)
            if(!nonechivalent[i][j])
            {
                if(j != q02)
                {
                    del(j,i);
                    j--;
                }
                else
                {
                    del(i,j);
                    j = i;
                }
            }

    for(int i = 0; i < n2; i++)
        for(int j = 0; j < m2; j++)
            inapoi[M2[i][alf2[j]]].push_back(i);

    for(int i = 0; i < n2; i++)
        if(F2[i])
        {
            accesibil[i] = true;
            vizitari.push_back(i);
        }
    for(int i = 0; i < vizitari.size(); i++)
        for(int j : inapoi[vizitari[i]])
            if(!accesibil[j])
            {
                accesibil[j] = true;
                vizitari.push_back(j);
            }
    for(int i = 0; i < n2; i++)
        if(!accesibil[i])
        {
            del(i, -1);
            i--;
        }

    queue<int> Q;
    viz[q02] = true;
    Q.push(q02);

    while(!Q.empty())
    {
        int x = Q.front();
        Q.pop();
        for(int i = 0; i < m2; i++)
        {
            char a = alf2[i];
            int y = M2[x][a];
            if(!viz[y] && y != -1)
            {
                viz[y] = true;
                Q.push(y);
            }
        }
    }

    for(int i = 0; i < n2; i++)
        if(!viz[i])
        {
            del(i, -1);
            i--;
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

    for(int i = 0; i < n; i++)
        for(int j = 0; j < m; j++)
            M[i][alf[j]] = -1;

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
        M[x][a] = y;
    }


    DFA_to_DFAmin();

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
        for(int t = 0; t < m2; t++)
            if(M2[i][alf2[t]] != -1)
                fout << i << ' ' << alf2[t] << ' ' << M2[i][alf2[t]] << '\n';

    return 0;
}
