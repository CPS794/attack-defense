/*input
-5
2
6 4
3 6 4 2 0.4 0.4 0.2
2 5 3 0.6 0.4
3 4 2 0 0.3 0.3 0.4
2 2 0 0.5 0.5
2 4 0 0.5 0.5
*/

/*input
-5
5
3 2 2 1 1
3 5 3 1 0.3 0.4 0.3
2 3 0 0.4 0.6
3 7 3 2 0.6 0.1 0.3
3 4 2 1 0.2 0.2 0.6
2 2 0 0.5 0.5

3 4 3 0 0.1 0.5 0.4
3 2 1 0 0.4 0.2 0.4
2 6 2 0.6 0.4
3 4 2 0 0.2 0.5 0.3
2 3 1 0.4 0.6

3 10 4 0 0.5 0.4 0.1
*/

/*
H
number_of_factory
Weight[1] Weight[2] ...
number_of_g_of_factory[1] g[1][0] g[1][1] g[1][2] ... q[1][0] q[1][1] q[1][2] ...
number_of_g_of_factory[2] g[2][0] g[2][1] g[2][2] ... q[2][0] q[2][1] q[2][2] ...
...
number_of_d_of_factory[1] d[1][0] d[1][1] ... s[1][0] s[1][1] ...
number_of_d_of_factory[2] d[2][0] d[2][1] ... s[2][0] s[2][1] ...
...
number_of_c c[1] c[2] ... beta[1] beta[2] ...
 */
#include <bits/stdc++.h>
using namespace std;

const int MAXN=7;
const int MAXM=7;
const int MOD=1000000007;
const int INF=2139062143;

struct Item
{
    double x;
    vector<int> g,d;
    int c;
    Item(){x=1;c=INF;}
    Item(double x):x(x){c=INF;}
    Item(double x,vector<int> g,vector<int> d):x(x),g(g),d(d){}
    const Item& operator =(const Item& p) {
        x=p.x;
        c=p.c;
        for (int i = 0; i < p.g.size(); ++i) g.push_back(p.g[i]);
        for (int i = 0; i < p.d.size(); ++i) d.push_back(p.d[i]);
        return *this;
    }
    const Item& operator *=(const Item& p)
    {
        x*=p.x;
        c=max(c,p.c);
        for (int i = 0; i < p.g.size(); ++i) g.push_back(p.g[i]);
        for (int i = 0; i < p.d.size(); ++i) d.push_back(p.d[i]);
        return *this;
    }
    double get_reliability(int* w, int h)
    {
        int reliability = 0;
        for (int i = 0; i < g.size(); ++i)
        {
            if (g[i]-d[i]<0)
            {
                reliability += (g[i]-d[i])*w[i];
            }
        }
        if (reliability < h)
        {
            // cout<<*this<<","<<reliability<<endl;
            return x;
        }
        return 0;
    }
    friend ostream& operator <<(ostream &os, const Item &p) 
    {
        os<<p.x<<"[";
        if (p.g.size()>0)
        {
            os<<"{";
            for (int i = 0; i < p.g.size(); ++i)
            {
                if (i>0)
                {
                    os<<",";
                }
                os<<p.g[i];
            }
            os<<"}";
        }
        if (p.d.size()>0)
        {
            os<<"{";
            for (int i = 0; i < p.d.size(); ++i)
            {
                if (i>0)
                {
                    os<<",";
                }
                os<<p.d[i];
            }
            os<<"}";
        }
        if (p.c!=INF)
        {
            os<<"{"<<p.c<<"}";
        }
        os<<"]";
        return os;
    }
};

int i,j,k,n,m,x,y,l,r,tcase,xcase;
int ta,h;
double td;
int number_of_factory,size_conveyor;
int size_g_q[MAXN],size_d_s[MAXN];
vector<Item> ua;
int weight[MAXN],c[MAXM];
double beta[MAXM];
vector<int> g[MAXN],d[MAXN];
vector<double> q[MAXN],s[MAXN];
vector<Item> u[MAXN],w[MAXN],eta[MAXM],delta[MAXN],ugf[MAXN];

const string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

    return buf;
}

void get_inputs()
{
    cin>>h;
    cout<<"H: "<<h<<endl;
    cin>>number_of_factory;
    cout << number_of_factory << " factories." << endl;
    for (int i = 0; i < number_of_factory; ++i)
    {
       cin>>weight[i];
    }
    cout << "W: ";
    for (int i = 0; i < number_of_factory; ++i)
    {
        cout<<weight[i]<<",";
    }
    cout<<endl;
    for (int i = 0; i < number_of_factory; ++i)
    {
        cin>>size_g_q[i];
        for (int j = 0; j < size_g_q[i]; ++j)
        {
            cin>>ta;
            g[i].push_back(ta);
        }
        for (int j = 0; j < size_g_q[i]; ++j)
        {
            cin>>td;
            q[i].push_back(td);
            u[i].push_back(Item(td));
            u[i][j].g.push_back(g[i][j]);
        }
        cin>>size_d_s[i];
        for (int j = 0; j < size_d_s[i]; ++j)
        {
            cin>>ta;
            d[i].push_back(ta);
        }
        for (int j = 0; j < size_d_s[i]; ++j)
        {
            cin>>td;
            s[i].push_back(td);
            w[i].push_back(Item(td));
            w[i][j].d.push_back(d[i][j]);
        }
    }
    cout << "g & q: "<<endl;
    for (int i = 0; i < number_of_factory; ++i)
    {
        for (int j = 0; j < size_g_q[i]; ++j)
        {
            cout<<g[i][j]<<"("<<q[i][j]<<"),";
        }
        cout<<endl;
    }
    cout<< "u: "<<endl;
    for (int i = 0; i < number_of_factory; ++i)
    {
        for (int j = 0; j < size_g_q[i]; ++j)
        {
            cout<<u[i][j]<<", ";
        }
        cout<<endl;
    }
    cout << "d & s: "<<endl;
    for (int i = 0; i < number_of_factory; ++i)
    {
        for (int j = 0; j < size_d_s[i]; ++j)
        {
            cout<<d[i][j]<<"("<<s[i][j]<<"),";
        }
        cout<<endl;
    }
    cout<< "w: "<<endl;
    for (int i = 0; i < number_of_factory; ++i)
    {
        for (int j = 0; j < size_d_s[i]; ++j)
        {
            cout<<w[i][j]<<", ";
        }
        cout<<endl;
    }
    cin>>size_conveyor;
    for (int i = 0; i < size_conveyor; ++i)
    {
        cin>>c[i];
    }
    for (int i = 0; i < size_conveyor; ++i)
    {
        cin>>beta[i];
    }
    cout << "c: "<<endl;
    for (int i = 0; i < size_conveyor; ++i)
    {
        cout<<c[i]<<"("<<beta[i]<<"),";
    }
    cout<<endl;
}

vector<Item> calculate_multiply(vector<Item> a, vector<Item> b)
{
    vector<Item> result;
    for (int i = 0; i <a.size(); ++i)
    {
        for (int j = 0; j < b.size(); ++j)
        {
            Item item = a[i];
            item *= b[j];
            result.push_back(item);
        }
    }
    // cout<<"result: ";
    // for (int i = 0; i < result.size(); ++i)
    // {
    //     cout<<result[i]<<",";
    // }
    // cout<<endl;
    return result;
}

double calculat_r()
{
    double result = 0;
    for (int i = 0; i < ugf[number_of_factory].size(); ++i)
    {
        result+=ugf[number_of_factory][i].get_reliability(weight,h);
    }
    result = 1 - result;
    return result;
}

int main()
{
    tcase = 1;
    // #ifdef Smile
    //     freopen("s.in","r",stdin);
    //     freopen("s.out","w",stdout);
    //     cin>>tcase;
    // #endif
    cout<<"========================= "<< currentDateTime() <<" =========================="<<endl;

    xcase=0;
    while (xcase++ < tcase)
    {
        cout << "Case #" << xcase << ": " << endl;
        get_inputs();
        for (int i = 0; i < number_of_factory; ++i)
        {
            delta[i] = calculate_multiply(u[i],w[i]);
        }
        ugf[0].push_back(Item(1));
        for (int i = 1; i <= number_of_factory; ++i)
        {
            ugf[i] = calculate_multiply(ugf[i-1],delta[i-1]);
        }
        double result = calculat_r();
        cout<<"R: "<<result<<endl;
    }

    cout<<"========================= "<< currentDateTime() <<" =========================="<<endl;

    #ifndef Smile
        system("pause");
    #endif
    return 0;
}