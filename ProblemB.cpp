/*input
-20
5
3 2 2 1 1

3 5 3 1 0.3 0.4 0.3
3 4 3 0 0.1 0.5 0.4

2 3 0 0.4 0.6
3 2 1 0 0.4 0.2 0.4

3 7 3 2 0.6 0.1 0.3
2 6 2 0.6 0.4

3 4 2 1 0.2 0.2 0.6
3 4 2 0 0.2 0.5 0.3

2 2 0 0.5 0.5
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

struct Factory
{
    int id;
    vector<int> g,d;
    vector<double> q,s;
    Factory()
    {
        id=INF;
        g.clear();
        d.clear();
        q.clear();
        s.clear();
    }
    Factory(int id):id(id)
    {
        id=INF;
        g.clear();
        d.clear();
        q.clear();
        s.clear();
    }
    const Factory& operator =(const Factory& p) {
        id = p.id;
        g.clear();
        d.clear();
        q.clear();
        s.clear();
        for (int i = 0; i < p.g.size(); ++i) g.push_back(p.g[i]);
        for (int i = 0; i < p.d.size(); ++i) d.push_back(p.d[i]);
        for (int i = 0; i < p.q.size(); ++i) q.push_back(p.q[i]);
        for (int i = 0; i < p.s.size(); ++i) s.push_back(p.s[i]);
        return *this;
    }
    bool operator <(const Factory &p) const {
        return id < p.id;
    }
    friend istream& operator >>(istream &is, Factory &p) {
        p.id = INF;
        p.g.clear();
        p.d.clear();
        p.q.clear();
        p.s.clear();
        int number_of_g_of_factory,number_of_d_of_factory,x;
        double y;
        is>>number_of_g_of_factory;
        for (int i = 0; i < number_of_g_of_factory; ++i)
        {
            is>>x;
            p.g.push_back(x);
        }
        for (int i = 0; i < number_of_g_of_factory; ++i)
        {
            is>>y;
            p.q.push_back(y);
        }
        is>>number_of_d_of_factory;
        for (int i = 0; i < number_of_d_of_factory; ++i)
        {
            is>>x;
            p.d.push_back(x);
        }
        for (int i = 0; i < number_of_d_of_factory; ++i)
        {
            is>>y;
            p.s.push_back(y);
        }
        return is;
    }
    friend ostream& operator <<(ostream &os, const Factory &p) 
    {
        os<<p.id<<endl;
        os<<"g: ";
        for (int i = 0; i < p.g.size(); ++i)
        {
            os<<p.g[i]<<",";
        }
        os<<endl;
        os<<"q: ";
        for (int i = 0; i < p.q.size(); ++i)
        {
            os<<p.q[i]<<",";
        }
        os<<endl;
        os<<"d: ";
        for (int i = 0; i < p.d.size(); ++i)
        {
            os<<p.d[i]<<",";
        }
        os<<endl;
        os<<"s: ";
        for (int i = 0; i < p.s.size(); ++i)
        {
            os<<p.s[i]<<",";
        }
        os<<endl;
        return os;
    }
};

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
        // if (h>=0) return 0;
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
double result_r,result_sr,result_mr;
int number_of_factory,size_conveyor;
int size_g_q[MAXN],size_d_s[MAXN];
vector<Item> ua,ugf_r,ugf_sr,ugf_mr;
int weight[MAXN],c[MAXM];
double beta[MAXM];
vector<int> g[MAXN],d[MAXN];
vector<double> q[MAXN],s[MAXN];
vector<Item> u[MAXN],w[MAXN],delta[MAXN],ugf[MAXN];
Factory f[MAXN];

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
        cin>>f[i];
        f[i].id = i;
        // cout<<f[i];
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

void prepare_data()
{
    for (int i = 0; i < MAXN; ++i)
    {
        g[i].clear();
        q[i].clear();
        d[i].clear();
        s[i].clear();
        u[i].clear();
        w[i].clear();
        delta[i].clear();
        ugf[i].clear();
    }
    for (int i = 0; i < number_of_factory; ++i)
    {
        for (int j = 0; j < f[i].g.size(); ++j)
        {
            g[i].push_back(f[i].g[j]);
            q[i].push_back(f[i].q[j]);
            u[i].push_back(Item(f[i].q[j]));
            u[i][j].g.push_back(f[i].g[j]);
        }
        for (int j = 0; j < f[i].d.size(); ++j)
        {
            d[i].push_back(f[i].d[j]);
            s[i].push_back(f[i].s[j]);
            w[i].push_back(Item(f[i].s[j]));
            w[i][j].d.push_back(f[i].d[j]);
        }
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
        for (int j = 0; j < f[i].g.size(); ++j)
        {
            cout<<g[i][j]<<"("<<q[i][j]<<"),";
        }
        cout<<endl;
    }
    cout<< "u: "<<endl;
    for (int i = 0; i < number_of_factory; ++i)
    {
        for (int j = 0; j < f[i].g.size(); ++j)
        {
            cout<<u[i][j]<<", ";
        }
        cout<<endl;
    }
    cout << "d & s: "<<endl;
    for (int i = 0; i < number_of_factory; ++i)
    {
        for (int j = 0; j < f[i].d.size(); ++j)
        {
            cout<<d[i][j]<<"("<<s[i][j]<<"),";
        }
        cout<<endl;
    }
    cout<< "w: "<<endl;
    for (int i = 0; i < number_of_factory; ++i)
    {
        for (int j = 0; j < f[i].d.size(); ++j)
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
    freopen("s.in","r",stdin);
    freopen("s.out","w",stdout);
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
        prepare_data();
        // get_inputs();
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
            result_r = calculat_r(ugf_r,weight,h);
            // cout<<"R: "<<result_r<<endl;
            ugf_sr=ugf_r;
            for (int i = 0; i < ugf_sr.size(); ++i)
            {
                ugf_sr[i].transfer_for_sr(weight,h);
            }
            result_sr = calculat_r(ugf_sr,weight,h);
            // cout<<"SR: "<<result_sr<<endl;
            ugf_mr=ugf_sr;
            for (int i = 0; i < ugf_mr.size(); ++i)
            {
                ugf_mr[i].transfer_for_mr(weight,h);
            }
            result_mr = calculat_r(ugf_mr,weight,h);
            // cout<<"MR: "<<result_mr<<endl;
            // get_inputs();
            // cout << "Case #" << xcase++ << ": " << endl;
            // cout<<"H: "<<h<<endl;
            // cout<<"R: "<<setprecision(12)<<result_r<<endl;
            // cout<<"SR: "<<setprecision(12)<<result_sr<<endl;
            // cout<<"MR: "<<setprecision(12)<<result_mr<<endl;
            cout<<setprecision(12)<<xcase++<<"\t"<<h<<"\t"<<result_r<<"\t"<<result_sr<<"\t"<<result_mr<<endl;
        }

        cout<<"#\tW\tR\tSR\tMR"<<endl;
        xcase=0;
        
        do
        {
            prepare_data();
            // get_inputs();
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
            int h=-5;
            result_r = calculat_r(ugf_r,weight,h);
            ugf_sr=ugf_r;
            for (int i = 0; i < ugf_sr.size(); ++i)
            {
                ugf_sr[i].transfer_for_sr(weight,h);
            }
            result_sr = calculat_r(ugf_sr,weight,h);
            ugf_mr=ugf_sr;
            for (int i = 0; i < ugf_mr.size(); ++i)
            {
                ugf_mr[i].transfer_for_mr(weight,h);
            }
            result_mr = calculat_r(ugf_mr,weight,h);
            cout<<setprecision(12)<<xcase++<<"\t";
            for (int i = 0; i < number_of_factory; ++i)
            {
                cout<<f[i].id+1;
            }
            cout<<"\t"<<result_r<<"\t"<<result_sr<<"\t"<<result_mr<<endl;
        }
        while (next_permutation(f,f+number_of_factory));
    }

    cout<<"========================= "<< currentDateTime() <<" =========================="<<endl;

    #ifndef Smile
        system("pause");
    #endif
    return 0;
}