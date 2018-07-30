#include<bits/stdc++.h>
using namespace std;
#define ll long long
#define pqq priority_queue
#define itt ::iterator
void setIO(string y)
{
    if(y == "")
        return ;
#ifndef ybw
    freopen((y + ".in").c_str(), "r", stdin);
    freopen((y + ".out").c_str(), "w", stdout);
#endif
}
const int maxn = 10 + 1e5;
struct EDGE
{
    int v, next;
} e[2 * (maxn)];
ll f[maxn], dep[maxn], size[maxn], son[maxn], top[maxn], id[maxn], rk[maxn],
cnt, head[maxn], tt = 1, n, m, mod, rt;
ll add(int x, int y)
{
    e[++tt].v = y;
    e[tt].next = head[x];
    head[x] = tt;
}
//=============================加边以及定义=================================
void dfs1(int u, int fa)
{
    f[u] = fa;
    dep[u] = dep[fa] + 1;
    size[u] = 1;
    for(int i = head[u]; i; i = e[i].next)
    {
        int v = e[i].v;
        if(v != fa)
        {
            dfs1(v, u);
            size[u] += size[v];
            if(size[v] > size[son[u]])
                son[u] = v;
        }
    }
}
void dfs2(int x, int topf)
{
    id[x] = ++cnt;
    top[x] = topf;
    if(!son[x])
        return;
    dfs2(son[x], topf);
    for(int i = head[x]; i; i = e[i].next)
    {
        int y = e[i].v;
        if(y == f[x] || y == son[x])
            continue;
        dfs2(y, y);
    }
}
//---------------------------树链剖分-------------------------------------
ll sum[maxn], a[maxn], de[maxn],
dei[maxn], b[maxn];
int lowbit(int n)
{
    return n & -n;
}
ll qu(ll a[], int x)
{
    ll ans = 0;
    for(; x; x -= lowbit(x))
        ans = (ans + a[x]) % mod;
    return ans;
}
void upd(ll a[], int x, int ans)
{
    for(; x <= n; x += lowbit(x))
        a[x] = (a[x] + ans) % mod;
}
void Upd(int x, int y, int z)
{
    upd(de, x, z);
    upd(de, y + 1, -z);
    upd(dei, x, z * x);
    upd(dei, y + 1, -z * (y + 1));
}
long long Qu(int l, int r)
{
    long long suml = (sum[l - 1] + (l * qu(de, l - 1)) % mod - qu(dei,
                      l - 1) + mod) % mod;
    long long sumr = (sum[r] + ((r + 1) * qu(de, r)) % mod - qu(dei,
                      r) + mod) % mod;
    return (mod + sumr - suml) % mod;
}
//---------------------------树状数组-------------------------------------
void ups(int x, int y, int k)
{
    k %= mod;
    while(top[x] != top[y])
    {
        if(dep[top[x]] < dep[top[y]])
            swap(x, y);
        Upd(id[top[x]], id[x], k);
        x = f[top[x]];
    }
    if(dep[x] > dep[y])
        swap(x, y);
    Upd(id[x], id[y], k);
}
int qs(int x, int y)
{
    int ans = 0;
    while(top[x] != top[y])
    {
        if(dep[top[x]] < dep[top[y]])
            swap(x, y);
        ans += Qu(id[top[x]], id[x]);
        ans %= mod;
        x = f[top[x]];
        // cout << ans << endl;
    }
    if(dep[x] > dep[y])
        swap(x, y);
    ans += Qu(id[x], id[y]);
    return ans % mod;
}
//=======================LCA进阶==========================================
int main()
{
    setIO("");
    cin >> n >> m >> rt >> mod;
    for(int i = 1; i <= n; i++)
        cin >> a[i];
    for(int i = 1; i < n; i++)
    {
        int x, y;
        cin >> x >> y;
        add(x, y);
        add(y, x);
    }
    cnt = 1;
    dep[0] = 0;
    dfs1(rt, 0);
    dfs2(rt, rt);
    for(int i = 1; i <= n; i++)
        b[id[i]] = a[i];
    memcpy(a, b, sizeof(a));
    for(int i = 1; i <= n; i++)
        sum[i] = sum[i - 1] + a[i];
    //    cout<<1<<endl;
    for(int i = 1; i <= m; i++)
    {
        int x, z, y, t;
        cin >> t;
        if(t == 1)
        {
            cin >> x >> y >> z;
            z %= mod;
            ups(x, y, z);
        }
        if(t == 2)
        {
            cin >> x >> y;
            cout << qs(x, y) << endl;
        }
        if(t == 3)
        {
            cin >> x >> z;
            y = id[x] + size[x] - 1;
            z %= mod;
            Upd(id[x], y, z);
        }
        if(t == 4)
        {
            cin >> x;
            int l = id[x], r = id[x] + size[x] - 1;
            printf("%lld\n", Qu(l, r));
        }
    }
    return 0;
}