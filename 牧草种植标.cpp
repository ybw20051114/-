#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<iostream>
#include<ctime>
#include<queue>
using namespace std;

const int N = 2 * 100010, S = 30, D = 20;
struct node
{
    int x, y, next;
} a[2 * N];
struct trnode
{
    int l, r, lc, rc, d;
} t[2 * N];
int n, m, len, num, first[N], dep[N], f[N][S], tot[N], zs[N], dfn[N], top[N],
    c0[N], c1[N], delta[N];
char s[10];

void add(int x, int d)
{
    for(int i = x; i <= n; i += (i & (-i)))
        c0[i] += d, c1[i] += d * x;
}

int getsum(int x)
{
    int a0 = 0, a1 = 0;
    for(int i = x; i >= 1; i -= (i & (-i)))
        a0 += c0[i], a1 += c1[i];
    return a0 * (x + 1) - a1;
}


void ins(int x, int y)
{
    a[++len].x = x;
    a[len].y = y;
    a[len].next = first[x];
    first[x] = len;
}

void dfs(int x, int fa)
{
    dep[x] = dep[fa] + 1;
    f[x][0] = fa;
    tot[x] = 1;
    zs[x] = 0;
    for(int i = first[x]; i; i = a[i].next)
    {
        int y = a[i].y;
        if(y == fa)
            continue;
        dfs(y, x);
        tot[x] += tot[y];
        if(zs[x] == 0 || tot[y] > tot[zs[x]])
            zs[x] = y;
    }
}

void find_top(int x, int fa)
{
    dfn[x] = ++num;
    if(zs[x])
    {
        top[zs[x]] = top[x];
        find_top(zs[x], x);
    }
    for(int i = first[x]; i; i = a[i].next)
    {
        int y = a[i].y;
        if(y == fa || y == zs[x])
            continue;
        top[y] = y;
        find_top(y, x);
    }
}

int solve(int x, int y, int tmp)
{
    int tx = top[x], ty = top[y], ans = 0;
    while(tx != ty)
    {
        if(dep[tx] < dep[ty])
            swap(x, y), swap(tx, ty);
        if(tmp == 0)
        {
            add(dfn[tx], 1);
            add(dfn[x] + 1, -1);
        }
        else
            ans += getsum(dfn[x]) - getsum(dfn[tx] - 1);
        x = f[top[x]][0];
        tx = top[x];
    }
    if(x == y)
        return ans;
    else
    {
        if(dep[x] < dep[y])
            swap(x, y);
        if(tmp == 0)
        {
            add(dfn[y] + 1, 1);
            add(dfn[x] + 1, -1);
        }
        else
            ans += getsum(dfn[x]) - getsum(dfn[y]);
        return ans;
    }
}

void lca_init()
{
    for(int j = 1; j <= D; j++)
        for(int i = 1; i <= n; i++)
            f[i][j] = f[f[i][j - 1]][j - 1];
}

int lca_query(int x, int y)
{
    if(dep[x] < dep[y])
        swap(x, y);
    for(int i = D; i >= 0; i--)
    {
        if(f[x][i] == 0)
            continue;
        if(dep[f[x][i]] >= dep[y])
            x = f[x][i];
    }
    if(x == y)
        return x;
    for(int i = D; i >= 0; i--)
    {
        if(f[x][i] != f[y][i])
            x = f[x][i], y = f[y][i];
    }
    return f[x][0];
}

int main()
{
    scanf("%d%d", &n, &m);
    int x, y, z;
    len = 0;
    num = 0;
    memset(first, 0, sizeof(first));
    memset(f, 0, sizeof(f));
    memset(c0, 0, sizeof(c0));
    memset(c1, 0, sizeof(c1));
    memset(dep, 0, sizeof(dep));
    memset(tot, 0, sizeof(tot));
    memset(zs, 0, sizeof(zs));
    memset(dfn, 0, sizeof(dfn));
    for(int i = 1; i < n; i++)
    {
        scanf("%d%d", &x, &y);
        ins(x, y);
        ins(y, x);
    }
    dfs(1, 0);
    top[1] = 1;
    find_top(1, 0);
    lca_init();
    for(int i = 1; i <= m; i++)
    {
        scanf("%s", s);
        scanf("%d%d", &x, &y);
        if(s[0] == 'P')
        {
            z = lca_query(x, y);
            solve(x, z, 0);
            solve(y, z, 0);
        }
        else
        {
            printf("%d\n", solve(x, y, 1));
        }
    }
    return 0;
}