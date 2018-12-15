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

/*input
-5
3
4 3 2
2 3 1 0.6 0.4
2 4 2 0.3 0.7
2 1 0 0.5 0.5

2 2 0 0.5 0.5
2 3 0 0.7 0.3
2 1 0 0.6 0.4

3 10 4 0 0.5 0.4 0.1
*/

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
const int INF=2139062143;
const double EPS=1e-12;

bool cmp(double a,double b) {
    return abs(a-b)<EPS ? 0 : 1;
}

struct Item
{
    long double x;
    vector<int> g,d;
    int c,reliability;
    Item()
    {
        x=1;
        c=INF;
        reliability=INF;
        g.clear();
        d.clear();
    }
    Item(long double x):x(x)
    {
        c=INF;
        reliability=INF;
        g.clear();
        d.clear();
    }
    const Item& operator =(const Item& p) {
        x=p.x;
        c=p.c;
        reliability=p.reliability;
        g.clear();
        d.clear();
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
    long double get_reliability(int* w, int h)
    {
        // if (c!=4 && c!=0)
        // {
        //     cout<<(*this)<<endl;
        // }
        if (h>=0) return 0;
        reliability = 0;
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
    void transfer_for_sr(int* w, int h)
    {
        if (cmp(this->get_reliability(w,h),0) && c>0)
        {
            // cout<<(*this)<<":"<<this->get_reliability(w,h)<<" - ";
            int l = 0;
            int r = g.size()-1;
            while (l<g.size() && r>=0 && c>0)
            {
                while (l<g.size() && r>=0 && g[l]-d[l]>=0) l++;
                while (l<g.size() && r>=0 && g[r]-d[r]<=0) r--;
                if (l<g.size() && r>=0)
                {
                    int delta = min(d[l]-g[l],g[r]-d[r]);
                    delta = min(delta,c);
                    g[l]+=delta;
                    g[r]-=delta;
                    c-=delta;
                }
            }
            // this->get_reliability(w,h);
            // cout<<(*this)<<":"<<this->get_reliability(w,h)<<endl;
        }
    }
    void transfer_for_mr(int* w, int h)
    {
        int l = 0;
        int r = g.size()-1;
        if (cmp(this->get_reliability(w,h),0) && c>0)
        {
            // cout<<(*this)<<":"<<this->get_reliability(w,h)<<" - ";
            while (l<r && c>0)
            {
                while (l<r && g[l]-d[l]>=0) l++;
                while (l<r && g[r]<=0) r--;
                if (l<r)
                {
                    int delta = min(d[l]-g[l],g[r]);
                    delta = min(delta,c);
                    g[l]+=delta;
                    g[r]-=delta;
                    c-=delta;
                }
            }
            // this->get_reliability(w,h);
            // cout<<(*this)<<":"<<this->get_reliability(w,h)<<endl;
        }
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
        if (p.reliability!=INF)
        {
            os<<"{"<<p.reliability<<"}";
        }
        os<<"]";
        return os;
    }
};

int i,j,k,n,m,x,y,l,r,tcase,xcase;
int ta,min_h;
double td;
int number_of_factory,size_conveyor;
int size_g_q[MAXN],size_d_s[MAXN];
vector<Item> ua,ugf_r,ugf_sr,ugf_mr;
int weight[MAXN],c[MAXM];
double beta[MAXM];
vector<int> g[MAXN],d[MAXN];
vector<double> q[MAXN],s[MAXN];
vector<Item> u[MAXN],w[MAXN],delta[MAXN],ugf[MAXN];

const string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

    return buf;
}

void inputs()
{
    cin>>min_h;
    cin>>number_of_factory;
    for (int i = 0; i < number_of_factory; ++i)
    {
       cin>>weight[i];
    }
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
    cin>>size_conveyor;
    for (int i = 0; i < size_conveyor; ++i)
    {
        cin>>c[i];
    }
    for (int i = 0; i < size_conveyor; ++i)
    {
        cin>>beta[i];
    }
}

void get_inputs()
{
    cout<<"Minimum H: "<<min_h<<endl;
    cout << number_of_factory << " factories." << endl;
    cout << "W: ";
    for (int i = 0; i < number_of_factory; ++i)
    {
        cout<<weight[i]<<",";
    }
    cout<<endl;
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

vector<Item> calculate_multiply(vector<Item> a, int* c, double* beta, int size_conveyor)
{
    vector<Item> result;
    for (int i = 0; i <a.size(); ++i)
    {
        for (int j = 0; j < size_conveyor; ++j)
        {
            Item item = a[i];
            item.x *= beta[j];
            item.c = c[j];
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

double calculat_r(vector<Item> u, int* weight, int h)
{
    // cout<<"u: ";
    // for (int i = 0; i < u.size(); ++i)
    // {
    //     cout<<u[i]<<",";
    // }
    // cout<<endl;
    double result = 0;
    for (int i = 0; i < u.size(); ++i)
    {
        result+=u[i].get_reliability(weight,h);
    }
    // cout<<"r: "<<setprecision(10)<<result<<endl;
    result = 1 - result;
    return result;
}

int main()
{
    tcase = 1;
    // freopen("s.in","r",stdin);
    // #ifdef Smile
    //     freopen("s.in","r",stdin);
    //     freopen("s.out","w",stdout);
    //     cin>>tcase;
    // #endif
    cout<<"========================= "<< currentDateTime() <<" =========================="<<endl;

    xcase=0;
    while (xcase++ < tcase)
    {
        inputs();
        get_inputs();
        cout<<endl;
        cout<<"#\tH\tR\tSR\tMR"<<endl;

        for (int i = 0; i < number_of_factory; ++i)
        {
            delta[i] = calculate_multiply(u[i],w[i]);
        }
        ugf[0].push_back(Item(1));
        for (int i = 1; i <= number_of_factory; ++i)
        {
            ugf[i] = calculate_multiply(ugf[i-1],delta[i-1]);
        }
        ugf_r=calculate_multiply(ugf[number_of_factory],c,beta,size_conveyor);
        for (int h = min_h; h <= 0; ++h)
        {
            double result_r = calculat_r(ugf_r,weight,h);
            // cout<<"R: "<<result_r<<endl;
            ugf_sr=ugf_r;
            for (int i = 0; i < ugf_sr.size(); ++i)
            {
                ugf_sr[i].transfer_for_sr(weight,h);
            }
            double result_sr = calculat_r(ugf_sr,weight,h);
            // cout<<"SR: "<<result_sr<<endl;
            ugf_mr=ugf_sr;
            for (int i = 0; i < ugf_mr.size(); ++i)
            {
                ugf_mr[i].transfer_for_mr(weight,h);
            }
            double result_mr = calculat_r(ugf_mr,weight,h);
            // cout<<"MR: "<<result_mr<<endl;
            // get_inputs();
            // cout << "Case #" << xcase++ << ": " << endl;
            // cout<<"H: "<<h<<endl;
            // cout<<"R: "<<setprecision(12)<<result_r<<endl;
            // cout<<"SR: "<<setprecision(12)<<result_sr<<endl;
            // cout<<"MR: "<<setprecision(12)<<result_mr<<endl;
            cout<<setprecision(12)<<xcase++<<"\t"<<h<<"\t"<<result_r<<"\t"<<result_sr<<"\t"<<result_mr<<endl;
        }
    }

    cout<<"========================= "<< currentDateTime() <<" =========================="<<endl;

    #ifndef Smile
        system("pause");
    #endif
    return 0;
}